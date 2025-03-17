#pragma once

class Graphics;
class ModelDraw;
class Emitter;
class Trail;

/// <summary>
/// �`�悷����̂��܂Ƃ߂ĊǗ�����}�l�[�W���[
/// </summary>
class RenderManager
{
public:
	// ���ꂼ��̃R���|�[�l���g��ǉ�
	static void Add(ModelDraw* component);
	static void Add(Emitter* component);
	static void Add(Trail* component);
	// �V���h�E�}�b�v���쐬
	static void CreateShadowMap();
	// ���f����`��
	static void RenderObjects();
	// �p�[�e�B�N����`��
	static void RenderParticle();

	// ���ꂼ��̃R���|�[�l���g���폜
	static void Remove(ModelDraw* component);
	static void Remove(Emitter* component);
	static void Remove(Trail* component);

	// ���ꂼ��̃R���|�[�l���g�����ׂăN���A
	static void Clear();
private:
	// �R���|�[�l���g�z��
	static std::vector<ModelDraw*> s_modelDraws;
	static std::vector<Emitter*>   s_emitters;
	static std::vector<Trail*>     s_trail;

	// �V���h�E�}�b�v�̖���
	static const int SHADOWMAP_MAX = 4;
};
