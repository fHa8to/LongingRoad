#include "Camera.h"
#include "DxLib.h"
#include "Pad.h"
#include <cmath>
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

namespace
{
	//�J�����̕`��͈�
	constexpr float kMinimumDrawingrange = 50.0f;
	constexpr float kMaximumDrawingrange =5000.0f;

	//�J�����̍��W
	constexpr float kCameraPosY = 100.0f;

	// ���񑬓x
	constexpr float kAngleSpeed = 0.05f;	

	//�����_�̍��W
	constexpr float kCameraTargetY = 30.0f;

	//�J�����̋���
	constexpr float kCameraDist = 100.0f;


	constexpr float AngleSpeed = 0.05f;	// ���񑬓x
}


Camera::Camera()
{
		

}


Camera::~Camera()
{
	// �����Ȃ�.
}

void Camera::Init()
{
	//���s100�`5000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(kMinimumDrawingrange, kMaximumDrawingrange);


	//������
	m_cameraPos = VGet(0.0f, 0.0f, 0.0f);
	
	m_cameraTarget = VGet(0.0f, 0.0f, 0.0f);

	m_cameraAngle = 0.0f;

	m_angleH = 0.0f;

	m_pad = 0;
}


void Camera::Update(VECTOR mplayerPos)
{

#if _DEBUG

	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
	{
		m_cameraPos.x += 500.0f;
		m_cameraPos.z += 600.0f;
	}

#endif

		m_pad = GetJoypadDirectInputState(DX_INPUT_PAD1,&m_input);


	if (m_input.Rx > 0)
	{
		m_angleH += AngleSpeed;


	}
	if (m_input.Rx < 0)
	{
		m_angleH -= kAngleSpeed;


	}


	
	m_cameraPos.x = cosf(m_angleH) * kCameraDist;
	m_cameraPos.y = kCameraPosY;
	m_cameraPos.z = sinf(m_angleH) * kCameraDist;

	// �J�������W
	m_cameraPos = VAdd(mplayerPos, m_cameraPos);

	// �����_
	m_cameraTarget = VAdd(mplayerPos, VGet(0.0f, kCameraTargetY, 0.0f));

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

}

void Camera::Draw()
{

#ifdef _DEBUG
	DrawFormatString(160, 200, GetColor(255, 255, 255), "�J�����̍��W(%.2f,%.2f,%.2f)", m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);

	DrawFormatString(250, 250, GetColor(255, 255, 255), "%d", m_input.Rx);
#endif
}

