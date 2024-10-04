//--------------------------------------------------------------------------------------
// File: Menu.h
//
// ���j���[
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Menu.h"
#include "UserInterface.h"

#include "Framework/BinaryFile.h"
#include "Framework/DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

using namespace DirectX;

Menu::Menu() 
    : m_menuIndex(0)
    ,m_windowHeight(0)
    ,m_windowWidth(0)
    ,m_pDR(nullptr)
    ,m_baseTexturePath(nullptr)
{
    m_userInterface.clear();
}

Menu::~Menu()
{
    for (auto& userInterface : m_userInterface)
    {
        userInterface.reset();
    }
    m_userInterface.clear();
    m_base.clear();
    m_baseWindow.reset();
}

void Menu::Initialize(DX::DeviceResources* pDR,int width,int height)
{
	m_pDR = pDR;
    m_windowWidth = width;
    m_windowHeight = height;

    //  �w�i�ƂȂ�E�B���h�E�摜��ǂݍ���
    m_baseTexturePath = L"Resources/Textures/window.png";

    // �ׂ��ׂ�
    // HEKISADO

    //  ���摜��ǂݍ���
    Add(L"Resources/Textures/LeftLeg.png"
        , SimpleMath::Vector2(100, 100)
        , SimpleMath::Vector2(0.5f,0.5f)
        , tito::ANCHOR::MIDDLE_CENTER);

}

void Menu::Update()
{
    auto keystate = Keyboard::Get().GetState();
    m_tracker.Update(keystate);
    if (m_tracker.pressed.Right)
    {
        //  ���L�[����������A�I����1�i�߂�
        m_menuIndex += 1;
        //  ���j���[�A�C�e�����̍ő�l�𒴂��Ȃ��悤�ɐ���
        m_menuIndex %= m_userInterface.size();
    }
    if (m_tracker.pressed.Left)
    {
        //  ���L�[����������A�I����1�߂��B
        //  �������A�I���̃I�[�o�[������Z�̗]��ŕ␳���邽�߁A�i�A�C�e���̍ő�� - 1�j�𑫂��ĕK���]��Ōv�Z����`�ɂ��Ă����B
        //  �ȉ��̎��́A�Ⴆ�΃��j���[��4��������A���ݒl��3�𑫂��Ƃ������B
        //  ��j�I�𒆂̃��j���[��0�`3�܂ł������2�Ԗڂ������ꍇ�A
        //  (2 + (4 - 1)) % 4 = 1 �� �I�𒆂̔ԍ���1������
        //  �Ƃ�������
        m_menuIndex += static_cast<unsigned int>(m_userInterface.size()) - 1;
        m_menuIndex %= m_userInterface.size();
    }

    //  �e�A�C�e���ɕ\������摜�̏����T�C�Y��ݒ肷��
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_base[i]->SetScale(m_base[i]->GetBaseScale());
        m_userInterface[i]->SetScale(m_userInterface[i]->GetBaseScale());
    }

    //  �I�𒆂̏����T�C�Y���擾����
    SimpleMath::Vector2 select = m_userInterface[m_menuIndex]->GetBaseScale();
    //  �I����ԂƂ��邽�߂̕ω��p�T�C�Y���Z�o����
    SimpleMath::Vector2 selectScale = SimpleMath::Vector2::Lerp(m_userInterface[m_menuIndex]->GetBaseScale(), SimpleMath::Vector2::One, 1);
    //  �I����Ԃ͏�����ԁ{30���̑傫���Ƃ���
    select += selectScale * 0.3f;
    //  �Z�o��̃T�C�Y�����݂̃T�C�Y�Ƃ��Đݒ肷��
    m_userInterface[m_menuIndex]->SetScale(select);
    //  �w�i�p�̃E�B���h�E�摜�ɂ����������̒l��ݒ肷��
    m_base[m_menuIndex]->SetScale(select);
}

void Menu::Render()
{
    for (int i = 0;i < m_userInterface.size();i++)
    {
        //  �A�C�e���p�E�B���h�E�w�i��\��
        m_base[i]->Render();
        //  ���ۂɕ\���������A�C�e���摜��\��
        m_userInterface[i]->Render();
    }
}

void Menu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, tito::ANCHOR anchor)
{
    //  ���j���[�Ƃ��ăA�C�e����ǉ�����
    std::unique_ptr<tito::UserInterface> userInterface = std::make_unique<tito::UserInterface>();
    //  �w�肳�ꂽ�摜��\�����邽�߂̃A�C�e�����쐬����
    userInterface->Create(m_pDR
        , path
        , position
        , scale
        , anchor);
    userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

    //  �A�C�e����V�����ǉ�
    m_userInterface.push_back(std::move(userInterface));

    //  �w�i�p�̃E�B���h�E�摜���ǉ�����
    std::unique_ptr<tito::UserInterface> base = std::make_unique<tito::UserInterface>();
    base->Create(m_pDR
        , m_baseTexturePath
        , position
        , scale
        , anchor);
    base->SetWindowSize(m_windowWidth, m_windowHeight);

    //  �w�i�p�̃A�C�e�����V�����ǉ�����
    m_base.push_back(std::move(base));

}


