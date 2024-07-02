#include "PostEffect.h"
#include "../../Base/Log/Log.h"
#include "../GraphicsRenderer/GraphicsRenderer.h"
#include "../DirextX12/DirectX12.h"

void PostEffect::Initialize() {
	CreateRootSignature();
	CreatePSO();
}

void PostEffect::PostDraw() {
	DirectX12::GetInstance()->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, DirectX12::GetInstance()->GetGPUDescriptorHandle(220));

	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void PostEffect::CreateRootSignature() {
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	//DescriptorRangeの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//RootSignature生成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootParameter作成。
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	//Sampler
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	//シリアライズしてバイナリする
	LRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		LogText(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリをもとに生成
	hr = DirectX12::GetInstance()->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void PostEffect::CreatePSO() {
	//InputLayoutの設定
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = false;

	//Shader
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Resources/Shaders/PostEffects/FullScreen.VS.hlsl", L"vs_6_0");
	//Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Resources/Shaders/PostEffects/CopyImage.PS.hlsl", L"ps_6_0");
	//Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Resources/Shaders/PostEffects/GrayScale.PS.hlsl", L"ps_6_0");
	//Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Resources/Shaders/PostEffects/Vignette.PS.hlsl", L"ps_6_0");
	//Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Resources/Shaders/PostEffects/BoxFilter.PS.hlsl", L"ps_6_0");
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Resources/Shaders/PostEffects/GaussianFilter.PS.hlsl", L"ps_6_0");

	//blendDesc
	D3D12_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//rasterizerDesc
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	
	////PSOを生成する-----------------------------------------------------------------------------------------------
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc = {};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	graphicsPipelineState = nullptr;
	HRESULT hr = DirectX12::GetInstance()->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert((SUCCEEDED(hr)));
}