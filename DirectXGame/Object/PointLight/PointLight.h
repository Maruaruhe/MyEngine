#pragma once
#include <d3d12.h>
#include "../../Base/DirextX12/DirectX12.h"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

struct Point {
	Vector4 color;
	Vector3 position;
	float intensity;
};

class PointLight
{
public:
	void Initialize();
	void Update();

	Point* light;
	Microsoft::WRL::ComPtr<ID3D12Resource> pointLightResource;
private:

};

