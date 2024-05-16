#pragma once
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <vector>
#include <map>
#include "../../Base/DirextX12/DirectX12.h"
#include <dxcapi.h>
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../Camera/Camera.h"
#include "../../Math/struct.h"	
#include "../Light/Lighting.h"

#include "../Model/Model.h"

#include "../../Base/Input/Input.h"

#pragma comment(lib,"dxcompiler.lib")

struct KeyframeVector3 {
	Vector3 value;
	float time;
};

struct KeyframeQuartenion {
	Quaternion value;
	float time;
};

struct NodeAnimation {
	std::vector<KeyframeVector3> translate;
	std::vector<KeyframeQuartenion> rotate;
	std::vector<KeyframeVector3> scale;
};

struct Animation {
	float duration;
	std::map<std::string, NodeAnimation> nodeAnimations;
};

Animation LoadAnimationFile(const std::string& directoryPath, const std::string& filename);
Vector3 CalculateValue(const std::vector<KeyframeVector3>& keyframes, float time);
Quaternion CalculateValue(const std::vector<KeyframeQuartenion>& keyframes, float time);