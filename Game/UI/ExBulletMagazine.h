#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Number.h"

class ExBulletMagazine
{
public:
	ExBulletMagazine();
	~ExBulletMagazine();
	void SetSpriteBatch(DirectX::SpriteBatch* spriteBatch);
	void Initialize(int number);
	void Update(float elapsedTime, int number);
	void Render(bool isActive);
private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;

	//êîéöÇÃÉTÉCÉY
	RECT m_size;

	std::unique_ptr<Number> m_number;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bulletTexture;

	float m_alpha;
	int m_digit;
};