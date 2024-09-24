#include "Pad.h"
#include "DxLib.h"

namespace
{
	int lastPad = 0;

	int nowPad = 0;
}

namespace Pad
{

	void Update()
	{
		lastPad = nowPad;

		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool IsPress(int key)
	{
		return (nowPad & key);
	}

	bool IsTrigger(int key)
	{

		bool isNow = (nowPad & key);
		bool isLast = (lastPad & key);

		if (isNow && !isLast)
		{
			return true;
		}
		return false;
	}

	bool IsRelase(int key)
	{
		bool isNow = (nowPad & key);	// ���̃t���[��
		bool isLast = (lastPad & key);	// �O�̃t���[��

		if (!isNow && isLast)		// �O��̃t���[���͉�����Ă���
		{
			return true;
		}
		return false;
	}
}
