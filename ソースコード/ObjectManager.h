#pragma once
#include <memory>
#include <list>
#include <functional>

class Enemy;
class Player;
class Time;

class ObjectManager
{

public:
	//プレイヤーが存在するかどうか
	bool IsPlayerEnable();

	//敵が存在するか
	bool IsEnemyEnable();

	//ゲームクリアしたかどうか
	bool IsGameClear() const { return m_isGameClear; }

	void Update();
	void Draw();

	//ロックオンされている敵を更新
	void LockOnEnemyUpdate();

	//ロックオンされている敵ポインター取得
	const Enemy* const GetLockOnEnemy() { return m_pLockOnEnemy; }

	//プレイヤーポインタ取得
	Player* const GetPlayer();


	//強制的にゲームクリアにする
	void SetGameClear();
private:
	//ステージをクリアした際の更新処理
	void GameClearUpdate();
private:

	//ロックオンされている敵ポインター
	Enemy* m_pLockOnEnemy = nullptr;


	std::shared_ptr<Time> m_pBossDeadNextSceneTime;
	bool m_isGameClear = false;

	bool m_isBossDead = false;


};

