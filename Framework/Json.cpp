#include "pch.h"
#include "Framework/Json.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

void Json::LoadJsonFile(const std::wstring& fileName, std::vector<std::string>& object, std::vector<DirectX::SimpleMath::Vector3>& pos)
{
        std::stringstream ss;
        std::string graphData;
        std::vector<std::string> m_graphData;


        // 入力ファイルストリーム
        std::ifstream in(fileName, std::ifstream::in);
        // 出力ファイルストリームをオープンする
        in.open(fileName, std::ifstream::in);
        if (!in.is_open()) return;

        // ストリングストリーム
        ss << in.rdbuf();
        // 配列をクリアする
        m_graphData.clear();
        // 改行までの一行分の文字列を取得する
        while (std::getline(ss, graphData))
        {
            // グラフデータから不要な文字を消去する
            graphData.erase(std::remove_if(
                graphData.begin(),
                graphData.end(),
                [](char c) {
                    return (
                        c == '\r' || c == '\t' || c == ' ' || c == '\n' || c == '\"' ||
                        c == '[' || c == ']' || c == '{' || c == '}'
                        );
                }),
                graphData.end()
            );
            if (!graphData.empty())
            {
                // 配列にワードを追加する
                m_graphData.push_back(graphData);
            }
        }
        // 入力ファイルストリームをクローズする
        in.close();
        // 正常終了


        int position;
        // JSONデータをパースする
        for (int index = 0; index < (int)m_graphData.size(); index++)
        {
            std::string objectName;
            float x = 0.0f, y = 0.0f, z = 0.0f;
            // 書式：vertex_number:0,position:x:-40,y:0,z:60
            std::stringstream ss(m_graphData[index].c_str());
            // 頂点番号の文字列を検索する
            position = (int)m_graphData[index].find("Object_Name:");
            // 文字列が見つからない場合
            if (position == std::string::npos)
                continue;
            // "vertex_number:"を削除する
            m_graphData[index].replace(position, strlen("Object_Name:"), "");
            // ",position:x:"を検索する
            position = (int)m_graphData[index].find(",position:x:");
            // ",position:x:"を空文字に置き換える
            m_graphData[index].replace(position, strlen(",position:x:"), " ");
            // ",y:"を探索する
            position = (int)m_graphData[index].find(",y:");
            // ",y:"を空文字に置き換える
            m_graphData[index].replace(position, strlen(",y:"), " ");
            // ",z:"を探索する
            position = (int)m_graphData[index].find(",z:");
            // ",z:"を空文字に置き換える
            m_graphData[index].replace(position, strlen(",z:"), " ");
            ss.clear();
            ss.str(m_graphData[index]);
            // 頂点の座標を取得する
            ss >> objectName >> x >> y >> z;
            object.emplace_back(objectName);
            pos.emplace_back(DirectX::SimpleMath::Vector3{ x,y,z });
        }
}
