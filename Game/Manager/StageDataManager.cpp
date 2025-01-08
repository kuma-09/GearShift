#include "pch.h"
#include "StageDataManager.h"

int StageDataManager::s_nowStageNum;
int StageDataManager::s_clearTime;

void StageDataManager::SetStageNum(int num)
{
	s_nowStageNum = num;
}

void StageDataManager::SetClearTime(int time)
{
	s_clearTime = time;
}

int StageDataManager::GetClearTime()
{
	return s_clearTime;
}

std::wstring StageDataManager::GetStageFileName()
{
	std::wstring fileName = L"Stage" + std::to_wstring(s_nowStageNum) + L".json";
	return fileName;
}
