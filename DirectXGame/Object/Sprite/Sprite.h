#pragma once
#include "../../Base/DirextX12/DirectX12.h"
#include <dxcapi.h>
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../../Math/struct.h"	
#include <wrl.h>

#pragma comment(lib,"dxcompiler.lib")

class Sprite
{
public:
	void Initialize(SpriteData spriteData);

	void SetPosition(Vector2 LB, Vector2 LT, Vector2 RB, Vector2 RT);
	VertexData GetPosition() { return vertexData[0], vertexData[1], vertexData[2], vertexData[3]; }

	void CreateVertexResource();

	void CreateVertexBufferView();

	void DataResource();

	void Release();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateIndexResource();

	void Update(Vector4& color, const Transform& transform_);

	void Draw();
private:
	DirectX12* directX12 = nullptr;;

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
	bool isInvisible_;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};


