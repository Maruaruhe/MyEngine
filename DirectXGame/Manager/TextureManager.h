#pragma once
#include <array>
#include <d3d12.h>
#include "../../externals/DirectXTex/DirectXTex.h"

#include "../Base/DirextX12/DirectX12.h"
#include "../Base/Log/Log.h"

class TextureManager
{
public:
	static TextureManager* GetInstance();

	void Initialize();
	void LoadTexture(const std::string& filePath);

	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);
private:

	struct TextureData{
		std::string filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	static uint32_t kSRVIndexTop;

	std::vector<TextureData> textureDatas;

	//static const size_t kMaxSRVCount = 2056;
	//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

};

