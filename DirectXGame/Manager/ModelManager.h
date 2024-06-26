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

	void Initialize();
	void LoadModel(const std::string& filePath);
	ModelData GetModel(const std::string& filePath);
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

	Node ReadNode(aiNode* node);
	Skelton CreateSkelton(const Node& rootNode);
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);
	SkinCluster CreateSkinCluster(const Skelton& skelton, const ModelData& modelData);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	std::map<std::string, ModelData> modelDatas;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;
};

