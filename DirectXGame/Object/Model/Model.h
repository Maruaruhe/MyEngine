#pragma once
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <vector>
#include "../../Base/DirextX12/DirectX12.h"
#include <dxcapi.h>
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
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

	void CreateVertexResource();

	void CreateVertexBufferView();

	void DataResource();

	void Release();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateDirectionalLightResource();

	void Update(Vector4& color, const Transform& transform, const Transform& transform_, DirectionalLight& directionalLight);

	void Draw();

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	DirectX12* directX12 = nullptr;
	Input* input_ = nullptr;
	ModelData modelData;

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

	Material* materialData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;

	TransformationMatrix* transformationMatrix;
	//Matrix4x4* wvpData;

	Transform transform_;
	//Matrix4x4 worldMatrix_;

	DirectionalLight* directionalLight_;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	bool useMonsterBall = true;

private:
	VertexData* vertexData;

	float num = 0.0625f;
	const float pi = 3.14f;
};