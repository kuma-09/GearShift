#pragma once
#include "Framework/Graphics.h"
#include "Component.h"

/// <summary>
/// トレイルを生成するコンポーネントクラス
/// </summary>
class Trail : public Component
{
public:
	// コンストラクタ
	Trail();
	// デストラクタ
	~Trail();
	// 初期化処理
	void Initialize(const wchar_t* path,int bufferSize, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	// 更新処理
	void Update(float elapsedTime);
	// 描画処理
	void Render();
	// トレイルを生成する座標を保存
	void SetPos(DirectX::XMFLOAT3 head, DirectX::XMFLOAT3 tail);
	// 保存している座標配列をクリア
	void ClearBuffer();
private:

	// 描画用のプリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// トレイルの座標を保存する構造体
	struct PosBuffer
	{
		DirectX::XMFLOAT3 head; //先端の位置
		DirectX::XMFLOAT3 tail; //末端の位置
	};

	// 3D変換で使う行列とトレイルのカラー
	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
		DirectX::XMVECTOR color;
	};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	// 座標を保存する配列
	std::vector<PosBuffer> m_buffer;
	//	入力レイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// トレイルのカラー
	DirectX::XMVECTORF32 m_color;
	// 保存できる座標の数
	int m_maxBufferSize;
	// 現在保存している座標の数
	int m_bufferCount;
};