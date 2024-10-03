#pragma once
#include "Framework/Graphics.h"

class UI
{
public:
	UI(const wchar_t* path, DirectX::SimpleMath::Vector2 pos);
	~UI();

	void Initialize();
	void Render();

	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	ID3D11ShaderResourceView* GetTexture() { return m_texture.Get(); }


private:

	Graphics* m_graphics;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	DirectX::SimpleMath::Vector2 m_position;

};