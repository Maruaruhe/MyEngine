#include "Particle.h"
#include <assert.h>
#include "../../Base/GlobalVariables/GlobalVariables.h"
#include "../../Base/GraphicsRenderer/GraphicsRenderer.h"
#include "../../Manager/TextureManager.h"
#include "../../Manager/ModelManager.h"

#include <numbers>

void Particle::InitializePosition() {
	modelData = ModelManager::GetInstance()->GetModel("plane");
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
}

void Particle::Initialize(const std::string& filename) {
	InitializePosition();
	CreateMaterialResource();
	CreateVertexBufferView();
	CreateInstance();
	CreateSRV();

	emitter.count = 3;
	emitter.frequency = 60.0f;
	emitter.frequencyTime = 0.0f;

	TextureManager::GetInstance()->LoadTexture(filename);
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(filename);
}

void Particle::Update() {
	material->uvTransform = MakeIdentity4x4();

	ImGui::Begin("Particle");
	if (ImGui::Button("Add Particle")) {
		particles.splice(particles.end(), Emit(emitter));
	}
	ImGui::DragFloat3("EmitterTranslate", &emitter.transform.translate.x, 0.01f, -100.0f, 100.0f);
	ImGui::Text("ListSize %d", particles.size());
	ImGui::Text("instanceNum %d", numInstance);
	ImGui::End();

	emitter.frequencyTime += 1.0f;
	if (emitter.frequency <= emitter.frequencyTime) {
		particles.splice(particles.end(), Emit(emitter));
		emitter.frequencyTime -= emitter.frequency;
	}

	if (camera) {

		Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
		Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, camera->cameraMatrix);
		billboardMatrix.m[3][0] = 0.0f;
		billboardMatrix.m[3][1] = 0.0f;
		billboardMatrix.m[3][2] = 0.0f;


		numInstance = 0;

		for (std::list<ParticleInfo>::iterator particleIterator = particles.begin(); particleIterator != particles.end();) {
			if ((*particleIterator).liftTime <= (*particleIterator).currentTime) {
				particleIterator = particles.erase(particleIterator);
				continue;
			}


			if (numInstance < kNumInstance) {
				(*particleIterator).transform.translate += (*particleIterator).velocity;
				(*particleIterator).currentTime += 0.1f;

				float alpha = 1.0f - (*particleIterator).currentTime / (*particleIterator).liftTime;
				(*particleIterator).color.w = alpha;

				Matrix4x4 scaleMatrix = MakeScaleMatrix((*particleIterator).transform.scale);
				Matrix4x4 translateMatrix = MakeTranslateMatrix((*particleIterator).transform.translate);

				Matrix4x4 worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
				Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->viewMatrix, camera->projectionMatrix));

				instancingData[numInstance].WVP = worldViewProjectionMatrix;
				instancingData[numInstance].World = worldMatrix;

				instancingData[numInstance].color = (*particleIterator).color;
				//instancingData[numInstance].color = { 0.0f,1.0f,1.0f,1.0f };
				instancingData[numInstance].color.w = (*particleIterator).color.w;

				ImGui::DragFloat4("ALPHA", &(*particleIterator).color.x);
				//instancingData[index].color = particles[index].color;


				++numInstance;
			}

			++particleIterator;
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

	GraphicsRenderer::GetInstance()->SetRootSignatureAndPSO(true);

	//directX12_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//wvp用のCBufferの場所を設定
	//DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());

	//koko
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	DirectX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureIndex));
	//描画！　（DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	DirectX12::GetInstance()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), numInstance, 0, 0);
}

ParticleInfo Particle::MakeNewParticle() {
	Scope scope = { -0.01f,0.01f };
	Vector3 r = RandomGenerator::GetInstance()->getRandom({ scope, scope, scope });

	ParticleInfo particleInfo;
	particleInfo.transform.translate = r + emitter.transform.translate;
	particleInfo.transform.scale = { 1.0f,1.0f,1.0f };
	particleInfo.transform.rotate = {};
	particleInfo.velocity = r;

	Scope color = { 0,255 };
	ScopeVec4 colorVec4 = { color,color,color,{255,255} };

	particleInfo.color = RandomGenerator::getColorRandom(colorVec4);

	Scope lScope = { 6.0f,18.0f };
	float randomLife = RandomGenerator::GetInstance()->getRandom(lScope);
	particleInfo.liftTime = randomLife;
	particleInfo.currentTime = 0.0f;

	return particleInfo;
}

std::list<ParticleInfo> Particle::Emit(const Emitter& emitter) {
	std::list<ParticleInfo> particless;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particless.push_back(MakeNewParticle());
	}
	return particless;
}

void Particle::CreateVertexBufferView() {
	vertexResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView = {};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

void Particle::CreateMaterialResource() {
	materialResource_ = DirectX12::GetInstance()->CreateBufferResource(sizeof(Material));
	material = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material));

	material->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	material->enableLighting = false;
	material->enablePhong = false;
}

void Particle::CreateInstance() {
	instancingResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(ParticleForGPU) * kNumInstance);

	instancingData = nullptr;

	instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

	for (uint32_t index = 0; index < kNumInstance; ++index) {
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].World = MakeIdentity4x4();
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