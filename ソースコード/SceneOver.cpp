#include "SceneOver.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "Game.h"
#include "Pad.h"
#include "Player.h"

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
	constexpr int kStandByAnimIndex = 29;

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


SceneOver::SceneOver():
	m_handle(false),
	modelHandle(false),
	m_pos(VGet(0.0f,0.0f,0.0f)),
	m_cameraPos(VGet(0.0f, 0.0f, 0.0f))

{
}

SceneOver::~SceneOver()
{
	DeleteGraph(m_handle);


	MV1DeleteModel(modelHandle);
	MV1DeleteModel(modelHandle2);

}

void SceneOver::Init()
{
	isSceneEnd = false;

	fadeAlpha = kFadeValue;

	m_handle = LoadGraph("data/data/GameOver.png");

	modelHandle = MV1LoadModel("data/model/knight.mv1");

	modelHandle2 = MV1LoadModel("data/model/tileHigh_forest.mv1");

	//���f���̃T�C�Y����
	MV1SetScale(modelHandle, VGet(kExpansion, kExpansion, kExpansion));
	MV1SetScale(modelHandle2, VGet(2000, 50, 400));

	//�A�j���[�V�����̏����ݒ�
	m_currentAnimNo = MV1AttachAnim(modelHandle, kStandByAnimIndex, -1, true);

	SetFontSize(kFontSize);

	m_pos = VGet(kPosX, kPosY, kPosZ);

	m_cameraPos.z = kCameraZ;
	m_cameraPos.y = kCameraY;
	m_cameraPos.x = kCameraX;;
	SetCameraPositionAndTarget_UpVecY(m_cameraPos, VGet(100, 200, 10));

}

std::shared_ptr<SceneBase> SceneOver::Update()
{

	if (Pad::IsTrigger(PAD_INPUT_1))	// �p�b�h��1�{�^��or�L�[�{�[�h��Z�L�[
	{

		isSceneEnd = true;
		PlaySoundFile(kBgmButton, DX_PLAYTYPE_BACK);

	}

	if (isSceneEnd && fadeAlpha >= kFadeValue)
	{
		return std::make_shared<SceneTitle>();

	}

	//���f���̈ʒu�X�V
	MV1SetPosition(modelHandle, m_pos);
	MV1SetPosition(modelHandle2, VGet(kPosX - 100, kPosY - 100, kPosZ));


	//�t���[���C���A�A�E�g
	if (isSceneEnd)
	{
		fadeAlpha += kFadeUpDown;
		if (fadeAlpha > kFadeValue)
		{
			fadeAlpha = kFadeValue;
		}
	}
	else
	{
		fadeAlpha -= kFadeUpDown;
		if (fadeAlpha < 0)
		{
			fadeAlpha = 0;
		}
	}



	return shared_from_this();
}

void SceneOver::Draw()
{
	DrawGraph(0, 0, m_handle, true);

	MV1DrawModel(modelHandle);
	MV1DrawModel(modelHandle2);

#ifdef _DEBUG

	DrawString(8, 8, "SceneOver", GetColor(255, 255, 255));

#endif

	//�t�F�[�h�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha); //�������ŕ\��
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); //�s�����ɖ߂��Ă���


}

void SceneOver::End()
{
}

void SceneOver::Animation()
{
	if (m_prevAnimNo != -1)
	{
		//test 8�t���[���Ő؂�ւ�
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;
		//�ύX��̃A�j���[�V����������ݒ肷��
		MV1SetAttachAnimBlendRate(modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(modelHandle, m_currentAnimNo, m_animBlendRate);
	}
	bool isLoop = UpdateAnim(m_currentAnimNo);
	if (isLoop)
	{
		UpdateAnim(m_prevAnimNo);
	}
	ChangeAnim(kStandByAnimIndex);
}

bool SceneOver::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(modelHandle, attachNo);

	//�A�j���[�V�����i�߂�
	now += 0.5f;

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(modelHandle, attachNo);
	bool isLoop = false;

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(modelHandle, attachNo, now);
	return isLoop;
}

void SceneOver::ChangeAnim(int animIndex)
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

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(modelHandle, m_currentAnimNo, m_animBlendRate);
}
