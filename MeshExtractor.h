#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "MeshData.h"
#include <fbxsdk.h>

class MeshExtractor
{
public:
    /// <summary>
    /// �V�[���ɂ���m�[�h���ċA�I�ɏ��񂵃��b�V���������m�[�h���A
    /// �m�[�h�����L�[�Ƃ��ĘA�z�z��ɕۑ�����<para></para>
    /// �Ȃ��A�m�[�h���̏d���͂Ȃ��Ƃ����O��ł���
    /// </summary>
    static void RetrieveMeshList(FbxNode* node, std::unordered_map<std::string, FbxNode*>& meshList);

    /// <summary>
    /// meshList���璸�_,�C���f�b�N�X,�e�N�X�`���t�@�C���������o��meshData�ɕۑ�
    /// </summary>
    static bool ExtractMeshData(std::unordered_map<std::string, FbxNode*>& meshList,
        std::vector<MeshData>& meshData);

    /// <summary>
    /// meshList���璸�_,�C���f�b�N�X,�e�N�X�`���t�@�C���������o��meshData�ɕۑ�
    /// </summary>
    /// <param name="start,stopTime">�A�j���[�V�����̊J�n,�I���̎���</param>
    static bool ExtractAnimMeshData(std::unordered_map<std::string, FbxNode*>& meshList,
        std::vector<AnimMeshData>& meshData, FbxAnimEvaluator* animEval,
        const FbxTime& startTime, const FbxTime& stopTime, float fps = 60.0f);
};
