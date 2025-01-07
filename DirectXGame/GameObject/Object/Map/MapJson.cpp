#include "MapJson.h"
#include "../../../Base/Load/LoadCSV.h"
#include "../../../Manager/FileManager.h"
#include "../../../Manager/TextureManager.h"

using namespace MyEngine;

void MapJson::Initialize(Camera* camera, std::string filename) {
    level_ = FileManager::GetInstance()->LoadJsonFile("Json/", filename);
    for (auto& object : level_.objects) {
        object.model.SetCamera(camera);
    }
}

void MapJson::Update() {
    for (auto& object : level_.objects) {
        object.model.Update();
    }
}

void MapJson::Draw() {
    for (auto& object : level_.objects) {
        object.model.Draw();
    }
}

void MapJson::CheckCollision(AABB pAABB, Vector3 move, Vector3* fixVector) {
    Vector3 distance{};

    for (ObjectData& wall : level_.objects) {
        AABB wallAABB;
        wallAABB.CreateWallAABB(wall.model.transform);

        if (wallAABB.CheckCollision(pAABB)) {
            //X軸の当たり判定
            if (pAABB.max.x < wallAABB.max.x && wall.toRight) {//壁に対して右方面へ衝突してる時
                distance.x = pAABB.max.x - wallAABB.min.x;
                if (fabs(move.x) + dis >= fabs(distance.x)) {//移動量が差分より大きかったら調整
                    fixVector->x = -fabs(distance.x);
                }
            }
            if (pAABB.max.x >= wallAABB.max.x && wall.toLeft) {//左方面へ衝突してる時
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

    for (ObjectData& wall : level_.objects) {
        AABB wallAABB;
        wallAABB.CreateWallAABB(wall.model.transform);

        if (wallAABB.CheckCollision(pAABB)) {
            //Z軸の当たり判定
            if (pAABB.max.z < wallAABB.max.z && wall.toFront) {//壁に前方面へ衝突
                distance.z = pAABB.max.z - wallAABB.min.z;
                if (fabs(move.z) + dis >= fabs(distance.z)) {//移動量が差分より大きかったら調整
                    fixVector->z = -fabs(distance.z);
                }
            }
            if (pAABB.max.z >= wallAABB.max.z && wall.toBack) {//後方面へ
                distance.z = pAABB.min.z - wallAABB.max.z;
                if (fabs(move.z) + dis >= fabs(distance.z)) {//移動量が差分より大きかったら調整
                    fixVector->z = fabs(distance.z);
                }
            }
        }
    }

    //fixVectorに応じてpAABBの座標の調整
    pAABB.min.z += fixVector->z;
    pAABB.max.z += fixVector->z;
}

void MapJson::CheckCollisionFloor(AABB pAABB, Vector3 move, Vector3* fixVector, bool* isFloor) {
    Vector3 distance{};
    int collisoinCount = 0;

    for (ObjectData& wall : level_.objects) {
        AABB wallAABB;
        wallAABB.CreateWallAABB(wall.model.transform);
        if (wall.toTop || wall.toBot) {
            if (wallAABB.CheckCollision(pAABB)) {
                //Y軸の当たり判定
                if (pAABB.max.y > wallAABB.max.y && wall.toBot) {//壁に下方面へ衝突
                    distance.y = pAABB.max.y - wallAABB.min.y;
                    if (fabs(move.y) + dis >= fabs(distance.y)) {//移動量が差分より大きかったら調整
                        fixVector->y = -fabs(distance.y);
                        *isFloor = true;
                    }
                }
                if (pAABB.max.y < wallAABB.max.y && wall.toTop) {//上方面へ
                    distance.y = pAABB.min.y - wallAABB.max.y;
                    if (fabs(move.y) + dis >= fabs(distance.y)) {//移動量が差分より大きかったら調整
                        fixVector->y = fabs(distance.y);
                        *isFloor = true;
                    }
                }
                *isFloor = true;
                collisoinCount++;
            }
        }
    }
    if (collisoinCount <= 0) {
        *isFloor = false;
    }
}