#include "pch.h"
#include "StageDataManager.h"

int StageDataManager::s_nowStageNum;
int StageDataManager::s_clearTime;

// �X�e�[�W�̃t�@�C���l�[�����擾
std::wstring StageDataManager::GetStageFileName()
{
	std::wstring fileName = L"Stage" + std::to_wstring(s_nowStageNum) + L".json";
	return fileName;
}
