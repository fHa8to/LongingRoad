#include "SceneManager.h"
#include "SceneTitle.h"
#include "Pad.h"

SceneManager::SceneManager() :
	m_pScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	m_pScene = std::make_shared<SceneTitle>();
	m_pScene->Init();

}

void SceneManager::Update()
{
	Pad::Update();

	std::shared_ptr<SceneBase> pNext = m_pScene->Update();
	if (pNext != m_pScene)
	{
		//���ݏ������̃V�[���̏I������
		m_pScene->End();

		//Update���Ԃ����V�����V�[���̊J�n�������s��
		m_pScene = pNext;
		m_pScene->Init();
	}

}

void SceneManager::Draw()
{
	m_pScene->Draw();
}

void SceneManager::End()
{
	m_pScene->End();
}

