#include "SceneGame.h"
#include "DxLib.h"
#include "Camera.h"
#include "SceneClear.h"
#include "SceneOver.h"
#include "Game.h"
#include "Pad.h"

namespace
{
	//�t�H���g�̃T�C�Y
	constexpr int kFontSize = 20;

	//�t�F�[�h�C���A�t�F�[�h�A�E�g�̐��l
	constexpr int kFadeValue = 255;

	//�t�F�[�h�l�̑���
	constexpr int kFadeUpDown = 8;

	//���f���̃t�@�C����
	const char* const kModelFilename = "data/model/tileHigh_forest.mv1";


	//���f���̃T�C�Y�ύX
	constexpr float kExpansionX = 500.0f;
	constexpr float kExpansionY = 150.0f;
	constexpr float kExpansionZ = 500.0f;
}


void DrawGrid()
{
	for (int x = -80; x <= 80; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -80), VGet(static_cast<float>(x), 0, 80), 0xffff00);
	}
	for (int z = -80; z <= 80; z += 10)
	{
		DrawLine3D(VGet(-80, 0, static_cast<float>(z)), VGet(80, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-�̕�����������₷���悤�ɕ\����ǉ�����
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(500, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(-500, 0, 0));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 500));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	}

	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -500));
	if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
	{
		DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
	}
}


SceneGame::SceneGame():
	m_isPlayerHit(false),
	m_isEnemyHit(false),
	m_isAttackHit(false),
	m_pos(VGet(0.0f, -300.0f, 0.0f))
{
	m_pEnemy = std::make_shared<Enemy>();
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();

	//3D���f���̓ǂݍ���
	m_modelHandle = MV1LoadModel(kModelFilename);

	m_bgm = LoadSoundMem("data/sound/fantasy02.mp3");
	m_bgmPlayer = LoadSoundMem("data/sound/Player.mp3");
	m_bgmEnemy = LoadSoundMem("data/sound/AxeSound.mp3");


	PlaySoundMem(m_bgm, DX_PLAYTYPE_LOOP);

}

SceneGame::~SceneGame()
{

}

void SceneGame::Init()
{
	MV1SetScale(m_modelHandle, VGet(kExpansionX, kExpansionY, kExpansionZ));
	m_pPlayer->Init();
	m_pEnemy->Init();
	m_pCamera->Init();

	SetFontSize(kFontSize);


	m_playerHp = P_HP_MAX;
	m_playerDrawValue = m_playerHp * P_DRAW_SIZE;

	m_enemyHp = E_HP_MAX;
	m_enemyDrawValue = m_enemyHp * E_DRAW_SIZE;

}

std::shared_ptr<SceneBase> SceneGame::Update()
{
	

	int pTargetValut = m_playerHp * P_DRAW_SIZE;

	if (m_playerDrawValue > pTargetValut)
	{
		m_playerDrawValue--;
	}
	int eTargetValut = m_enemyHp * E_DRAW_SIZE;

	if (m_enemyDrawValue > eTargetValut)
	{
		m_enemyDrawValue--;
	}
	
	m_cameraPos = m_pCamera->GetCameraPos();

	m_pPlayer->Update(m_cameraPos);

	m_playerPos = m_pPlayer->GetPos();

	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);

	m_isEnemyHit = m_pPlayer->SphereHitFlag(m_pEnemy);


	m_pEnemy->Update(m_playerPos);
	m_pCamera->Update(m_playerPos);

	MV1SetPosition(m_modelHandle, m_pos);



	VECTOR toEnemy = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());


	float length = VSize(toEnemy);

	//�v���C���[�ƃG�l�~�[�����������ꍇ
	if (m_isPlayerHit)
	{

		VECTOR posVec;
		VECTOR moveVec;

		PlaySoundMem(m_bgmPlayer, DX_PLAYTYPE_BACK);
		//�G�l�~�[�̃x�N�g�����W����v���C���[�̃x�N�g�����W���������x�N�g��
		posVec = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());

		moveVec = VScale(posVec,  - (m_pPlayer->GetRadius() + m_pEnemy->GetRadius()));
		m_pPlayer->SetPos(VAdd(m_pPlayer->GetPos(), moveVec));

		m_playerHp -= 1;

	}

	//�v���C���[�U�������������ꍇ
	if (Pad::IsTrigger(PAD_INPUT_1) &&m_isEnemyHit)
	{
		VECTOR posVec2;
		VECTOR moveVec2;

		PlaySoundMem(m_bgmEnemy, DX_PLAYTYPE_BACK);
		//�v���C���[�̃x�N�g�����W����G�l�~�[�̃x�N�g�����W���������x�N�g��
		posVec2 = VSub(m_pPlayer->GetPos(),m_pEnemy->GetPos());

		moveVec2 = VScale(posVec2,  - (m_pPlayer->GetRadius() + m_pEnemy->GetRadius()));
		m_pEnemy->SetPos(VAdd(m_pEnemy->GetPos(), moveVec2));

		m_enemyHp -= 1;
	}


	//�v���C���[��HP���[���ɂȂ�����
	if (m_playerHp <= 0)
	{
		StopSoundMem(m_bgm);
		//SceneOver�ɑJ��
		return std::make_shared<SceneOver>();


	}

	//�G�l�~�[��HP���[���ɂȂ�����
	if (m_enemyHp <= 0)
	{
		StopSoundMem(m_bgm);
		//SceneClear�ɑJ��
		return std::make_shared<SceneClear>();


	}
	return shared_from_this();	//���g��shared_ptr��Ԃ�




}

void SceneGame::Draw()
{
	// �R�c���f���̕`��
	MV1DrawModel(m_modelHandle);


#ifdef _DEBUG

	DrawGrid();
	
#endif

	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pCamera->Draw();

	//�v���C���[��HP�o�[
	int playerColor = GetColor(0, 255, 0);
	DrawFormatString(15, 600, GetColor(0, 255, 255), "HP ", m_playerDrawValue);
	DrawFillBox(48, 598, 352, 618, GetColor(255, 255, 255));
	DrawFillBox(50, 600, 100 + m_playerHp * P_DRAW_SIZE, 616, playerColor);
	
	//�G�l�~�[��HP�o�[
	int enemyColor = GetColor(255, 0, 0);
	DrawFormatString(15, 50, GetColor(0, 255, 255), "HP ", m_enemyDrawValue);
	DrawFillBox(48, 48, 352, 68, GetColor(255, 255, 255));
	DrawFillBox(50, 50, 100 + m_enemyHp * E_DRAW_SIZE, 66, enemyColor);


#ifdef _DEBUG

	
	DrawString(8, 8, "SceneGame", GetColor(255, 255, 255));

#endif




}

void SceneGame::End()
{
}

