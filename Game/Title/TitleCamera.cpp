#include "pch.h"
#include "TitleCamera.h"
#include "Game/Components/Look.h"

TitleCamera::TitleCamera()
{
	AddComponent<Look>();
	GetComponent<Look>()->Initialize();
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Initialize(GameObject* target)
{
	GetComponent<Look>()->SetTarget(this,target);
}

void TitleCamera::Update(float elapsedTime)
{
}
