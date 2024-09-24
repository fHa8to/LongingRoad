#pragma once
#include "SceneBase.h"
#include <array>
#include <string>

class SceneDebug : public SceneBase
{
public:
	enum class SceneType
	{
		Debug,			//�f�o�b�O
		Main,			//���C��
		Option,			//�I�v�V����
		GameOver,		//�Q�[���I�[�o�[
		GameClear,		//�Q�[���N���A
		Title,			//�^�C�g��

		SceneNum,		//�V�[���̐�
	};
public:
	SceneDebug();
	virtual ~SceneDebug() {}

	virtual void Init() override final;
	virtual void End() override final;

	virtual SceneBase* Update() override final;
	virtual void Draw() override final;

	//�V�[���̎�ގ擾
	virtual SceneKind GetSceneKind() { return SceneKind::Debug; }

private:
	//�J�[�\���ԍ��A�b�v�f�[�g
	void CursorUpdate();
	//���̃V�[���A�b�v�f�[�g
	SceneBase* SceneUpdate();

private:
	//�J�[�\���̔ԍ�
	int m_cursorNum;
	//�V�[���ړ��p������
	std::array<std::string, static_cast<int>(SceneType::SceneNum)> m_sceneString;

};

