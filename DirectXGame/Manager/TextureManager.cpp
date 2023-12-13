#include "TextureManager.h"

TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;

	return &instance;
}

void TextureManager::LoadTexture(uint32_t index) {

}