#pragma once
// イージング関数
namespace Easing
{
	float Linear(float time, float targetTime);

	float InSine(float time, float targetTime);
	float InQuad(float time, float targetTime);
	float InCubic(float time, float targetTime);
	float InQuart(float time, float targetTime);

	float OutSine(float time, float targetTime);
	float OutQuad(float time, float targetTime);
	float OutCubic(float time, float targetTime);
	float OutQuart(float time, float targetTime);

	float InOutSine(float time, float targetTime);
	float InOutQuad(float time, float targetTime);
	float InOutCubic(float time, float targetTime);
	float InOutQuart(float time, float targetTime);

	DirectX::SimpleMath::Vector3 InOutCubic(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, float t);
	DirectX::SimpleMath::Vector3 InOutQuart(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, float t);
};
