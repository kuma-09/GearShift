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


        // ���̓t�@�C���X�g���[��
        std::ifstream in(fileName, std::ifstream::in);
        // �o�̓t�@�C���X�g���[�����I�[�v������
        in.open(fileName, std::ifstream::in);
        if (!in.is_open()) return;

        // �X�g�����O�X�g���[��
        ss << in.rdbuf();
        // �z����N���A����
        m_graphData.clear();
        // ���s�܂ł̈�s���̕�������擾����
        while (std::getline(ss, graphData))
        {
            // �O���t�f�[�^����s�v�ȕ�������������
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
                // �z��Ƀ��[�h��ǉ�����
                m_graphData.push_back(graphData);
            }
        }
        // ���̓t�@�C���X�g���[�����N���[�Y����
        in.close();
        // ����I��


        int position;
        // JSON�f�[�^���p�[�X����
        for (int index = 0; index < (int)m_graphData.size(); index++)
        {
            std::string objectName;
            float x = 0.0f, y = 0.0f, z = 0.0f;
            // �����Fvertex_number:0,position:x:-40,y:0,z:60
            std::stringstream ss(m_graphData[index].c_str());
            // ���_�ԍ��̕��������������
            position = (int)m_graphData[index].find("Object_Name:");
            // �����񂪌�����Ȃ��ꍇ
            if (position == std::string::npos)
                continue;
            // "vertex_number:"���폜����
            m_graphData[index].replace(position, strlen("Object_Name:"), "");
            // ",position:x:"����������
            position = (int)m_graphData[index].find(",position:x:");
            // ",position:x:"���󕶎��ɒu��������
            m_graphData[index].replace(position, strlen(",position:x:"), " ");
            // ",y:"��T������
            position = (int)m_graphData[index].find(",y:");
            // ",y:"���󕶎��ɒu��������
            m_graphData[index].replace(position, strlen(",y:"), " ");
            // ",z:"��T������
            position = (int)m_graphData[index].find(",z:");
            // ",z:"���󕶎��ɒu��������
            m_graphData[index].replace(position, strlen(",z:"), " ");
            ss.clear();
            ss.str(m_graphData[index]);
            // ���_�̍��W���擾����
            ss >> objectName >> x >> y >> z;
            object.emplace_back(objectName);
            pos.emplace_back(DirectX::SimpleMath::Vector3{ x,y,z });
        }
}
