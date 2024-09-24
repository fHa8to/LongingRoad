#include "Boss.h"
#include "Knight.h"
#include <math.h>

namespace
{
	//�G�̑���
	constexpr float kSpeed = 2.0f;

	//�̗�
	constexpr int kMaxHp = 100;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�A�j���[�V����
	constexpr int kWalkAnimIndex = 6;
	constexpr int kAttackAnimIndex = 82;
	constexpr int kWaitAnimIndex = 89;
	constexpr int kDeathAnimIndex = 3;

	//���
	constexpr float kSkyAboveY = 100.0f;

	//�����蔻��̒���
	constexpr float kAdj = 45.0f;

	//�����蔻��̑傫��
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
	//�G�̏����ʒu����^�[�Q�b�g�ʒu�Ɍ������x�N�g���𐶐�����
	//�n�_����I�_�Ɍ������x�N�g�������߂�ꍇ�A�I�_�̍��W-�n�_�̍��W�ŋ��߂�
	VECTOR toTarget = VSub(playerPos, m_pos);

	//�x�N�g���̒�����kSpeed�ɂ��Ă��
	//�x�N�g���̐��K���@�������P�ɂ���
	toTarget = VNorm(toTarget);
	//kSpeed�ł�����
	m_impactPos.x = toTarget.x * kSpeed;
	m_impactPos.y = toTarget.y + (m_skyAboveY - 1.0f);
	m_impactPos.z = toTarget.z * kSpeed;

	m_impactPos = VAdd(m_pos, m_impactPos);

	m_longRangeAttackCollision.SetCenter(m_impactPos.x, m_impactPos.y, m_impactPos.z, 100, 100, 100);
}

void Boss::Move(Knight* knight)
{
	VECTOR playerPos = knight->GetPlayerPos();
	// �R�c���f���Q����R�c���f���P�Ɍ������x�N�g�����Z�o
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan2 ���g�p���Ċp�x���擾
	float Angle = atan2(SubVector.x, SubVector.z);

	//�v���C���[�̕���������
	MV1SetRotationXYZ(m_handle, VGet(0.0f, Angle + DX_PI_F, 0.0f));

	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_handle, m_pos);
}

void Boss::Animation()
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
}

bool Boss::UpdateAnim(int attachNo)
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

void Boss::ChangeAnim(int animIndex)
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
