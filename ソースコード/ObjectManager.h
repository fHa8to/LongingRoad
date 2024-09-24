#pragma once
#include <memory>
#include <list>
#include <functional>

class Enemy;
class Player;
class Time;

class ObjectManager
{

public:
	//�v���C���[�����݂��邩�ǂ���
	bool IsPlayerEnable();

	//�G�����݂��邩
	bool IsEnemyEnable();

	//�Q�[���N���A�������ǂ���
	bool IsGameClear() const { return m_isGameClear; }

	void Update();
	void Draw();

	//���b�N�I������Ă���G���X�V
	void LockOnEnemyUpdate();

	//���b�N�I������Ă���G�|�C���^�[�擾
	const Enemy* const GetLockOnEnemy() { return m_pLockOnEnemy; }

	//�v���C���[�|�C���^�擾
	Player* const GetPlayer();


	//�����I�ɃQ�[���N���A�ɂ���
	void SetGameClear();
private:
	//�X�e�[�W���N���A�����ۂ̍X�V����
	void GameClearUpdate();
private:

	//���b�N�I������Ă���G�|�C���^�[
	Enemy* m_pLockOnEnemy = nullptr;


	std::shared_ptr<Time> m_pBossDeadNextSceneTime;
	bool m_isGameClear = false;

	bool m_isBossDead = false;


};

