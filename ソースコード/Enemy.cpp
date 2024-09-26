#include "Enemy.h"
#include "DxLib.h"
#include "Player.h"
#include <cmath>

namespace
{
	//モデルのファイル名
	//const char* const kModelFilename = "data/model/Barbarian.mv1";
	const char* const kModelFilename = "data/model/Bee.mv1";

	//モデルのサイズ変更
	constexpr float kExpansion = 0.1f;

	//敵の速さ
	constexpr float kSpeed = 0.3f;

	//最大まですすめるところ
	constexpr float kMaxX = 200.0f;
	constexpr float kMaxZ = 200.0f;

	//アニメーション番号
	constexpr int kIdleAnimIndex = 1;		//待機
	constexpr int kWalkAnimIndex = 2;		//歩き
	constexpr int kAttackAnimIndex = 30;	//攻撃


	//攻撃判定が発生するまでにかかる時間
	constexpr float kAttackFrameStart = 20;
	//攻撃判定が終了するまでにかかる時間
	constexpr float kAttackFrameEnd = 30;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//当たり判定
	constexpr float kAddposY = 10.0f;

	//角度
	constexpr float kAngle = 270.0f * DX_PI_F / 180.0f;

}

Enemy::Enemy():
	m_modelHandle(-1),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_angle(0.0f),
	m_pos(VGet(0,0,0)),
	m_radius(6.0f),
	m_isAttacking(false),
	m_isAttack(false)

{
	m_pPlayer = std::make_shared<Player>();
	//3Dモデルの読み込み
	m_modelHandle = MV1LoadModel(kModelFilename);

	m_state = kMove;

}

Enemy::~Enemy()
{

}

void Enemy::Init()
{

	//エネミーの初期位置設定
	m_pos = VGet(-100.0f, 0.0f, 0.0f);

	m_currentAnimNo = MV1AttachAnim(m_modelHandle, kWalkAnimIndex, -1, false);
	m_prevAnimNo - 1;
	m_animBlendRate = 1.0f;

	m_state = kMove;
	
	MV1SetScale(m_modelHandle, VGet(kExpansion, kExpansion, kExpansion));


}

void Enemy::Update(VECTOR playerPos)
{	

	//プレイヤーの座標
	VECTOR toTarget = VSub(playerPos, m_pos);

	toTarget = VNorm(toTarget);
	m_distance.x = toTarget.x * kSpeed;
	m_distance.z = toTarget.z * kSpeed;


	m_pos = VAdd(m_pos, m_distance);

	if (m_distance.x <= 6.0f)
	{
		Attack();
	}

	if (m_distance.x > 6.5f)
	{
		m_state = kMove;
	}

	//モデルの向きを変える
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan を使用して角度を取得
	float Angle = atan2(SubVector.x, SubVector.z);

	//プレイヤーの方向を向く
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, Angle + DX_PI_F, 0.0f));

	//アニメーション
	Animation();


	StageProcess();

	
	// ３Dモデルのポジション設定
	MV1SetPosition(m_modelHandle, m_pos);

}

void Enemy::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawFormatString(0, 32, 0xffffff, "Enemy(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

#endif

}

//球の当たり判定
bool Enemy::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	if (m_state == kAttack)
	{
		float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
		float delY = ((m_pos.y + kAddposY) - (pPlayer->GetPos().y + kAddposY)) *
			((m_pos.y + kAddposY) - (pPlayer->GetPos().y + kAddposY));
		float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

		//球と球の距離
		float Distance = sqrt(delX + delY + delZ);

		//球と球の距離が剣とエネミーの半径よりも小さい場合
		if (Distance < m_radius + pPlayer->GetRadius())
		{

			return true;
		}
	}


	return false;
}




void Enemy::Attack()
{
	m_isAttack = true;
	m_state = kAttack;

}

void Enemy::Animation()
{
	if (m_prevAnimNo != -1)
	{
		//test 8フレームで切り替え
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//変更後のアニメーション割合を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
	}
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	if (m_state == kMove)
	{

		m_isAttack = false;
		m_isAttacking = false;

		if (isLoop)
		{
			ChangeAnim(kWalkAnimIndex);
		}
	}
	if (m_state == kAttack)
	{
		if (m_isAttacking != m_isAttack)
		{
			m_isAttacking = m_isAttack;
			if (m_isAttacking)
			{
				//ChangeAnim(kAttackAnimIndex);
			}
		}
	}

}

bool Enemy::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);	//現在の再生カウントを取得

	//現在再生中のアニメーションの総カウントを取得
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;
	//アニメーション進める
	now += 0.5f;

	if (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//進めた時間の設定
	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);

	return isLoop;
}

void Enemy::ChangeAnim(int animIndex)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//切り替えの瞬間は変更後のアニメーションが再生される
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}


//移動範囲
void Enemy::StageProcess()
{
	//ステージ外に出ないようにする
	if (m_pos.z > kMaxZ)
	{
		m_pos.z = kMaxZ;
	}
	else if (m_pos.z < -kMaxZ)
	{
		m_pos.z = -kMaxZ;
	}

	if (m_pos.x < -kMaxX)
	{
		m_pos.x = -kMaxX;
	}
	else if (m_pos.x > kMaxX)
	{
		m_pos.x = kMaxX;
	}

}


