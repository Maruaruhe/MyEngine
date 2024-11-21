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

class Model
{
public:
	void Initialize(const std::string& filename);

	void Update();

	void Draw() const;

	Vector3 GetWorldPosition(){
		return { worldMatrix.m[3][0],worldMatrix.m[3][1] ,worldMatrix.m[3][2] };
	}

	void SetCamera(Camera* Tcamera) { this->camera = Tcamera; }
	Camera* GetCamera() { return camera; }

	void SetModel(const std::string& filePath);

private:
	void InitializePosition(const std::string& filename);

	void ApplyGlobalVariables();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateVertexBufferView();

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	void DrawMatrix4x4(const char* title, const Matrix4x4& matrix);

public:
	Material* material = nullptr;
	Transform transform;
	bool isParent;

	Transform uvTransform;
	Matrix4x4 localMatrix;
	uint32_t textureIndex = 0;
private:

	Matrix4x4 worldMatrix;

	//DirectX12* directX12 = nullptr;
	KeyInput* input_ = nullptr;
	ModelData modelData;
	Camera* camera = nullptr;
	VertexData* vertexData = nullptr;
	TransformationMatrix* transformationMatrix = nullptr;

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

	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

private:

	float num = 0.0625f;
	const float pi = 3.14f;
};