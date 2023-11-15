#pragma once
#include "DirectX12.h"
#include <dxcapi.h>
#include "Vector4.h"
#include "Matrix4x4.h"
#include "struct.h"
#include <wrl.h>

#pragma comment(lib,"dxcompiler.lib")

class Sprite
{
public:
	void Initialize(DirectX12* directX12, SpriteData spriteData);

	void CreateVertexResource();

	void CreateVertexBufferView();

	void DataResource();

	void Release();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateIndexResource();

	void Update(Vector4& color, Transform& transform_);

	void Draw();
private:
	DirectX12* directX12_;

	Transform transform;
	Transform cameraTransform;
	Transform uvTransform;

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc;
	//実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//頂点リソースにデータを書き込む
	//Vector4* vertexData;

	VertexData* vertexData;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	Material* materialData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;

	TransformationMatrix* transformationMatrix;
	//Matrix4x4* wvpData;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	uint32_t* indexData;

	Transform transform_;
	//Matrix4x4 worldMatrix_;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};


