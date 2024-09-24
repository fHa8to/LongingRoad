#pragma once
#include "SceneBase.h"
#include <array>
#include <string>

class SceneDebug : public SceneBase
{
public:
	enum class SceneType
	{
		Debug,			//デバッグ
		Main,			//メイン
		Option,			//オプション
		GameOver,		//ゲームオーバー
		GameClear,		//ゲームクリア
		Title,			//タイトル

		SceneNum,		//シーンの数
	};
public:
	SceneDebug();
	virtual ~SceneDebug() {}

	virtual void Init() override final;
	virtual void End() override final;

	virtual SceneBase* Update() override final;
	virtual void Draw() override final;

	//シーンの種類取得
	virtual SceneKind GetSceneKind() { return SceneKind::Debug; }

private:
	//カーソル番号アップデート
	void CursorUpdate();
	//次のシーンアップデート
	SceneBase* SceneUpdate();

private:
	//カーソルの番号
	int m_cursorNum;
	//シーン移動用文字列
	std::array<std::string, static_cast<int>(SceneType::SceneNum)> m_sceneString;

};

