#pragma once
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <vector>
#include "../../Base/DirextX12/DirectX12.h"
#include <dxcapi.h>
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../Camera/Camera.h"
#include "../../Math/struct.h"	
#include "../Light/Lighting.h"

#include "../../Base/Input/Input.h"

#pragma comment(lib,"dxcompiler.lib")

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};

class Model
{
public:
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	void Initialize(const std::string& filename);

	void InitializePosition(const std::string& filename);

	void ApplyGlobalVariables();

	void CreateMaterialResource();

	void CreateVertexBufferView();

	void CreateTransformationMatrixResource();

	void Update();

	void Draw();

	void SetCamera(Camera* camera) { this->camera = camera; }

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

public:
	Material* material;
	Transform transform;

private:
	DirectX12* directX12 = nullptr;
	Input* input_ = nullptr;
	ModelData modelData;
	Camera* camera = nullptr;

	std::string forg;

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc;
	//実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//頂点リソースにデータを書き込む

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;


	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;

	//Matrix4x4* wvpData;
	//Matrix4x4 worldMatrix_;

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	bool useMonsterBall = true;

private:
	VertexData* vertexData;

	float num = 0.0625f;
	const float pi = 3.14f;
};