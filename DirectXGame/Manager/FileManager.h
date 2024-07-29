#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <vector>
#include <numbers>


#include "../../nlohmann/json.hpp"
//#include "../Math/math.h"
#include "../Object/Model/Model.h"

struct ColliderData {
	std::string type;
	Vector3 center;
	Vector3 size;
};
struct LevelData {

	struct ObjectData {
		std::string type;
		std::string file_name;
		Transform transform;
		std::map<std::string, std::unique_ptr<ObjectData>> children;
	};
	std::map<std::string, std::unique_ptr<ObjectData>> objects;
};


/* FileManagerクラス */
class FileManager {

private:

	// コンストラクタ、デストラクタ
	FileManager() = default;
	~FileManager() = default;
	FileManager(const FileManager&) = delete;
	const FileManager& operator=(const FileManager&) = delete;

public:

	// インスタンスの取得
	static FileManager* GetInstance() {
		static FileManager instance;
		return&instance;
	}

	// 初期化処理
	void Initialize();

	// JSONファイル読み込み
	void LoadJsonFile(const std::string& routeFilePath, const std::string& fileName);


#pragma region Get

	LevelData::ObjectData* GetObjectData(const std::string& key) const {

		if (!levelData_) {
			return nullptr;
		}

		auto it = levelData_->objects.find(key);
		if (it != levelData_->objects.end()) {
			return it->second.get();
		}

		return nullptr;
	}

	Transform GetObjectTransform(const std::string& key) const {

		auto it = levelData_->objects.find(key);
		if (it != levelData_->objects.end()) {
			return it->second.get()->transform;
		}

		return Transform();
	}

#pragma endregion 

#pragma region Set

#pragma endregion 


private:

	// オブジェクトの走査
	void ScanningObjects(nlohmann::json& object, std::map<std::string, std::unique_ptr<LevelData::ObjectData>>& objects);

	void CreateModels();

private:

	// JSONファイルから読み込んだ情報をまとめおく変数
	std::unique_ptr<LevelData> levelData_;

//	std::vector

	// 配置するための変数
	std::vector<Model> levelModels;
//	std::map<const std::string, std::unique_ptr<LevelObject>> levelObjectMap_;
};

