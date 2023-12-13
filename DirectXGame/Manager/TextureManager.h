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
	void LoadTexture(uint32_t index);

private:

	static const size_t kMaxSRVCount = 2056;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, kMaxSRVCount> textureBuffers_;

};

