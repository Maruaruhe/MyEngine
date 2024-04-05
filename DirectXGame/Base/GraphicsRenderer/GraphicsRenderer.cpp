#include "GraphicsRenderer.h"
#include <assert.h>

GraphicsRenderer* GraphicsRenderer::GetInstance() {
	static GraphicsRenderer instance;

	return &instance;
}


void GraphicsRenderer::Initialize() {
	directX12 = DirectX12::GetInstance();
	InitializeDXC();
	MakeRootSignature();
	MakeRootSignatureForParticle();

	SetInputLayout();

	SetBlendState();
	SetBlendStateForParticle();

	SetRasterizerState();
	ShaderCompile();
	MakePSO();
	MakePSOForParticle();

	ViewportScissor();
}

void GraphicsRenderer::InitializeDXC() {
	dxcUtils = nullptr;
	dxcCompiler = nullptr;
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));

	includeHandler = nullptr;
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
}
IDxcBlob* GraphicsRenderer::CompileShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtiles,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler
) {
	//1.hlslファイルを読む---------------------------------------------------------------------------------------------------------
	//これからシェーダーをコンパイルする旨をログに出す
	LogText(ConvertString(std::format(L"Begin CompilerShader, path:{}, profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
    hr = dxcUtiles->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//2.Compileする----------------------------------------------------------------------------------------------------------------
	LPCWSTR arguments[] = {
		filePath.c_str(),//コンパイル対象のhlslファイル名
		L"-E",L"main",//エントリーポイントの指定。基本的にmain以外にしない
		L"-T",profile,//ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",//デバッグ用の情報を埋め込む
		L"-Od",//最適化を外しておく
		L"-Zpr",//メモリレイアウトは最優先
	};
	//Shaderを実際にコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler,//includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	//3.警告・エラーがでていないか確認する----------------------------------------------------------------------
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		LogText(shaderError->GetStringPointer());
		//警告・エラーダメ絶対
		assert(false);
	}

	//4.Compile結果を受け取って返す----------------------------------------------------------------------------------------
	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	LogText(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;
}

void GraphicsRenderer::DecideCommand() {
	directX12->GetCommandList()->RSSetViewports(1, &viewport);
	directX12->GetCommandList()->RSSetScissorRects(1, &scissorRect);
	directX12->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	directX12->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());
}

void GraphicsRenderer::SetRootSignatureAndPSO(bool n) {
	if (n) {
	 //   directX12->GetCommandList()->SetGraphicsRootSignature(rootSignatureForParticle.Get());
		//directX12->GetCommandList()->SetPipelineState(graphicsPipelineStateForParticle.Get());
	}
	else {
		directX12->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		directX12->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());
	}
}


void GraphicsRenderer::MakeRootSignature() {
	descriptionRootSignature = {};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[0] = {};
	rootParameters[1] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[3].Descriptor.ShaderRegister = 1;

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[4].Descriptor.ShaderRegister = 2;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

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

	signatureBlob = nullptr;
	errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		LogText(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	rootSignature = nullptr;
	hr = directX12->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

void GraphicsRenderer::MakeRootSignatureForParticle() {
	//descriptionRootSignatureForParticle = {};
	//descriptionRootSignatureForParticle.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//descriptorRange[0].BaseShaderRegister = 0;
	//descriptorRange[0].NumDescriptors = 1;
	//descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	//descriptorRangeForInstancing[0].BaseShaderRegister = 0; //0から始まる
	//descriptorRangeForInstancing[0].NumDescriptors = 1;
	//descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //SRVを使う
	//descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//rootParametersForParticle[0] = {};
	//rootParametersForParticle[1] = {};
	//rootParametersForParticle[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParametersForParticle[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParametersForParticle[0].Descriptor.ShaderRegister = 0;

	//rootParametersForParticle[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootParametersForParticle[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	//rootParametersForParticle[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	//rootParametersForParticle[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	//		
	//rootParametersForParticle[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootParametersForParticle[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParametersForParticle[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	//rootParametersForParticle[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
	//		
	//rootParametersForParticle[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParametersForParticle[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParametersForParticle[3].Descriptor.ShaderRegister = 1;

	//rootParametersForParticle[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParametersForParticle[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParametersForParticle[4].Descriptor.ShaderRegister = 2;

	//descriptionRootSignatureForParticle.pParameters = rootParametersForParticle;
	//descriptionRootSignatureForParticle.NumParameters = _countof(rootParametersForParticle);

	//D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	//staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	//staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	//staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	//staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	//staticSamplers[0].ShaderRegister = 0;
	//staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//descriptionRootSignatureForParticle.pStaticSamplers = staticSamplers;
	//descriptionRootSignatureForParticle.NumStaticSamplers = _countof(staticSamplers);

	//signatureBlob = nullptr;
	//errorBlob = nullptr;
	//hr = D3D12SerializeRootSignature(&descriptionRootSignatureForParticle, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	//if (FAILED(hr)) {
	//	LogText(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	//	assert(false);
	//}
	//rootSignatureForParticle = nullptr;
	//hr = directX12->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignatureForParticle));
	//assert(SUCCEEDED(hr));
}

void GraphicsRenderer::SetInputLayout() {

	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc = {};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);
}
void GraphicsRenderer::SetBlendState() {
	blendDesc = {};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}

void GraphicsRenderer::SetBlendStateForParticle() {
	//blendDescForParticle = {};
	//blendDescForParticle.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//blendDescForParticle.RenderTarget[0].BlendEnable = true;
	//blendDescForParticle.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//blendDescForParticle.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	//blendDescForParticle.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	//blendDescForParticle.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	//blendDescForParticle.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//blendDescForParticle.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}


void GraphicsRenderer::SetRasterizerState() {
	rasterizerDesc = {};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}
void GraphicsRenderer::ShaderCompile() {
	vertexShaderBlob = CompileShader(L"Object3d.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(vertexShaderBlob != nullptr);
	pixelShaderBlob = CompileShader(L"Object3d.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(pixelShaderBlob != nullptr);
	
	//particleVertexShaderBlob = CompileShader(L"Particle.VS.hlsl", L"vs_6_0", dxcUtils, dxcCompiler, includeHandler);
	//assert(vertexShaderBlob != nullptr);
	//particlePixelShaderBlob = CompileShader(L"Particle.PS.hlsl", L"ps_6_0", dxcUtils, dxcCompiler, includeHandler);
	//assert(pixelShaderBlob != nullptr);
}
void GraphicsRenderer::MakePSO() {
	//PSOを生成する-----------------------------------------------------------------------------------------------
	graphicsPipelineStateDesc = {};
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
	graphicsPipelineStateDesc.DepthStencilState = directX12->GetDepthStencilDesc();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	graphicsPipelineState = nullptr;
	hr = directX12->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert((SUCCEEDED(hr)));
}
void GraphicsRenderer::MakePSOForParticle() {
	//PSOを生成する-----------------------------------------------------------------------------------------------
	/*graphicsPipelineStateDescForParticle = {};
	graphicsPipelineStateDescForParticle.pRootSignature = rootSignatureForParticle.Get();
	graphicsPipelineStateDescForParticle.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDescForParticle.VS = { particleVertexShaderBlob->GetBufferPointer(),particleVertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDescForParticle.PS = { particlePixelShaderBlob->GetBufferPointer(),particlePixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDescForParticle.BlendState = blendDescForParticle;
	graphicsPipelineStateDescForParticle.RasterizerState = rasterizerDesc;
	graphicsPipelineStateDescForParticle.NumRenderTargets = 1;
	graphicsPipelineStateDescForParticle.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDescForParticle.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDescForParticle.SampleDesc.Count = 1;
	graphicsPipelineStateDescForParticle.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipelineStateDescForParticle.DepthStencilState = directX12->GetDepthStencilDesc();
	graphicsPipelineStateDescForParticle.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	graphicsPipelineStateForParticle = nullptr;
	hr = directX12->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDescForParticle, IID_PPV_ARGS(&graphicsPipelineStateForParticle));
	assert((SUCCEEDED(hr)));*/
}

//これ
void GraphicsRenderer::MakeVertexResource() {
	//VertexResourceを生成する--------------------------------------------------------------------------------
	//頂点リソース用のヒープの作成の設定
	uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	vertexResourceDesc = {};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vector4) * 3;
	//バッファの場合はこれらは１にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	vertexResource = nullptr;
	hr = directX12->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));
}

//kore
void GraphicsRenderer::MakeVertexBufferView() {
	vertexBufferView = {};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 3;
	vertexBufferView.StrideInBytes = sizeof(Vector4);
}

//kore
void GraphicsRenderer::DateResource(Vector2* leftBot, Vector2* midTop, Vector2* rightBot) {
	vertexDate = nullptr;
	//wakeru
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexDate));
	//
	//LeftBottom
	vertexDate[0] = { leftBot->x,leftBot->y	,0.0f,1.0f };
	//MiddleTop
	vertexDate[1] = { midTop->x,midTop->y,0.0f,1.0f };
	//RightBottom
	vertexDate[2] = { rightBot->x,rightBot->y,0.0f,1.0f };
}
void GraphicsRenderer::ViewportScissor() {
	viewport = {};
	viewport.Width = float(kClientWidth);
	viewport.Height = float(kClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	scissorRect = {};
	scissorRect.left = 0;
	scissorRect.right = kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = kClientHeight;
}

void GraphicsRenderer::Release() {
	//vertexResource->Release();
	//graphicsPipelineState->Release();
	//signatureBlob->Release();
	//if (errorBlob) {
	//	errorBlob->Release();
	//}
	//rootSignature->Release();
	//pixelShaderBlob->Release();
	//vertexShaderBlob->Release();
}