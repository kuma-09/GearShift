#pragma once
#include "Framework/Graphics.h"

class HPUI
{
public:
	HPUI();
	~HPUI();

	void Initialize();
	void Update(std::vector<float> HP);
	void Render();

private:

	Graphics* m_graphics;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Ltexture;

	float m_colorR[6];
	float m_colorG[6];

};

