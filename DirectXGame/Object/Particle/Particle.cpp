#include "Particle.h"
#include <assert.h>
#include "../../Base/GlobalVariables/GlobalVariables.h"
#include "../../Manager/TextureManager.h"
#include "../../Manager/ModelManager.h"

#define INSTANCECOUNT 10

void Particle::InitializePosition(const std::string& filename) {
	modelData = ModelManager::GetInstance()->GetModel(filename);
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(VertexData) * modelData.vertices.size());
}

void Particle::Initialize(const std::string& filename) {

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	InitializePosition(filename);
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();

	//GlobalVariables
	forg = filename;
	GlobalVariables::GetInstance()->CreateGroup(forg);
	GlobalVariables::GetInstance()->AddItem(forg, "Translate", transform.translate);
	GlobalVariables::GetInstance()->AddItem(forg, "Scale", transform.scale);
	GlobalVariables::GetInstance()->AddItem(forg, "Rotate", transform.rotate);
	//


	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/uvChecker.png");
}

void Particle::ApplyGlobalVariables() {
	transform.translate = GlobalVariables::GetInstance()->GetVector3Value(forg, "Translate");
	transform.scale = GlobalVariables::GetInstance()->GetVector3Value(forg, "Scale");
	transform.rotate = GlobalVariables::GetInstance()->GetVector3Value(forg, "Rotate");
}

void Particle::Update() {
	ApplyGlobalVariables();

	material->uvTransform = MakeIdentity4x4();

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewprojectionMatrix = camera->viewProjectionMatrix;
		worldViewProjectionMatrix = Multiply(worldMatrix, viewprojectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	transformationMatrix->WVP = worldViewProjectionMatrix;
	transformationMatrix->World = worldMatrix;
}

void Particle::Draw() {
	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	//koko
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, DirectX12::GetInstance()->GetSrvHandleGPU());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), INSTANCECOUNT, 0, 0);
}

void Particle::SetModel(const std::string& filePath) {

}

void Particle::CreateVertexBufferView() {
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView = {};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Particle::CreateMaterialResource() {
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(Material));
	material = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material));

	material->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material->enableLighting = false;
	material->enablePhong = false;
}

void Particle::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(TransformationMatrix) * INSTANCECOUNT);
	//データを書き込む
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	for (uint32_t index = 0; index < INSTANCECOUNT; ++index) {
		transformationMatrix[index].WVP = MakeIdentity4x4();
		transformationMatrix[index].World = MakeIdentity4x4();
	}
}

void Particle::CreateSRV() {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = INSTANCECOUNT;
	srvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU = DirectX12::GetInstance()->GetCPUDescriptorHandle(3);
	D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU = DirectX12::GetInstance()->GetGPUDescriptorHandle(3);

	DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(wvpResource_.Get(), &srvDesc, srvHandleCPU);
}