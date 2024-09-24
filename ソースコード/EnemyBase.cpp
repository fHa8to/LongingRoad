#include "DxLib.h"
#include "EnemyBase.h"

namespace
{
	//���f���̃T�C�Y�̑傫���̒���
	constexpr float kExpansion = 0.75f;


	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
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
	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_handle, m_pos);

	// �R�c���f���̕`��
	MV1DrawModel(m_handle);
}


void EnemyBase::OnDamage()
{
}

void EnemyBase::End()
{
	MV1DeleteModel(m_handle);
}
