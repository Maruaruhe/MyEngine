#include "ModelManager.h"

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
	joints.push_back(joint);

	for (const Node& child : node.children) {
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}

	return joint.index;
}