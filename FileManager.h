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
    /// テクスチャのファイルパスを取得<para></para>
    /// 同一メッシュに複数のテクスチャが設定されているものには未対応
    /// </summary>
    static const char* RetrieveTextureFilePath(FbxMesh* mesh, FbxNode* node);

    /// <summary>
    /// ファイルパスのディレクトリの区切りまでを省いたファイル名を抽出しfileNameに保存
    /// </summary>
    static void ParseFileName(const char* filePath, std::string& fileName);

    static bool SaveMeshData(const std::vector<MeshData>& meshData, std::string& saveFileName,
        float scale, float rotX, float rotY, float rotZ);

    static bool SaveAnimMeshData(const std::vector<AnimMeshData>& meshData, std::string& saveFileName,
        float scale, float rotX, float rotY, float rotZ);
};