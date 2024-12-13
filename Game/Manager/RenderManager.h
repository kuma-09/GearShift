#pragma once

class ModelDraw;
class Emitter;
class Trail;

class RenderManager
{
public:
	static void Add(ModelDraw* component);
	static void Add(Emitter* component);
	static void Add(Trail* component);
	static void CreateShadowMap();
	static void RenderObjects();
	static void RenderParticle();
	static void Remove(ModelDraw* component);
	static void Remove(Emitter* component);
	static void Remove(Trail* component);
	static void Clear();
private:
	static std::vector<ModelDraw*> s_modelDraws;
	static std::vector<Emitter*>   s_emitters;
	static std::vector<Trail*>     s_trail;
};
