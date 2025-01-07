#include "Skydome.h"
#include "../../Manager/ModelManager.h"

void Skydome::Initialize(const std::string& filename) {
	ModelManager::GetInstance()->LoadModel(filename);
	model = std::make_unique<Model>();
	model->Initialize(filename);
}
void Skydome::Update() {

	model->Update();

}
void Skydome::Draw() {
	model->Draw();
}