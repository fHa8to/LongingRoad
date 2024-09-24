#include "Boss.h"
#include "Knight.h"
#include <math.h>

namespace
{
	//敵の速さ
	constexpr float kSpeed = 2.0f;

	//体力
	constexpr int kMaxHp = 100;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アニメーション
	constexpr int kWalkAnimIndex = 6;
	constexpr int kAttackAnimIndex = 82;
	constexpr int kWaitAnimIndex = 89;
	constexpr int kDeathAnimIndex = 3;

	//上空
	constexpr float kSkyAboveY = 100.0f;

	//当たり判定の調整
	constexpr float kAdj = 45.0f;

	//当たり判定の大きさ
	constexpr float kModelWidth = 80.0f;
	constexpr float kModelHeight = 100.0f;
	constexpr float kModelDepth = 100.0f;
}


Boss::Boss(int handle, VECTOR playerPos) :EnemyBase(handle), m_skyAboveY(kSkyAboveY)
{

	m_hp = kMaxHp;

	m_cannonballHandle = MV1LoadModel("data/model/enemy/attack/cannonball.mv1");
}

Boss::~Boss()
{
}

void Boss::Update(Knight* knight, VECTOR playerPos)
{
	Move(knight);
	AttackPattern(playerPos);

	m_enemyCollision.SetCenter(m_pos.x - kAdj, m_pos.y, m_pos.z - kAdj, kModelWidth, kModelHeight, kModelDepth);
}

void Boss::SetPosX(VECTOR playerPos)
{
}

void Boss::AttackPattern(VECTOR playerPos)
{
	m_distance = VSub(playerPos,m_pos);

	if (m_distance.x < 10.0f)
	{
		LongRangeAttack(playerPos);
	}
	else
	{
		Attack(playerPos);
	}
}

void Boss::Attack(VECTOR playerPos)
{
	m_count++;

	m_attackCollision.SetCenter(m_pos.x, m_pos.y, m_pos.z, 100, 100, 100);

	if (m_count <= 300)
	{
		m_attackCollision.SetCenter(0, 0, 0, 0, 0, 0);
		m_count = 0;
	}
}

void Boss::LongRangeAttack(VECTOR playerPos)
{
	//敵の初期位置からターゲット位置に向かうベクトルを生成する
	//始点から終点に向かうベクトルを求める場合、終点の座標-始点の座標で求める
	VECTOR toTarget = VSub(playerPos, m_pos);

	//ベクトルの長さをkSpeedにしてやる
	//ベクトルの正規化　長さを１にする
	toTarget = VNorm(toTarget);
	//kSpeedでかける
	m_impactPos.x = toTarget.x * kSpeed;
	m_impactPos.y = toTarget.y + (m_skyAboveY - 1.0f);
	m_impactPos.z = toTarget.z * kSpeed;

	m_impactPos = VAdd(m_pos, m_impactPos);

	m_longRangeAttackCollision.SetCenter(m_impactPos.x, m_impactPos.y, m_impactPos.z, 100, 100, 100);
}

void Boss::Move(Knight* knight)
{
	VECTOR playerPos = knight->GetPlayerPos();
	// ３Ｄモデル２から３Ｄモデル１に向かうベクトルを算出
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan2 を使用して角度を取得
	float Angle = atan2(SubVector.x, SubVector.z);

	//プレイヤーの方向を向く
	MV1SetRotationXYZ(m_handle, VGet(0.0f, Angle + DX_PI_F, 0.0f));

	//ポジションの設定
	MV1SetPosition(m_handle, m_pos);
}

void Boss::Animation()
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
}

bool Boss::UpdateAnim(int attachNo)
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

void Boss::ChangeAnim(int animIndex)
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
