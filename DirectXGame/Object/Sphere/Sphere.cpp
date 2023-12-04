#include "Sphere.h"

void Sphere::InitializePosition() {
		const uint32_t kSubdivision = 16;
		const float kLonEvery = (2 * pi) / kSubdivision;
		const float kLatEvery = (pi) / kSubdivision;

		for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
			float lat = -pi / 2.0f + kLatEvery * latIndex;

			for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
				uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
				float lon = lonIndex * kLonEvery;

				//1
				//spData[start].first.Left_.position.x = cos(lat) * cos(lon);
				//spData[start].first.Left_.position.y = sin(lat);
				//spData[start].first.Left_.position.z = cos(lat) * sin(lon);
				//spData[start].first.Left_.position.w = 1.0f;

				//spData[start].first.Top_.position.x = cos(lat + kLatEvery) * cos(lon);
				//spData[start].first.Top_.position.y = sin(lat + kLatEvery);
				//spData[start].first.Top_.position.z = cos(lat + kLatEvery) * cos(lat + kLatEvery) * sin(lon);
				//spData[start].first.Top_.position.w = 1.0f;

				//spData[start].first.Right_.position.x = cos(lat) * cos(lon + kLonEvery);
				//spData[start].first.Right_.position.y = sin(lat);
				//spData[start].first.Right_.position.z = cos(lat) * sin(lon + kLonEvery);
				//spData[start].first.Right_.position.w = 1.0f;

				////2
				//spData[start].second.Left_.position.x = cos(lat + kLatEvery) * cos(lon);
				//spData[start].second.Left_.position.y = sin(lat + kLatEvery);
				//spData[start].second.Left_.position.z = cos(lat + kLatEvery) * cos(lat + kLatEvery) * sin(lon);
				//spData[start].second.Left_.position.w = 1.0f;

				//spData[start].second.Top_.position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
				//spData[start].second.Top_.position.y = sin(lat + kLatEvery);
				//spData[start].second.Top_.position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
				//spData[start].second.Top_.position.w = 1.0f;

				//spData[start].second.Right_.position.x = cos(lat) * cos(lon + kLonEvery);
				//spData[start].second.Right_.position.y = sin(lat);
				//spData[start].second.Right_.position.z = cos(lat) * sin(lon + kLonEvery);
				//spData[start].second.Right_.position.w = 1.0f;

				//
				vertexData[start].position.x = cos(lat) * cos(lon);
				vertexData[start].position.y = sin(lat);
				vertexData[start].position.z = cos(lat) * sin(lon);
				vertexData[start].position.w = 1.0f;

				vertexData[start].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertexData[start].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				vertexData[start].normal.x = vertexData[start].position.x;
				vertexData[start].normal.y = vertexData[start].position.y;
				vertexData[start].normal.z = vertexData[start].position.z;



				vertexData[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
				vertexData[start + 1].position.y = sin(lat + kLatEvery);
				vertexData[start + 1].position.z = cos(lat + kLatEvery) * sin(lon);
				vertexData[start + 1].position.w = 1.0f;

				vertexData[start + 1].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertexData[start + 1].texcoord.y = 1.0f - num - (float(latIndex) / float(kSubdivision));

				vertexData[start + 1].normal.x = vertexData[start + 1].position.x;
				vertexData[start + 1].normal.y = vertexData[start + 1].position.y;
				vertexData[start + 1].normal.z = vertexData[start + 1].position.z;


				vertexData[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
				vertexData[start + 2].position.y = sin(lat);
				vertexData[start + 2].position.z = cos(lat) * sin(lon + kLonEvery);
				vertexData[start + 2].position.w = 1.0f;

				vertexData[start + 2].texcoord.x = num + (float(lonIndex) / float(kSubdivision));
				vertexData[start + 2].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				vertexData[start + 2].normal.y = vertexData[start + 2].position.y;
				vertexData[start + 2].normal.z = vertexData[start + 2].position.z;
				vertexData[start + 2].normal.x = vertexData[start + 2].position.x;



				//2
				vertexData[start + 3].position.x = cos(lat + kLatEvery) * cos(lon);
				vertexData[start + 3].position.y = sin(lat + kLatEvery);
				vertexData[start + 3].position.z = cos(lat + kLatEvery) * sin(lon);
				vertexData[start + 3].position.w = 1.0f;

				vertexData[start + 3].texcoord.x = float(lonIndex) / float(kSubdivision);
				vertexData[start + 3].texcoord.y = 1.0f - num - (float(latIndex) / float(kSubdivision));

				vertexData[start + 3].normal.x = vertexData[start + 3].position.x;
				vertexData[start + 3].normal.y = vertexData[start + 3].position.y;
				vertexData[start + 3].normal.z = vertexData[start + 3].position.z;


				vertexData[start + 4].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
				vertexData[start + 4].position.y = sin(lat + kLatEvery);
				vertexData[start + 4].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
				vertexData[start + 4].position.w = 1.0f;

				vertexData[start + 4].texcoord.x = num + (float(lonIndex) / float(kSubdivision));
				vertexData[start + 4].texcoord.y = 1.0f - num - (float(latIndex) / float(kSubdivision));

				vertexData[start + 4].normal.x = vertexData[start + 4].position.x;
				vertexData[start + 4].normal.y = vertexData[start + 4].position.y;
				vertexData[start + 4].normal.z = vertexData[start + 4].position.z;


				vertexData[start + 5].position.x = cos(lat) * cos(lon + kLonEvery);
				vertexData[start + 5].position.y = sin(lat);
				vertexData[start + 5].position.z = cos(lat) * sin(lon + kLonEvery);
				vertexData[start + 5].position.w = 1.0f;

				vertexData[start + 5].texcoord.x = num + (float(lonIndex) / float(kSubdivision));
				vertexData[start + 5].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);

				vertexData[start + 5].normal.x = vertexData[start + 5].position.x;
				vertexData[start + 5].normal.y = vertexData[start + 5].position.y;
				vertexData[start + 5].normal.z = vertexData[start + 5].position.z;

			}
		}
}

void Sphere::Initialize(DirectX12* directX12) {
	directX12_ = directX12;
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
	CreateVertexResource();
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();
	CreateDirectionalLightResource();
	DataResource();

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	InitializePosition();
}

void Sphere::Update(Vector4& color, Transform& transform_,DirectionalLight& direcionalLight) {
	transform.translate = transform_.translate;
	transform.rotate = transform_.rotate;
	transform.scale = transform_.scale;
	materialData_->uvTransform = MakeIdentity4x4();
	transformationMatrix->World = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(transformationMatrix->World, Multiply(viewMatrix, projectionMatrix));
	transformationMatrix->WVP = worldViewProjectionMatrix;
	materialData_->color = color;
	directionalLight_->color = direcionalLight.color;
	directionalLight_->direction = direcionalLight.direction;
	directionalLight_->intensity = direcionalLight.intensity;

	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
}

void Sphere ::Draw() {
	directX12_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	directX12_->GetCommandList()->SetGraphicsRootDescriptorTable(2, useMonsterBall ? directX12_->GetSrvHandleGPU2() : directX12_->GetSrvHandleGPU());
	directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	directX12_->GetCommandList()->DrawInstanced(1536, 1, 0, 0);
}

void Sphere::CreateVertexResource() {
	vertexResource = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(VertexData) * 1536);
}

void Sphere::CreateVertexBufferView() {
	vertexBufferView = {};
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点３つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 1536;
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Sphere::CreateMaterialResource() {
	materialResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	materialData_->enableLighting = true;

	//materialResourceSprite = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(Material));
}

void Sphere::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(TransformationMatrix));
	//データを書き込む
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
}

void Sphere::CreateDirectionalLightResource() {
	directionalLightResource = directX12_->CreateBufferResource(directX12_->GetDevice(), sizeof(DirectionalLight));
	directionalLight_ = nullptr;
	directionalLightResource->Map(0,nullptr, reinterpret_cast<void**>(&directionalLight_));
}

void Sphere::DataResource() {
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

void Sphere::Release() {
	/*vertexResource->Release();
	materialResource_->Release();
	directionalLightResource->Release();*/
}