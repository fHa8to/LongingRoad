#pragma once
#include "DxLib.h"
#include <memory>


class Player;
class Enemy
{
public:
	Enemy();
	virtual ~Enemy();

	void Init();
	void Update(VECTOR playerPos);
	void Draw();

	//プレイヤーと敵の当たり判定
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//攻撃と敵の当たり判定
	//bool SphereHitFlag2(std::shared_ptr<Player> pPlayer);



	float GetRadius() { return m_radius; }

	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

private:

	void Attack();

	//アニメーション関連
	void  Animation();

	bool UpdateAnim(int attachNo);

	void ChangeAnim(int animIndex);

	//ステージ外に出ないようにする処理
	void StageProcess();


private:
	std::shared_ptr<Player> m_pPlayer;
private:
	//エネミーの状態
	enum  state
	{
		kMove,			//動いている
		kAttack,		//攻撃中
	};

private:

	//状態
	int m_state;

	int modelHandle;		//モデルハンドル

	//アニメーション情報
	int currentAnimNo;
	int prevAnimNo;
	float animBlendRate;

	//当たり判定の半径
	float m_radius;

	//表示情報
	VECTOR m_pos;
	VECTOR playerPos;

	float Angle;

	float angle;

	//攻撃しているかを保持する
	bool isAttacking;

	int isAttack;

	//進む距離
	VECTOR m_distance;

};

