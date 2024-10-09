#pragma once
#include "Framework/Graphics.h"
#include "Framework/DeviceResources.h"
#include "Framework/Resources.h"

class HitEffect
{
public:



	void Set(DirectX::SimpleMath::Vector3 pos);

	void Update(float elapsedTime);

	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void Finalize();

private:
	Graphics* m_graphics;
	
	std::unique_ptr<ID3D11ShaderResourceView> m_texture;

	std::vector<DirectX::SimpleMath::Vector3> m_positions;
	std::vector<float> m_lifeTimes;
	const float MAX_LIFETIME = 9;

};