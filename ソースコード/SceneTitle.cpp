#include "SceneTitle.h"
#include "DxLib.h"
#include "SceneGame.h"
#include "Game.h"
#include "Pad.h"

namespace
{
	//�t�H���g�̃T�C�Y
	constexpr int kFontSize = 32;

	//�����̈ʒu
	constexpr int kFontPosX = 500;
	constexpr int kFontPosY = 500;


	//���f���̏����ʒu
	constexpr float kPosX = 300.0f;

	constexpr float kPosY = 100.0f;

	constexpr float kPosZ = 0.0f;

	//���f���̃T�C�Y�ύX
	constexpr float kExpansion = 100.0f;

	//�A�j�����[�V�����̔ԍ�
	//�ҋ@���[�V����
	constexpr int kStandByAnimIndex = 73;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 4.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̐��l
	constexpr int kFadeValue = 255;

	//�t�F�[�h�l�̑���
	constexpr int kFadeUpDown = 8;

	//�J�������
	constexpr float kCameraX = 0.0f;
	constexpr float kCameraY = 150.0f;
	constexpr float kCameraZ = -600.0f;

	//BGM�̃t�@�C����
	const char* const kBgmButton = "data/sound/Titledecide.mp3";

}

SceneTitle::SceneTitle():
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraPos(VGet(0.0f, 0.0f, 0.0f)),
	m_handle(false),
	m_modelHandle(false),
	m_state(kWait)
{

}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_handle);


	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_modelHandle2);

}

void SceneTitle::Init()
{
	m_isSceneEnd = false;

	m_fadeAlpha = kFadeValue;

	m_handle = LoadGraph("data/data/LongingRoad.png");

	m_modelHandle = MV1LoadModel("data/model/knight.mv1");

	m_modelHandle2 = MV1LoadModel("data/model/tileHigh_forest.mv1");

	//���f���̃T�C�Y����
	MV1SetScale(m_modelHandle, VGet(kExpansion, kExpansion, kExpansion));
	MV1SetScale(m_modelHandle2, VGet(2000, 50, 400));

	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, kStandByAnimIndex, -1, true);

	SetFontSize(kFontSize);

	m_pos = VGet(kPosX, kPosY, kPosZ);


	m_cameraPos.z = kCameraZ;
	m_cameraPos.y = kCameraY;
	m_cameraPos.x = kCameraX;;
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, VGet(100, 200, 10));


}

std::shared_ptr<SceneBase> SceneTitle::Update()
{

	if (Pad::IsTrigger(PAD_INPUT_1))	// �p�b�h��1�{�^��or�L�[�{�[�h��Z�L�[
	{

		m_isSceneEnd = true;
		PlaySoundFile(kBgmButton, DX_PLAYTYPE_BACK);


	}

	if(m_isSceneEnd && m_fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneGame>();

	}

	//���f���̈ʒu�X�V
	MV1SetPosition(m_modelHandle, m_pos);
	MV1SetPosition(m_modelHandle2, VGet(kPosX - 100, kPosY - 100, kPosZ));
	


	//�t���[���C���A�A�E�g
	if (m_isSceneEnd)
	{
		m_fadeAlpha += kFadeUpDown;
		if (m_fadeAlpha > kFadeValue)
		{
			m_fadeAlpha = kFadeValue;
		}
	}
	else
	{
		m_fadeAlpha -= kFadeUpDown;
		if (m_fadeAlpha < 0)
		{
			m_fadeAlpha = 0;
		}
	}

	return shared_from_this();
}


void SceneTitle::Draw()
{
	DrawGraph(0, 0, m_handle, true);

	MV1DrawModel(m_modelHandle);
	MV1DrawModel(m_modelHandle2);
	DrawString(kFontPosX, kFontPosY, "A�{�^���ŊJ�n", 0x000000);

#ifdef _DEBUG

	DrawString(0, 0, "SceneTitle", GetColor(255, 255, 255));

#endif

	//�t�F�[�h�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha); //�������ŕ\��
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���

}

void SceneTitle::End()
{
}
void SceneTitle::Animation()
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
	if (m_state == kWait)
	{
		if (isLoop)
		{
			UpdateAnim(m_prevAnimNo);
		}
	}
		ChangeAnim(kStandByAnimIndex);
}

bool SceneTitle::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);

	//�A�j���[�V�����i�߂�
	now += 0.5f;

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);
	return isLoop;
}

void SceneTitle::ChangeAnim(int animIndex)
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

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}
