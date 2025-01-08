#pragma once
#include <string>

class StageDataManager
{
public :
	static void SetStageNum(int num);
	static void SetClearTime(int time);
	static int GetClearTime();
	static std::wstring GetStageFileName();
private:
	static int s_nowStageNum;
	static int s_clearTime;
};

