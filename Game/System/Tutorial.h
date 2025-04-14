#pragma once
#include "Game/UI/UI.h"

class Tutorial
{
public:
	Tutorial();
	~Tutorial();
	void Initialize(int stageNum);
	void Update(float elapsedTime);
	void Render();

private:
	std::unique_ptr<UI> m_ui;
	int   m_tutorialNum;
	const int   MAX_TUTORIAL_NUM = 4;
	float m_totalTime;

	// �\�������܂ł̎���
	const float TARGETTIME = 1.0f;
	// �\�����I���܂ł̎���
	const float RESIDUALTIME = 4.0f;
	// ���v�̎���
	const float RETURNTIME = 5.0f;

	// �`���[�g���A����\������X�e�[�W
	const int TUTORIAL_STAGENUM = 1;

	float m_posX;
	float m_reversePosX;

	bool m_isActive;
};
