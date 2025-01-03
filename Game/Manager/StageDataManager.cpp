#include "pch.h"
#include "StageDataManager.h"

int StageDataManager::s_nowStageNum;

void StageDataManager::SetStageNum(int Num)
{
	s_nowStageNum = Num;
}

std::wstring StageDataManager::GetStageFileName()
{
	std::wstring fileName = L"Stage" + std::to_wstring(s_nowStageNum) + L".json";
	return fileName;
}
