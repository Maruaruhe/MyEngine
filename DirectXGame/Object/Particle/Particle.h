//#pragma once
//#include <fstream>
//#include <sstream>
//#include <stdint.h>
//#include <vector>
//#include <list>
//#include "../../Base/DirextX12/DirectX12.h"
//#include <dxcapi.h>
//#include "../../Math/Vector4.h"
//#include "../../Math/Matrix4x4.h"
//#include "../../Math/RandomGenerator.h"
//#include "../Camera/Camera.h"
//#include "../../Math/struct.h"	
//#include "../Light/Lighting.h"
//
//#include "../../Base/Input/Input.h"
//
//#pragma comment(lib,"dxcompiler.lib")
//
//#define kNumInstance 100
//
//struct ParticleForGPU {
//	Matrix4x4 WVP;
//	Matrix4x4 World;
//	Vector4 color;
//};
//
//struct Emitter {
//	Transform transform;
//	uint32_t count;
//	float frequency;
//	float frequencyTime;
//};
//
//class Particle
//{
//public:
//	void Initialize(const std::string& filename);
//
//	void Update();
//
//	void Draw();
//
//	void SetCamera(Camera* camera) { this->camera = camera; }
//
//	void SetModel(const std::string& filePath);
//
//private:
//	ParticleInfo MakeNewParticle();
//
//	std::list<ParticleInfo> Emit(const Emitter& emitter);
//
//	void InitializePosition();
//
//	void ApplyGlobalVariables();
//
//	void CreateMaterialResource();
//
//	void CreateVertexBufferView();
//
//	void CreateInstance();
//
//	void CreateSRV();
//
//	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
//
//public:
//	Material* material = nullptr;
//
//	//ParticleInfo particles[kNumInstance];
//	std::list<ParticleInfo> particles;
//
//	ParticleForGPU* instancingData;
//
//private:
//	uint32_t textureIndex = 0;
//	Emitter emitter{};
//
//	uint32_t numInstance = 0;
//
//
//	//DirectX12* directX12 = nullptr;
//	Input* input_ = nullptr;
//	ModelData modelData;
//	VertexData* vertexData = nullptr;
//	Camera* camera = nullptr;
//
//	uint32_t descriptorSizeSRV{};
//
//	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
//	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
//
//	std::string forg;
//
//	//頂点リソース用のヒープの設定
//	D3D12_HEAP_PROPERTIES uploadHeapProperties;
//	//頂点リソースの設定
//	D3D12_RESOURCE_DESC vertexResourceDesc;
//	//実際に頂点リソースを作る
//	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
//	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
//
//	//頂点バッファビューを作成する
//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
//
//	//頂点リソースにデータを書き込む
//
//	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
//
//	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
//
//	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;
//
//	const int32_t kClientWidth = 1280;
//	const int32_t kClientHeight = 720;
//
//private:
//
//	float num = 0.0625f;
//	const float pi = 3.14f;
//};