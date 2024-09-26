#pragma once
#include "DxLib.h"
#include <memory>

class Enemy;
class Player
{
public:
	Player();
	virtual ~Player();

	void Init();
	void Update(VECTOR cameraPos);
	void Draw();

	//���a�̎擾
	float GetRadius() { return m_radius; }

	//�v���C���[�̍��W���擾	
	const VECTOR GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }



	//�U���ƓG�̂����蔻��
	bool SphereHitFlag(std::shared_ptr<Enemy> pEnemy);


private:


	bool UpdateAnim(int attachNo);

	void ChangeAnim(int animIndex);

	//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ��鏈��
	void StageProcess();


private:
	std::shared_ptr<Enemy> m_pEnemy;
private:


	//����
	enum  direction
	{
		kRight,
		kLeft,
		kUp,
		kDown
	};


private:
	int		m_modelHandle;	//���f���n���h��

	//�A�j���[�V�������
	int m_currentAnimNo;
	int m_prevAnimNo;
	float m_animBlendRate;

	//���
	int m_state;

	//�����Ă������
	int m_direction;

	//A�{�^�������񉟂�����
	int m_countAButton;

	//X�{�^�������񉟂�����
	int m_countXButton;


	//�����Ă��邩��ێ�����
	bool m_isMove;



	bool m_isAttacking;


	//HP
	int m_hp;


	//�\�����
	VECTOR m_pos;

	VECTOR m_attackPos;
	//�J�����̈ʒu
	VECTOR m_cameraPos;

	float m_angle;

	//�����蔻��̔��a
	float m_radius;

	//�J�������
	float m_cameraAngle;


	//�����Ă��邩
	bool m_iskStandby;

	//�����Ă��邩
	bool m_isWalk;

	//�����Ă��邩
	bool m_isRun;

	//�U�����Ă��邩
	bool m_isAttack;


};

