#pragma once
#include "collision.h"

/// <summary>
/// スクロールマネージャー
/// </summary>
/// <param name="screenWidth">ウィンドウの幅ドット数</param>
/// <param name="screenHeight">ウィンドウの高さドット数</param>
/// <param name="worldRect">ステージ全体の矩形</param>
void scrollManagerInfo(int _screenWidth, int _screenHeight, const sHitRect& worldRect);

/// <summary>
/// スクロールの更新処理
/// </summary>
/// <param name="deltaTime">ウィンドウ更新時間</param>
void scrollUpdate(float deltaTime,float px);

/// <summary>
/// スクロールの描画オフセットXを取得
/// </summary>]
/// <returns>方向オフセット</returns>
int scrollGetOffset();

/// <summary>
/// ワールド座標系でのスクリーン矩形を返す
/// </summary>
/// <returns>ワールド座標系でのスクリーン矩形</returns>
sHitRect scrollGetWorldScreenRect();
