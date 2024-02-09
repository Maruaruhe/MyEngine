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


	textureDatas.resize(textureDatas.size() + 1);
	TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	textureData.metaData = CreatemipImages(filePath).GetMetadata();
	textureData.resource = CreateResource(textureData.metaData);

	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
	textureData.srvHandleCPU = DirectX12::GetInstance().
	textureData.srvHandleGPU =

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};


	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; }
	);
	if (it != textureDatas.end()) {
		return;
	}
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {
	auto it = 
	if () {
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex) {
	assert(textureIndex > DirectX12::kMaxSRVCount);

//	TextureData& textureData =
	//	return textureData.srvHandleGPU;

}

//void TextureManager::Finalize() {
//	delete instance;
//}


DirectX::ScratchImage TextureManager::CreatemipImages(const std::string& filePath) {
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	return mipImages;
}

Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateResource(DirectX::TexMetadata metadata) {

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
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//Resourceを生成する
	ID3D12Resource* resource = nullptr;
	HRESULT hr = DirectX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}