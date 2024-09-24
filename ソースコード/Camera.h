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

    //ジョイパッドの出力
    DINPUT_JOYSTATE m_input;

    //ジョイパッドの出力の保持
    int m_pad;

    // 水平角度
    float m_angleH;


    float m_cameraAngle;

    //カメラの座標
    VECTOR m_cameraPos;

    //カメラの注視点
    VECTOR m_cameraTarget;

    //プレイヤーの座標
    VECTOR m_playerTarget;

};

