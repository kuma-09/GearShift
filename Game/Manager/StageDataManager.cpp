#include "pch.h"
#include "StageDataManager.h"

int StageDataManager::s_nowStageNum;
int StageDataManager::s_clearTime;

// ステージのファイルネームを取得
std::wstring StageDataManager::GetStageFileName()
{
	std::wstring fileName = L"Stage" + std::to_wstring(s_nowStageNum) + L".json";
	return fileName;
}
