#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"


class Particle
{
public:

	void Initialize();

	void Update(float elapsedTime);

	void Render(DirectX::SimpleMath::Matrix wolrd);
	void Render(DirectX::SimpleMath::Vector3 pos);

	void Finalize();
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;
	Resources* m_resources;

	DirectX::VertexPositionColorTexture m_vertices[4];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ

	struct positionLife
	{
		DirectX::SimpleMath::Matrix world;
		float lifeTime;
	};

	std::vector<positionLife> m_positionLife;

};