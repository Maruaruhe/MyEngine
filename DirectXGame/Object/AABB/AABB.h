#pragma once
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
//
//struct Segment {
//	Vector3 start, end;
//};
//
/// <summary>
/// AABBクラス
/// </summary>
class AABB final {
public: // ** パブリックなメンバ変数 ** //

	// 最小点
	Vector3 min;
	// 最大店
	Vector3 max;

	// トランスフォーム
	Transform transform;

private: // ** メンバ変数 ** //


public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// <para>メンバ変数をクリアする</para>
	/// </summary>
	void Initialize();

	/// <summary>
	/// ヒット判定用関数
	/// <para>ほかのAABB同士で衝突しているかを検知する関数</para>
	/// </summary>
	bool CheckCollision(Transform transform);

	bool CheckCollision(AABB a);

	//bool CheckLineCollision(const Segment& segment);

	//bool CheckModelCollision(LWP::Object::WorldTransform transform1);

	/// <summary>
	/// TransformからAABBを作る関数
	/// </summary>
	void CreateAABB(Transform transform);

	void CreateModelAABB(Transform transform);

	//Vector3 NearestPoint(const Segment& segment);

private: // ** プライベートなメンバ関数（上で宣言した関数内に全部詰め込まないこと） ** //


	int Min(int num1, int num2);

	float Min(float num1, float num2);

	int Max(int num1, int num2);

	float Max(float num1, float num2);
	/// <summary>
	/// ファイルからマップチップのデータを読み取る関数
	/// </summary>
};
//bool CheckLineCollision(Segment a, Segment b);

//float CrossProduct(Vector3 a, Vector3 b);