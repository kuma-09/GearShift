#include "pch.h"
#include "HP.h"

HP::HP()
	:m_hp{}
{
}

HP::~HP()
{

}

/// <summary>
/// ‰Šú‰»ˆ—
/// </summary>
/// <param name="hp">‰ŠúHP</param>
void HP::Initialize(int hp)
{
	SetHP(hp);
}

void HP::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}
