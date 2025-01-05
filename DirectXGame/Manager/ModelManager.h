#pragma once
#include <array>
#include <string>
#include <map>
#include <d3d12.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../../externals/DirectXTex/DirectXTex.h"

#include "../Base/DirextX12/DirectX12.h"
#include "../Base/Log/Log.h"

#include "../Object/Model/Model.h"

#include "../Math/struct.h"

struct Joint {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	Matrix4x4 skeltonSpaceMatrix;
	std::string name;
	std::vector<int32_t> children;
	int32_t index;
	std::optional<int32_t> parent;
};

struct Skelton {
	int32_t root;
	std::map<std::string, int32_t> jointMap;
	std::vector<Joint> joints;
};


class ModelManager
{
public:
	static ModelManager* GetInstance();

	/// <summary>
	/// Initialize
	/// </summary>
	void Initialize();
	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadModel(const std::string& filePath);
	/// <summary>
	/// モデル読み込みGLTF
	/// </summary>
	/// <param name="filePath"></param>
	void LoadGLTF(const std::string& filePath);

	//Getter
	ModelData GetModel(const std::string& filePath);
	/// <summary>
	/// LoadFile
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filename"></param>
	/// <returns></returns>
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

	//Node読み込み
	Node ReadNode(aiNode* node);
	//Skelton作成
	Skelton CreateSkelton(const Node& rootNode);
	//Joint作成
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	//SkinCluster作成
	SkinCluster CreateSkinCluster(const Skelton& skelton, const ModelData& modelData);

	//MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	uint32_t index_ = 500;
private:
	std::map<std::string, ModelData> modelDatas;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;
};

