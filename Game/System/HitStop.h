#pragma once
class HitStop
{
public:
	static void Update(float elapsedTime) { s_time -= elapsedTime; };
	static void SetStopTime(float time) { s_time = time; }
	static bool GetIsStop() { return s_time > 0 ? true : false; }
private:
	static float s_time;
};