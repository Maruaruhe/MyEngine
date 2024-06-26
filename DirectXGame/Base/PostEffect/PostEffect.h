#pragma once
#include "../DirextX12/DirectX12.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <string>
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>

class PostEffect
{
public:
	void Initialize();

	void CreateRootSignature();
	void CreatePSO();

	void PostDraw();
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
};

