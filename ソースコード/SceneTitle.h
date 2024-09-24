#pragma once
#include "DxLib.h"
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();
	virtual void Draw();
	virtual void End();

private:

	//�A�j���[�V�����֘A
	void  Animation();

	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

private:
	enum state
	{
		kWait,			//�ҋ@��
	};

private:

	//���
	int m_state;

	//���f��
	int modelHandle;
	int modelHandle2;

	int handle;

	int fadeAlpha;

	bool isSceneEnd;

	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//�n���h��
	int m_handle;

	//���f���̍��W
	VECTOR m_pos;

	//�J�����̍��W
	VECTOR m_cameraPos;

};

