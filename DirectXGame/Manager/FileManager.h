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

struct ObjectData {
	bool isItem;
	bool toLeft;
	bool toRight;
	bool toTop;
	bool toBot;
	Model model;
};

struct LevelData {
	std::vector<ObjectData> objects;
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
	//void LoadJsonFile(const std::string& routeFilePath, const std::string& fileName);
	LevelData LoadJsonFile(const std::string& routeFilePath, const std::string& fileName);


#pragma region Get

#pragma endregion 

#pragma region Set

#pragma endregion 


private:

	// オブジェクトの走査
	void ScanningObjects(nlohmann::json& object, std::vector<ObjectData>* objects);

	void CreateModels();

private:

public:

	// JSONファイルから読み込んだ情報をまとめおく変数
//	std::unique_ptr<LevelData> levelData_;
//
////	std::vector
//
//	// 配置するための変数
//	std::vector<Model> levelModels;
//	std::map<const std::string, std::unique_ptr<LevelObject>> levelObjectMap_;
};

