#pragma once
#include "../../Math/Vector4.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <string>
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>

#include "../Log/Log.h"
#include "../DirextX12/DirectX12.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

#define kNumPSO 2

class GraphicsRenderer
{
public:
	void Initialize();

	void InitializeDXC();

	IDxcBlob* CompileShader(
		const std::wstring& filepath,
		const wchar_t* profile,
		IDxcUtils* dxcUtiles,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);
	void DecideCommand();
	//void CloseCommand(DirectX12* directX12);

	void MakeRootSignature();
	void SetInputLayout();
	void SetBlendState();
	void SetRasterizerState();
	void ShaderCompile();
	void MakePSO();
	//
	void MakeVertexResource();

	void MakeVertexBufferView();
	void DateResource(Vector2* leftBot, Vector2* midTop, Vector2* rightBot);
	void ViewportScissor();

	void Release();
private:
	DirectX12* directX12 = nullptr;
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//InitializeDXC
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler;
	IDxcIncludeHandler* includeHandler;
	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature[kNumPSO]{};
	D3D12_ROOT_PARAMETER rootParameters[kNumPSO][5];

	ID3DBlob* signatureBlob[kNumPSO];
	ID3DBlob* errorBlob[kNumPSO];

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature[kNumPSO];
	//SetInputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[kNumPSO][3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc[kNumPSO]{};
	//SetBlendState
	D3D12_BLEND_DESC blendDesc[kNumPSO]{};
	//SetRasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc[kNumPSO]{};
	//ShaderCompile
	IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;

	IDxcBlob* particleVertexShaderBlob;
	IDxcBlob* particlePixelShaderBlob;
	//MakePSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc[kNumPSO]{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState[kNumPSO];
	//MakeVertexResource
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	//MakeVertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	//DateResource
	Vector4* vertexDate;
	Vector4* materialDate;
	//ViewportScissor
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	D3D12_DESCRIPTOR_RANGE descriptorRange[kNumPSO][1];
};

