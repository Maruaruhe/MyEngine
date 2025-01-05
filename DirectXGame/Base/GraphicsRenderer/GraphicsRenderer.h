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
	ANIME = 2,
	SPRITE = 3
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
	void MakeRootSignatureForSprite();


	void SetInputLayout();
	void SetInputLayoutForSkinning();

	void SetBlendState();
	void SetBlendStateForParticle();
	void SetRasterizerState();
	void ShaderCompile();
	void MakePSO();
	void MakePSOForParticle();
	void MakePSOForSkinning();
	void MakePSOForSprite();

	void SetRootSignatureAndPSO(int n);

	void ViewportScissor();

	void Release();
private:
	DirectX12* directX12_ = nullptr;
	HRESULT hr_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	//InitializeDXC
	IDxcUtils* dxcUtils_ =nullptr;
	IDxcCompiler3* dxcCompiler_;
	IDxcIncludeHandler* includeHandler_;

	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	D3D12_ROOT_PARAMETER rootParameters_[7];
	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureForParticle_{};
	D3D12_ROOT_PARAMETER rootParametersForParticle_[3];
	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureForSkinning_{};
	D3D12_ROOT_PARAMETER rootParametersForSkinning_[8];

	//MakeRootSignature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignatureForSprite_{};
	D3D12_ROOT_PARAMETER rootParametersForSprite_[8];


	ID3DBlob* signatureBlob_;
	ID3DBlob* errorBlob_;


	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignatureForParticle_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignatureForSkinning_;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignatureForSprite_;

	//SetInputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
	D3D12_INPUT_ELEMENT_DESC inputElementDescsForSkinning_[5] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDescForSkinning_{};
	//SetBlendState
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_BLEND_DESC blendDescForParticle_{};
	//SetRasterizerState
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	//ShaderCompile
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_;
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_;

	Microsoft::WRL::ComPtr<IDxcBlob> particleVertexShaderBlob_;
	Microsoft::WRL::ComPtr<IDxcBlob> particlePixelShaderBlob_;

	Microsoft::WRL::ComPtr<IDxcBlob> skinningVertexShaderBlob_;
	//MakePSO
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState_;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescForParticle_{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForParticle_;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescForSkinning_{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForSkinning_;
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescForSprite_{};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateForSprite_;


	//MakeVertexResource
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
	//MakeVertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//DateResource
	Vector4* vertexDate_;
	Vector4* materialDate_;
	//ViewportScissor
	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	GraphicsRenderer() = default;
	~GraphicsRenderer() = default;
	GraphicsRenderer(GraphicsRenderer&) = delete;
	GraphicsRenderer& operator=(GraphicsRenderer&) = delete;
};

