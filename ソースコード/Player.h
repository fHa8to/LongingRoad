#pragma once
#include "DxLib.h"
#include <memory>

class Enemy;
class Player
{
public:
	Player();
	virtual ~Player();

	void Init();
	void Update(VECTOR cameraPos);
	void Draw();

	//半径の取得
	float GetRadius() { return m_radius; }

	//プレイヤーの座標を取得	
	const VECTOR GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }



	//攻撃と敵のあたり判定
	bool SphereHitFlag(std::shared_ptr<Enemy> pEnemy);


private:


	bool UpdateAnim(int attachNo);

	void ChangeAnim(int animIndex);

	//ステージ外に出ないようにする処理
	void StageProcess();


private:
	std::shared_ptr<Enemy> m_pEnemy;
private:


	//方向
	enum  direction
	{
		kRight,
		kLeft,
		kUp,
		kDown
	};


private:
	int		m_modelHandle;	//モデルハンドル

	//アニメーション情報
	int m_currentAnimNo;
	int m_prevAnimNo;
	float m_animBlendRate;

	//状態
	int m_state;

	//向いている方向
	int m_direction;

	//Aボタンを何回押したか
	int m_countAButton;

	//Xボタンを何回押したか
	int m_countXButton;


	//動いているかを保持する
	bool m_isMove;



	bool m_isAttacking;


	//HP
	int m_hp;


	//表示情報
	VECTOR m_pos;

	VECTOR m_attackPos;
	//カメラの位置
	VECTOR m_cameraPos;

	float m_angle;

	//当たり判定の半径
	float m_radius;

	//カメラ情報
	float m_cameraAngle;


	//動いているか
	bool m_iskStandby;

	//歩いているか
	bool m_isWalk;

	//走っているか
	bool m_isRun;

	//攻撃しているか
	bool m_isAttack;


};

