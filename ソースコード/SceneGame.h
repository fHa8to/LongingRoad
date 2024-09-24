#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "SceneBase.h"

#define P_HP_MAX		5
#define P_DRAW_SIZE		50
#define E_HP_MAX		5
#define E_DRAW_SIZE		50

class Camera;
class Player;
class Enemy;

class SceneGame : public SceneBase
{
public:
	SceneGame();
	virtual ~SceneGame();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();


private:
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;



private:
	//�t�F�[�h�C���A�A�E�g
	int m_fadeAlpha;

	//�V�[���I���t���O
	bool m_isSceneEnd;

	//�Q�[���N���A�Ȃ̂��̃t���O
	bool m_isGameClear;

	//�v���C���[�ɓ����������̃t���O
	bool m_isHitPlayer;

	bool m_isEnemyHit;

	//�v���C���[��HP��0�ɂȂ������̃t���O
	bool m_isHp0;

	//�X�e�[�W���N���A������
	bool m_isStageClear;

	//�v���C���[�̍��W
	VECTOR m_playerPos;

	//�J�����̍��W
	VECTOR m_cameraPos;

	int fadeAlpha;

	bool isSceneEnd;


	//�����蔻��
	bool m_isPlayerHit;
	bool m_isAttackHit;
	bool m_isGimmickHit;

	int modelHandle;

	int playerHp;
	int playerDrawValue;
	int enemyHp;
	int enemyDrawValue;

	int bgm;
	int bgmPlayer;
	int bgmEnemy;

	VECTOR m_pos;

};

