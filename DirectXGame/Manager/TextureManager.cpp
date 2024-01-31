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


	textureDatas.resize(textureDatas.size() + 1 + kSRVIndexTop);
	TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	//textureData.metaData = 
	//textureData. 

	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size());
	//textureData.srvHandleCPU = 
	//textureData.srvHandleGPU =

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
	//auto it = 
	//if () {
		//uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		//return textureIndex;
	//}
	assert(0);
	return 0;
}

//D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(uint32_t textureIndex) {
//	assert(textureIndex > DirectX12::kMaxSRVCount);
//
////	TextureData& textureData =
//	//	return textureData.srvHandleGPU;
//
//}