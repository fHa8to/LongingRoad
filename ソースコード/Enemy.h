#pragma once
#include "DxLib.h"
#include <memory>


class Player;
class Enemy
{
public:
	Enemy();
	virtual ~Enemy();

	void Init();
	void Update(VECTOR playerPos);
	void Draw();

	//�v���C���[�ƓG�̓����蔻��
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�U���ƓG�̓����蔻��
	//bool SphereHitFlag2(std::shared_ptr<Player> pPlayer);



	float GetRadius() { return m_radius; }

	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

private:

	void Attack();

	//�A�j���[�V�����֘A
	void  Animation();

	bool UpdateAnim(int attachNo);

	void ChangeAnim(int animIndex);

	//�X�e�[�W�O�ɏo�Ȃ��悤�ɂ��鏈��
	void StageProcess();


private:
	std::shared_ptr<Player> m_pPlayer;
private:
	//�G�l�~�[�̏��
	enum  state
	{
		kMove,			//�����Ă���
		kAttack,		//�U����
	};

private:

	//���
	int m_state;

	int modelHandle;		//���f���n���h��

	//�A�j���[�V�������
	int currentAnimNo;
	int prevAnimNo;
	float animBlendRate;

	//�����蔻��̔��a
	float m_radius;

	//�\�����
	VECTOR m_pos;
	VECTOR playerPos;

	float Angle;

	float angle;

	//�U�����Ă��邩��ێ�����
	bool isAttacking;

	int isAttack;

	//�i�ދ���
	VECTOR m_distance;

};

