#include <fstream>

#include <fbxsdk.h>
using namespace fbxsdk;
#pragma comment(lib,"libfbxsdk-md.lib")

#include "FileManager.h"
#include "Utilities.h"

const char* SRC_FILEPATH = "src\\";
const char* DST_FILEPATH = "dst\\";

const char* FileManager::RetrieveTextureFilePath(FbxMesh* mesh, FbxNode* node)
{
	for (int i = 0;i < node->GetMaterialCount();i++)
	{
		FbxSurfaceMaterial* material = node->GetMaterial(i);
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		for (int j = 0;j < prop.GetSrcObjectCount<FbxFileTexture>();j++)
		{
			FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(j);
			if (!texture) continue;
			return texture->GetRelativeFileName();
		}
	}

	return "White";
}

void FileManager::ParseFileName(const char* filePath, std::string& fileName)
{
	fileName.clear();
	fileName = filePath;

	int last = (int)fileName.find_last_of('\\');
	if (last == std::string::npos) return;
	fileName = fileName.substr(last + 1, fileName.size() - last - 1);
}

bool FileManager::SaveMeshData(const std::vector<MeshData>& meshData,  std::string& saveFileName, float scale, float rotX, float rotY, float rotZ)
{
	saveFileName = DST_FILEPATH + saveFileName;
	std::ofstream fout;
	fout.open(saveFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!fout) return false;

	//���b�V���̌���ۑ�(4�o�C�g)
	unsigned int meshNum = (unsigned int)meshData.size();
	fout.write((char*)&meshNum, sizeof(unsigned int));

	//���f���f�[�^�����p�ϊ��s��̎Z�o(�X�P�[��,��]�������{���p)
	XMMATRIX transform = XMMatrixScaling(scale, -scale, scale);
	transform *= XMMatrixRotationX(XMConvertToRadians(rotX));
	transform *= XMMatrixRotationY(XMConvertToRadians(rotY));
	transform *= XMMatrixRotationZ(XMConvertToRadians(rotZ));

	//���b�V���f�[�^�̕ۑ�
	for (size_t i = 0;i < meshData.size();i++)
	{
		//���_�̌���ۑ�(4�o�C�g)
		unsigned int vertexNum = (unsigned int)meshData[i].vertices.size();
		fout.write((char*)&vertexNum, sizeof(unsigned int));

		//�C���f�b�N�X�̌���ۑ�(4�o�C�g)
		unsigned int indexNum = (unsigned int)meshData[i].indices.size();
		fout.write((char*)&indexNum, sizeof(unsigned int));

		//���_�f�[�^�̕ۑ�(sizeof(VertexNormal) * vertexNum�o�C�g)
		fout.write((char*)meshData[i].vertices.data(), sizeof(VertexNormal) * vertexNum);

		//�C���f�b�N�X�f�[�^�̕ۑ�(sizeof(unsigned short) * indexNum�o�C�g)
		fout.write((char*)meshData[i].indices.data(), sizeof(unsigned short) * indexNum);

		//�e�N�X�`���t�@�C������ۑ�(���C�h����256������)
		wchar_t textureNameBuff[256];
		size_t nameSize = meshData[i].textureFileName.size();
		nameSize = (nameSize >= 255) ? 255 : nameSize;
		meshData[i].textureFileName.copy(textureNameBuff, nameSize);
		textureNameBuff[nameSize] = '\0';
		fout.write((char*)textureNameBuff, sizeof(wchar_t) * 256);
	}

	//�����p�ϊ��s������b�V���f�[�^�̌�ɕۑ�
	std::vector<float> adjust;
	Utilities::SetMatrixToContainer(transform, adjust);
	fout.write((char*)adjust.data(), sizeof(float) * adjust.size());

	fout.close();

	return true;
}

bool FileManager::SaveAnimMeshData(const std::vector<AnimMeshData>& meshData, std::string& saveFileName, float scale, float rotX, float rotY, float rotZ)
{
	saveFileName = DST_FILEPATH + saveFileName;
	std::ofstream fout;
	fout.open(saveFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	if (!fout) return false;

	//���b�V���̌���ۑ�(4�o�C�g)
	unsigned int meshNum = (unsigned int)meshData.size();
	fout.write((char*)&meshNum, sizeof(unsigned int));

	//���f���f�[�^�����p�ϊ��s��̎Z�o(�X�P�[��,��]�������{���p)
	XMMATRIX transform = XMMatrixScaling(scale, -scale, scale);
	transform *= XMMatrixRotationX(XMConvertToRadians(rotX));
	transform *= XMMatrixRotationY(XMConvertToRadians(rotY));
	transform *= XMMatrixRotationZ(XMConvertToRadians(rotZ));

	//���b�V���f�[�^�̕ۑ�
	for (size_t i = 0;i < meshData.size();i++)
	{
		//���_�̌���ۑ�(4�o�C�g)
		unsigned int vertexNum = (unsigned int)meshData[i].vertices.size();
		fout.write((char*)&vertexNum, sizeof(unsigned int));

		//�C���f�b�N�X�̌���ۑ�(4�o�C�g)
		unsigned int indexNum = (unsigned int)meshData[i].indices.size();
		fout.write((char*)&indexNum, sizeof(unsigned int));

		//���_�f�[�^�̕ۑ�(sizeof(VertexAnim) * vertexNum�o�C�g)
		fout.write((char*)meshData[i].vertices.data(), sizeof(VertexAnim) * vertexNum);

		//�C���f�b�N�X�f�[�^�̕ۑ�(sizeof(unsigned short) * indexNum�o�C�g)
		fout.write((char*)meshData[i].indices.data(), sizeof(unsigned short) * indexNum);

		//�A�j���[�V�����֘A�̕ۑ�
		std::vector<float> inverseMatrices;
		for (size_t j = 0;j < meshData[i].animData.boneNum;j++)
		{
			Utilities::SetMatrixToContainer(meshData[i].inverseGPoseMatrices[j], inverseMatrices);
		}
		std::vector<float> poseMatrices;
		for (size_t j = 0;j < meshData[i].animData.frameNum;j++)
		{
			for (size_t k = 0;k < meshData[i].animData.boneNum;k++)
			{
				Utilities::SetMatrixToContainer(meshData[i].poseMatrices[j][k], poseMatrices);
			}
		}
		//AnimData�\����,�t�o�C���h�|�[�Y�s��,���|�[�Y�s��̏��ɕۑ�
		fout.write((char*)&(meshData[i].animData), sizeof(AnimData));
		fout.write((char*)inverseMatrices.data(), sizeof(float) * inverseMatrices.size());
		fout.write((char*)poseMatrices.data(), sizeof(float) * poseMatrices.size());

		//�e�N�X�`���t�@�C������ۑ�(���C�h����256������)
		wchar_t textureNameBuff[256];
		size_t nameSize = meshData[i].textureFileName.size();
		nameSize = (nameSize >= 255) ? 255 : nameSize;
		meshData[i].textureFileName.copy(textureNameBuff, nameSize);
		textureNameBuff[nameSize] = '\0';
		fout.write((char*)textureNameBuff, sizeof(wchar_t) * 256);
	}

	//�����p�ϊ��s������b�V���f�[�^�̌�ɕۑ�
	std::vector<float> adjust;
	Utilities::SetMatrixToContainer(transform, adjust);
	fout.write((char*)adjust.data(), sizeof(float) * adjust.size());

	fout.close();

	return true;
}