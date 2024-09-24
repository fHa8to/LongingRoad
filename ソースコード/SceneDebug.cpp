#include "SceneDebug.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneClear.h"
#include "SceneOver.h"
#include "Pad.h"

namespace
{
	//初期カーソルポジション
	constexpr float kCursorPosX = 500.0f;
	constexpr float kCursorPosY = 300.0f;

	//カーソル移動量
	constexpr float kCursorMovement = 50.0f;
}

SceneDebug::SceneDebug()
{
	m_sceneString[static_cast<int>(SceneType::Debug)] = "Debug";
	m_sceneString[static_cast<int>(SceneType::Main)] = "Main";
	m_sceneString[static_cast<int>(SceneType::Option)] = "Option";
	m_sceneString[static_cast<int>(SceneType::GameOver)] = "GameOver";
	m_sceneString[static_cast<int>(SceneType::GameClear)] = "GameClear";
	m_sceneString[static_cast<int>(SceneType::Title)] = "Title";

}

void SceneDebug::Init()
{
}

void SceneDebug::End()
{
}

SceneBase* SceneDebug::Update()
{
	//ボタンを押されたら選択されたシーンに遷移
	if (Pad::isTrigger(PAD_INPUT_1))
	{
		return SceneUpdate();
	}

	CursorUpdate();

	UpdateFade();

	return nullptr;
}

void SceneDebug::Draw()
{
	for (int i = 0; i < static_cast<int>(m_sceneString.size()); i++)
	{
		DrawString(static_cast<int>(kCursorPosX), static_cast<int>(kCursorPosY + (i * kCursorMovement)),
			m_sceneString[i].c_str(), 0xffffff);

		if (m_cursorNum == i)
		{
			DrawString(static_cast<int>(kCursorPosX), static_cast<int>(kCursorPosY + (i * kCursorMovement)),
				m_sceneString[i].c_str(), 0xff0000);
		}
	}

	SceneBase::DrawFade();

}

void SceneDebug::CursorUpdate()
{
	if (Pad::isTrigger(PAD_INPUT_UP))
	{
		m_cursorNum--;
		if (m_cursorNum < 0)
		{
			m_cursorNum = static_cast<int>(m_sceneString.size() - 1);
		}
	}

	if (Pad::isTrigger(PAD_INPUT_DOWN))
	{
		m_cursorNum++;
		if (m_cursorNum >= static_cast<int>(m_sceneString.size()))
		{
			m_cursorNum = 0;
		}
	}

}

SceneBase* SceneDebug::SceneUpdate()
{
	SceneBase* nextScene = nullptr;

	std::shared_ptr<SceneManager> SceneManager = nullptr;

	//選択されたシーンに遷移
	switch (m_cursorNum)
	{
	case static_cast<int>(SceneType::Debug):
		nextScene = new SceneDebug;
		break;
	case static_cast<int>(SceneType::MainEasy):
		nextScene = new SceneMain(Game::Level::Easy);
		break;
	case static_cast<int>(SceneType::MainNormal):
		nextScene = new SceneMain(Game::Level::Normal);
		break;
	case static_cast<int>(SceneType::MainHard):
		nextScene = new SceneMain(Game::Level::Hard);
		break;
	case static_cast<int>(SceneType::Option):
		nextScene = new SceneOption(this);
		break;
	case static_cast<int>(SceneType::GameOver):
		mainSceneManager = std::make_shared<MainSceneManager>(Game::Level::Easy);
		nextScene = new SceneGameOver(mainSceneManager);;
		break;
	case static_cast<int>(SceneType::GameClear):
		mainSceneManager = std::make_shared<MainSceneManager>(Game::Level::Easy);
		mainSceneManager->GetObjectManager()->SetGameClear();
		nextScene = new SceneGameClear(mainSceneManager);
		break;
	case static_cast<int>(SceneType::Title):
		nextScene = new SceneTitle(-1);
		break;
	case static_cast<int>(SceneType::StageSelect):
		nextScene = new SceneStageSelect(-1);
		break;
	case static_cast<int>(SceneType::Tutorial):
		nextScene = new SceneTutorial();
		break;
	default:
		break;
	}
	return nextScene;
}
