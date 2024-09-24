#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "SceneBase.h"

#define P_HP_MAX		5
#define P_DRAW_SIZE		50
#define E_HP_MAX		5
#define E_DRAW_SIZE		50

class Camera;
class Player;
class Enemy;

class SceneGame : public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();


private:
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;



private:
	//フェードイン、アウト
	int m_fadeAlpha;

	//シーン終了フラグ
	bool m_isSceneEnd;

	//ゲームクリアなのかのフラグ
	bool m_isGameClear;

	//プレイヤーに当たったかのフラグ
	bool m_isHitPlayer;

	bool m_isEnemyHit;

	//プレイヤーのHPが0になったかのフラグ
	bool m_isHp0;

	//ステージをクリアしたか
	bool m_isStageClear;

	//プレイヤーの座標
	VECTOR m_playerPos;

	//カメラの座標
	VECTOR m_cameraPos;

	int fadeAlpha;

	bool isSceneEnd;


	//当たり判定
	bool m_isPlayerHit;
	bool m_isAttackHit;
	bool m_isGimmickHit;

	int modelHandle;

	int playerHp;
	int playerDrawValue;
	int enemyHp;
	int enemyDrawValue;

	int bgm;
	int bgmPlayer;
	int bgmEnemy;

	VECTOR m_pos;

};

