#pragma once
#include <variant>
#include <map>
#include <string>
#include "Vector4.h"
#include "ImGuiWND.h"

struct Item {
	std::variant<int32_t, float, Vector3> value;
};

struct Group {
	std::map<std::string, Item> items;
};

class GlobalVariables
{
public:
	static GlobalVariables* GetInstance();

	void Update();

	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	//コピーコンストラクタ
	
	//コピー代入演算子


	//AllDatas
	std::map<std::string, Group> datas_;
};

