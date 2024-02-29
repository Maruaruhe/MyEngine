#include "ModelManager.h"

void ModelManager::Initialize() {

}

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;

	return &instance;
}

void ModelManager::LoadModel(const std::string& filePath) {
	//読み込み済みモデルを検索
	if (models.contains(filePath)) {
		return;
	}

	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(filePath);

	models.insert(std::make_pair(filePath, std::move(model)));
}

Model* ModelManager::FindModel(const std::string& filePath) {
	//読み込み済みモデルを検索
	if (models.contains(filePath)) {
		return models.at(filePath).get();
	}
	return nullptr;
}