#include "pch.h"
#include "Cloud3.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"

Cloud3::Cloud3()
{
	AddComponent<ModelDraw>();
}

Cloud3::~Cloud3()
{
}

void Cloud3::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Cloud3));
}

void Cloud3::Update(float elapsedTime)
{
}
