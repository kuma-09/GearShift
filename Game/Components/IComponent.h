#pragma once

class GameObject;

/// <summary>
/// GameObjectクラスに追加するコンポーネントの基底クラス
/// </summary>
class IComponent
{
public:
	// コンストラクタ
	IComponent() {};
	// デストラクタ
	virtual ~IComponent() = default;

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 親を指定
	void SetOwner(GameObject* a_pObject){ m_pOwner = a_pObject;}

	// 親を取得
	GameObject* GetOwner() { return m_pOwner; }

private:

	// 親
	GameObject* m_pOwner = nullptr;
};
