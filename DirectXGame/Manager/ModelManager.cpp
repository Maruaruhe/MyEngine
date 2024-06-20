#include "ModelManager.h"
#include "../Object/Sphere/Sphere.h"
#include "../Object/Model/Model.h"

void ModelManager::Initialize() {

}

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;

	return &instance;
}

ModelData ModelManager::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	Assimp::Importer importer;

	std::string filePath = directoryPath + "/" + filename + ".obj";
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex{};
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal.x *= -1.0f;
				vertex.texcoord = { texcoord.x,texcoord.y };

				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
			for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
				aiMaterial* material = scene->mMaterials[materialIndex];
				if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
					aiString textureFilePath;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
					modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
				}
			}
		}
		//SkinCluster構築用のデータ取得
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			//Jointごとの格納領域
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			JointWeightData& jointWeightData = modelData.skinClusterData[jointName];

			//InverseBindPoseMatrixの抽出
			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
			aiVector3D scale, translate;
			aiQuaternion rotate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate);
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix({ scale.x,scale.y,scale.z }, { rotate.x,-rotate.y,-rotate.z,rotate.w }, { -translate.x,translate.y,translate.z });
			jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

			//Weight情報を取り出す
			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight,bone->mWeights[weightIndex].mVertexId });
			}
		}
	}

	return modelData;
}

ModelData ModelManager::LoadModelFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;
	Assimp::Importer importer;

	std::string filePath = directoryPath + "/" + filename + ".gltf";
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes());

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		//
		modelData.vertices.resize(mesh->mNumVertices);
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
			//右手系->左手系への変換
			modelData.vertices[vertexIndex].position = { -position.x,position.y,position.z,1.0f };
			modelData.vertices[vertexIndex].normal = { -normal.x,normal.y,normal.z };
			modelData.vertices[vertexIndex].texcoord = { texcoord.x,texcoord.y };
		}

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				modelData.indices.push_back(vertexIndex);

				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex{};
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x, normal.y, normal.z, };
				vertex.texcoord = { texcoord.x,texcoord.y };

				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
			for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
				aiMaterial* material = scene->mMaterials[materialIndex];
				if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
					aiString textureFilePath;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
					modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
				}
			}
		}
	}
	modelData.rootNode = ReadNode(scene->mRootNode);
	//modelData.skelton = CreateSkelton(modelData.rootNode);

	return modelData;
}

//ModelData ModelManager::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
//	ModelData modelData;
//	std::vector<Vector4> positions;
//	std::vector<Vector3> normals;
//	std::vector<Vector2> texcoords;
//	std::string line;
//
//	std::ifstream file(directoryPath + "/" + filename + ".obj");
//	assert(file.is_open());
//
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier;
//
//		if (identifier == "v") {
//			Vector4 position;
//			s >> position.x >> position.y >> position.z;
//			position.w = 1.0f;
//			positions.push_back(position);
//		}
//		else if (identifier == "vt") {
//			Vector2 texcoord;
//			s >> texcoord.x >> texcoord.y;
//			texcoords.push_back(texcoord);
//		}
//		else if (identifier == "vn") {
//			Vector3 normal;
//			s >> normal.x >> normal.y >> normal.z;
//			normals.push_back(normal);
//		}
//		else if (identifier == "f") {
//			VertexData triangle[3];
//			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
//				std::string vertexDefinition;
//				s >> vertexDefinition;
//
//				std::istringstream v(vertexDefinition);
//				uint32_t elementIndices[3];
//				for (int32_t element = 0; element < 3; ++element) {
//					std::string index;
//					std::getline(v, index, '/');
//					elementIndices[element] = std::stoi(index);
//				}
//				Vector4 position = positions[elementIndices[0] - 1];
//				Vector2 texcoord = texcoords[elementIndices[1] - 1];
//				Vector3 normal = normals[elementIndices[2] - 1];
//				position.x *= -1;
//				normal.x *= -1;
//				texcoord.x = 1.0f - texcoord.x;
//				texcoord.y = 1.0f - texcoord.y;
//				VertexData vertex = { position,texcoord,normal };
//				modelData.vertices.push_back(vertex);
//				triangle[faceVertex] = { position,texcoord,normal };
//			}
//			modelData.vertices.push_back(triangle[2]);
//			modelData.vertices.push_back(triangle[1]);
//			modelData.vertices.push_back(triangle[0]);
//		}
//		else if (identifier == "mtllib") {
//			std::string materialFilename;
//			s >> materialFilename;
//
//			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
//		}
//	}
//	return modelData;
//}

void ModelManager::LoadModel(const std::string& filePath) {
	//読み込み済みモデルを検索
	if (modelDatas.contains(filePath)) {
		return;
	}

	ModelData modelData;
	modelData = LoadModelFile("Resources", filePath);
	//modelData = LoadObjFile("Resources", filePath);

	modelDatas.insert(std::make_pair(filePath, std::move(modelData)));
}

ModelData ModelManager::GetModel(const std::string& filePath) {
	//読み込み済みモデルを検索
	if (modelDatas.contains(filePath)) {
		return modelDatas.at(filePath);
	}
	return {};
}

Node ModelManager::ReadNode(aiNode* node) {
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;
	aiLocalMatrix.Transpose();
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	result.localMatrix.m[0][1] = aiLocalMatrix[0][1];
	result.localMatrix.m[0][2] = aiLocalMatrix[0][2];
	result.localMatrix.m[0][3] = aiLocalMatrix[0][3];	
	
	result.localMatrix.m[1][0] = aiLocalMatrix[1][0];
	result.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	result.localMatrix.m[1][2] = aiLocalMatrix[1][2];
	result.localMatrix.m[1][3] = aiLocalMatrix[1][3];	
	
	result.localMatrix.m[2][0] = aiLocalMatrix[2][0];
	result.localMatrix.m[2][1] = aiLocalMatrix[2][1];
	result.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	result.localMatrix.m[2][3] = aiLocalMatrix[2][3];	
	
	result.localMatrix.m[3][0] = aiLocalMatrix[3][0];
	result.localMatrix.m[3][1] = aiLocalMatrix[3][1];
	result.localMatrix.m[3][2] = aiLocalMatrix[3][2];
	result.localMatrix.m[3][3] = aiLocalMatrix[3][3];

	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	aiVector3D scale, translate;
	aiQuaternion rotate;

	node->mTransformation.Decompose(scale, rotate, translate);
	result.transform.scale = { scale.x,scale.y,scale.z };
	result.transform.rotate = { rotate.x,-rotate.y, -rotate.z, rotate.w };
	result.transform.translate = { -translate.x,translate.y,translate.z };
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

	return result;
}

Skelton ModelManager::CreateSkelton(const Node& rootNode) {
	Skelton skelton;
	skelton.root = CreateJoint(rootNode, {}, skelton.joints);

	for (const Joint& joint : skelton.joints) {
		skelton.jointMap.emplace(joint.name, joint.index);
	}
	return skelton;
}

int32_t ModelManager::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints) {
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeltonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size());
	joint.parent = parent;

	joint.model.Initialize("walk");
	joint.model.transform.scale = { 0.1f,0.1f,0.1f };

	joints.push_back(joint);

	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}

	return joint.index;
}

SkinCluster ModelManager::CreateSkinCluster(const Skelton& skelton, const ModelData& modelData) {
	SkinCluster skinCluster;

	//Palette用のResourceを確保
	skinCluster.paletteResource = DirectX12::GetInstance()->CreateBufferResource(sizeof(WellForGPU) * skelton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette,skelton.joints.size() };
	skinCluster.paletteSrvHandle.first = DirectX12::GetInstance()->GetCPUDescriptorHandle(2);
	skinCluster.paletteSrvHandle.second = DirectX12::GetInstance()->GetGPUDescriptorHandle(2);

	//Palette用のsrvを作成
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skelton.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	DirectX12::GetInstance()->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);

	//influence用のResourceを確保
	skinCluster.influenceResource= DirectX12::GetInstance()->CreateBufferResource(sizeof(VertexInfluence) * modelData.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.mappedInfluence = { mappedInfluence,modelData.vertices.size() };

	//influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	//InverseBindPoseMatrixの保存領域を作成
	skinCluster.inverseBindPoseMatrices.resize(skelton.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentity4x4);

	//ModelDataのSkinCluster情報を解析してinfluenceの中身を埋める
	for (const auto& jointWeight : modelData.skinClusterData) {
		auto it = skelton.jointMap.find(jointWeight.first);
		if (it == skelton.jointMap.end()) {
			continue;
		}
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) {
				if (currentInfluence.weights[index] == 0.0f) {
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}

	}

	return skinCluster;
}