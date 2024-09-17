#include "pch.h"	
#include "GameObject.h"

void GameObject::UpdateWorld()
{
	using namespace DirectX::SimpleMath;

	Matrix world = Matrix::CreateScale(GetScale());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	world *= Matrix::CreateTranslation(GetPosition());

	SetWorld(world);
}

void GameObject::ComponentsUpdate(float elapsedTime)
{
	for (auto&& spComp : m_umComponents)
	{
		spComp.second->Update(elapsedTime);
	}
}