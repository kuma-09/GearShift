#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"

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
	int m_number;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	DirectX::SimpleMath::Vector2 m_pos;

	//�����̃T�C�Y
	RECT m_size;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_bulletTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_comboTexture;

	float m_alpha;
	int m_digit;
};