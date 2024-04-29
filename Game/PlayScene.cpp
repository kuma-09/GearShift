#include "pch.h"
#include "PlayScene.h"
#include "cmath"

void PlayScene::Initialize()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    m_model = std::make_unique<DirectX::Model>();
    m_model = DirectX::Model::CreateFromCMO(m_graphics->GetDeviceResources()->GetD3DDevice(), L"Resources/Models/dice.cmo", *m_graphics->GetFX());

    m_basicEffect = m_graphics->GetBasicEffect();
    m_primitiveBatch = m_graphics->GetPrimitiveBatch();

    // ビュー行列を作成
    Vector3 eye = Vector3(0, 5, 10);
    Vector3 target = Vector3::Zero;
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);

    // 射影行列を作成する
    Matrix projection = Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(45.0f),
        static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().right) / static_cast<float>(m_graphics->GetDeviceResources()->GetOutputSize().bottom),
        0.1f, 100.0f
    );
    m_graphics->SetProjectionMatrix(projection);

    m_angle = 0;
    m_position = Vector3(0,0,5);
    m_center = Vector3(-3,0,20);

    // 四角形の頂点座標を定義する…左下基準のコの字、頂点順の指定でDrawQuadが使える
    m_vertices[0] = { Vector3(-5.0f ,5.0f , 5.0f),Vector4(1,1,1,1), Vector2(0.0f, 0.0f) };	//左上
    m_vertices[1] = { Vector3( 5.0f ,5.0f , 5.0f),Vector4(1,1,1,1), Vector2(1.0f, 0.0f) };	    //右上
    m_vertices[2] = { Vector3(-5.0f ,5.0f ,-5.0f),Vector4(1,1,1,1), Vector2(0.0f, 1.0f) };	//左下
    m_vertices[3] = { Vector3( 5.0f ,5.0f ,-5.0f),Vector4(1,1,1,1), Vector2(1.0f, 1.0f) };	    //右下


    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),		// デバイスコンテキスト
        L"Resources/Textures/Daylight_Box.png",	// 画像ファイルのパス
        nullptr,								// 内部的なテクスチャ
        m_texture.ReleaseAndGetAddressOf()		// シェーダリソースビュー(表示用)
    );

    
}

void PlayScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;

    const auto& gp = m_inputManager->GetGamePadTracker();

    Vector3 dot = m_position - m_center;
    float radian = atan2f(dot.x, dot.z);

    // 回転行列を生成する
    Matrix rotation = Matrix::CreateRotationY(radian);
    Vector3 velocity = Vector3::Zero;





    if (gp->dpadUp)
    {
        velocity += SPEED_FB * rotation.Forward();


    }
    if (gp->dpadDown)
    {
        velocity += SPEED_FB * -rotation.Forward();


    }
    if (gp->dpadRight)
    {
        velocity += SPEED_RL * dot.Length() * rotation.Right();

        m_angle -= 0.05f;
    }
    if (gp->dpadLeft)
    {
        velocity += SPEED_RL * dot.Length() * -rotation.Right();
        
        m_angle += 0.05f;
    }
    m_position += velocity;

    
    dot = m_position - m_center;
    radian = atan2f(dot.x, dot.z);

    // 回転行列を生成する
    rotation = Matrix::CreateRotationY(radian);

    // ビュー行列を作成
    Vector3 eye = m_position + 10 * -rotation.Forward() + 5 * rotation.Up();
    Vector3 target = m_center;
    Matrix view = Matrix::CreateLookAt(eye, target, Vector3::UnitY);
    m_graphics->SetViewMatrix(view);
}

void PlayScene::Render()
{
    using namespace DirectX::SimpleMath;

    auto view = m_graphics->GetViewMatrix();
    auto projection = m_graphics->GetProjectionMatrix();

    auto context = m_deviceResources->GetD3DDeviceContext();


    m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

    Matrix world = Matrix::Identity;
    
    Vector3 dot = m_position - m_center;
    float radian = atan2f(dot.x, dot.z);
    
    world = Matrix::CreateRotationY(radian);
    world *= Matrix::CreateTranslation(m_position);

    // エフェクトを変更する→座標系を設定する
    m_basicEffect->SetWorld(world);				// ワールド行列
    m_basicEffect->SetView(view);				// ビュー行列
    m_basicEffect->SetProjection(projection);	// 射影行列
    m_basicEffect->SetTexture(m_texture.Get());	// テクスチャ
    m_basicEffect->Apply(context);				// ベーシックエフェクトを更新する


    m_model->Draw(context, *m_graphics->GetCommonStates(), world, m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    m_model->Draw(context, *m_graphics->GetCommonStates(), Matrix::CreateTranslation(m_center), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
    m_model->Draw(context, *m_graphics->GetCommonStates(), Matrix::CreateTranslation(m_center + Vector3(0,0,5)), m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());


    m_graphics->DrawPrimitiveEnd();



    // プリミティブバッチで描画する
    m_primitiveBatch->Begin();
    m_primitiveBatch->DrawQuad(m_vertices[0], m_vertices[1], m_vertices[3], m_vertices[2]);
    m_primitiveBatch->End();

}

void PlayScene::Finalize()
{

}