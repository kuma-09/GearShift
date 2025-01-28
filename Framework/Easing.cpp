#include "pch.h"
#include "Easing.h"
#include <cmath>
#include <algorithm>

#define PI 3.141592653589793 // ‰~Žü—¦

namespace Easing
{
	float Linear(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		return time / targetTime;
	}

	float InSine(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return 1 - cos((time * PI) / 2);
	}

	float InQuad(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return time * time;
	}

	float InCubic(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return time * time * time;
	}

	float InQuart(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return time * time * time * time;
	}


	float OutSine(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return sin((time * PI) / 2);
	}

	float OutQuad(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return 1 - (1 - time) * (1 - time);
	}

	float OutCubic(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return 1 - pow(1 - time, 3);
	}

	float OutQuart(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return 1 - pow(1 - time, 4);
	}


	float InOutSine(float time, float targetTime,bool isReturn)
	{
		if (targetTime < time && !isReturn) return 1;
		time /= targetTime;
		return -(cos(PI * time) - 1) / 2;
	}

	float InOutQuad(float time, float targetTime, bool isReturn)
	{
		if (targetTime < time && !isReturn) return 1;

		time /= targetTime;
		return time < 0.5 ? 2 * time * time : 1 - pow(-2 * time + 2, 2) / 2;
	}

	float InOutCubic(float time, float targetTime, bool isReturn)
	{
		if (targetTime < time && !isReturn) return 1;
		time /= targetTime;
		return time < 0.5 ? 4 * time * time * time : 1 - pow(-2 * time + 2, 3) / 2;
	}

	float InOutQuart(float time, float targetTime, bool isReturn)
	{
		if (targetTime < time && !isReturn) return 1;
		time /= targetTime;
		return time < 0.5 ? 8 * time * time * time * time : 1 - pow(-2 * time + 2, 4) / 2;
	}

	DirectX::SimpleMath::Vector3 InOutCubic(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, float t)
	{
		t = std::max(std::min(1.0f,t),0.0f);
		if (t < 0.5f) {
			t = 4 * t * t * t; // ‘O”¼
		}
		else {
			t = (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; // Œã”¼
		}
		return DirectX::SimpleMath::Vector3::Lerp(start, end, t);
	}

	DirectX::SimpleMath::Vector3 InOutQuart(const DirectX::SimpleMath::Vector3& start, const DirectX::SimpleMath::Vector3& end, float t) {
		t = std::max(std::min(1.0f, t), 0.0f);
		if (t < 0.5f) {
			t = 8 * t * t * t * t; // ‘O”¼
		}
		else {
			t = 1 - std::pow(-2 * t + 2, 4) / 2; // Œã”¼
		}
		return DirectX::SimpleMath::Vector3::Lerp(start, end, t);
	}
}
