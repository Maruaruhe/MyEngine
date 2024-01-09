#pragma once
#include <stdint.h>
#include "../../Base/DirextX12/DirectX12.h"
#include <dxcapi.h>
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../../Math/struct.h"	
#include "../Light/Lighting.h"

#include <wrl.h>

#pragma comment(lib,"dxcompiler.lib")


class Sphere
{
public:
	void Initialize(DirectX12* directX12);

	void InitializePosition();

	void CreateVertexResource();

	void CreateVertexBufferView();

	void DataResource();

	void Release();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateDirectionalLightResource();

	void Update(Vector4& color, Transform& transform_, const Transform& cameraTransform, DirectionalLight& directionalLight);

	void Draw();

	void SetPosition(Vector3& position) { transform.translate = position; }
private:
	DirectX12* directX12_;

	Transform transform;
	Transform cameraTransform;

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

