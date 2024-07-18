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

enum Type {
	MODEL = 0,
	PARTICLE = 1,
	ANIME = 2
};

class GraphicsRenderer
{
public:
	static GraphicsRenderer* GetInstance();

	void Initialize();

	void InitializeDXC();

	IDxcBlob* CompileShader(
		const std::wstring& filepath,
		const wchar_t* profile
	);
	void DecideCommand();
	//void CloseCommand(DirectX12* directX12);

	void MakeRootSignature();
	void MakeRootSignatureForParticle();
	void MakeRootSignatureForSkinning();


	void SetInputLayout();
	void SetInputLayoutForSkinning();

	void SetBlendState();
	void SetBlendStateForParticle();
	void SetRasterizerState();
	void ShaderCompile();
	void MakePSO();
	void MakePSOForParticle();
	void MakePSOForSkinning();

	void SetRootSignatureAndPSO(int n);

	void ViewportScissor();

	void Release();
private:
	DirectX12* directX12 = nullptr;
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	//InitializeDXC
	IDxcUtils* dxcUtils =nullptr;
	IDxcCompiler3* dxcCompiler;
	IDxcIncludeHandler* includeHandler;
	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	D3D12_ROOT_PARAMETER rootParameters[5];
	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureForParticle{};
	D3D12_ROOT_PARAMETER rootParametersForParticle[3];
	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureForSkinning{};
	D3D12_ROOT_PARAMETER rootParametersForSkinning[6];


	ID3DBlob* signatureBlob;
	ID3DBlob* errorBlob;


	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignatureForParticle;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignatureForSkinning;
	//SetInputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_ELEMENT_DESC inputElementDescsForSkinning[5] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDescForSkinning{};
	//SetBlendState
	D3D12_BLEND_DESC blendDesc{};
	D3D12_BLEND_DESC blendDescForParticle{};
	//SetRasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//ShaderCompile
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob;

	Microsoft::WRL::ComPtr<IDxcBlob> particleVertexShaderBlob;
	Microsoft::WRL::ComPtr<IDxcBlob> particlePixelShaderBlob;

	Microsoft::WRL::ComPtr<IDxcBlob> skinningVertexShaderBlob;
	//MakePSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescForParticle{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForParticle;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescForSkinning{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForSkinning;


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

	GraphicsRenderer() = default;
	~GraphicsRenderer() = default;
	GraphicsRenderer(GraphicsRenderer&) = delete;
	GraphicsRenderer& operator=(GraphicsRenderer&) = delete;
};

