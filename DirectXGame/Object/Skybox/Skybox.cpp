#include "Skybox.h"
#include "../../Base/Log/Log.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"

#include "../../Manager/TextureManager.h"

void Skybox::Initialize() {
	TextureManager::GetInstance()->LoadTexture("Resources/rostock_laage_airport_4k.dds");
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/rostock_laage_airport_4k.dds");

	CreateRootSignature();
	CreatePSO();

	CreateVertexBufferView();
	CreateIndexResource();
	CreateMaterialResource();
	CreateTransformationMatrixResource();

	// 右面。描画インデックスは [0, 1, 2][2, 1, 3]
	vertexData[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[1].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[2].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[3].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 2; indexData[4] = 1; indexData[5] = 3;

	// 左面。描画インデックスは [4, 5, 6][6, 5, 7]
	vertexData[4].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData[7].position = { -1.0f, -1.0f, 1.0f, 1.0f };

	indexData[6] = 4; indexData[7] = 5; indexData[8] = 6;
	indexData[9] = 6; indexData[10] = 5; indexData[11] = 7;

	// 前面。描画インデックスは [8, 9, 10][10, 9, 11]
	vertexData[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[10].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[11].position = { 1.0f, -1.0f, 1.0f, 1.0f };

	indexData[12] = 8; indexData[13] = 9; indexData[14] = 10;
	indexData[15] = 10; indexData[16] = 9; indexData[17] = 11;

	// 後面。描画インデックスは [12, 13, 14][14, 13, 15]
	vertexData[12].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[13].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };

	indexData[18] = 12; indexData[19] = 13; indexData[20] = 14;
	indexData[21] = 14; indexData[22] = 13; indexData[23] = 15;

	// 上面。描画インデックスは [16, 17, 18][18, 17, 19]
	vertexData[16].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[17].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[18].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[19].position = { 1.0f, 1.0f, 1.0f, 1.0f };

	indexData[24] = 16; indexData[25] = 17; indexData[26] = 18;
	indexData[27] = 18; indexData[28] = 17; indexData[29] = 19;

	// 下面。描画インデックスは [20, 21, 22][22, 21, 23]
	vertexData[20].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[21].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	indexData[30] = 20; indexData[31] = 21; indexData[32] = 22;
	indexData[33] = 22; indexData[34] = 21; indexData[35] = 23;

	transform = { {100.0f,100.0f,100.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
}
void Skybox::Update() {
	material->uvTransform = MakeIdentity4x4();

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		//if (isParent) {
		//	worldMatrix = Multiply(worldMatrix, camera->cameraMatrix);
		//}
		const Matrix4x4& viewprojectionMatrix = camera->viewProjectionMatrix;
		worldViewProjectionMatrix = worldMatrix * viewprojectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	transformationMatrix->WVP = worldViewProjectionMatrix;
	transformationMatrix->World = worldMatrix;
}
void Skybox::Draw() {
	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	DirectX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	DirectX12::GetInstance()->GetCommandList()->SetPipelineState(graphicsPipelineState.Get());

	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	//koko
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(kNumVertices_), 1, 0, 0, 0);
}

void Skybox::CreateVertexBufferView() {
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * kNumVertices_);

	vertexBufferView = {};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * kNumVertices_);
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//std::memcpy(vertexData, 0, sizeof(VertexData) * kNumVertices_);
}

void Skybox::CreateIndexResource() {
	indexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(uint32_t) * uint32_t(kNumIndices_));
	indexBufferView = {};
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * uint32_t(kNumIndices_);
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
}

void Skybox::CreateMaterialResource() {
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Material));
	material = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material));

	material->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Skybox::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
}

void Skybox::CreateRootSignature() {
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
	D3D12_ROOT_PARAMETER rootParameters[3] = {};
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

void Skybox::CreatePSO() {
	//InputLayoutの設定
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//Shader
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Skybox.VS.hlsl", L"vs_6_0");
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = GraphicsRenderer::GetInstance()->CompileShader(L"Skybox.PS.hlsl", L"ps_6_0");

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