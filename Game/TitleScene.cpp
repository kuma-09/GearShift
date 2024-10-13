#include "pch.h"
#include "TitleScene.h"
#include "Game.h"
#include "Game/Components/Camera.h" 
#include "Framework/Resources.h"
#include "Framework/BinaryFile.h"

/// <summary>
/// �C���v�b�g���C�A�E�g
/// </summary>
const std::vector<D3D11_INPUT_ELEMENT_DESC> TitleScene::INPUT_LAYOUT =
{
    { "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR" ,   0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

void TitleScene::Initialize(Game* game)
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    m_graphics = Graphics::GetInstance();
    m_deviceResources = m_graphics->GetDeviceResources();
    m_inputManager = InputManager::GetInstance();

    auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();

    SetGame(game);
 


    m_titleLogo = std::make_unique<UI>(L"Resources/Textures/GearShiftLogo.png", Vector2{ 0,0 });
    m_titleLogo->Initialize();

    m_skydome = std::make_unique<SkyDome>();
    m_skydome->Initialize({ 0,-20,0 });
    
    m_player = std::make_unique<TitlePlayer>(this);
    m_player->Initialize();
    
    m_camera = std::make_unique<TitleCamera>();
    m_camera->Initialize(m_player.get());
    m_camera->SetPosition(Vector3{ 0,5,-20 });

    // ���C�g�̈ʒu
    m_lightPosition = Vector3{ 100,200,-200 };

    // ���C�g�̉�]
    m_lightRotate = SimpleMath::Quaternion::CreateFromYawPitchRoll(
        XMConvertToRadians(0.0f), XMConvertToRadians(45.0f), 0.0f);

    m_lightTheta = 100.0f;
    
    m_floorModel = Resources::GetInstance()->GetFloorModel();

    m_floorPosition.push_back({ -100,0,-100 });
    m_floorPosition.push_back({  100,0,-100 });
    m_floorPosition.push_back({ -100,0, 100 });
    m_floorPosition.push_back({  100,0, 100 });

    RECT rect = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

    // �����_�[�e�N�X�`���̍쐬�i�V���h�E�}�b�v�p�j
    m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
    m_shadowMapRT->SetDevice(device);
    m_shadowMapRT->SetWindow(rect);

    // �f�v�X�X�e���V���̍쐬�i�V���h�E�}�b�v�p�j
    m_shadowMapDS = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
    m_shadowMapDS->SetDevice(device);
    m_shadowMapDS->SetWindow(rect);

    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    // �萔�o�b�t�@�̍쐬
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// �m�ۂ���T�C�Y�i16�̔{���Őݒ肷��j
    // GPU (�ǂݎ���p) �� CPU (�������ݐ�p) �̗�������A�N�Z�X�ł��郊�\�[�X
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU�����e��ύX�ł���悤�ɂ���
    DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_CBuffer.ReleaseAndGetAddressOf()));

    // �萔�o�b�t�@�̍쐬
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer2));	// �m�ۂ���T�C�Y�i16�̔{���Őݒ肷��j
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// GPU�̓ǂݎ��Ə������݂��\�Ȉ�ʓI�ȃ��\�[�X
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
    bufferDesc.CPUAccessFlags = 0;	// CPU�̓A�N�Z�X���Ȃ��̂�0
    DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_CBuffer2.ReleaseAndGetAddressOf()));

    // �萔�o�b�t�@�̓��e�X�V
    ConstantBuffer2 cb = {};
    cb.fCosTheta = cosf(XMConvertToRadians(m_lightTheta / 2.0f));
    context->UpdateSubresource(m_CBuffer2.Get(), 0, nullptr, &cb, 0, 0);

    BinaryFile vs_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS_Depth.cso");
    BinaryFile ps_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS_Depth.cso");
    BinaryFile vs = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS.cso");
    BinaryFile ps = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS.cso");

    //	�C���v�b�g���C�A�E�g�̍쐬
    device->CreateInputLayout(&INPUT_LAYOUT[0],
        static_cast<UINT>(INPUT_LAYOUT.size()),
        vs_depth.GetData(), vs_depth.GetSize(),
        m_inputLayout.GetAddressOf());

    // ���_�V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
    DX::ThrowIfFailed(
        device->CreateVertexShader(vs_depth.GetData(), vs_depth.GetSize(), nullptr, m_VS_Depth.ReleaseAndGetAddressOf())
    );

    // �s�N�Z���V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
    DX::ThrowIfFailed(
        device->CreatePixelShader(ps_depth.GetData(), ps_depth.GetSize(), nullptr, m_PS_Depth.ReleaseAndGetAddressOf())
    );

    // ���_�V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
    DX::ThrowIfFailed(
        device->CreateVertexShader(vs.GetData(), vs.GetSize(), nullptr, m_VS.ReleaseAndGetAddressOf())
    );

    // �s�N�Z���V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
    DX::ThrowIfFailed(
        device->CreatePixelShader(ps.GetData(), ps.GetSize(), nullptr, m_PS.ReleaseAndGetAddressOf())
    );

    // �T���v���[�̍쐬�i�V���h�E�}�b�v�p�j
    D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
    sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
    device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());


}


void TitleScene::Update(float elapsedTime)
{
    using namespace DirectX::SimpleMath;
    
    //const auto& gp = m_inputManager->GetGamePadTracker();
    const auto& kb = m_inputManager->GetKeyboardTracker();

    m_skydome->Update(elapsedTime);
    m_camera->Update(elapsedTime);

    m_player->Update(elapsedTime);

    if (kb->pressed.Space)
    {
        GetGame()->ChangeScene(GetGame()->GetPlayScene());
    }

    if (kb->pressed.Up)
    {
        m_lightPosition -= Vector3{ 0,0,1 };
    }
    if (kb->pressed.Down)
    {
        m_lightPosition += Vector3{ 0,0,1 };
    }
}

void TitleScene::Render()
{
    using namespace DirectX;
    using namespace DirectX::SimpleMath;

    auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
    auto state = m_graphics->GetCommonStates();


    // ���C�g�̕���
    SimpleMath::Vector3 lightDir = SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_lightRotate);

    // �r���[�s����쐬
    auto view = SimpleMath::Matrix::CreateLookAt(
        m_lightPosition,
        m_lightPosition + lightDir,
        SimpleMath::Vector3::UnitY
    );

    // �ˉe�s����쐬
    auto proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
        XMConvertToRadians(m_lightTheta), 1.0f, 0.1f, 1000.0f);

    auto rtv = m_shadowMapRT->GetRenderTargetView();
    auto srv = m_shadowMapRT->GetShaderResourceView();
    auto dsv = m_shadowMapDS->GetDepthStencilView();



    // �����_�[�^�[�Q�b�g��ύX�ishadowMapRT�j
    context->ClearRenderTargetView(rtv, SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
    context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
    // �s�N�Z���V�F�[�_�[���烊�\�[�X���A���o�C���h
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    context->PSSetShaderResources(0, 1, nullSRV);
    // �s�N�Z���V�F�[�_�[���烊�\�[�X���A���o�C���h
    context->OMSetRenderTargets(1, &rtv, dsv);

    // �r���[�|�[�g��ݒ�
    D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
    context->RSSetViewports(1, &vp);

    D3D11_MAPPED_SUBRESOURCE mappedResource;


    // GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
    context->Map(m_CBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    ConstantBuffer cbuffer = {};
    SimpleMath::Matrix m = view * proj;
    cbuffer.lightViewProj = XMMatrixTranspose(m);
    cbuffer.lightPosition = m_lightPosition;
    cbuffer.lightDirection = lightDir;
    cbuffer.lightAmbient = SimpleMath::Color(0.3f, 0.3f, 0.3f);

    *static_cast<ConstantBuffer*>(mappedResource.pData) = cbuffer;

    // GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
    context->Unmap(m_CBuffer.Get(), 0);



	////	�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	//context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuffer, 0, 0);

	////	�V�F�[�_�[�Ƀo�b�t�@��n��
	//ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	////	���_�V�F�[�_���s�N�Z���V�F�[�_���A�����l��n��
	//context->VSSetConstantBuffers(0, 1, cb);
	//context->PSSetConstantBuffers(0, 1, cb);

    Matrix world = Matrix::CreateTranslation(Vector3{ 0,1,0 });

    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, world, view, proj, false, [&]()
        {
            //	�C���v�b�g���C�A�E�g�̓o�^
            context->IASetInputLayout(m_inputLayout.Get());
            context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
            context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
        });



    // -------------------------------------------------------------------------- //
    // �����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
    // -------------------------------------------------------------------------- //
    auto renderTarget = m_graphics->GetDeviceResources()->GetRenderTargetView();
    auto depthStencil = m_graphics->GetDeviceResources()->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    view = m_graphics->GetViewMatrix();
    proj = m_graphics->GetProjectionMatrix();

    context->OMSetRenderTargets(1, &renderTarget, depthStencil);
    auto const viewport = m_graphics->GetDeviceResources()->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_skydome->Render();
    //m_player->Render();
    //for (auto& position : m_floorPosition)
    //{
    //    for (int i = 0; i < 2; i++)
    //    {
    //        for (int n = 0; n < 2; n++)
    //        {
    //            Matrix world = Matrix::CreateTranslation(Vector3{ -50 + float(i) * 100 ,1, -50 + float(n) * 100 } + position);
    //            m_floorModel->Draw(context, *state, world, view, proj, false);
    //        }
    //    }
    //}

    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, world, view, proj, false);

    world =  Matrix::CreateTranslation(m_lightPosition);

    Resources::GetInstance()->GetCubeModel()->Draw(context, *state, world, view, proj, false);

    //���̕`��
    m_floorModel->Draw(context, *state, Matrix::Identity, view, proj, false, [&]()
        {
            // �萔�o�b�t�@�̐ݒ�
            ID3D11Buffer* cbuf[] = { m_CBuffer.Get(),m_CBuffer2.Get()};
            context->VSSetConstantBuffers(1, 1, cbuf);
            context->PSSetConstantBuffers(1, 2, cbuf);

            // �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
            context->PSSetShaderResources(1, 1, &srv);

            // �e�N�X�`���T���v���[�̐ݒ�
            ID3D11SamplerState* samplers[] = { state->LinearWrap(), m_shadowMapSampler.Get() };
            context->PSSetSamplers(0, 2, samplers);

            //	�C���v�b�g���C�A�E�g�̓o�^
            context->IASetInputLayout(m_inputLayout.Get());

            // �V�F�[�_�[�̐ݒ�
            context->VSSetShader(m_VS.Get(), nullptr, 0);
            context->PSSetShader(m_PS.Get(), nullptr, 0);
        }
    );


    m_titleLogo->Render();



    // ���\�[�X�̊��蓖�Ă���������ishadowMapRT�j
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(1, 1, nullsrv);

    m_spriteBatch->Begin();
    //m_spriteBatch->Draw(srv, Vector2::Zero);
    m_spriteBatch->End();
}

void TitleScene::Finalize()
{

}
