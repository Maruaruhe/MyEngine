#include "Map.h"
#include "../../../Base/Load/LoadCSV.h"

void Map::Initialize(Camera* camera){
    mapData = LoadMapData("testMap");
    CreateWall(camera);
}

void Map::Update(){
	for (Wall wall : walls) {
		wall.Update();
	}
}

void Map::Draw(){
	for (Wall wall : walls) {
		wall.Draw();
	}
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
                    walls.push_back(newWall);
                }
            }
        }
    }
}