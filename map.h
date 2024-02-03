#pragma once
#include <DxLib.h>
#include "collision.h"
#include"scrollManager.h"

/// <summary>
/// マップデータ構造体
/// </summary>
struct sMapLayer
{
	int** mapData; // マップチップの2次元配列
	int   mapXNum; // マップチップが横方向に並んでいる数
	int   mapYNum; // マップチップが縦方向に並んでいる数
};

/// <summary>
/// マップ初期化
/// </summary>
void mapInit();

/// <summary>
/// マップ描画
/// </summary>
void mapDraw();

/// <summary>
/// マップ後始末
/// </summary>
void mapFinalize();

/// <summary>
/// マップ後始末
/// </summary>
/// <param name ="checkRect">マップと当たっているか調査したい矩形</param>
/// <returns>マップと衝突したかをtrue/falseで返す</returns>
bool mapHitCalc(sHitRect& checkRect);

/// <summary>
/// マップのインデックス位置からブロック番号を返す
/// </summary>
/// <param name ="ix">マップのインデックス番号x</param>
/// <param name ="iy">マップのインデックス番号y</param>
/// <returns>マップのブロック番号を返す</returns>
unsigned char mapGetBlockType(int ix, int iy);

/// <summary>
/// 座標位置からブロック番号を返す
/// </summary>
/// <param name ="ix">x座標</param>
/// <param name ="iy">y座標</param>
/// <returns>マップのブロック番号を返す</returns>
unsigned char mapGetBlockTypeXY(int x, int y);