#include "GameScene.h"

void GameScene::Initialize()
{
	input = Input::GetInstance();
	light.color = { 1.0f,1.0f,1.0f,1.0f };
	light.direction = { 0.0f,-1.0f,0.0f };
	light.intensity = 1.0f;

	colorVolume[0] = 1.0f;
	colorVolume[1] = 1.0f;
	colorVolume[2] = 1.0f;

	player_ = std::make_unique<Player>();
	camera = std::make_unique<Camera>();

	player_->Initialize("ghostPori");
	camera->Initialize();

	LoadEnemyPopDate();
	UpdateEnemyPopCommands();
	for (Enemy* enemy : enemies_) {
		enemy->SetIsAlive(true);
	}
}

void GameScene::Update() {

	Vector4 color = {colorVolume[0],colorVolume[1],colorVolume[2],1.0f};

	player_->Update(color, camera->GetTransform(), light);
	Vector4 eColor = { 0.0f,0.0f,0.0f,1.0f };
	for (Enemy* enemy : enemies_) {
		enemy->Update(eColor, camera->GetTransform(), light);
	}
	isCollsion();
	if (!checkEnemyAlive()) {
		sceneNo = CLEAR;
	}

	if (input->PushKey(DIK_Q)) {
		sceneNo = CLEAR;
	}
}

void GameScene::isCollsion() {
	//Vector3 pPos = player_->GetBulletTransform().translate;
	std::list<Bullet*> bullets = player_->GetBullets();
	for (Enemy* enemy : enemies_) {
		for (Bullet* bullet : bullets) {
			Vector3 pPos = bullet->GetPosition();
			Vector3 ePos = enemy->GetTransform().translate;
			Vector3 dis = Subtract(pPos, ePos);
			if ((dis.x * dis.x + dis.y * dis.y + dis.z * dis.z) <= 2) {
				enemy->SetIsAlive(false);
			}
		}
	}
}


void GameScene::Draw() {
	player_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
}

void GameScene::LoadEnemyPopDate() {

	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	std::string line;
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			spawnEnemy(Vector3(x, y, z));
			//break;
		}
	}
}

void GameScene::spawnEnemy(Vector3 pos) {
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize("ghostPori");
	newEnemy->SetPosition(pos);
	newEnemy->SetIsAlive(true);
	enemies_.push_back(newEnemy);
}


bool GameScene::checkEnemyAlive() {
	for (Enemy* enemy : enemies_) {
		if (enemy->GetIsAlive()) {
			return true;
		}
	}
		return false;
}