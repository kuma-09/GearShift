#include "pch.h"
#include "HP.h"

// �R���X�g���N�^
HP::HP()
	:m_hp{}
{
}

// �f�X�g���N�^
HP::~HP()
{

}

/// <summary>
/// ����������
/// </summary>
/// <param name="hp">����HP</param>
void HP::Initialize(int hp)
{
	SetHP(hp);
}

void HP::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}
