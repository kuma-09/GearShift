#include "pch.h"
#include "Easing.h"

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


	float InOutSine(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return -(cos(PI * time) - 1) / 2;
	}

	float InOutQuad(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return time < 0.5 ? 2 * time * time : 1 - pow(-2 * time + 2, 2) / 2;
	}

	float InOutCubic(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return time < 0.5 ? 4 * time * time * time : 1 - pow(-2 * time + 2, 3) / 2;
	}

	float InOutQuart(float time, float targetTime)
	{
		if (targetTime < time) return 1;
		time /= targetTime;
		return time < 0.5 ? 8 * time * time * time * time : 1 - pow(-2 * time + 2, 4) / 2;
	}
}
