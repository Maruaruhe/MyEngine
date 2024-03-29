#include "Triangle.h"

void Triangle::Initialize(TriangleData triangleData) {
	directX12 = DirectX12::GetInstance();
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();
	DataResource();

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = triangleData.Left_.position;
	vertexData[0].texcoord = triangleData.Left_.texcoord;
	//上
	vertexData[1].position = triangleData.Top_.position;
	vertexData[1].texcoord = triangleData.Top_.texcoord;
	//右下
	vertexData[2].position = triangleData.Right_.position;
	vertexData[2].texcoord = triangleData.Right_.texcoord;
}

void Triangle::Update(Vector4& color,Transform& transform_) {
	transform.translate = transform_.translate;
	transform.rotate = transform_.rotate;
	transform.scale = transform_.scale;
	transformationMatrix->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(transformationMatrix->World, Multiply(viewMatrix, projectionMatrix));
	transformationMatrix->WVP = worldViewProjectionMatrix;
	materialData_->color = color;
}

void Triangle::Draw() {
	directX12->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	directX12->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	directX12->GetCommandList()->SetGraphicsRootDescriptorTable(2, directX12->GetSrvHandleGPU());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	directX12->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void Triangle::CreateVertexResource() {
	vertexResource = directX12->CreateBufferResource(sizeof(VertexData) * 6);
}

void Triangle::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Triangle::CreateMaterialResource() {
	materialResource_ = directX12->CreateBufferResource(sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	materialData_->enableLighting = false;
}

void Triangle::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = directX12->CreateBufferResource(sizeof(Matrix4x4));
	//データを書き込む
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
}

void Triangle::DataResource() {
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Triangle::Release() {
	//vertexResource->Release();
	//materialResource_->Release();
}