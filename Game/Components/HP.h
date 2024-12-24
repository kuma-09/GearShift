#pragma once
#include "IComponent.h"


class HP : public IComponent
{
public:
	HP();
	~HP();

	void Initialize(int hp);
	void Update(float elapsedTime);

	void SetHP(int hp) { m_hp = hp; }
	int  GetHP() { return m_hp; }

private:
	int m_hp;
};

