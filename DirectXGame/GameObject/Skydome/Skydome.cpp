#include "Skydome.h"

void Skydome::Initialize(const std::string& filename) {
	model_.Initialize(filename);

	transform.translate = { 0,0,0 };
	transform.scale = { 20,20,20 };
	transform.rotate = { 0,0,0 };
}
void Skydome::Update() {

}
void Skydome::Draw() {
	model_.Draw();
}