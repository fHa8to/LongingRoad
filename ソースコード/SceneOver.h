#pragma once
#include "SceneBase.h"
#include "DxLib.h"

class SceneOver : public SceneBase
{
public:
	SceneOver();
	virtual ~SceneOver();

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
	int m_modelHandle;
	int m_modelHandle2;

	int m_handle;

	int m_fadeAlpha;

	bool m_isSceneEnd;

	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������


	//���f���̍��W
	VECTOR m_pos;

	//�J�����̍��W
	VECTOR m_cameraPos;


};

