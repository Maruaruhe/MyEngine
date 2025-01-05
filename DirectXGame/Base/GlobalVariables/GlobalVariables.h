#pragma once
#include <variant>
#include <map>
#include <string>
#include <fstream>
#include "../../Math/Vector4.h"
#include "../ImGui/ImGuiWND.h"
#include "../../../externals/nlohmann/json.hpp"

struct Item {
	std::variant<int32_t, float, Vector3> value;
};

struct Group {
	std::map<std::string, Item> items;
};

//グローバル変数
class GlobalVariables
{
public:
	static GlobalVariables* GetInstance();

	//Update
	void Update();

	//グループの作成
	void CreateGroup(const std::string& groupName);
	//ファイルのセーブ
	void SaveFile(const std::string& groupName);
	//全ファイルロード
	void LoadFiles();
	//ファイルロード
	void LoadFile(const std::string& groupName);

	//Setter
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	//変数の追加
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	void AddItem(const std::string& groupName, const std::string& key, float value);
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	//Getter
	int32_t GetIntValue(const std::string& groupName, const std::string& key)const;
	float GetFloatValue(const std::string& groupName, const std::string& key)const;
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key)const;

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	//コピーコンストラクタ
	
	//コピー代入演算子


	//AllDatas
	std::map<std::string, Group> datas_;

	using json = nlohmann::json;

	const std::string kDirectoryPath = "Resources/GlobalVariables/";
};

