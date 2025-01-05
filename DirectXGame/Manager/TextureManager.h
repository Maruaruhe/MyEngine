#pragma once
#include <array>
#include <string>
#include <vector>
#include <d3d12.h>
#include "../../externals/DirectXTex/DirectXTex.h"
#include "../../externals/DirectXTex/d3dx12.h"

#include "../Base/DirextX12/DirectX12.h"
#include "../Base/Log/Log.h"

//TextureManagerクラス
class TextureManager
{
public:
	static TextureManager* GetInstance();

	/// <summary>
	/// Initialize
	/// </summary>
	void Initialize();
	/// <summary>
	/// LoadTexture
	/// </summary>
	/// <param name="filePath"></param>
	void LoadTexture(const std::string& filePath);
	/// <summary>
	/// テクスチャリソースの作成
	/// </summary>
	/// <param name="matadata"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& matadata);

	//Getter
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

private:

	struct TextureData{
		std::string filePath;
		DirectX::TexMetadata metaData;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	static uint32_t kSRVIndexTop;

	std::vector<TextureData> textureDatas_;

	// Fence
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_{};

	//static const size_t kMaxSRVCount = 2056;
	//std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

};

