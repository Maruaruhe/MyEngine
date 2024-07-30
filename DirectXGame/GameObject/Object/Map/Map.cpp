#include "Map.h"
#include "../../../Base/Load/LoadCSV.h"

void Map::Initialize(Camera* camera){
    mapData = LoadMapData("testMap");
    CreateWall(camera);
    CreateFloor(camera);
    CreateRoof(camera);

    w.Initialize({ 0,0,3 }, { 1,1, 1 });
    w.model.SetCamera(camera);

    walls.push_back(w);
}

void Map::Update(){
	for (Wall wall : walls) {
		wall.Update();
	}
    floor.Update();
    roof.Update();
}

void Map::Draw(){
	for (Wall wall : walls) {
		wall.Draw();
	}
    floor.Draw();
    roof.Draw();
}

std::vector<std::vector<int>> Map::LoadMapData(const std::string& filename) {

    std::vector<std::vector<int>> map;//配列の読み込み先

    std::string file = directoryPath + filename + ".csv";
    std::ifstream stageifs(file);
    std::string line;

    int y{ 0 };    // マップチップ

    while (getline(stageifs, line)) {
        // ','で一列の情報を区切る
        std::vector<std::string> strvec = split(line, ',');

        // X軸、Y軸のループ
        map.push_back(std::vector<int>());  // Y軸１行追加
        for (int x = 0; x < strvec.size(); x++) {
            map[y].push_back(std::stoi(strvec.at(x)));  // X軸１列追加
        }
        y++;
    }

    return map;
}

void Map::CreateWall(Camera* camera) {
    std::vector<std::vector<int>> data = mapData;

    //x方面へのモデルを作る

    for (int z = 0; z < data.size(); z++) {
        int i = 0;

        for (int x = 0; x < data[0].size(); x++) {
            if (data[z][x] == WALL) {
                i++;
            }
            else {
                if (i >= 2) { //二つ以上続いた&ブロックが途切れた時に生成する
                    Wall newWall;
                    Transform transform;

                    transform.translate = { float(x - i) , 0.0f, float(-z - 1) };
                    transform.scale = { float(i) , kMapHeight,1.0f };
                    transform.rotate = { 0.0f,0.0f,0.0f };

                    for (int j = 0; j < i; j++) {
                        data[z][x - j - 1] = 0; //生成済のマップチップを0(NONE)に
                    }

                    i = 0;

                    newWall.Initialize(transform.translate, transform.scale);
                    newWall.model.SetCamera(camera);
                    newWall.model.material->enableLighting = true;
                    walls.push_back(newWall);
                }
                else { //一つしか続かなかったら生成しない
                    i = 0;
                }
            }
        }
    }

    //z方面へのモデルを作る

    for (int x = 0; x < data[0].size(); x++) {
        int i = 0;

        for (int z = 0; z < data.size(); z++) {
            if (data[z][x] == WALL) {
                i++;
            }
            else { //ブロックが途切れた時に生成する
                if (i >= 1) {
                    Wall newWall;
                    Transform transform;

                    transform.translate = { float(x) ,0.0f, float(-z) };
                    transform.scale = { 1.0f ,kMapHeight ,float(i) };
                    transform.rotate = { 0.0f,0.0f,0.0f };

                    for (int j = 0; j < i; j++) {
                        data[z - j - 1][x] = 0; //生成済のマップチップを0(NONE)に
                    }

                    i = 0;

                    newWall.Initialize(transform.translate, transform.scale);
                    newWall.model.SetCamera(camera);
                    newWall.model.material->enableLighting = true;
                    walls.push_back(newWall);
                }
            }
        }
    }
}

void Map::CreateFloor(Camera* camera) {
    std::vector<std::vector<int>> data = mapData;

    Vector2 floorScale = { float(data[0].size()), float(data.size()) };

    Transform transform;
    transform.translate = { 0.0f,-1.0f,-floorScale.y + 1.0f };
    transform.scale = { floorScale.x - 1.0f,1.0f,floorScale.y - 1.0f };
    transform.rotate = { 0.0f,0.0f,0.0f };

    floor.Initialize(transform.translate, transform.scale);
    floor.model.SetCamera(camera);
}

void Map::CreateRoof(Camera* camera) {
    std::vector<std::vector<int>> data = mapData;

    Vector2 floorScale = { float(data[0].size()), float(data.size()) };

    Transform transform;
    transform.translate = { 0.0f,kMapHeight - 3.0f,-floorScale.y + 1.0f };
    transform.scale = { floorScale.x - 1.0f,1.0f,floorScale.y - 1.0f };
    transform.rotate = { 0.0f,0.0f,0.0f };

    roof.Initialize(transform.translate, transform.scale);
    roof.model.SetCamera(camera);
}

void Map::CheckCollision(AABB pAABB, Vector2 move, Vector3* fixVector) {
    Vector3 distance{};

    for (Wall wall : walls) {
        AABB wallAABB;
        wallAABB.CreateModelAABB(wall.model.transform);

        if (wallAABB.CheckCollision(pAABB)) {
            //X軸の当たり判定
            if (pAABB.max.x < wallAABB.max.x) {//壁に対して右方面へ衝突してる時
                distance.x = pAABB.max.x - wallAABB.min.x;
                if (fabs(move.x) + dis >= fabs(distance.x)) {//移動量が差分より大きかったら調整
                    fixVector->x = -fabs(distance.x);
                }
            }
            else {//左方面へ衝突してる時
                distance.x = pAABB.min.x - wallAABB.max.x;
                if (fabs(move.x) + dis >= fabs(distance.x)) {//移動量が差分より大きかったら調整
                    fixVector->x = fabs(distance.x);
                }
            }
        }
    }

    //fixVectorに応じてpAABBの座標の調整
    pAABB.min.x += fixVector->x;
    pAABB.max.x += fixVector->x;

    for (Wall wall : walls) {
        AABB wallAABB;
        wallAABB.CreateModelAABB(wall.model.transform);

        if (wallAABB.CheckCollision(pAABB)) {
            //Z軸の当たり判定
            if (pAABB.max.z < wallAABB.max.z) {//壁に前方面へ衝突
                distance.z = pAABB.max.z - wallAABB.min.z;
                if (fabs(move.y) + dis >= fabs(distance.z)) {//移動量が差分より大きかったら調整
                    fixVector->z = -fabs(distance.z);
                }
            }
            else {//後方面へ
                distance.z = pAABB.min.z - wallAABB.max.z;
                if (fabs(move.y) + dis >= fabs(distance.z)) {//移動量が差分より大きかったら調整
                    fixVector->z = fabs(distance.z);
                }
            }
        }
    }
}