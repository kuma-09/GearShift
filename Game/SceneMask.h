#pragma once
#include "Framework/Graphics.h"

class SceneMask
{
public:

	SceneMask();

	bool Update(float elapsedTime);
	void Render();

	const bool IsOpen() { return m_isOpen; }
	const bool IsClose() { return m_isClose; }

	void Open() { m_isOpen = true;   m_alpha = 1.f; }
	void Close() { m_isClose = true; m_alpha = 0.f; }


private:
	bool m_isOpen;
	bool m_isClose;
	float m_alpha;
	float m_windowSize;

	Graphics* m_graphics;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

};