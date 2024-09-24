#include "DxLib.h"
#include "EnemyBase.h"

namespace
{
	//モデルのサイズの大きさの調整
	constexpr float kExpansion = 0.75f;


	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;
}

EnemyBase::EnemyBase(int ModelHandle)
{
	m_handle = ModelHandle;
	m_state = kMove;
	MV1SetScale(m_handle, VGet(kExpansion, kExpansion, kExpansion));
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Init()
{
}

void EnemyBase::Draw()
{
#if _DEBUG
	m_enemyCollision.Draw(0x000000,true);
#endif
	//ポジションの設定
	MV1SetPosition(m_handle, m_pos);

	// ３Ｄモデルの描画
	MV1DrawModel(m_handle);
}


void EnemyBase::OnDamage()
{
}

void EnemyBase::End()
{
	MV1DeleteModel(m_handle);
}
