#pragma once
#include <string>

class StageDataManager
{
public :
	static void SetStageNum(int Num);
	static std::wstring GetStageFileName();
private:
	static int s_nowStageNum;
};

