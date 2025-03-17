#pragma once

class Graphics;
class ModelDraw;
class Emitter;
class Trail;

/// <summary>
/// 描画するものをまとめて管理するマネージャー
/// </summary>
class RenderManager
{
public:
	// それぞれのコンポーネントを追加
	static void Add(ModelDraw* component);
	static void Add(Emitter* component);
	static void Add(Trail* component);
	// シャドウマップを作成
	static void CreateShadowMap();
	// モデルを描画
	static void RenderObjects();
	// パーティクルを描画
	static void RenderParticle();

	// それぞれのコンポーネントを削除
	static void Remove(ModelDraw* component);
	static void Remove(Emitter* component);
	static void Remove(Trail* component);

	// それぞれのコンポーネントをすべてクリア
	static void Clear();
private:
	// コンポーネント配列
	static std::vector<ModelDraw*> s_modelDraws;
	static std::vector<Emitter*>   s_emitters;
	static std::vector<Trail*>     s_trail;

	// シャドウマップの枚数
	static const int SHADOWMAP_MAX = 4;
};
