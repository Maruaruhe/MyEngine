#include "Particle.h"
#include <assert.h>
#include "../../Base/GlobalVariables/GlobalVariables.h"
#include "../../Manager/TextureManager.h"
#include "../../Manager/ModelManager.h"

#include <numbers>

void Particle::InitializePosition(const std::string& filename) {
	modelData = ModelManager::GetInstance()->GetModel(filename);
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(VertexData) * modelData.vertices.size());
}

void Particle::Initialize(const std::string& filename) {
	InitializePosition(filename);
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateInstance();
	CreateSRV();

	emitter.count = 3;
	emitter.frequency = 0.5f;
	emitter.frequencyTime = 0.0f;

	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath("Resources/uvChecker.png");
}

void Particle::Update() {
	material->uvTransform = MakeIdentity4x4();

	if (camera) {

		Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
		Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, camera->cameraMatrix);
		billboardMatrix.m[3][0] = 0.0f;
		billboardMatrix.m[3][1] = 0.0f;
		billboardMatrix.m[3][2] = 0.0f;


		uint32_t numInstance = 0;

		for (uint32_t index = 0; index < kNumInstance; ++index) {
			/// if (particles[index].liftTime <= particles[index].currentTime) {
			//	continue;
			//} /

			particles[index].transform.translate += particles[index].velocity;
			particles[index].currentTime += 0.01f;

			Matrix4x4 scaleMatrix = MakeScaleMatrix(particles[index].transform.scale);
			Matrix4x4 translateMatrix = MakeTranslateMatrix(particles[index].transform.translate);

			Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->viewMatrix, camera->projectionMatrix));

			instancingData[index].WVP = worldViewProjectionMatrix;
			instancingData[index].World = worldMatrix;
			//instancingData[index].color = particles[index].color;


			++numInstance;
		}
	}
	else {

	}
}

void Particle::Draw() {
	DirectX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);	//VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけばよい
	DirectX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());

	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	//DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(true);
	//koko
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, DirectX12::GetInstance()->GetSrvHandleGPU());
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), kNumInstance, 0, 0);
}

ParticleInfo Particle::MakeNewParticle() {
	Scope scope = { -0.01f,0.01f };
	Vector3 r = RandomGenerator::GetInstance()->getRandom({ scope, scope, scope });

	ParticleInfo particleInfo;
	particleInfo.transform.translate = r;
	particleInfo.transform.scale = { 1.0f,1.0f,1.0f };
	particleInfo.transform.rotate = {};
	particleInfo.velocity = r;

	Scope cScope = { 0.0f,1.00f };
	Vector3 cR = RandomGenerator::GetInstance()->getRandom({ scope, scope, scope });

	particleInfo.color = { cR.x,cR.y,cR.z,1.0f };

	Scope lScope = { 1.0f,3.0f };
	float randomLife = RandomGenerator::GetInstance()->getRandom(lScope);
	particleInfo.liftTime = randomLife;
	particleInfo.currentTime = 0.0f;

	return particleInfo;
}

std::list<ParticleInfo> Particle::Emit(const Emitter& emitter){

	std::list<ParticleInfo> particles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle());
	}
	return particles;
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

void Particle::CreateInstance() {
	instancingResource = DirectX12::GetInstance()->CreateBufferResource(DirectX12::GetInstance()->GetDevice(), sizeof(ParticleForGPU) * kNumInstance);

	instancingData = nullptr;

	instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

	for (uint32_t index = 0; index < kNumInstance; ++index) {
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].World = MakeIdentity4x4();
		instancingData[index].color = { 1.0f,1.0f,1.0f,1.0f };

		Scope cScope = { 0.0f,1.00f };
		Vector3 cR = RandomGenerator::GetInstance()->getRandom({ cScope,cScope ,cScope });

		instancingData[index].color = { cR.x,cR.y,cR.z,1.0f };
	}

	for (uint32_t index = 0; index < kNumInstance; ++index) {
		particles[index].transform.scale = { 1.0f,1.0f,1.0f };
		particles[index].transform.rotate = { 0.0f,0.0f,0.0f };
		//transforms[index].transform.translate = { index 0.1f,index * 0.1f ,index * 0.1f };

		Scope scope = { -0.01f,0.01f };
		Vector3 r = RandomGenerator::GetInstance()->getRandom({ scope, scope, scope });
		particles[index].velocity += r;
	}
}

void Particle::CreateSRV() {
	descriptorSizeSRV = DirectX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	instancingSrvHandleCPU = DirectX12::GetInstance()->GetCPUDescriptorHandle(3);
	instancingSrvHandleGPU = DirectX12::GetInstance()->GetGPUDescriptorHandle(3);
	DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
}

MaterialData Particle::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
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