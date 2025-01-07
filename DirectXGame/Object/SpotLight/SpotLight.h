#pragma once
#include <d3d12.h>
#include "../../Base/DirextX12/DirectX12.h"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

struct Spot {
	Vector4 color;
	Vector3 position;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
	float padding[2];
};

class SpotLight
{
public:
	/// <summary>
	/// Initialize
	/// </summary>
	void Initialize();
	/// <summary>
	/// Update
	/// </summary>
	void Update();

	Spot* light_;
	Microsoft::WRL::ComPtr<ID3D12Resource> spotLightResource_;
private:

};

