#include "MapJson.h"
#include "../../../Base/Load/LoadCSV.h"
#include "../../../Manager/FileManager.h"
#include "../../../Manager/TextureManager.h"

using namespace MyEngine;

MapJson::MapJson(){}

MapJson::~MapJson() {
}

void MapJson::Initialize(Camera* camera, std::string filename, Player* player) {
    level_ = FileManager::GetInstance()->LoadJsonFile("Json/", filename);

    CreateWall(camera);
    CreateDoor(camera);
    CreateEnemy(camera, player);
}

void MapJson::Update() {
    for (Wall wall : walls_) {
        wall.Update();
    }
    for (Door door : doors_) {
        door.Update();
    }
}

void MapJson::Draw() {
    for (Wall wall : walls_) {
        wall.Draw();
    }
    for (Door door : doors_) {
        door.Draw();
    }
}

void MapJson::CheckCollision(AABB pAABB, Vector3 move, Vector3* fixVector) {
    Vector3 distance{};

    for (Wall wall : walls_) {
        AABB wallAABB;
        wallAABB.CreateWallAABB(wall.model_.transform);

        if (wallAABB.CheckCollision(pAABB)) {
            //X軸の当たり判定
            if (pAABB.max.x < wallAABB.max.x && wall.direction_.toRight) {//壁に対して右方面へ衝突してる時
                distance.x = pAABB.max.x - wallAABB.min.x;
                if (fabs(move.x) + dis >= fabs(distance.x)) {//移動量が差分より大きかったら調整
                    fixVector->x = -fabs(distance.x);
                }
            }
            if (pAABB.max.x >= wallAABB.max.x && wall.direction_.toLeft) {//左方面へ衝突してる時
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

    for (Wall wall : walls_) {
        AABB wallAABB;
        wallAABB.CreateWallAABB(wall.model_.transform);

        if (wallAABB.CheckCollision(pAABB)) {
            //Z軸の当たり判定
            if (pAABB.max.z < wallAABB.max.z && wall.direction_.toFront) {//壁に前方面へ衝突
                distance.z = pAABB.max.z - wallAABB.min.z;
                if (fabs(move.z) + dis >= fabs(distance.z)) {//移動量が差分より大きかったら調整
                    fixVector->z = -fabs(distance.z);
                }
            }
            if (pAABB.max.z >= wallAABB.max.z && wall.direction_.toBack) {//後方面へ
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

void MapJson::CheckCollisionFloor(AABB pAABB, Vector3* move, Vector3* fixVector, bool* isFloor) {
    Vector3 distance{};
    int collisoinCount = 0;

    for (Wall wall : walls_) {
        AABB wallAABB;
        wallAABB.CreateWallAABB(wall.model_.transform);
        if (wall.direction_.toTop || wall.direction_.toBot) {
            if (wallAABB.CheckCollision(pAABB)) {
                //Y軸の当たり判定
                if (pAABB.max.y > wallAABB.max.y && wall.direction_.toBot) {//壁に下方面へ衝突
                    distance.y = pAABB.min.y - wallAABB.max.y;
                    if (fabs(move->y) + dis >= fabs(distance.y)) {//移動量が差分より大きかったら調整
                        fixVector->y = fabs(distance.y);
                        *isFloor = true;
                        collisoinCount++;
                    }
                }
                if (pAABB.max.y < wallAABB.max.y && wall.direction_.toTop) {//上方面へ
                    distance.y = pAABB.max.y - wallAABB.min.y;
                    if (fabs(move->y) + dis >= fabs(distance.y)) {//移動量が差分より大きかったら調整
                        fixVector->y = -fabs(distance.y);
                        move->y = 0.0f;
                    }
                }
            }
        }
    }
    if (collisoinCount <= 0) {
        *isFloor = false;
    }
}

bool MapJson::CheckCollisionWithEye(const Segment& eyeSegment) {
    for (Door door : doors_) {
            AABB doorAABB;
            doorAABB.CreateWallAABB(door.model_.transform);
            //線分との当たり判定チェック
            if (doorAABB.CheckLineCollision(eyeSegment)) {
                return true;
            }
        }
    return false;
}

void MapJson::CreateWall(Camera* camera){
    for (int i = 0; i < level_.objects.size(); i++) {
        if (level_.objects[i].direction.toBack || level_.objects[i].direction.toBot || level_.objects[i].direction.toFront || level_.objects[i].direction.toLeft || level_.objects[i].direction.toRight || level_.objects[i].direction.toTop) {
            Wall newWall;
            newWall.Initialize(level_.objects[i].transform, level_.objects[i].direction,level_.objects[i].filename);
            newWall.model_.SetCamera(camera);

            walls_.push_back(newWall);
        }
    }
}

void MapJson::CreateDoor(Camera* camera){
    for (int i = 0; i < level_.objects.size(); i++) {
        if (level_.objects[i].isDoor){
            Door newDoor;
            newDoor.Initialize(level_.objects[i].transform, level_.objects[i].filename);
            newDoor.model_.SetCamera(camera);

            doors_.push_back(newDoor);
        }
    }
}

void MapJson::CreateEnemy(Camera* camera, Player* player) {
    for (int i = 0; i < level_.objects.size(); i++) {
        if (level_.objects[i].isEnemy) {
            Trace newDoor;
            newDoor.Initialize(level_.objects[i].transform, level_.objects[i].filename);
            newDoor.model_.SetCamera(camera);
            newDoor.SetPlayer(player);

            traces_.push_back(newDoor);
        }
    }
}

void MapJson::CreateWayPoint(Camera* camera){

}

std::vector<std::shared_ptr<mapItem>> MapJson::CreateItem(Camera* camera, std::string filename){
    std::vector<std::shared_ptr<mapItem>> items_;

    level_ = FileManager::GetInstance()->LoadJsonFile("Json/", filename);

    for (int i = 0; i < level_.objects.size(); i++) {
        if (level_.objects[i].isItem) {
            std::shared_ptr<mapItem> newItem;
            newItem = std::make_shared<mapItem>();
            newItem->Initialize(level_.objects[i].transform, level_.objects[i].filename);
            newItem->model_.SetCamera(camera);

            items_.push_back(newItem);
        }
    }

    return items_;
}
