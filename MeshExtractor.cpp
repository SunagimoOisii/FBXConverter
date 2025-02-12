#include <iostream>

#include "MeshExtractor.h"
#include "Utilities.h"
#include "FileManager.h"
#include "Math.h"

void MeshExtractor::RetrieveMeshList(FbxNode* node, std::unordered_map<std::string, FbxNode*>& meshList)
{
	if (node == nullptr) return;

	for (int i = 0;i < node->GetNodeAttributeCount();i++)
	{
		FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(i);
		if (attr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			meshList[node->GetName()] = node;
		}
	}

	for (int i = 0;i < node->GetChildCount();i++)
	{
		RetrieveMeshList(node->GetChild(i), meshList);
	}
}

bool MeshExtractor::ExtractMeshData(std::unordered_map<std::string, FbxNode*>& meshList, std::vector<MeshData>& meshData)

{
	std::cout << "MeshData Num = " << meshList.size() << std::endl;
	if (meshList.size() == 0) return false;
	meshData.clear();

	//meshList����K�v�ȃ��b�V�������擾
	for (auto i = meshList.begin();i != meshList.end();i++)
	{
		MeshData mData;
		std::cout << "Node Name = " << (*i).first << std::endl;
		FbxNode* node = (*i).second;
		FbxMesh* mesh = node->GetMesh();

		//�e�N�X�`���t�@�C�����̎擾
		std::string tBuff;
		FileManager::ParseFileName(FileManager::RetrieveTextureFilePath(mesh, node), tBuff);
		Utilities::ConvertString(tBuff, mData.textureFileName);
		std::cout << "\tTexture FileName = " << tBuff << std::endl;

		//���_���̎擾(���_���W)
		FbxVector4* vertexBuff = mesh->GetControlPoints();
		for (int i = 0;i < mesh->GetControlPointsCount();i++)
		{
			VertexNormal v{};
			v.pos = XMFLOAT3((float)vertexBuff[i][0], (float)vertexBuff[i][1],
				(float)vertexBuff[i][2]);
			v.uv	 = Utilities::ZeroVec2; //UV���͌�Ŏ擾����������
			v.normal = Utilities::ZeroVec3;

			mData.vertices.push_back(v);
		}

		//���C���[���̎擾(UV���W)
		FbxLayerElementUV* layerUV = mesh->GetLayer(0)->GetUVs();
		FbxGeometryElement::EMappingMode   uvMapping = layerUV->GetMappingMode();
		FbxGeometryElement::EReferenceMode uvRef = layerUV->GetReferenceMode();
		//�}�b�s���O,���t�@�����X�͈ȉ��̂ݑΉ�����
		assert(uvMapping == FbxGeometryElement::eByControlPoint ||
			uvMapping == FbxGeometryElement::eByPolygonVertex);
		assert(uvRef == FbxGeometryElement::eDirect ||
			uvRef == FbxGeometryElement::eIndexToDirect);

		//���C���[���̎擾(�@��)
		FbxLayerElementNormal* layerNormal = mesh->GetLayer(0)->GetNormals();
		FbxGeometryElement::EMappingMode   nMapping = layerNormal->GetMappingMode();
		FbxGeometryElement::EReferenceMode nRef = layerNormal->GetReferenceMode();
		assert(nMapping == FbxGeometryElement::eByControlPoint ||
			nMapping == FbxGeometryElement::eByPolygonVertex);
		assert(nRef == FbxGeometryElement::eDirect ||
			nRef == FbxGeometryElement::eIndexToDirect);

		// �e�|���S���̒��_���m�F���A�ʒu���W����v��UV���W�݈̂قȂ钸�_�������
		// MeshData��vertices�ɐV���Ȓ��_�Ƃ��Ēǉ�����
		std::vector<bool> checkList(mData.vertices.size());
		for (int i = 0;i < checkList.size();i++)
		{
			checkList[i] = false;
		}
		std::vector<std::vector<int>> addedVertex;
		for (int polyIndex = 0;polyIndex < mesh->GetPolygonCount();polyIndex++)
		{
			for (int vIndex = 2;vIndex >= 0;vIndex--) //�O�p�`�|���S���ɍ��킹������
			{
				//���_���擾
				int vertexIndex, uvIndex;
				vertexIndex = mesh->GetPolygonVertex(polyIndex, vIndex);
				if (uvRef == FbxGeometryElement::eDirect)
				{
					uvIndex = vertexIndex;
				}
				else
				{
					uvIndex = mesh->GetTextureUVIndex(polyIndex, vIndex,
						FbxLayerElement::eTextureDiffuse);
				}

				//UV���擾
				//FBX��UV���W�͏�����t�]���Ă��邽�ߑΉ�������
				FbxVector2 uvData = layerUV->GetDirectArray().GetAt(uvIndex);
				XMFLOAT2   uv = { (float)uvData.mData[0],1.0f - (float)uvData.mData[1] };

				//�@�����擾
				int normalIndex;
				if (nRef == FbxGeometryElement::eDirect)
				{
					normalIndex = vertexIndex;
				}
				else
				{
					normalIndex = layerNormal->GetIndexArray().GetAt(vertexIndex);
				}
				FbxVector4 nData = layerNormal->GetDirectArray().GetAt(normalIndex);
				XMFLOAT3 normal = { (float)nData.mData[0],(float)nData.mData[1],
					(float)nData.mData[2] };

				//���m�F�̒��_�Ȃ�擾����UV���W��ݒ肵checkList��true�ɂ���
				if (!checkList[vertexIndex])
				{
					mData.vertices[vertexIndex].uv = uv;
					mData.vertices[vertexIndex].normal = normal;
					checkList[vertexIndex] = true;
				}
				//�m�F�ςݒ��_��UV���W���قȂ�Ȃ�ʓr�������s��
				else if (!Utilities::IsEqualVector2D(mData.vertices[vertexIndex].uv, uv) ||
					!Utilities::IsEqualVector3D(mData.vertices[vertexIndex].normal, normal))
				{
					//�ʒu,UV���W,�@�������Ɉ�v���钸�_������Βǉ��������s��Ȃ�
					bool hit = false;
					for (int i = 0;i < addedVertex.size();i++)
					{
						if (vertexIndex == addedVertex[i][0] &&
							Utilities::IsEqualVector2D(mData.vertices[addedVertex[i][1]].uv, uv) &&
							Utilities::IsEqualVector3D(mData.vertices[addedVertex[i][1]].normal, normal))
						{
							hit = true;
							vertexIndex = addedVertex[i][1];
							break;
						}
					}

					//���_�̒ǉ�����
					if (!hit)
					{
						VertexNormal v = { mData.vertices[vertexIndex].pos,uv,normal };
						mData.vertices.push_back(v);
						std::vector<int> add;
						add.push_back(vertexIndex);
						add.push_back((int)mData.vertices.size() - 1);
						addedVertex.push_back(add);
						vertexIndex = add[1];
					}
				}
				//�C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�o�^
				mData.indices.push_back((unsigned short)vertexIndex);
			}
		}
		std::cout << "\tVertex Num = " << mData.vertices.size() << std::endl;
		std::cout << "\tIndex Num = " << mData.indices.size() << std::endl;
		meshData.push_back(mData);
	}

	return true;
}

bool MeshExtractor::ExtractAnimMeshData(std::unordered_map<std::string, FbxNode*>& meshList, std::vector<AnimMeshData>& meshData, FbxAnimEvaluator* animEval, const FbxTime& startTime, const FbxTime& stopTime, float fps)

{
	std::cout << "MeshData Num = " << meshList.size() << std::endl;
	if (meshList.size() == 0) return false;
	meshData.clear();

	//meshList����K�v�ȃ��b�V�������擾
	for (auto i = meshList.begin();i != meshList.end();i++)
	{
		AnimMeshData mData;
		std::cout << "Node Name = " << (*i).first << std::endl;
		FbxNode* node = (*i).second;
		FbxMesh* mesh = node->GetMesh();

		//�e�N�X�`���t�@�C�����̎擾
		std::string tBuff;
		FileManager::ParseFileName(FileManager::RetrieveTextureFilePath(mesh, node), tBuff);
		Utilities::ConvertString(tBuff, mData.textureFileName);
		std::cout << "\tTexture FileName = " << tBuff << std::endl;

		//���_���̎擾(���_���W)
		FbxVector4* vertexBuff = mesh->GetControlPoints();
		for (int i = 0;i < mesh->GetControlPointsCount();i++)
		{
			VertexAnim v{};
			v.pos = XMFLOAT3((float)vertexBuff[i][0], (float)vertexBuff[i][1],
				(float)vertexBuff[i][2]);
			v.uv	 = Utilities::ZeroVec2; //UV���͌�Ŏ擾����������
			v.normal = Utilities::ZeroVec3;

			//�{�[��ID,�e���l�̏�����
			for (int j = 0;j < ANIM_BONE_MAX_NUM;j++)
			{
				v.boneID[j] = 0;
				v.weight[j] = 0.0f;
			}

			mData.vertices.push_back(v);
		}

		//���C���[���̎擾(UV���W)
		FbxLayerElementUV* layerUV = mesh->GetLayer(0)->GetUVs();
		FbxGeometryElement::EMappingMode   uvMapping = layerUV->GetMappingMode();
		FbxGeometryElement::EReferenceMode uvRef = layerUV->GetReferenceMode();
		//�}�b�s���O,���t�@�����X�͈ȉ��̂ݑΉ�����
		assert(uvMapping == FbxGeometryElement::eByControlPoint ||
			uvMapping == FbxGeometryElement::eByPolygonVertex);
		assert(uvRef == FbxGeometryElement::eDirect ||
			uvRef == FbxGeometryElement::eIndexToDirect);

		//���C���[���̎擾(�@��)
		FbxLayerElementNormal* layerNormal = mesh->GetLayer(0)->GetNormals();
		FbxGeometryElement::EMappingMode   nMapping = layerNormal->GetMappingMode();
		FbxGeometryElement::EReferenceMode nRef = layerNormal->GetReferenceMode();
		assert(nMapping == FbxGeometryElement::eByControlPoint ||
			nMapping == FbxGeometryElement::eByPolygonVertex);
		assert(nRef == FbxGeometryElement::eDirect ||
			nRef == FbxGeometryElement::eIndexToDirect);

		//�{�[�����̎擾
		//�{�[���̌����擾������ɍ��킹�t�o�C���h�s��z���������
		assert(mesh->GetDeformerCount(FbxDeformer::eSkin) == 1);
		FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(0, FbxDeformer::eSkin);
		int fbxclusterNum = skin->GetClusterCount();
		mData.inverseGPoseMatrices.resize(fbxclusterNum);

		std::vector<int> boneNum(mData.vertices.size());
		for (size_t i = 0;i < boneNum.size();i++)
		{
			boneNum[i] = 0;
		}
		for (int i = 0;i < fbxclusterNum;i++)
		{
			FbxCluster* cluster = skin->GetCluster(i);
			int* points = cluster->GetControlPointIndices();
			double* weights = cluster->GetControlPointWeights();
			for (int j = 0;j < cluster->GetControlPointIndicesCount();j++)
			{
				int vIndex = points[j];
				assert(boneNum[vIndex] < ANIM_BONE_MAX_NUM);
				mData.vertices[vIndex].boneID[boneNum[vIndex]] = i;
				mData.vertices[vIndex].weight[boneNum[vIndex]] = (float)weights[j];
				boneNum[vIndex]++;
			}

			FbxAMatrix gPoseMat;
			cluster->GetTransformLinkMatrix(gPoseMat);
			Utilities::CopyMatrix(gPoseMat.Inverse(), mData.inverseGPoseMatrices[i]);
		}

		//�A�j���[�V�����J�n����������Ԋu��
		//�e�t���[���ł̌��|�[�Y�s��z����擾
		float start = (float)startTime.GetSecondDouble();
		float stop = (float)stopTime.GetSecondDouble();
		int frame = 0;
		mData.animData.boneNum = skin->GetClusterCount();
		mData.animData.interval = 1.0f / fps;
		while ((start + frame * mData.animData.interval < stop))
		{
			FbxTime time;
			time.SetSecondDouble((double)start + frame * mData.animData.interval);
			std::vector<XMMATRIX> pMatrices(mData.animData.boneNum);
			for (int i = 0;i < mData.animData.boneNum;i++)
			{
				FbxCluster* cluster = skin->GetCluster(i);
				FbxNode* boneNode = cluster->GetLink();
				Utilities::CopyMatrix(animEval->GetNodeGlobalTransform(boneNode, time), pMatrices[i]);
			}
			mData.poseMatrices.push_back(pMatrices);
			frame++;
		}
		mData.animData.frameNum = (unsigned short)mData.poseMatrices.size();

		//�e�|���S���̒��_���m�F���A�ʒu���W����v��UV���W�݈̂قȂ钸�_�������
		//MeshData��vertices�ɐV���Ȓ��_�Ƃ��Ēǉ�����
		std::vector<std::vector<int>> addedVertex;
		std::vector<bool> checkList(mData.vertices.size());
		for (int i = 0;i < checkList.size();i++)
		{
			checkList[i] = false;
		}
		for (int polyIndex = 0;polyIndex < mesh->GetPolygonCount();polyIndex++)
		{
			for (int vIndex = 2;vIndex >= 0;vIndex--) //�O�p�`�|���S���ɍ��킹������
			{
				//���_���擾
				int vertexIndex, uvIndex;
				vertexIndex = mesh->GetPolygonVertex(polyIndex, vIndex);
				if (uvRef == FbxGeometryElement::eDirect)
				{
					uvIndex = vertexIndex;
				}
				else
				{
					uvIndex = mesh->GetTextureUVIndex(polyIndex, vIndex,
						FbxLayerElement::eTextureDiffuse);
				}

				//UV���擾
				//FBX��UV���W�͏�����t�]���Ă��邽�ߑΉ�������
				FbxVector2 uvData = layerUV->GetDirectArray().GetAt(uvIndex);
				XMFLOAT2   uv = { (float)uvData.mData[0],1.0f - (float)uvData.mData[1] };

				//�@�����擾
				int normalIndex;
				if (nRef == FbxGeometryElement::eDirect)
				{
					normalIndex = vertexIndex;
				}
				else
				{
					normalIndex = layerNormal->GetIndexArray().GetAt(vertexIndex);
				}
				FbxVector4 nData = layerNormal->GetDirectArray().GetAt(normalIndex);
				XMFLOAT3 normal = { (float)nData.mData[0],(float)nData.mData[1],
					(float)nData.mData[2] };

				//���m�F�̒��_�Ȃ�擾����UV���W��ݒ肵checkList��true�ɂ���
				if (!checkList[vertexIndex])
				{
					mData.vertices[vertexIndex].uv = uv;
					mData.vertices[vertexIndex].normal = normal;
					checkList[vertexIndex] = true;
				}
				//�m�F�ςݒ��_��UV���W���قȂ�Ȃ�ʓr�������s��
				else if (!Utilities::IsEqualVector2D(mData.vertices[vertexIndex].uv, uv) ||
					!Utilities::IsEqualVector3D(mData.vertices[vertexIndex].normal, normal))
				{
					//�ʒu,UV���W,�@�������Ɉ�v���钸�_������Βǉ��������s��Ȃ�
					bool hit = false;
					for (int i = 0;i < addedVertex.size();i++)
					{
						if (vertexIndex == addedVertex[i][0] &&
							Utilities::IsEqualVector2D(mData.vertices[addedVertex[i][1]].uv, uv) &&
							Utilities::IsEqualVector3D(mData.vertices[addedVertex[i][1]].normal, normal))
						{
							hit = true;
							vertexIndex = addedVertex[i][1];
							break;
						}
					}

					//���_�̒ǉ�����
					if (!hit)
					{
						VertexAnim v = { mData.vertices[vertexIndex].pos,uv,normal };
						for (int i = 0;i < ANIM_BONE_MAX_NUM;i++)
						{
							v.boneID[i] = mData.vertices[vertexIndex].boneID[i];
							v.weight[i] = mData.vertices[vertexIndex].weight[i];
						}
						mData.vertices.push_back(v);
						std::vector<int> add;
						add.push_back(vertexIndex);
						add.push_back((int)mData.vertices.size() - 1);
						addedVertex.push_back(add);
						vertexIndex = add[1];
					}
				}
				//�C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�o�^
				mData.indices.push_back((unsigned short)vertexIndex);
			}
		}
		std::cout << "\tVertex Num = " << mData.vertices.size() << std::endl;
		std::cout << "\tIndex Num = " << mData.indices.size() << std::endl;
		meshData.push_back(mData);
	}

	return true;
}