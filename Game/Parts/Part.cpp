#include "pch.h"
#include "Part.h"
#include "Game/Components/HP.h"
#include "Game/Components/ModelDraw.h"

void Part::CreateShadow()
{
	GetComponent<ModelDraw>()->CreateShadow(GetWorld());
}

void Part::Render()
{
	GetComponent<ModelDraw>()->Render(GetWorld(), false);
}
