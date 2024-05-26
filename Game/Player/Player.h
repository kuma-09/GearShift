#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"

class Enemy;


class Player
{
public:

	void Initialize();

	void Update(float elapsedTime);

	void Render();

	void Finalize();

	void SetTarget(Enemy* enemy);
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;
	Resources* m_resources;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;


	std::unique_ptr<DirectX::BoundingBox> m_boundingBox;

	const float SPEED_FB = 5.0f;
	const float SPEED_RL = 5.0f;


	DirectX::SimpleMath::Vector3	m_position;
	DirectX::SimpleMath::Quaternion m_quaternion;

	Enemy* m_targetEnemy;
};