#include "DxLib.h"
#include "Knight.h"
#include "Bee.h"
#include <math.h>

namespace
{
	//敵の速さ
	constexpr float kSpeed = 2.0f;

	//敵の最大HP
	constexpr int kMaxHp = 50;


	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アニメーション
	constexpr int kRunAnimIndex = 2;
	constexpr int kAttackAnimIndex = 0;
	constexpr int kDamageAnimIndex = 3;
	constexpr int kDeathAnimIndex = 1;

	//発生位置
	constexpr int kAppearanceX = 750;

	//当たり判定の調整
	constexpr float kAdj = 45.0f;

	//当たり判定の大きさ
	constexpr float kModelWidth = 80.0f;
	constexpr float kModelHeight = 100.0f;
	constexpr float kModelDepth = 100.0f;

	//プレイヤーの位置
	constexpr float kPlayerPos1X = -4500.0f;

	constexpr float kPlayerPos2X = 265.0f;

	constexpr float kPlayerPos3X = 6175.0f;

	//ハチの初期位置
	constexpr float kPosY = 240.0f;

	//ノックバック
	constexpr float kKnockback = 50.0f;

	constexpr float kMaxKnockback = 250.0f;

}

Bee::Bee(int m_handle,VECTOR playerPos) :EnemyBase(m_handle), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1),m_isAttacking(false),m_isAttack(false), m_playerNum(1)
{
	//アニメーションの初期設定
	m_currentAnimNo = MV1AttachAnim(m_handle, kRunAnimIndex, -1, true);

	m_hp = kMaxHp;

	//座標設定
	SetPosX(playerPos);
}

Bee::~Bee()
{

}
void Bee::Update(Knight* knight,VECTOR playerPos)
{
	ComingPlayer(knight);

	Animation();

	m_enemyCollision.SetCenter(m_pos.x - kAdj, m_pos.y, m_pos.z - kAdj, kModelWidth, kModelHeight, kModelDepth);
}


void Bee::HitAttack(Rect playerAttack)
{
	if (m_enemyCollision.IsCollsion(playerAttack))
	{
		m_hp -= 5;
		m_pos.x += kKnockback;
		m_attackHits++;
	}
	if (m_attackHits == 3)
	{
		m_pos.x += kMaxKnockback;
		m_attackHits = 0;
	}
}

void Bee::HitPlayer(Knight* knight, Rect playerCollision)
{
	if (m_enemyCollision.IsCollsion(playerCollision))
	{
		knight->HitBody();
	}
}

void Bee::SetPosX(VECTOR playerPos)
{
	if (playerPos.x <= kPlayerPos1X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x <= kPlayerPos2X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
	else if (playerPos.x <= kPlayerPos3X)
	{
		m_randomPosX = GetRand(kAppearanceX);
		m_randomPosX = m_randomPosX + playerPos.x;
		m_pos = VGet(m_randomPosX, kPosY, 0.0f);
	}
}

void Bee::ComingPlayer(Knight* knight)
{
	VECTOR playerPos = knight->GetPlayerPos();
	//敵の初期位置からターゲット位置に向かうベクトルを生成する
	//始点から終点に向かうベクトルを求める場合、終点の座標-始点の座標で求める
	VECTOR toTarget = VSub(playerPos, m_pos); 

	//ベクトルの長さをkSpeedにしてやる
	//ベクトルの正規化　長さを１にする
	toTarget = VNorm(toTarget);
	//kSpeedでかける
	m_distance.x = toTarget.x * kSpeed;
	m_distance.y = toTarget.y * kSpeed;
	m_distance.z = toTarget.z * kSpeed;

	m_pos = VAdd(m_pos, m_distance);

	if (m_distance.x <= 0.0f  )
	{
		Attack();
	}

	if (m_distance.x > 1.0f)
	{
		m_state = kMove;
	}


	// ３Ｄモデル２から３Ｄモデル１に向かうベクトルを算出
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan2 を使用して角度を取得
	float Angle = atan2(SubVector.x, SubVector.z);

	//プレイヤーの方向を向く
	MV1SetRotationXYZ(m_handle, VGet(0.0f, Angle + DX_PI_F , 0.0f));


	//ポジションの設定
	MV1SetPosition(m_handle, m_pos);
}

void Bee::Attack()
{
	m_isAttack = true;
	m_state = kAttack;
}

void Bee::Animation()
{
	if (m_prevAnimNo != -1)
	{
		//test 8フレームで切り替え
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//変更後のアニメーション割合を設定する
		MV1SetAttachAnimBlendRate(m_handle, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_handle, m_currentAnimNo, m_animBlendRate);
	}
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	if (m_state == kMove)
	{

		m_isAttack = false;
		m_isAttacking = false;
		if (isLoop)
		{
			ChangeAnim(kRunAnimIndex);
		}
		
		
	}
	if (m_state == kAttack)
	{

		if (m_isAttacking != m_isAttack)
		{
			m_isAttacking = m_isAttack;
			if (m_isAttacking)
			{
				ChangeAnim(kAttackAnimIndex);
			}
		}		
	}
	if (m_state == kDamage)
	{

	}
	if (m_state == kDeath)
	{

	}
}

bool Bee::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_handle, attachNo);

	//アニメーション進める
	now += 0.5f;

	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_handle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//進めた時間に設定
	MV1SetAttachAnimTime(m_handle, attachNo, now);
	return isLoop;
}

void Bee::ChangeAnim(int animIndex)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_handle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_handle, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_handle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_handle, m_currentAnimNo, m_animBlendRate);
}
