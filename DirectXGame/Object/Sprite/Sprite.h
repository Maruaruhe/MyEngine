#pragma once
#include "../../Base/DirextX12/DirectX12.h"
#include "../../Manager/TextureManager.h"
#include <dxcapi.h>
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../../Math/struct.h"	
#include <wrl.h>

#pragma comment(lib,"dxcompiler.lib")

class Sprite
{
public:
	void Initialize(Vector2 size, std::string textureFilePath);

	void Update();

	void Draw();

	void SetPosition(Vector2 translate);

	void SetTexcoord(Vector2 LT, Vector2 LB, Vector2 RT, Vector2 RB);

private:
	VertexData GetPosition() { return vertexData[0], vertexData[1], vertexData[2], vertexData[3]; }

	void CreateVertexResource();

	void CreateVertexBufferView();

	void Release();

	void CreateMaterialResource();

	void CreateTransformationMatrixResource();

	void CreateIndexResource();

	void CreateVertexData(Vector2 size);

public:
	Transform transform;
	Transform uvTransform;

	Material* materialData_;
	bool isActive;

	Vector2 anchorPoint = { 0.0f,0.0f };
	Vector2 size_;

private:
	uint32_t textureIndex = 0;

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

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;

	TransformationMatrix* transformationMatrix;
	//Matrix4x4* wvpData;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	uint32_t* indexData;

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

};


