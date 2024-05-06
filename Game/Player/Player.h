#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"

class Enemy;


class Player
{
public:

	void Initialize();

	void Update();

	void Render();

	void Finalize();

	void SetTarget(Enemy* enemy);
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;
	Resources* m_resources;
	


	std::unique_ptr<DirectX::BoundingBox> m_boundingBox;

	const float SPEED_FB = 0.1f;
	const float SPEED_RL = 0.05f;

	float m_angle;

	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_center;

	Enemy* m_targetEnemy;
};