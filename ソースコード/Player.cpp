#include "Player.h"
#include "Enemy.h"
#include "Pad.h"
#include <cmath>

namespace
{
	//モデルのファイル名
	const char* const kModelFilename = "data/model/Knight.mv1";



	//アニメーション番号
	constexpr int kIdleAnimIndex = 1;		//待機
	constexpr int kWalkAnimIndex = 2;		//歩き
	constexpr int kRunAnimIndex = 7;		//走り
	constexpr int kAttackAnimIndex = 30;	//攻撃
	constexpr int kAnimIndex = 3;

	

	//走る速さ
	constexpr float kDash = 15.0f;

	//初期座標
	constexpr float kPosX = 400.0f;
	constexpr float kPosY = 0.0f;

	//モデルのサイズ変更
	constexpr float kExpansion = 10.0f;

	//最大まですすめるところ
	constexpr float kMaxX = 200.0f;
	constexpr float kMaxZ = 200.0f;

	//当たり判定
	constexpr float kAddposY = 10.0f;


	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 0.5f;		//プレイヤーの最大移動速度
	constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値

		//回転量
	constexpr float kRotateRight = 270.0f * DX_PI_F / 180.0f;
	constexpr float kRotateLeft = 90.0f * DX_PI_F / 180.0f;
	constexpr float kRotateUp = 180.0f * DX_PI_F / 180.0f;
	constexpr float kRotateDown = 0.0f;


	//BGMのファイル名
	const char* const kBgmAttack = "data/sound/Attack.mp3";

}

Player::Player():
	modelHandle(-1),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_pos(),
	angle(0.0f),
	m_isAttack(false),
	m_cameraAngle(0.0f),
	m_radius(6.0f)
{
	//3Dモデルの読み込み
	modelHandle = MV1LoadModel(kModelFilename);
}

Player::~Player()
{
}

void Player::Init()
{	


	MV1SetScale(modelHandle, VGet(kExpansion, kExpansion, kExpansion));

	//アニメーションの初期設定
	m_currentAnimNo = MV1AttachAnim(modelHandle, kIdleAnimIndex, -1, false);
	m_prevAnimNo - 1;
	m_animBlendRate = 1.0f;

	m_pos = VGet(10.0f, 0.0f, 0.0f);


	//MV1SetPosition(modelHandle, m_pos);

	m_isAttack = false;

}

void Player::Update(VECTOR cameraPos)
{
	
	//アニメーションの切り替え
	if (m_prevAnimNo != -1)
	{
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f)	m_animBlendRate = 1.0f;
		//変更前のアニメーション100%
		MV1SetAttachAnimBlendRate(modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
		//変更後のアニメーション0%
		MV1SetAttachAnimBlendRate(modelHandle, m_currentAnimNo, m_animBlendRate);

	}

	//アニメーションを進める
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	StageProcess();



	//ボタンを押したら攻撃アニメーションを再生する
	if (!m_isAttack)
	{

		//攻撃
		if (Pad::IsTrigger PAD_INPUT_1)
		{

			m_isAttack = true;
			ChangeAnim(kAttackAnimIndex);
			PlaySoundFile(kBgmAttack, DX_PLAYTYPE_BACK);

		}	
		else
		{	
			int analogX = 0;
			int analogZ = 0;

			GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);


			//アナログスティックの入力の10% ~ 80%の範囲を使用する

			//ベクトルの長さが最大で1000になる
			//プレイヤーの最大移動速度は0.01f / frame

			VECTOR move = VGet(analogX, 0.0f, -analogZ);	//ベクトルの長さは0～1000

			//0.00 ~ 0.01の長さにする
			//ベクトルの長さを取得する
			float len = VSize(move);
			//ベクトルの長さを0.0~1.0の割合に変換する
			float rate = len / kAnalogInputMax;

			//アナログスティック無効な範囲を除外する
			rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
			rate = min(rate, 1.0f);
			rate = max(rate, 0.0f);

			//速度が決定できるので移動ベクトルに反映する
			move = VNorm(move);
			float speed = kMaxSpeed * rate;
			move = VScale(move, speed);

			MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
			move = VTransform(move, mtx);

			if (VSquareSize(move) > 0.0f)
			{
				angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
			}

			m_pos = VAdd(m_pos, move);

			if (Pad::IsTrigger(PAD_INPUT_2))
			{
				ChangeAnim(kRunAnimIndex);

				move = VScale(move, speed + kDash);

				m_pos = VAdd(m_pos, move);
			}
			if (Pad::IsRelase(PAD_INPUT_2))
			{
				ChangeAnim(kIdleAnimIndex);
			}

			//移動
			if (Pad::IsTrigger(PAD_INPUT_RIGHT))
			{
				m_direction = kDown;
				ChangeAnim(kWalkAnimIndex);

				MV1SetRotationXYZ(modelHandle, VGet(0, kRotateDown, 0));

			}
			if (Pad::IsRelase(PAD_INPUT_RIGHT))
			{
				ChangeAnim(kIdleAnimIndex);

			}

			//移動
			if (Pad::IsTrigger(PAD_INPUT_LEFT))
			{
				m_direction = kUp;

				ChangeAnim(kWalkAnimIndex);

				MV1SetRotationXYZ(modelHandle, VGet(0, kRotateUp, 0));

			}
			if (Pad::IsRelase(PAD_INPUT_LEFT))
			{
				ChangeAnim(kIdleAnimIndex);

			}

			//移動
			if (Pad::IsTrigger(PAD_INPUT_UP))
			{
				m_direction = kRight;

				ChangeAnim(kWalkAnimIndex);

				MV1SetRotationXYZ(modelHandle, VGet(0, kRotateRight, 0));

			}
			if (Pad::IsRelase(PAD_INPUT_UP))
			{
				ChangeAnim(kIdleAnimIndex);

			}

			//移動
			if (Pad::IsTrigger(PAD_INPUT_DOWN))
			{
				m_direction = kLeft;

				ChangeAnim(kWalkAnimIndex);

				MV1SetRotationXYZ(modelHandle, VGet(0, kRotateLeft, 0));

			}
			if (Pad::IsRelase(PAD_INPUT_DOWN))
			{
				ChangeAnim(kIdleAnimIndex);

			}
		}
	}
	else
	{
		//攻撃アニメーションが終了したら待機アニメーションを再生する
		if (isLoop)
		{
			m_isAttack = false;
			ChangeAnim(kIdleAnimIndex);

		}

	}


	MV1SetPosition(modelHandle, m_pos);
	MV1SetRotationXYZ(modelHandle, VGet(0, angle, 0));
	
}

void Player::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);

#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xff00ff, true);

#endif


}




bool Player::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(modelHandle, attachNo);	//現在の再生カウントを取得

	//現在再生中のアニメーションの総カウントを取得
	float total = MV1GetAttachAnimTotalTime(modelHandle, attachNo);
	bool isLoop = false;
	//アニメーション進める
	now += 0.5f;

	if (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//進めた時間の設定
	MV1SetAttachAnimTime(modelHandle, attachNo, now);

	return isLoop;
}

void Player::ChangeAnim(int animIndex)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(modelHandle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(modelHandle, animIndex, -1, false);

	//切り替えの瞬間は変更後のアニメーションが再生される
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(modelHandle, m_currentAnimNo, m_animBlendRate);
}


//移動範囲
void Player::StageProcess()
{

	//ステージ外にでないようにする
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

//攻撃の当たり判定
bool Player::SphereHitFlag(std::shared_ptr<Enemy> pEnemy)
{

	float delX;
	float delY;
	float delZ;

	if (m_direction == kDown)
	{
		delX = (m_pos.x - pEnemy->GetPos().x) * (m_pos.x - pEnemy->GetPos().x);
		delY = ((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY)) *
			((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY));
		delZ = ((m_pos.z + 10)- pEnemy->GetPos().z) * ((m_pos.z + 10) - pEnemy->GetPos().z);

		//球と球の距離
		float Distance = sqrt(delX + delY + delZ);

		//球と球の距離が剣とエネミーの半径よりも小さい場合
		if (Distance < m_radius + pEnemy->GetRadius())
		{

			return true;
		}

	}
	if (m_direction == kUp)
	{
		delX = (m_pos.x - pEnemy->GetPos().x) * (m_pos.x - pEnemy->GetPos().x);
		delY = ((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY)) *
			((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY));
		delZ = ((m_pos.z - 10) - pEnemy->GetPos().z) * ((m_pos.z - 10) - pEnemy->GetPos().z);

		//球と球の距離
		float Distance = sqrt(delX + delY + delZ);

		//球と球の距離が剣とエネミーの半径よりも小さい場合
		if (Distance < m_radius + pEnemy->GetRadius())
		{

			return true;
		}

	}
	if (m_direction == kRight)
	{
		delX = ((m_pos.x - 10) - pEnemy->GetPos().x) * ((m_pos.x - 10) - pEnemy->GetPos().x);
		delY = ((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY)) *
			((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY));
		delZ = (m_pos.z - pEnemy->GetPos().z) * (m_pos.z - pEnemy->GetPos().z);

		//球と球の距離
		float Distance = sqrt(delX + delY + delZ);

		//球と球の距離が剣とエネミーの半径よりも小さい場合
		if (Distance < m_radius + pEnemy->GetRadius())
		{

			return true;
		}

	}
	if (m_direction == kLeft)
	{
		delX = ((m_pos.x + 10) - pEnemy->GetPos().x) * ((m_pos.x + 10) - pEnemy->GetPos().x);
		delY = ((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY)) *
			((m_pos.y + kAddposY) - (pEnemy->GetPos().y + kAddposY));
		delZ = (m_pos.z - pEnemy->GetPos().z) * (m_pos.z - pEnemy->GetPos().z);

		//球と球の距離
		float Distance = sqrt(delX + delY + delZ);

		//球と球の距離が剣とエネミーの半径よりも小さい場合
		if (Distance < m_radius + pEnemy->GetRadius())
		{

			return true;
		}

	}

	return false;
}
