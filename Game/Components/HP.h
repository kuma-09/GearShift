#pragma once
#include "Component.h"

/// <summary>
/// HPコンポーネントクラス
/// </summary>
class HP : public Component
{
public:
	// コンストラクタ
	HP();
	// デストラクタ
	~HP();
	// 初期化処理
	void Initialize(int hp);
	// 更新処理
	void Update(float elapsedTime);

	// HPを上書き
	void SetHP(int hp) { m_hp = hp; }
	// 現在のHPを取得
	int  GetHP() { return m_hp; }

private:
	// HP保存変数
	int m_hp;
};

