#pragma once

/// <summary>
/// Json�t�@�C����������擾����֐�
/// </summary>
namespace Json
{
	// �I�u�W�F�N�g�̖��O�ƍ��W���擾����
	void LoadJsonFile(const std::wstring& fileName,std::vector<std::string>& object, std::vector<DirectX::SimpleMath::Vector3>& pos);
}