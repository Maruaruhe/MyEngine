#pragma once
#include "../../Base/DirextX12/DirectX12.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <string>
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>

#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../Camera/Camera.h"
#include "../../Math/struct.h"	
#include "../Light/Lighting.h"

class Skybox
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	void CreateRootSignature();
	void CreatePSO();

	void CreateVertexBufferView();
	void CreateMaterialResource();
	void CreateTransformationMatrixResource();
	void CreateIndexResource();
public:

	Camera* camera = nullptr;
private:
	Transform transform;
	float initScale_ = 100.0f;
	uint32_t textureIndex = 0;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	// 定数の頂点数とインデックス数
	const size_t kNumVertices_ = 24; // 各面4頂点、6面で計24頂点
	const size_t kNumIndices_ = 36;  // 各面6インデックス、6面で計36インデックス

	//resources
	Material* material = nullptr;
	VertexData* vertexData = nullptr;
	TransformationMatrix* transformationMatrix = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	uint32_t* indexData;

	//実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//頂点リソースにデータを書き込む
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource_;
};

