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
	Model* FindModel(const std::string& filePath);

private:
	std::map<std::string, std::unique_ptr<Model>> models;

	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(ModelManager&) = delete;
	ModelManager& operator=(ModelManager&) = delete;
};

