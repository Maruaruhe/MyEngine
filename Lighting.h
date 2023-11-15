#pragma once
#include "Vector4.h"

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

class Lighting
{
public:
	//void Initialize();

private:
	DirectionalLight* directionalLight;
};

