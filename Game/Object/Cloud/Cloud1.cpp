#include "pch.h"
#include "Cloud1.h"
#include "Framework/Resources.h"
#include "Game/Components/ModelDraw.h"

Cloud1::Cloud1()
{
	AddComponent<ModelDraw>();
}

Cloud1::~Cloud1()
{
}

void Cloud1::Initialize()
{
	GetComponent<ModelDraw>()->Initialize(Resources::GetInstance()->GetModel(Resources::Cloud1));
}

void Cloud1::Update(float elapsedTime)
{
}
