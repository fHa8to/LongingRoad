#include "Player.h"
#include "Enemy.h"
#include "Pad.h"
#include <cmath>

namespace
{
	//���f���̃t�@�C����
	const char* const kModelFilename = "data/model/Knight.mv1";



	//�A�j���[�V�����ԍ�
	constexpr int kIdleAnimIndex = 1;		//�ҋ@
	constexpr int kWalkAnimIndex = 2;		//����
	constexpr int kRunAnimIndex = 7;		//����
	constexpr int kAttackAnimIndex = 30;	//�U��
	constexpr int kAnimIndex = 3;

	

	//���鑬��
	constexpr float kDash = 15.0f;

	//�������W
	constexpr float kPosX = 400.0f;
	constexpr float kPosY = 0.0f;

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 10.0f;

	//�ő�܂ł����߂�Ƃ���
	constexpr float kMaxX = 200.0f;
	constexpr float kMaxZ = 200.0f;

	//�����蔻��
	constexpr float kAddposY = 10.0f;


	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�A�i���O�X�e�B�b�N�ɂ��ړ��֘A
	constexpr float kMaxSpeed = 0.5f;		//�v���C���[�̍ő�ړ����x
	constexpr float kAnalogRangeMin = 0.1;	//�A�i���O�X�e�B�b�N�̓��͔���͈�
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�l

		//��]��
	constexpr float kRotateRight = 270.0f * DX_PI_F / 180.0f;
	constexpr float kRotateLeft = 90.0f * DX_PI_F / 180.0f;
	constexpr float kRotateUp = 180.0f * DX_PI_F / 180.0f;
	constexpr float kRotateDown = 0.0f;


	//BGM�̃t�@�C����
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
	//3D���f���̓ǂݍ���
	modelHandle = MV1LoadModel(kModelFilename);
}

Player::~Player()
{
}

void Player::Init()
{	


	MV1SetScale(modelHandle, VGet(kExpansion, kExpansion, kExpansion));

	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(modelHandle, kIdleAnimIndex, -1, false);
	m_prevAnimNo - 1;
	m_animBlendRate = 1.0f;

	m_pos = VGet(10.0f, 0.0f, 0.0f);


	//MV1SetPosition(modelHandle, m_pos);

	m_isAttack = false;

}

void Player::Update(VECTOR cameraPos)
{
	
	//�A�j���[�V�����̐؂�ւ�
	if (m_prevAnimNo != -1)
	{
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f)	m_animBlendRate = 1.0f;
		//�ύX�O�̃A�j���[�V����100%
		MV1SetAttachAnimBlendRate(modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
		//�ύX��̃A�j���[�V����0%
		MV1SetAttachAnimBlendRate(modelHandle, m_currentAnimNo, m_animBlendRate);

	}

	//�A�j���[�V������i�߂�
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);

	StageProcess();



	//�{�^������������U���A�j���[�V�������Đ�����
	if (!m_isAttack)
	{

		//�U��
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


			//�A�i���O�X�e�B�b�N�̓��͂�10% ~ 80%�͈̔͂��g�p����

			//�x�N�g���̒������ő��1000�ɂȂ�
			//�v���C���[�̍ő�ړ����x��0.01f / frame

			VECTOR move = VGet(analogX, 0.0f, -analogZ);	//�x�N�g���̒�����0�`1000

			//0.00 ~ 0.01�̒����ɂ���
			//�x�N�g���̒������擾����
			float len = VSize(move);
			//�x�N�g���̒�����0.0~1.0�̊����ɕϊ�����
			float rate = len / kAnalogInputMax;

			//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����
			rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
			rate = min(rate, 1.0f);
			rate = max(rate, 0.0f);

			//���x������ł���̂ňړ��x�N�g���ɔ��f����
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

			//�ړ�
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

			//�ړ�
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

			//�ړ�
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

			//�ړ�
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
		//�U���A�j���[�V�������I��������ҋ@�A�j���[�V�������Đ�����
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
	// �R�c���f���̕`��
	MV1DrawModel(modelHandle);

#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xff00ff, true);

#endif


}




bool Player::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(modelHandle, attachNo);	//���݂̍Đ��J�E���g���擾

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾
	float total = MV1GetAttachAnimTotalTime(modelHandle, attachNo);
	bool isLoop = false;
	//�A�j���[�V�����i�߂�
	now += 0.5f;

	if (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃ̐ݒ�
	MV1SetAttachAnimTime(modelHandle, attachNo, now);

	return isLoop;
}

void Player::ChangeAnim(int animIndex)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(modelHandle, m_prevAnimNo);
	}

	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(modelHandle, animIndex, -1, false);

	//�؂�ւ��̏u�Ԃ͕ύX��̃A�j���[�V�������Đ������
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(modelHandle, m_currentAnimNo, m_animBlendRate);
}


//�ړ��͈�
void Player::StageProcess()
{

	//�X�e�[�W�O�ɂłȂ��悤�ɂ���
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

//�U���̓����蔻��
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

		//���Ƌ��̋���
		float Distance = sqrt(delX + delY + delZ);

		//���Ƌ��̋��������ƃG�l�~�[�̔��a�����������ꍇ
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

		//���Ƌ��̋���
		float Distance = sqrt(delX + delY + delZ);

		//���Ƌ��̋��������ƃG�l�~�[�̔��a�����������ꍇ
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

		//���Ƌ��̋���
		float Distance = sqrt(delX + delY + delZ);

		//���Ƌ��̋��������ƃG�l�~�[�̔��a�����������ꍇ
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

		//���Ƌ��̋���
		float Distance = sqrt(delX + delY + delZ);

		//���Ƌ��̋��������ƃG�l�~�[�̔��a�����������ꍇ
		if (Distance < m_radius + pEnemy->GetRadius())
		{

			return true;
		}

	}

	return false;
}
