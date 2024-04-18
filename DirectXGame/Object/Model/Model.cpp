#include "Model.h"
#include <assert.h>
#include "../../Base/GlobalVariables/GlobalVariables.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"
#include "../../Manager/TextureManager.h"
#include "../../Manager/ModelManager.h"

void Model::InitializePosition(const std::string& filename) {
	modelData = ModelManager::GetInstance()->GetModel(filename);
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
}

void Model::Initialize(const std::string& filename) {

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	isParent = false;

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

	ApplyGlobalVariables();
	//


	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/uvChecker.png");
}

void Model::ApplyGlobalVariables() {
	transform.translate = GlobalVariables::GetInstance()->GetVector3Value(forg, "Translate");
	transform.scale = GlobalVariables::GetInstance()->GetVector3Value(forg, "Scale");
	transform.rotate = GlobalVariables::GetInstance()->GetVector3Value(forg, "Rotate");
}

void Model::Update() {
	material->uvTransform = MakeIdentity4x4();

	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		if (isParent) {
			worldMatrix = Multiply(worldMatrix, camera->cameraMatrix);
		}
		const Matrix4x4& viewprojectionMatrix = camera->viewProjectionMatrix;
		worldViewProjectionMatrix = Multiply(worldMatrix, viewprojectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	transformationMatrix->WVP = worldViewProjectionMatrix;
	transformationMatrix->World = worldMatrix;
}

void Model::Draw() {
	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(false);

	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	//koko
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, DirectX12::GetInstance()->GetSrvHandleGPU());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void Model::CreateVertexBufferView() {
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	
	vertexBufferView = {};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Model::CreateMaterialResource() {
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Material));
	material = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material));

	material->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material->enableLighting = false;
	material->enablePhong = false;
}

void Model::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	MaterialData materialData;
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;

			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}