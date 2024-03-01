#pragma once
#include <array>
#include <string>
#include <map>
#include <d3d12.h>
#include "../../externals/DirectXTex/DirectXTex.h"

#include "../Base/DirextX12/DirectX12.h"
#include "../Base/Log/Log.h"

#include "../Object/Model/Model.h"

class ModelManager
{
public:
	static ModelManager* GetInstance();

	void Initialize();
	void LoadModel(const std::string& filePath);
	ModelData GetModel(const std::string& filePath);
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	std::map<std::string, ModelData> modelDatas;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;
};

