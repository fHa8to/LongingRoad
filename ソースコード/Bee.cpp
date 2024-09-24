#include "DxLib.h"
#include "Knight.h"
#include "Bee.h"
#include <math.h>

namespace
{
	//�G�̑���
	constexpr float kSpeed = 2.0f;

	//�G�̍ő�HP
	constexpr int kMaxHp = 50;


	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�A�j���[�V����
	constexpr int kRunAnimIndex = 2;
	constexpr int kAttackAnimIndex = 0;
	constexpr int kDamageAnimIndex = 3;
	constexpr int kDeathAnimIndex = 1;

	//�����ʒu
	constexpr int kAppearanceX = 750;

	//�����蔻��̒���
	constexpr float kAdj = 45.0f;

	//�����蔻��̑傫��
	constexpr float kModelWidth = 80.0f;
	constexpr float kModelHeight = 100.0f;
	constexpr float kModelDepth = 100.0f;

	//�v���C���[�̈ʒu
	constexpr float kPlayerPos1X = -4500.0f;

	constexpr float kPlayerPos2X = 265.0f;

	constexpr float kPlayerPos3X = 6175.0f;

	//�n�`�̏����ʒu
	constexpr float kPosY = 240.0f;

	//�m�b�N�o�b�N
	constexpr float kKnockback = 50.0f;

	constexpr float kMaxKnockback = 250.0f;

}

Bee::Bee(int m_handle,VECTOR playerPos) :EnemyBase(m_handle), m_animBlendRate(-1), m_currentAnimNo(-1), m_prevAnimNo(-1),m_isAttacking(false),m_isAttack(false), m_playerNum(1)
{
	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(m_handle, kRunAnimIndex, -1, true);

	m_hp = kMaxHp;

	//���W�ݒ�
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
	//�G�̏����ʒu����^�[�Q�b�g�ʒu�Ɍ������x�N�g���𐶐�����
	//�n�_����I�_�Ɍ������x�N�g�������߂�ꍇ�A�I�_�̍��W-�n�_�̍��W�ŋ��߂�
	VECTOR toTarget = VSub(playerPos, m_pos); 

	//�x�N�g���̒�����kSpeed�ɂ��Ă��
	//�x�N�g���̐��K���@�������P�ɂ���
	toTarget = VNorm(toTarget);
	//kSpeed�ł�����
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


	// �R�c���f���Q����R�c���f���P�Ɍ������x�N�g�����Z�o
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan2 ���g�p���Ċp�x���擾
	float Angle = atan2(SubVector.x, SubVector.z);

	//�v���C���[�̕���������
	MV1SetRotationXYZ(m_handle, VGet(0.0f, Angle + DX_PI_F , 0.0f));


	//�|�W�V�����̐ݒ�
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
		//test 8�t���[���Ő؂�ւ�
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//�ύX��̃A�j���[�V����������ݒ肷��
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
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_handle, attachNo);

	//�A�j���[�V�����i�߂�
	now += 0.5f;

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_handle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_handle, attachNo, now);
	return isLoop;
}

void Bee::ChangeAnim(int animIndex)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_handle, m_prevAnimNo);
	}

	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_handle, animIndex, -1, false);

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_handle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_handle, m_currentAnimNo, m_animBlendRate);
}
