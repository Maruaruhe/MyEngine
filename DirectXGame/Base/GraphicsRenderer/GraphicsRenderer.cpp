#include "GraphicsRenderer.h"
#include <assert.h>

using namespace MyEngine;

GraphicsRenderer* GraphicsRenderer::GetInstance() {
	static GraphicsRenderer instance;

	return &instance;
}


void GraphicsRenderer::Initialize() {
	directX12_ = DirectX12::GetInstance();
	InitializeDXC();
	MakeRootSignature();
	MakeRootSignatureForParticle();
	MakeRootSignatureForSkinning();

	//InputElement
	SetInputLayout();
	SetInputLayoutForSkinning();

	SetBlendState();
	SetBlendStateForParticle();

	SetRasterizerState();
	ShaderCompile();

	MakePSO();
	MakePSOForParticle();
	MakePSOForSkinning();

	ViewportScissor();
}

void GraphicsRenderer::InitializeDXC() {
	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;
	hr_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr_));
	hr_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr_));

	includeHandler_ = nullptr;
	hr_ = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr_));
}
IDxcBlob* GraphicsRenderer::CompileShader(
	const std::wstring& filePath,
	const wchar_t* profile
) {
	//1.hlslファイルを読む---------------------------------------------------------------------------------------------------------
	//これからシェーダーをコンパイルする旨をログに出す
	LogText(ConvertString(std::format(L"Begin CompilerShader, path:{}, profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
    hr_ = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr_));
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
	hr_ = dxcCompiler_->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		includeHandler_,//includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr_));

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
	hr_ = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr_));
	//成功したログを出す
	LogText(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;
}

void GraphicsRenderer::DecideCommand() {
	directX12_->GetCommandList()->RSSetViewports(1, &viewport_);
	directX12_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);
	directX12_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
	directX12_->GetCommandList()->SetPipelineState(graphicsPipelineState_.Get());
}

void GraphicsRenderer::SetRootSignatureAndPSO(int n) {
	if (n == PARTICLE) {
	    directX12_->GetCommandList()->SetGraphicsRootSignature(rootSignatureForParticle_.Get());
		directX12_->GetCommandList()->SetPipelineState(graphicsPipelineStateForParticle_.Get());
	}
	else if(n == MODEL) {
		directX12_->GetCommandList()->SetGraphicsRootSignature(rootSignature_.Get());
		directX12_->GetCommandList()->SetPipelineState(graphicsPipelineState_.Get());
	}
	else if (n == ANIME) {
		directX12_->GetCommandList()->SetGraphicsRootSignature(rootSignatureForSkinning_.Get());
		directX12_->GetCommandList()->SetPipelineState(graphicsPipelineStateForSkinning_.Get());
	}
	else if (n == SPRITE) {
		directX12_->GetCommandList()->SetGraphicsRootSignature(rootSignatureForSprite_.Get());
		directX12_->GetCommandList()->SetPipelineState(graphicsPipelineStateForSprite_.Get());
	}
}


void GraphicsRenderer::MakeRootSignature() {
	descriptionRootSignature_ = {};
	descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeEnvTexture[1] = {};
	descriptorRangeEnvTexture[0].BaseShaderRegister = 1;
	descriptorRangeEnvTexture[0].NumDescriptors = 1;
	descriptorRangeEnvTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeEnvTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters_[0] = {};
	rootParameters_[1] = {};
	rootParameters_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[0].Descriptor.ShaderRegister = 0;

	rootParameters_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters_[1].Descriptor.ShaderRegister = 0;

	rootParameters_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	
	rootParameters_[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CAmera
	rootParameters_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[3].Descriptor.ShaderRegister = 1;

	//Light
	rootParameters_[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[4].Descriptor.ShaderRegister = 2;

	//SpotLight
	rootParameters_[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters_[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[5].Descriptor.ShaderRegister = 3;

	rootParameters_[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters_[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters_[6].DescriptorTable.pDescriptorRanges = descriptorRangeEnvTexture;
	rootParameters_[6].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeEnvTexture);

	descriptionRootSignature_.pParameters = rootParameters_;
	descriptionRootSignature_.NumParameters = _countof(rootParameters_);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);

	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr_)) {
		LogText(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}
	rootSignature_ = nullptr;
	hr_ = directX12_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr_));
}

void GraphicsRenderer::MakeRootSignatureForParticle() {
	descriptionRootSignatureForParticle_ = {};
	descriptionRootSignatureForParticle_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParametersForParticle_[0] = {};
	rootParametersForParticle_[1] = {};
	rootParametersForParticle_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForParticle_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForParticle_[0].Descriptor.ShaderRegister = 0;

	rootParametersForParticle_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersForParticle_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParametersForParticle_[1].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParametersForParticle_[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
			
	rootParametersForParticle_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersForParticle_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForParticle_[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParametersForParticle_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
			
	//rootParametersForParticle[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParametersForParticle[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParametersForParticle[3].Descriptor.ShaderRegister = 1;

	//rootParametersForParticle[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParametersForParticle[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParametersForParticle[4].Descriptor.ShaderRegister = 2;

	descriptionRootSignatureForParticle_.pParameters = rootParametersForParticle_;
	descriptionRootSignatureForParticle_.NumParameters = _countof(rootParametersForParticle_);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignatureForParticle_.pStaticSamplers = staticSamplers;
	descriptionRootSignatureForParticle_.NumStaticSamplers = _countof(staticSamplers);

	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignatureForParticle_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr_)) {
		LogText(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}
	rootSignatureForParticle_ = nullptr;
	hr_ = directX12_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignatureForParticle_));
	assert(SUCCEEDED(hr_));
}

void GraphicsRenderer::MakeRootSignatureForSkinning() {
	//RootSignature生成
	descriptionRootSignatureForSkinning_ = {};
	descriptionRootSignatureForSkinning_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeTexture[1] = {};
	descriptorRangeTexture[0].BaseShaderRegister = 1;
	descriptorRangeTexture[0].NumDescriptors = 1;
	descriptorRangeTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParametersForSkinning_[0] = {};
	rootParametersForSkinning_[1] = {};

	rootParametersForSkinning_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSkinning_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSkinning_[0].Descriptor.ShaderRegister = 0;

	rootParametersForSkinning_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSkinning_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParametersForSkinning_[1].Descriptor.ShaderRegister = 0;

	rootParametersForSkinning_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersForSkinning_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSkinning_[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParametersForSkinning_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	rootParametersForSkinning_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSkinning_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSkinning_[3].Descriptor.ShaderRegister = 1;
							
	rootParametersForSkinning_[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSkinning_[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSkinning_[4].Descriptor.ShaderRegister = 2;

	rootParametersForSkinning_[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSkinning_[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSkinning_[5].Descriptor.ShaderRegister = 3;

	rootParametersForSkinning_[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersForSkinning_[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParametersForSkinning_[6].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParametersForSkinning_[6].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	rootParametersForSkinning_[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersForSkinning_[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSkinning_[7].DescriptorTable.pDescriptorRanges = descriptorRangeTexture;
	rootParametersForSkinning_[7].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeTexture);

	descriptionRootSignatureForSkinning_.pParameters = rootParametersForSkinning_;
	descriptionRootSignatureForSkinning_.NumParameters = _countof(rootParametersForSkinning_);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignatureForSkinning_.pStaticSamplers = staticSamplers;
	descriptionRootSignatureForSkinning_.NumStaticSamplers = _countof(staticSamplers);

	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignatureForSkinning_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr_)) {
		LogText(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}
	rootSignatureForSkinning_ = nullptr;
	hr_ = directX12_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignatureForSkinning_));
	assert(SUCCEEDED(hr_));
}

void GraphicsRenderer::MakeRootSignatureForSprite() {
	descriptionRootSignatureForSprite_ = {};
	descriptionRootSignatureForSprite_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeEnvTexture[1] = {};
	descriptorRangeEnvTexture[0].BaseShaderRegister = 1;
	descriptorRangeEnvTexture[0].NumDescriptors = 1;
	descriptorRangeEnvTexture[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeEnvTexture[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParametersForSprite_[0] = {};
	rootParametersForSprite_[1] = {};
	rootParametersForSprite_[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSprite_[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSprite_[0].Descriptor.ShaderRegister = 0;

	rootParametersForSprite_[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSprite_[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParametersForSprite_[1].Descriptor.ShaderRegister = 0;

	rootParametersForSprite_[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersForSprite_[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSprite_[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParametersForSprite_[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	//CAmera
	rootParametersForSprite_[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSprite_[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSprite_[3].Descriptor.ShaderRegister = 1;

	//Light
	rootParametersForSprite_[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSprite_[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSprite_[4].Descriptor.ShaderRegister = 2;

	//SpotLight
	rootParametersForSprite_[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParametersForSprite_[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSprite_[5].Descriptor.ShaderRegister = 3;

	rootParametersForSprite_[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersForSprite_[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParametersForSprite_[6].DescriptorTable.pDescriptorRanges = descriptorRangeEnvTexture;
	rootParametersForSprite_[6].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeEnvTexture);

	descriptionRootSignatureForSprite_.pParameters = rootParametersForSprite_;
	descriptionRootSignatureForSprite_.NumParameters = _countof(rootParametersForSprite_);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignatureForSprite_.pStaticSamplers = staticSamplers;
	descriptionRootSignatureForSprite_.NumStaticSamplers = _countof(staticSamplers);

	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignatureForSprite_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr_)) {
		LogText(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}
	rootSignatureForSprite_ = nullptr;
	hr_ = directX12_->GetDevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignatureForSprite_));
	assert(SUCCEEDED(hr_));
}

void GraphicsRenderer::SetInputLayout() {
	inputElementDescs_[0].SemanticName = "POSITION";
	inputElementDescs_[0].SemanticIndex = 0;
	inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[1].SemanticName = "TEXCOORD";
	inputElementDescs_[1].SemanticIndex = 0;
	inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs_[2].SemanticName = "NORMAL";
	inputElementDescs_[2].SemanticIndex = 0;
	inputElementDescs_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc_ = {};
	inputLayoutDesc_.pInputElementDescs = inputElementDescs_;
	inputLayoutDesc_.NumElements = _countof(inputElementDescs_);
}

void GraphicsRenderer::SetInputLayoutForSkinning() {
	inputElementDescsForSkinning_[0].SemanticName = "POSITION";
	inputElementDescsForSkinning_[0].SemanticIndex = 0;
	inputElementDescsForSkinning_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescsForSkinning_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescsForSkinning_[1].SemanticName = "TEXCOORD";
	inputElementDescsForSkinning_[1].SemanticIndex = 0;
	inputElementDescsForSkinning_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescsForSkinning_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescsForSkinning_[2].SemanticName = "NORMAL";
	inputElementDescsForSkinning_[2].SemanticIndex = 0;
	inputElementDescsForSkinning_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescsForSkinning_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescsForSkinning_[3].SemanticName = "WEIGHT";
	inputElementDescsForSkinning_[3].SemanticIndex = 0;
	inputElementDescsForSkinning_[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescsForSkinning_[3].InputSlot = 1;
	inputElementDescsForSkinning_[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescsForSkinning_[4].SemanticName = "INDEX";
	inputElementDescsForSkinning_[4].SemanticIndex = 0;
	inputElementDescsForSkinning_[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;
	inputElementDescsForSkinning_[4].InputSlot = 1;
	inputElementDescsForSkinning_[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDescForSkinning_ = {};
	inputLayoutDescForSkinning_.pInputElementDescs = inputElementDescsForSkinning_;
	inputLayoutDescForSkinning_.NumElements = _countof(inputElementDescsForSkinning_);
}

void GraphicsRenderer::SetBlendState() {
	blendDesc_ = {};
	blendDesc_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc_.RenderTarget[0].BlendEnable = TRUE;
	blendDesc_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}

void GraphicsRenderer::SetBlendStateForParticle() {
	blendDescForParticle_ = {};
	blendDescForParticle_.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDescForParticle_.RenderTarget[0].BlendEnable = true;
	blendDescForParticle_.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDescForParticle_.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDescForParticle_.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDescForParticle_.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDescForParticle_.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDescForParticle_.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}


void GraphicsRenderer::SetRasterizerState() {
	rasterizerDesc_ = {};
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

void GraphicsRenderer::ShaderCompile() {
	vertexShaderBlob_ = CompileShader(L"Resources/Shader/Object3d.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);
	pixelShaderBlob_ = CompileShader(L"Resources/Shader/Object3d.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);
	
	particleVertexShaderBlob_ = CompileShader(L"Resources/Shader/Particle.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob_ != nullptr);
	particlePixelShaderBlob_ = CompileShader(L"Resources/Shader/Particle.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob_ != nullptr);

	skinningVertexShaderBlob_ = CompileShader(L"Resources/Shader/SkinningObject3D.VS.hlsl", L"vs_6_0");
	assert(skinningVertexShaderBlob_ != nullptr);
}
void GraphicsRenderer::MakePSO() {
	//PSOを生成する-----------------------------------------------------------------------------------------------
	graphicsPipelineStateDesc_ = {};
	graphicsPipelineStateDesc_.pRootSignature = rootSignature_.Get();
	graphicsPipelineStateDesc_.InputLayout = inputLayoutDesc_;
	graphicsPipelineStateDesc_.VS = { vertexShaderBlob_->GetBufferPointer(),vertexShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDesc_.PS = { pixelShaderBlob_->GetBufferPointer(),pixelShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDesc_.BlendState = blendDesc_;
	graphicsPipelineStateDesc_.RasterizerState = rasterizerDesc_;
	graphicsPipelineStateDesc_.NumRenderTargets = 1;
	graphicsPipelineStateDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc_.SampleDesc.Count = 1;
	graphicsPipelineStateDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipelineStateDesc_.DepthStencilState = directX12_->GetDepthStencilDesc();
	graphicsPipelineStateDesc_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	graphicsPipelineState_ = nullptr;
	hr_ = directX12_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc_, IID_PPV_ARGS(&graphicsPipelineState_));
	assert((SUCCEEDED(hr_)));
}
void GraphicsRenderer::MakePSOForParticle() {
	//PSOを生成する-----------------------------------------------------------------------------------------------
	graphicsPipelineStateDescForParticle_ = {};
	graphicsPipelineStateDescForParticle_.pRootSignature = rootSignatureForParticle_.Get();
	graphicsPipelineStateDescForParticle_.InputLayout = inputLayoutDesc_;
	graphicsPipelineStateDescForParticle_.VS = { particleVertexShaderBlob_->GetBufferPointer(),particleVertexShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDescForParticle_.PS = { particlePixelShaderBlob_->GetBufferPointer(),particlePixelShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDescForParticle_.BlendState = blendDescForParticle_;
	graphicsPipelineStateDescForParticle_.RasterizerState = rasterizerDesc_;
	graphicsPipelineStateDescForParticle_.NumRenderTargets = 1;
	graphicsPipelineStateDescForParticle_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDescForParticle_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDescForParticle_.SampleDesc.Count = 1;
	graphicsPipelineStateDescForParticle_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipelineStateDescForParticle_.DepthStencilState = directX12_->GetDepthStencilDescForParticle();
	graphicsPipelineStateDescForParticle_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	graphicsPipelineStateForParticle_ = nullptr;
	hr_ = directX12_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDescForParticle_, IID_PPV_ARGS(&graphicsPipelineStateForParticle_));
	assert((SUCCEEDED(hr_)));
}
void GraphicsRenderer::MakePSOForSkinning() {
	//PSOを生成する-----------------------------------------------------------------------------------------------
	graphicsPipelineStateDescForSkinning_ = {};
	graphicsPipelineStateDescForSkinning_.pRootSignature = rootSignatureForSkinning_.Get();
	graphicsPipelineStateDescForSkinning_.InputLayout = inputLayoutDescForSkinning_;
	graphicsPipelineStateDescForSkinning_.VS = { skinningVertexShaderBlob_->GetBufferPointer(),skinningVertexShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDescForSkinning_.PS = { pixelShaderBlob_->GetBufferPointer(),pixelShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDescForSkinning_.BlendState = blendDesc_;
	graphicsPipelineStateDescForSkinning_.RasterizerState = rasterizerDesc_;
	graphicsPipelineStateDescForSkinning_.NumRenderTargets = 1;
	graphicsPipelineStateDescForSkinning_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDescForSkinning_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDescForSkinning_.SampleDesc.Count = 1;
	graphicsPipelineStateDescForSkinning_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipelineStateDescForSkinning_.DepthStencilState = directX12_->GetDepthStencilDesc();
	graphicsPipelineStateDescForSkinning_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	graphicsPipelineStateForSkinning_ = nullptr;
	hr_ = directX12_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDescForSkinning_, IID_PPV_ARGS(&graphicsPipelineStateForSkinning_));
	assert((SUCCEEDED(hr_)));
}

void GraphicsRenderer::ViewportScissor() {
	viewport_ = {};
	viewport_.Width = float(kClientWidth);
	viewport_.Height = float(kClientHeight);
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	scissorRect_ = {};
	scissorRect_.left = 0;
	scissorRect_.right = kClientWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = kClientHeight;
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