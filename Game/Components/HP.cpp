#include "pch.h"
#include "HP.h"

HP::HP()
{

}

HP::~HP()
{

}

void HP::Initialize(int hp)
{
	SetHP(hp);
}

void HP::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}
