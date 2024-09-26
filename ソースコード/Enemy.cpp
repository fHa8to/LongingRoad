#include "Enemy.h"
#include "DxLib.h"
#include "Player.h"
#include <cmath>

namespace
{
	//���f���̃t�@�C����
	//const char* const kModelFilename = "data/model/Barbarian.mv1";
	const char* const kModelFilename = "data/model/Bee.mv1";

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 0.1f;

	//�G�̑���
	constexpr float kSpeed = 0.3f;

	//�ő�܂ł����߂�Ƃ���
	constexpr float kMaxX = 200.0f;
	constexpr float kMaxZ = 200.0f;

	//�A�j���[�V�����ԍ�
	constexpr int kIdleAnimIndex = 1;		//�ҋ@
	constexpr int kWalkAnimIndex = 2;		//����
	constexpr int kAttackAnimIndex = 30;	//�U��


	//�U�����肪��������܂łɂ����鎞��
	constexpr float kAttackFrameStart = 20;
	//�U�����肪�I������܂łɂ����鎞��
	constexpr float kAttackFrameEnd = 30;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�����蔻��
	constexpr float kAddposY = 10.0f;

	//�p�x
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
	//3D���f���̓ǂݍ���
	m_modelHandle = MV1LoadModel(kModelFilename);

	m_state = kMove;

}

Enemy::~Enemy()
{

}

void Enemy::Init()
{

	//�G�l�~�[�̏����ʒu�ݒ�
	m_pos = VGet(-100.0f, 0.0f, 0.0f);

	m_currentAnimNo = MV1AttachAnim(m_modelHandle, kWalkAnimIndex, -1, false);
	m_prevAnimNo - 1;
	m_animBlendRate = 1.0f;

	m_state = kMove;
	
	MV1SetScale(m_modelHandle, VGet(kExpansion, kExpansion, kExpansion));


}

void Enemy::Update(VECTOR playerPos)
{	

	//�v���C���[�̍��W
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

	//���f���̌�����ς���
	VECTOR SubVector = VSub(playerPos, m_pos);

	// atan ���g�p���Ċp�x���擾
	float Angle = atan2(SubVector.x, SubVector.z);

	//�v���C���[�̕���������
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, Angle + DX_PI_F, 0.0f));

	//�A�j���[�V����
	Animation();


	StageProcess();

	
	// �RD���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_modelHandle, m_pos);

}

void Enemy::Draw()
{
	// �R�c���f���̕`��
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawFormatString(0, 32, 0xffffff, "Enemy(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

#endif

}

//���̓����蔻��
bool Enemy::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	if (m_state == kAttack)
	{
		float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
		float delY = ((m_pos.y + kAddposY) - (pPlayer->GetPos().y + kAddposY)) *
			((m_pos.y + kAddposY) - (pPlayer->GetPos().y + kAddposY));
		float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

		//���Ƌ��̋���
		float Distance = sqrt(delX + delY + delZ);

		//���Ƌ��̋��������ƃG�l�~�[�̔��a�����������ꍇ
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
		//test 8�t���[���Ő؂�ւ�
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//�ύX��̃A�j���[�V����������ݒ肷��
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
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);	//���݂̍Đ��J�E���g���擾

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;
	//�A�j���[�V�����i�߂�
	now += 0.5f;

	if (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃ̐ݒ�
	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);

	return isLoop;
}

void Enemy::ChangeAnim(int animIndex)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//�؂�ւ��̏u�Ԃ͕ύX��̃A�j���[�V�������Đ������
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}


//�ړ��͈�
void Enemy::StageProcess()
{
	//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ���
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


