#include "TextureManager.h"

uint32_t TextureManager::kSRVIndexTop = 1;

void TextureManager::Initialize() {
	textureDatas.reserve(DirectX12::kMaxSRVCount);
}

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;

	return &instance;
}

void TextureManager::LoadTexture(const std::string& filePath) {
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);
	if (it != textureDatas.end()) {
		return;
	}

	assert(textureDatas.size() + kSRVIndexTop < DirectX12::kMaxSRVCount);

	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);

	HRESULT hr;
	if (filePathW.ends_with(L".dds")) { //ddsで終わっていたらddsとみなす。
		hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else {
		hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
	}
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImages{};
	if (DirectX::IsCompressed(image.GetMetadata().format)) {
		mipImages = std::move(image); //圧縮フォーマットならそのままMOVE
	}
	else {
		hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 4, mipImages);
	}
	assert(SUCCEEDED(hr));

	textureDatas.resize(textureDatas.size() + 1 + kSRVIndexTop);
	TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	textureData.metaData = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(textureData.metaData);

	//for (size_t mipLevel = 0; mipLevel < textureData.metaData.mipLevels; ++mipLevel) {
	//	const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
	//	hr = textureData.resource->WriteToSubresource(
	//		UINT(mipLevel),
	//		nullptr,
	//		img->pixels,
	//		UINT(img->rowPitch),
	//		UINT(img->slicePitch)
	//	);
	//	assert(SUCCEEDED(hr));
	//}
	ID3D12Resource* intermediateResource = UploadTextureData(textureData.resource.Get(), mipImages, DirectX12::GetInstance()->GetDevice().Get(), DirectX12::GetInstance()->GetCommandList().Get());
	//commandList close
	DirectX12::GetInstance()->GetCommandList().Get()->Close();

	ID3D12CommandList* commandLists[] = { DirectX12::GetInstance()->GetCommandList().Get() };
	DirectX12::GetInstance()->GetCommandQueue().Get()->ExecuteCommandLists(1, commandLists);
	//実行を待つ
	DirectX12::GetInstance()->SetFenceValue(1);
	DirectX12::GetInstance()->GetCommandQueue().Get()->Signal(DirectX12::GetInstance()->GetFence().Get(), UINT(DirectX12::GetInstance()->GetFenceValue()));
	if (DirectX12::GetInstance()->GetFence().Get()->GetCompletedValue() < UINT(DirectX12::GetInstance()->GetFenceValue())) {
		DirectX12::GetInstance()->GetFence().Get()->SetEventOnCompletion(UINT(DirectX12::GetInstance()->GetFenceValue()), DirectX12::GetInstance()->GerFenceEvent());
		WaitForSingleObject(DirectX12::GetInstance()->GerFenceEvent(), INFINITE);
	}
	//allocator commandlist reset
	hr = DirectX12::GetInstance()->GetCommandAllocator().Get()->Reset();
	assert(SUCCEEDED(hr));
	hr = DirectX12::GetInstance()->GetCommandList().Get()->Reset(DirectX12::GetInstance()->GetCommandAllocator().Get(), nullptr);
	assert(SUCCEEDED(hr));
	//intermediateResource Release
	intermediateResource->Release();

	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
	textureData.srvHandleCPU = GetCPUDescriptorHandle(srvIndex);
	textureData.srvHandleGPU = GetGPUDescriptorHandle(srvIndex);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	if (textureData.metaData.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = UINT(textureData.metaData.mipLevels);
	}
	DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata) {
	//metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//Resourceを生成する
	ID3D12Resource* resource = nullptr;
	HRESULT hr = DirectX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}

[[nodiscard]]
ID3D12Resource* TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device, ID3D12GraphicsCommandList* commandlist) {
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	ID3D12Resource* intermediateResource = DirectX12::GetInstance()->CreateBufferResource(intermediateSize).Get();
	UpdateSubresources(commandlist, texture, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandlist->ResourceBarrier(1, &barrier);

	return intermediateResource;
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = DirectX12::GetInstance()->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (DirectX12::GetInstance()->GetdescriptorSizeSRV() * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = DirectX12::GetInstance()->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (DirectX12::GetInstance()->GetdescriptorSizeSRV() * index);
	return handleGPU;
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);

	if ( it != textureDatas.end()) {
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex) {
	assert(textureIndex < DirectX12::kMaxSRVCount);

	TextureData& textureData = textureDatas[textureIndex];
	return textureData.srvHandleGPU;
}