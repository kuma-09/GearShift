#pragma once
#include "Framework/Graphics.h"

class UI
{
public:
	UI();
	virtual ~UI() = default;

	virtual void Initialize() = 0;
	virtual void Render() = 0;

	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	void SetTexture(wchar_t path);
	ID3D11ShaderResourceView* GetTexture() { return m_texture.Get(); }


private:

	Graphics* m_graphics;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

};