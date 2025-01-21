#include "FileManager.h"
#include "ModelManager.h"
#include "TextureManager.h"

using namespace MyEngine;

// 初期化処理
void FileManager::Initialize()
{
	//levelData_ = std::make_unique<LevelData>();
}

// JSONファイル読み込み
LevelData FileManager::LoadJsonFile(const std::string& routeFilePath, const std::string& fileName)
{
	/* ---------- JSOnファイルを読み込んでみる ---------- */

	// 連結してフルパスを得る
	const std::string fullPath = "Resources/" + routeFilePath + fileName + ".json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullPath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}


	/* ---------- ファイルチェック ---------- */

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルエディタファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);


	/* ---------- オブジェクトの走査 ---------- */

	 // レベルデータ格納用インスタンスを生成
	LevelData level;

	// "objects"の全オブジェクトを走査
	if (deserialized.contains("objects") && deserialized["objects"].is_array()) {

		// 走査してく
		for (nlohmann::json& object : deserialized["objects"]) {

			ScanningObjects(object, &level.objects);
		}
	}

	return level;
}


// オブジェクトの走査
void FileManager::ScanningObjects(nlohmann::json& object, std::vector<ObjectData>* objects)
{
	// 各オブジェクトには必ず "type"データを入れているので
	// "type"が検出できなければ不正として実行を停止する
	assert(object.contains("type"));
	// 種類を取得
	std::string type = object["type"].get<std::string>();

	if (object["load_model"]) {
		/* ---------- メッシュの読み込み ---------- */

		// MESH
		if (type.compare("MESH") == 0) {

			// 新しくオブジェクトを作成
			ObjectData objectData;
			ModelManager::GetInstance()->LoadModel("2x2cube");
			TextureManager::GetInstance()->LoadTexture("Resources/Map/wall.png");

			// トランスフォームのパラメータ読み込み
			if (object.contains("transform")) {

				nlohmann::json& transform = object["transform"];
				// 平行移動
				objectData.transform.translate.x = (float)transform["translation"][0];
				objectData.transform.translate.y = (float)transform["translation"][2];
				objectData.transform.translate.z = (float)transform["translation"][1];
				// 回転角
				objectData.transform.rotate.x = -(float)transform["rotation"][0]; //*(float(std::numbers::pi) / 180.0f);
				objectData.transform.rotate.y = -(float)transform["rotation"][2];// *(float(std::numbers::pi) / 180.0f);
				objectData.transform.rotate.z = -(float)transform["rotation"][1];// *(float(std::numbers::pi) / 180.0f);
				// スケーリング
				objectData.transform.scale.x = (float)transform["scaling"][0];
				objectData.transform.scale.y = (float)transform["scaling"][2];
				objectData.transform.scale.z = (float)transform["scaling"][1];
			}

			if (object.contains("file_name")) {
				objectData.filename = object["file_name"];
			}

			//種類分け
			if (object.contains("is_item")) {
				objectData.isItem = object["is_item"];
			}
			if (object.contains("to_left")) {
				objectData.direction.toLeft = object["to_left"];
			}
			if (object.contains("to_right")) {
				objectData.direction.toRight = object["to_right"];
			}
			if (object.contains("to_front")) {
				objectData.direction.toFront = object["to_front"];
			}
			if (object.contains("to_back")) {
				objectData.direction.toBack = object["to_back"];
			}
			if (object.contains("to_top")) {
				objectData.direction.toTop = object["to_top"];
			}
			if (object.contains("to_bot")) {
				objectData.direction.toBot = object["to_bot"];
			}
			if (object.contains("is_door")) {
				objectData.isDoor = object["is_door"];
			}

			if (object.contains("children")) {
				nlohmann::json& child = object["children"];
				for (int i = 0; i < child.size(); i++) {
					// 種類を取得
					std::string childType = child[i]["type"].get<std::string>();

					// MESH
					if (childType.compare("MESH") == 0) {
						ScanningObjects(child[i], objects);
					}
				}
			}

			// オブジェクトを追加
			objects->push_back(objectData);
		}
	}
}

//void FileManager::CreateModels() {
//	// レベルデータからオブジェクトを生成、配置
//	for (auto& objectData : levelData_->objects) {
//
//		// ファイル名から登録済みモデルを検索
//		Model model;
//		model.Initialize("Box");
//		model.transform = FileManager::GetInstance()->GetObjectTransform("key");
//		levelModels.push_back(model);
//	}
//}