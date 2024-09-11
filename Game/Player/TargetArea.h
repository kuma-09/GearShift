#pragma once
#include "Framework/Graphics.h"
#include "Game/GameObject.h"

class Player;

class TargetArea
{
public:
	TargetArea();
	~TargetArea();

	void Initialize();
	bool Update(Player* plaer ,GameObject* target);
	void Render();
	void Finalize();

private:

	Graphics* m_graphics;

	float m_range;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
};

