#include "Framework/Graphics.h"
#include "Framework/InputManager.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"
#include "Game/Components/IComponent.h"
#include "Particle.h"


class Emitter: public IComponent
{
public:

	Emitter();
	~Emitter();

	void Initialize(const wchar_t* path, float size = 1.0f, float interval = 0.05f);

	void Update(float elapsedTime);

	void Render(DirectX::SimpleMath::Vector3 pos);

	void Finalize();
private:
	Graphics* m_graphics;
	DX::DeviceResources* m_deviceResources;
	InputManager* m_inputManager;
	Resources* m_resources;

	DirectX::VertexPositionColorTexture m_vertices[4];
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;		// テクスチャ

	std::vector<std::unique_ptr<Particle>> m_particles;

	float m_totalTime;
	float m_interval;


};