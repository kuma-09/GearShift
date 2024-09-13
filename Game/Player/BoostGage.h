#pragma once

class BoostGage
{
public:
	BoostGage();
	~BoostGage();

	void Render();

	float GetBoostPoint() { return m_boostPoint; }
	void UseBoostPoint(float usePoint) { m_boostPoint -= usePoint; }

private:
	float m_boostPoint;
};
