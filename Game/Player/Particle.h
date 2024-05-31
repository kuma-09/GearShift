#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"


class Particle
{
public:

	void Initialize(DirectX::SimpleMath::Vector3 pos);

	void Update(float elapsedTime);

	void Render(
		DirectX::VertexPositionColorTexture* v1,
		DirectX::VertexPositionColorTexture* v2,
		DirectX::VertexPositionColorTexture* v3,
		DirectX::VertexPositionColorTexture* v4,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture
	);

	void Finalize();
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;
	Resources* m_resources;


	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Vector3 m_pos;
	float m_lifeTime;

};