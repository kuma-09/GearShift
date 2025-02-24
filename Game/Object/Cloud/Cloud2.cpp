#include "pch.h"
#include "Cloud2.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"

Cloud2::Cloud2()
{
	AddComponent<ModelDraw>();
}

Cloud2::~Cloud2()
{
}

void Cloud2::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Cloud2));
}

void Cloud2::Update(float elapsedTime)
{
}
