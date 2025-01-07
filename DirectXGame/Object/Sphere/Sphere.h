#pragma once
#include <stdint.h>
#include "../../Base/DirextX12/DirectX12.h"
#include <dxcapi.h>
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../../Math/struct.h"	
#include "../Light/Lighting.h"
#include "../Camera/Camera.h"

#include <wrl.h>

#pragma comment(lib,"dxcompiler.lib")


class Sphere
{
public:
	void Initialize();

	void Update();

	void Draw();

	void SetCamera(Camera* Tcamera) { this->camera = Tcamera; }

private:

	void InitializePosition();

	void CreateVertexBufferView();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

public:
	Transform transform;
	Transform cameraTransform;
	Material* materialData;

private:
	MyEngine::DirectX12* directX12 = nullptr;
	Camera* camera = nullptr;

	TransformationMatrix* transformationMatrix;

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

