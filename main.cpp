#include <windows.h>
#include "DxLib.h"
#include "player.h"
#include "map.h"
#include "collision.h"
#include"scrollManager.h"
#pragma warning(disable:4996)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// DxLib初期化
	//ChangeWindowMode(TRUE);
	SetGraphMode(1920, 1080, 16);
	// 垂直同期を待たずにフリップ処理を行う
	SetWaitVSyncFlag(0);
	
	if (DxLib_Init() == -1)
	{
		return -1;
	}

	playerInit();
	mapInit();
	
    //ワールド座標
    sHitRect stageRect;
    stageRect.worldLX = 0;
    stageRect.worldLY = -64;
    stageRect.worldRX = 64 * 120;
    stageRect.worldRY = 64 * 20;
    scrollManagerInfo(1920, 1080, stageRect);
		;
	LONGLONG nowCount, prevCount;                      // マイクロ秒(100万分の1秒単位で時刻取得)
	nowCount = prevCount = GetNowHiPerformanceCount();
	float fixedDeltaTime = 1.0f / 60.0f;               // 60分の1秒 = 0.01666...秒
	float waitFrameTime = 16000;                       // 16000マイクロ秒 = 16ミリ秒 = 0.016秒
	float playerX = 0;
	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// deltaTime計測
		float deltaTime;
		nowCount = GetNowHiPerformanceCount();
		deltaTime = (nowCount - prevCount) / 1000000.0f;
		
		// 更新処理 120fps想定
		playerX = playerUpdate(fixedDeltaTime);
		
		// プレイヤーの当たり判定矩形
		sHitRect playerRect;
		playerRect = getPlayerHitRect();

		// もしぶつかったなら当たり判定ボックスからプレイヤー位置を修正する
		if (mapHitCalc(playerRect))
		{
			playerfixColPosition(playerRect);
		}

		// 足元チェック
		playerRect = playerGetGroundCollider();
		playerSetGroundFlg(mapHitCalc(playerRect));

		// 頭上チェック
		playerRect = playerGetHeadCollider();
		playerSetHeadHitFlg(mapHitCalc(playerRect));
		//スクロール更新処理
		scrollUpdate(deltaTime,playerX);
		
		//画面更新処理
		ClearDrawScreen();

		// マップ描画
		mapDraw();

		//プレイヤー描画
		playerDraw();

		ScreenFlip();

		// 1フレームの更新時刻がwaitFrameTimeになるまで待つ＝ 1/60秒毎の更新にする
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

