#include <windows.h>
#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include"scrollManager.h"
#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// DxLib������
	//ChangeWindowMode(TRUE);
	SetGraphMode(1920, 1080, 16);
	// ����������҂����Ƀt���b�v�������s��
	SetWaitVSyncFlag(0);
	
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	playerInit();
	mapInit();
	
    //���[���h���W
    sHitRect stageRect;
    stageRect.worldLX = 0;
    stageRect.worldLY = -64;
    stageRect.worldRX = 64 * 120;
    stageRect.worldRY = 64 * 20;
    scrollManagerInfo(1920, 1080, stageRect);
		;
	LONGLONG nowCount, prevCount;                      // �}�C�N���b(100������1�b�P�ʂŎ����擾)
	nowCount = prevCount = GetNowHiPerformanceCount();
	float fixedDeltaTime = 1.0f / 60.0f;               // 60����1�b = 0.01666...�b
	float waitFrameTime = 16000;                       // 16000�}�C�N���b = 16�~���b = 0.016�b
	float playerX = 0;
	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// deltaTime�v��
		float deltaTime;
		nowCount = GetNowHiPerformanceCount();
		deltaTime = (nowCount - prevCount) / 1000000.0f;
		
		// �X�V���� 120fps�z��
		playerX = playerUpdate(fixedDeltaTime);
		
		// �v���C���[�̓����蔻���`
		sHitRect playerRect;
		playerRect = getPlayerHitRect();

		// �����Ԃ������Ȃ瓖���蔻��{�b�N�X����v���C���[�ʒu���C������
		if (mapHitCalc(playerRect))
		{
			playerfixColPosition(playerRect);
		}

		// �����`�F�b�N
		playerRect = playerGetGroundCollider();
		playerSetGroundFlg(mapHitCalc(playerRect));

		// ����`�F�b�N
		playerRect = playerGetHeadCollider();
		playerSetHeadHitFlg(mapHitCalc(playerRect));
		//�X�N���[���X�V����
		scrollUpdate(deltaTime,playerX);
		
		//��ʍX�V����
		ClearDrawScreen();

		// �}�b�v�`��
		mapDraw();

		//�v���C���[�`��
		playerDraw();

		ScreenFlip();

		// 1�t���[���̍X�V������waitFrameTime�ɂȂ�܂ő҂� 1/60�b���̍X�V�ɂ���
		while (GetNowHiPerformanceCount() - nowCount < waitFrameTime)
		{
			;
		}

		prevCount = nowCount;
	}

	playerFinalize();
	mapFinalize();

	DxLib_End();
	return 0;
}

