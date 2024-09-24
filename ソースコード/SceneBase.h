#pragma once
#include <memory>

class SceneBase : public std::enable_shared_from_this<SceneBase>
{

public:
	SceneBase() {}
	virtual ~SceneBase() {}

	virtual void Init() = 0;

	virtual std::shared_ptr<SceneBase> Update() = 0;	//�V�[����ύX����ꍇ�͑J�ڐ�̃V�[���̃|�C���^

	virtual void Draw() = 0;	//���t���[���s���`�揈��
	virtual void End() = 0;		//�V�[���𔲂���Ƃ��̏���

};

