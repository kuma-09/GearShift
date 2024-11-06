#include "pch.h"
#include "UI.h"
#include "Framework/BinaryFile.h"
#include "Framework/DeviceResources.h"

///	<summary>
///	インプットレイアウト
///	</summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> UI::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

UI::UI(const wchar_t* path, DirectX::SimpleMath::Vector2 pos)
{
    using namespace DirectX;


    m_graphics = Graphics::GetInstance();
    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    

    // 画像をロードする
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(
            device,
            path,
            nullptr,
            m_texture.ReleaseAndGetAddressOf()
        )
    );

    m_position = pos;

	//	コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/UIVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/UIGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/UIPS.cso");

	//	インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	//	頂点シェーダ作成
	device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf());
    
    // ジオメトリシェーダ作成
    device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf());

	//	ピクセルシェーダ作成
    (device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf()));
}

UI::~UI()
{

}

void UI::Initialize()
{

}

void UI::Render()
{
	using namespace DirectX;
    m_spriteBatch->Begin();
    m_spriteBatch->Draw(GetTexture(),m_position);
    m_spriteBatch->End();

	//ID3D11DeviceContext1* context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	////	頂点情報(板ポリゴンの４頂点の座標情報）
	//VertexPositionColorTexture vertex[4] =
	//{
	//	VertexPositionColorTexture(SimpleMath::Vector3(0.0f,  0.0f, 0.0f),SimpleMath::Vector4::One,SimpleMath::Vector2(0.0f, 0.0f)),
	//};

	////	シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	//ConstBuffer cbuff;
	//	cbuff.matView = SimpleMath::Matrix::Identity;
	//	cbuff.matProj = SimpleMath::Matrix::Identity;
	//	cbuff.matWorld = SimpleMath::Matrix::Identity;
	//cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	////	受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	//context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	////	シェーダーにバッファを渡す
	//ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//context->VSSetConstantBuffers(0, 1, cb);
	//context->GSSetConstantBuffers(0, 1, cb);
	//context->PSSetConstantBuffers(0, 1, cb);

	////	画像用サンプラーの登録
	//ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	//context->PSSetSamplers(0, 1, sampler);

	////	半透明描画指定
	//ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	////	透明判定処理
	//context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	////	深度バッファに書き込み参照する
	//context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	////	カリングは左周り
	//context->RSSetState(m_states->CullNone());

	////	シェーダをセットする
	//context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	//context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	//context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	////	ピクセルシェーダにテクスチャを登録する。
	//for (int i = 0; i < m_texture.size(); i++)
	//{
	//	context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	//}

	////	インプットレイアウトの登録
	//context->IASetInputLayout(m_inputLayout.Get());

	////	板ポリゴンを描画
	//m_batch->Begin();
	//m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	//m_batch->End();

	////	シェーダの登録を解除しておく
	//context->VSSetShader(nullptr, nullptr, 0);
	//context->GSSetShader(nullptr, nullptr, 0);
	//context->PSSetShader(nullptr, nullptr, 0);
}