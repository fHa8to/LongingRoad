#pragma once
#include "SceneBase.h"
#include "DxLib.h"

class SceneClear : public SceneBase
{
public:
	SceneClear();
	virtual ~SceneClear();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:

	//アニメーション関連
	void  Animation();

	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);
private:
	enum state
	{
		kWait,			//待機中
	};
private:

	//モデル
	int modelHandle;
	int modelHandle2;

	//状態
	int m_state;

	int handle;

	int fadeAlpha;

	bool isSceneEnd;

	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//ハンドル
	int m_handle;

	//モデルの座標
	VECTOR m_pos;

	//カメラの座標
	VECTOR m_cameraPos;

};

