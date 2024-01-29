#include "Sprite.h"

void Sprite::Initialize(Vector3 leftTop, Vector3 rightBot) {
	directX12 = DirectX12::GetInstance();
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	uvTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	isInvisible_ = false;
	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();
	CreateIndexResource();
	DataResource();

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左上									   
	vertexData[1].position = { leftTop.x, leftTop.y, leftTop.z, 0 };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };
	//左下
	vertexData[0].position = { leftTop.x, rightBot.y, leftTop.z, 0 };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };
	//右上
	vertexData[3].position = { rightBot.x,leftTop.y,rightBot.z,0 };
	vertexData[3].texcoord = { 1.0f, 0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };
	//右下								
	vertexData[2].position = { rightBot.x,rightBot.y,rightBot.z,0 };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
}

void Sprite::Update(Vector4& color, const Transform& transform_) {
	/*ImGui::DragFloat2("uvTranslate", &uvTransform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("uvScale", &uvTransform.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("ucRotate", &uvTransform.rotate.z);*/

	transform.translate = transform_.translate;
	transform.rotate = transform_.rotate;
	transform.scale = transform_.scale;
	materialData_->uvTransform = MakeIdentity4x4();
	transformationMatrix->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(kCliantWidth), float(kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(transformationMatrix->World, Multiply(viewMatrix, projectionMatrix));
	transformationMatrix->WVP = worldViewProjectionMatrix;
	materialData_->color = color;

	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform.translate));
	materialData_->uvTransform = uvTransformMatrix;
}

void Sprite::Draw() {
	if (!isInvisible_) {
		directX12->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
		directX12->GetCommandList()->IASetIndexBuffer(&indexBufferView);	//VBVを設定
		//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
		directX12->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		directX12->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
		//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
		//wvp用のCBufferの場所を設定
		directX12->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

		directX12->GetCommandList()->SetGraphicsRootDescriptorTable(2, directX12->GetSrvHandleGPU());
		//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		//directX12_->GetCommandList()->DrawInstanced(6, 1, 0, 0);
		directX12->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
}

void Sprite::CreateVertexResource() {
	vertexResource = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(VertexData) * 6);
}

void Sprite::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Sprite::CreateMaterialResource() {
	materialResource_ = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	materialData_->enableLighting = false;
}

void Sprite::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(Matrix4x4));
	//データを書き込む
	//wvpData = nullptr;
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	//wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	//*wvpData = MakeIdentity4x4();
	transformationMatrix->WVP = MakeIdentity4x4();
}

void Sprite::CreateIndexResource() {
	indexResource = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(uint32_t) * 6);
	indexBufferView = {};
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;
}

void Sprite::DataResource() {
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Sprite::Release() {
}