#include "Model.h"
#include <assert.h>
#include "../../Base/GlobalVariables/GlobalVariables.h"

ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename + ".obj");
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				position.x *= -1;
				normal.x *= -1;
				texcoord.y = 1.0f - texcoord.y;
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };
			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;

			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
	return modelData;
}
void Model::InitializePosition(const std::string& filename) {
	modelData = LoadObjFile("Resources",filename);
	vertexResource = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView = {};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Model::Initialize(const std::string& filename) {
	directX12 = DirectX12::GetInstance();

	CreateMaterialResource();
	CreateTransformationMatrixResource();
	CreateDirectionalLightResource();

	/*const char* groupName = "Model";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->AddItem(groupName, "Translate", transform.translate);
	GlobalVariables::GetInstance()->AddItem(groupName, "Scale", transform.scale);
	GlobalVariables::GetInstance()->AddItem(groupName, "Rotate", transform.rotate);

	ApplyGlobalVariables();*/

	InitializePosition(filename);
}

void Model::ApplyGlobalVariables() {
	/*const char* groupName = "Model";
	transform.translate = GlobalVariables::GetInstance()->GetVector3Value(groupName, "Translate");
	transform.scale = GlobalVariables::GetInstance()->GetVector3Value(groupName, "Scale");
	transform.rotate = GlobalVariables::GetInstance()->GetVector3Value(groupName, "Rotate");*/
}

void Model::Update(Vector4& color, const Transform& transform, const Transform& cameraTransform, DirectionalLight& direcionalLight) {

	/*GlobalVariables::GetInstance()->SetValue("Model", "Translate", transform.translate);*/
	/*if (Input::GetInstance()->PushKey(DIK_S)) {
		transform.translate.z += 0.1f;
	}*/


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
}

void Model::Draw() {
	directX12->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	directX12->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX12->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	directX12->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	//koko
	directX12->GetCommandList()->SetGraphicsRootDescriptorTable(2, directX12->GetSrvHandleGPU2());
	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	directX12->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);
}

void Model::CreateMaterialResource() {
	materialResource_ = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	materialData_->enableLighting = false;
}

void Model::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(TransformationMatrix));
	//データを書き込む
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
}

void Model::CreateDirectionalLightResource() {
	directionalLightResource = directX12->CreateBufferResource(directX12->GetDevice(), sizeof(DirectionalLight));
	directionalLight_ = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight_));
}

void Model::Release() {
	//vertexResource->Release();
	//materialResource_->Release();
	//directionalLightResource->Release();
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