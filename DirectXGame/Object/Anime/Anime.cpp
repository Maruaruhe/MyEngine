#include "Anime.h"
#include <assert.h>
#include "../../Base/GlobalVariables/GlobalVariables.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"
#include "../../Manager/TextureManager.h"
#include "../../Manager/ModelManager.h"

#include "../../Math/MathOperator.h"

void Anime::InitializePosition(const std::string& filename) {
	modelData = ModelManager::GetInstance()->GetModel(filename);
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
}

void Anime::Initialize(const std::string& filename) {

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	isParent = false;

	InitializePosition(filename);
	anime = LoadAnimationFile("Resources/" , filename + ".gltf");
	skelton = ModelManager::GetInstance()->CreateSkelton(modelData.rootNode);
	skinCluster = ModelManager::GetInstance()->CreateSkinCluster(skelton, modelData);

	CreateMaterialResource();
	CreateVertexBufferView();
	CreateTransformationMatrixResource();
	CreateIndexResource();

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

void Anime::ApplyGlobalVariables() {
	transform.translate = GlobalVariables::GetInstance()->GetVector3Value(forg, "Translate");
	transform.scale = GlobalVariables::GetInstance()->GetVector3Value(forg, "Scale");
	transform.rotate = GlobalVariables::GetInstance()->GetVector3Value(forg, "Rotate");
}

void Anime::Update() {
	DrawMatrix4x4("localMatrix", localMatrix);
	ApplyGlobalVariables();

	material->uvTransform = MakeIdentity4x4();

	worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	animationTime += 1.0f / 60.0f;
	animationTime = std::fmod(animationTime, anime.duration);

	ApplyAnimation(skelton, anime, animationTime);
	UpdateSkelton(skelton);
	UpdateSkinCluster(skinCluster,skelton);

	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		if (isParent) {
			worldMatrix = Multiply(worldMatrix, camera->cameraMatrix);
		}
		const Matrix4x4& viewprojectionMatrix = camera->viewProjectionMatrix;
		worldViewProjectionMatrix = worldMatrix * viewprojectionMatrix;
		//worldViewProjectionMatrix = skelton.joints[0].skeltonSpaceMatrix * worldMatrix * viewprojectionMatrix;
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}
	//transformationMatrix->WVP = Multiply(Multiply(modelData.rootNode.localMatrix , worldMatrix), camera->viewProjectionMatrix);
	//transformationMatrix->World = Multiply(modelData.rootNode.localMatrix, worldMatrix);
	transformationMatrix->WVP = worldViewProjectionMatrix;
	transformationMatrix->World = worldMatrix;
}

void Anime::Draw() {
	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	DirectX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);	//
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(false);

	//wvp用のCBufferの場所を設定
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	//koko
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, DirectX12::GetInstance()->GetSrvHandleGPU());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(UINT(modelData.indices.size()), 1, 0, 0, 0);

	for (Joint& joint : skelton.joints) {
	//	joint.sphere
	}
}

void Anime::UpdateAnimation() {
	NodeAnimation& rootNodeAnimation = anime.nodeAnimations[modelData.rootNode.name];
	Vector3 translate = CalculateValue(rootNodeAnimation.translate, animationTime);
	Quaternion rotate = CalculateValue(rootNodeAnimation.rotate, animationTime);
	Vector3 scale = CalculateValue(rootNodeAnimation.scale, animationTime);

	localMatrix = MakeAffineMatrix(scale, rotate, translate);
}

void Anime::UpdateSkelton(Skelton& skelton) {
	for (Joint& joint : skelton.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) {
			joint.skeltonSpaceMatrix = joint.localMatrix * skelton.joints[*joint.parent].skeltonSpaceMatrix;

		}
		else {
			joint.skeltonSpaceMatrix = joint.localMatrix;
		}
	}
}

void Anime::UpdateSkinCluster(SkinCluster& skinCluster, const Skelton& skelton) {
	for (size_t jointIndex = 0; jointIndex < skelton.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
		skinCluster.mappedPalette[jointIndex].skeltonSpaceMatrix = skinCluster.inverseBindPoseMatrices[jointIndex] * skelton.joints[jointIndex].skeltonSpaceMatrix;
		skinCluster.mappedPalette[jointIndex].skeltonSpaceInverseTransposeMatrix = Transpose(Inverse(skinCluster.mappedPalette[jointIndex].skeltonSpaceMatrix));
	}
}

void Anime::ApplyAnimation(Skelton& skelton, const Animation& animation, float animationTime) {
	for (Joint& joint : skelton.joints) {
		//定昇のAnimationがあれば、値の適用を行う。
		if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = CalculateValue(rootNodeAnimation.translate, animationTime);
			joint.transform.rotate = CalculateValue(rootNodeAnimation.rotate, animationTime);
			joint.transform.scale = CalculateValue(rootNodeAnimation.scale, animationTime);
		}
	}
}

void Anime::CreateVertexBufferView() {
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView = {};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Anime::CreateMaterialResource() {
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Material));
	material = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material));

	material->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material->enableLighting = false;
	material->enablePhong = false;
}

void Anime::CreateTransformationMatrixResource() {
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	wvpResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	transformationMatrix = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrix));
	//単位行列を書き込んでおく
	transformationMatrix->WVP = MakeIdentity4x4();
}

void Anime::CreateIndexResource() {
	indexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(uint32_t) * modelData.indices.size());
	indexBufferViewSprite.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferViewSprite.SizeInBytes = UINT(sizeof(uint32_t) * modelData.indices.size());
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* mappedIndex = nullptr;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex));
	std::memcpy(mappedIndex, modelData.indices.data(), sizeof(uint32_t) * modelData.indices.size());
}

MaterialData Anime::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
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

void Anime::DrawMatrix4x4(const char* title, const Matrix4x4& matrix) {
#ifdef _DEBUG
	ImGui::Begin(title);
	ImGui::Text("Matrix4x4:");

	for (int i = 0; i < 4; ++i) {
		ImGui::Text("%.3f, %.3f, %.3f, %.3f",
			matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
	}
	ImGui::End();
#else
	(void)title;
	(void)matrix;
#endif // DEBUG
}