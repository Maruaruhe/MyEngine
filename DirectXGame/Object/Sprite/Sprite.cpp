#include "Sprite.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"

void Sprite::Initialize(Vector2 leftTop, Vector2 rightBot, std::string textureFilePath) {
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	uvTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	isActive = true;
	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();
	CreateIndexResource();
	CreateVertexData(leftTop, rightBot);

	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
}

void Sprite::Update() {
	materialData_->uvTransform = MakeIdentity4x4();
	transformationMatrix->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(kCliantWidth), float(kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(transformationMatrix->World, Multiply(viewMatrix, projectionMatrix));
	transformationMatrix->WVP = worldViewProjectionMatrix;
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };

	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform.translate));
	materialData_->uvTransform = uvTransformMatrix;
}

void Sprite::Draw() {
	if (isActive) {
		DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
		DirectX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView);	//VBVを設定
		//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
		DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(false);

		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

		//wvp用のCBufferの場所を設定
		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

		DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));
		
		//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
		DirectX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
}

void Sprite::CreateVertexResource() {
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * 6);
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
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	materialData_->enableLighting = false;
}

void Sprite::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Matrix4x4));
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
	indexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(uint32_t) * 6);
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

void Sprite::CreateVertexData(Vector2 leftTop, Vector2 rightBot) {
	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左上									   
	vertexData[1].position = { leftTop.x, leftTop.y, 0.0f, 1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };
	//左下
	vertexData[0].position = { leftTop.x, rightBot.y, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };
	//右上
	vertexData[3].position = { rightBot.x,leftTop.y, 0.0f, 1.0f };
	vertexData[3].texcoord = { 1.0f, 0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };
	//右下								
	vertexData[2].position = { rightBot.x,rightBot.y, 0.0f, 1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
}

void Sprite::Release() {
}

void Sprite::SetPosition(Vector2 translate) {
	transform.translate.x = translate.x;
	transform.translate.y = translate.y;
}

void Sprite::SetTexcoord(Vector2 LT, Vector2 LB, Vector2 RT, Vector2 RB) {
	//左上									   
	vertexData[1].texcoord = { LT };
	//左下
	vertexData[0].texcoord = { LB };
	//右上
	vertexData[3].texcoord = { RT };
	//右下								
	vertexData[2].texcoord = { RB };
}