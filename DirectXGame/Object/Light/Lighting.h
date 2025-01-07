#pragma once
#include <d3d12.h>
#include "../../Base/DirextX12/DirectX12.h"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

class Lighting
{
public:
	void Initialize();
	void Update();

	//std::unique_ptr<DirectionalLight >light;
	DirectionalLight* light;
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;
private:

};

