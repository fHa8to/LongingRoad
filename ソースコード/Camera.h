#pragma once
#include "DxLib.h"

class Player;

class Camera
{


public:
    Camera();
    virtual ~Camera();

    void Init();

    void Update(VECTOR mplayerPos);

    void Draw();

    VECTOR GetCameraPos() const noexcept { return m_cameraPos; }

private:

    //�W���C�p�b�h�̏o��
    DINPUT_JOYSTATE m_input;

    //�W���C�p�b�h�̏o�͂̕ێ�
    int m_pad;

    // �����p�x
    float m_angleH;


    float m_cameraAngle;

    //�J�����̍��W
    VECTOR m_cameraPos;

    //�J�����̒����_
    VECTOR m_cameraTarget;

    //�v���C���[�̍��W
    VECTOR m_playerTarget;

};

