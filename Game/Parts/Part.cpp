#include "pch.h"
#include "Part.h"
#include "Game/Components/HP.h"
#include "Game/Components/ModelDraw.h"

void Part::CreateShadow()
{
}

void Part::Render()
{
	GetComponent<ModelDraw>()->Render(false);
}
