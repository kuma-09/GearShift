#include "pch.h"	
#include "GameObject.h"

GameObject::GameObject()
	:
	m_owner{},
	m_typeID{}
{
}

void GameObject::ComponentsUpdate(float elapsedTime)
{
	for (auto&& spComp : m_umComponents)
	{
		spComp.second->Update(elapsedTime);
	}
}

void GameObject::ChildObjectsUpdate(float elapsedTime)
{
	for (auto&& spObje : m_umChildObjects)
	{
		spObje.second->Update(elapsedTime);
	}
}
