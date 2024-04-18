#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct TriangleData
{
	VertexData Left_;
	VertexData Top_;
	VertexData Right_;
};

struct SpriteData {
	VertexData LeftTop;
	VertexData RightBot;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	int32_t enablePhong;
	float shininess;
};

struct MaterialData {
	std::string textureFilePath;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};