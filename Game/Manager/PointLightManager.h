#pragma once

#include "Game/Components/PointLight.h"

/// <summary>
/// ポイントライトコンポーネントを管理するマネージャー
/// </summary>
class PointLightManager
{
public:
	// ポイントライトコンポーネントを追加
	static void Add(PointLight* component);
	// 更新処理
	static void Update();
	// コンポーネントを削除
	static void Remove(PointLight* component);
	// ポイントライトの配列を取得
	static std::vector<PointLight*> GetPointLights() { return s_pointLights; }
private:
	// ポイントライト配列
	static std::vector<PointLight*> s_pointLights;
};
