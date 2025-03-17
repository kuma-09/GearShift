#pragma once
#include <string>

/// <summary>
/// �X�e�[�W�f�[�^���Ǘ�����}�l�[�W���[
/// </summary>
class StageDataManager
{
public :
	// �X�e�[�W�i���o�[
	static void SetStageNum(int num) { s_nowStageNum = num; }
	static const int& GetStageNum() { return s_nowStageNum; }

	// �N���A�^�C��
	static void SetClearTime(int time) { s_clearTime = time; }
	static int GetClearTime(){ return s_clearTime; }

	// �X�e�[�W�̃t�@�C���l�[�����擾
	static std::wstring GetStageFileName();
private:
	// �X�e�[�W�i���o�[
	static int s_nowStageNum;
	// �N���A�^�C��
	static int s_clearTime;
};

