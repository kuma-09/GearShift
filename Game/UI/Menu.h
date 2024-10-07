#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"

class Menu
{
public:
	Menu(const wchar_t* path, DirectX::SimpleMath::Vector2 pos);
	~Menu();

	void Initialize();
	void Update();
	void Render();

	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	ID3D11ShaderResourceView* GetTexture() { return m_windowTexture.Get(); }


private:

	Graphics* m_graphics;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_windowTexture;
	DirectX::SimpleMath::Vector2 m_position;

};