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
    /// シーンにあるノードを再帰的に巡回しメッシュ情報を持つノードを、
    /// ノード名をキーとして連想配列に保存する<para></para>
    /// なお、ノード名の重複はないという前提である
    /// </summary>
    static void RetrieveMeshList(FbxNode* node, std::unordered_map<std::string, FbxNode*>& meshList);

    /// <summary>
    /// meshListから頂点,インデックス,テクスチャファイル名を取り出しmeshDataに保存
    /// </summary>
    static bool ExtractMeshData(std::unordered_map<std::string, FbxNode*>& meshList,
        std::vector<MeshData>& meshData);

    /// <summary>
    /// meshListから頂点,インデックス,テクスチャファイル名を取り出しmeshDataに保存
    /// </summary>
    /// <param name="start,stopTime">アニメーションの開始,終了の時刻</param>
    static bool ExtractAnimMeshData(std::unordered_map<std::string, FbxNode*>& meshList,
        std::vector<AnimMeshData>& meshData, FbxAnimEvaluator* animEval,
        const FbxTime& startTime, const FbxTime& stopTime, float fps = 60.0f);
};
