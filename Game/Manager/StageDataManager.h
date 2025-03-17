#pragma once
#include <string>

/// <summary>
/// ステージデータを管理するマネージャー
/// </summary>
class StageDataManager
{
public :
	// ステージナンバー
	static void SetStageNum(int num) { s_nowStageNum = num; }
	static const int& GetStageNum() { return s_nowStageNum; }

	// クリアタイム
	static void SetClearTime(int time) { s_clearTime = time; }
	static int GetClearTime(){ return s_clearTime; }

	// ステージのファイルネームを取得
	static std::wstring GetStageFileName();
private:
	// ステージナンバー
	static int s_nowStageNum;
	// クリアタイム
	static int s_clearTime;
};

