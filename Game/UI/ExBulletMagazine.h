#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"

class ExBulletMagazine
{
public:
	ExBulletMagazine();
	~ExBulletMagazine();
	void SetSpriteBatch(DirectX::SpriteBatch* spriteBatch);
	void Initialize(int tagertNumber);
	void Update();
	void Render();
private:
	int m_number;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;

	//êîéöÇÃÉTÉCÉY
	RECT m_size;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bulletTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_comboTexture;

	int digit;
};