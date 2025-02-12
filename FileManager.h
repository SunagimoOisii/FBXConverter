#pragma once
#include <string>
#include <vector>
#include "MeshData.h"

extern const char* SRC_FILEPATH;
extern const char* DST_FILEPATH;

class FileManager
{
public:
    /// <summary>
    /// �e�N�X�`���̃t�@�C���p�X���擾<para></para>
    /// ���ꃁ�b�V���ɕ����̃e�N�X�`�����ݒ肳��Ă�����̂ɂ͖��Ή�
    /// </summary>
    static const char* RetrieveTextureFilePath(FbxMesh* mesh, FbxNode* node);

    /// <summary>
    /// �t�@�C���p�X�̃f�B���N�g���̋�؂�܂ł��Ȃ����t�@�C�����𒊏o��fileName�ɕۑ�
    /// </summary>
    static void ParseFileName(const char* filePath, std::string& fileName);

    static bool SaveMeshData(const std::vector<MeshData>& meshData, std::string& saveFileName,
        float scale, float rotX, float rotY, float rotZ);

    static bool SaveAnimMeshData(const std::vector<AnimMeshData>& meshData, std::string& saveFileName,
        float scale, float rotX, float rotY, float rotZ);
};