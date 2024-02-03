#include "map.h"
#include "collision.h"
#pragma warning (disable:4996)

//////////////////////////////////////////////
// 定数定義
//////////////////////////////////////////////
const int mapChipSize = 64;  // マップチップ１個の大きさ
const int mapImgXNum = 16;    // マップチップ画像の横方向チップ数
const int mapImgYNum = 4;    // マップチップ画像の縦方向チップ数

sMapLayer layerHit;
sMapLayer layerNear;
sMapLayer layerBase;
sMapLayer layerBack;


int mapChipImg[64]; // 画像ハンドル配列

/// <summary>
/// マップローダー
/// </summary>
/// <param name="dst">初期化したいマップデータ</param>
/// <param name="mapCSVFileName">マップファイル名</param>
/// <returns>読み込みに成功したかをtrue falseで返却</returns>
bool mapLayerLoader(sMapLayer& dst,const char* mapCSVFileName);

/// <summary>
/// マップの破棄
/// </summary>
/// <param name="dst">メモリ開放したいマップデータ</param>
void mapUnloader(sMapLayer& dst);

/// <summary>
/// マップのレイヤーを描画
/// </summary>
/// <param name="layer">描画したいレイヤー</param>
void mapLayerDraw(sMapLayer& layer);

/// <summary>
/// 壁情報取得 あるブロックの壁の有無を調べ BlockInfoを返す
/// </summary>
/// <param name="x">調査したいブロックの横インデックス</param>
/// <param name="y">調査したいブロックの縦インデックス</param>
/// <returns>壁の有無情報 BLOCK_LEFT～ BLOCK_DOWNまでのビット値が重ねあわされて返却される</returns>
BlockInfo mapGetBlockInfo(int ix, int iy)
{
	BlockInfo ret = 0;
	
	// ブロックの1つ左が空白か？
	if (mapGetBlockType(ix - 1, iy) == 0)
	{
		ret |= BLOCK_LEFT;
	}
	// ブロックの一つ右が空白か？
	if (mapGetBlockType(ix + 1, iy) == 0)
	{
		ret |= BLOCK_RIGHT;
	}
	// ブロックの一つ上が空白か？
	if (mapGetBlockType(ix, iy - 1) == 0)
	{
		ret |= BLOCK_UP;
	}
	//ブロックの一つ下が空白か？
	if (mapGetBlockType(ix, iy + 1) == 0)
	{
		ret |= BLOCK_DOWN;
	}

	// retには4方向の壁の有無のビットが重ね合わされている
	return ret;
}

void mapInit()
{
	// 画像総数  横枚数x縦枚数,横方向個数,縦方向個数,チップ1枚の横サイズ, チップ1毎の縦サイズ, 画像配列先頭ポインタ 
	LoadDivGraph("data/island.png",mapImgXNum * mapImgYNum, mapImgXNum, mapImgYNum, mapChipSize, mapChipSize, mapChipImg);

	//マップ読み込み（レイヤーごとに）
	mapLayerLoader(layerHit, "data/island_hit.csv");
	mapLayerLoader(layerBack, "data/island_back.csv");
	mapLayerLoader(layerBase, "data/island_base.csv");
	mapLayerLoader(layerNear, "data/island_near.csv");

}

void mapDraw()
{
	mapLayerDraw(layerBack);
	mapLayerDraw(layerBase);
	mapLayerDraw(layerNear);
}

int scrollX=0;
void mapLayerDraw(sMapLayer& layer)
{
	
	scrollX=scrollGetOffset();
	
	for (int y = 0; y < layer.mapYNum; y++)
	{
		for (int x = 0; x < layer.mapXNum; x++)
		{
			int imgIndex = layer.mapData[y][x];              // map配列よりブロック種類を取得

			//描画しようとしているidが-1の時は描画をスキップ
			if (imgIndex == -1)
			{
				continue;
			}

			int imgHandle = mapChipImg[imgIndex];  // indexをつかって画像ハンドル配列から画像ハンドルを取得

			// マップチップ幅でブロック画像を敷き詰めて描画する
			// xは0,1,2・・・と変化する。 x * mapChipSize の計算は 0,64,128,196, ... とブロック幅ごとに増える
			DrawGraph(x * mapChipSize-scrollX, y * mapChipSize, imgHandle, TRUE);
		}
	}
}

void mapFinalize()
{
	for (int i = 0; i < mapImgXNum * mapImgYNum; i++)
	{
		DeleteGraph(mapChipImg[i]);
	}
	mapUnloader(layerHit);
	mapUnloader(layerBack);
	mapUnloader(layerBase);
	mapUnloader(layerNear);
}

bool mapHitCalc(sHitRect& checkRect)
{
	bool hitflg = false;
	sHitRect blockRect;

	blockRect.w = mapChipSize;
	blockRect.h = mapChipSize;

	// すべてのマップブロック vs 調査ブロックの衝突を調べる
	for (int iy = 0; iy < layerHit.mapYNum; iy++)
	{
		//マップブロックのY座標
		blockRect.worldLY = static_cast<float>(iy * mapChipSize);
		blockRect.worldRY = static_cast<float>((iy + 1) * mapChipSize);

		for (int ix = 0; ix < layerHit.mapXNum; ix++)
		{
			// 当たりブロックか 0→通れる 0以外→通れない 
			if (layerHit.mapData[iy][ix] != 0)
			{
				// マップブロックのX座標
				blockRect.worldLX = static_cast<float>(ix * mapChipSize);
				blockRect.worldRX = (ix + 1) * static_cast<float>(mapChipSize);

				// 当たっているか？
				if (isHitRect(checkRect, blockRect))
				{
					// 一度でもブロックと当たったらhitflgをtrueに
					hitflg = true;

					// ブロック情報（上下左右壁の有無を調べる）
					BlockInfo bi = mapGetBlockInfo(ix, iy);

					// ボックスのめり込み量を計算する
					// checkRectにはめり込み戻し量が入る
					calcFixHitRectPosition(checkRect, blockRect, bi);

				}
			}
		}
	}

	return hitflg;
}

unsigned char mapGetBlockType(int ix, int iy)
{
	if (ix < 0 || iy < 0 || ix >= layerHit.mapXNum || iy >= layerHit.mapYNum)
	{
		return 0;
	}
	return layerHit.mapData[iy][ix];
}

unsigned char mapGetBlockTypeXY(int x, int y)
{
	return mapGetBlockType(x / mapChipSize, y / mapChipSize);
}

bool mapLayerLoader(sMapLayer& dst, const char* mapCSVFileName)
{
	// dst初期化
	dst.mapData = NULL;
	dst.mapXNum = 0;
	dst.mapYNum = 0;

	// ファイルを開く
	FILE* fp;
	fp = fopen(mapCSVFileName, "rt");
	if (!fp)
	{
		printf("ファイルを開けません");
		return 0;
	}

	// ファイルを１行分空読みし、１行のデータ数を数える
	char c = 0;
	while (c != '\n')
	{
		// 読み取った文字がカンマなら1つカウント増やす
		c = fgetc(fp);
		if (',' == c)
		{
			dst.mapXNum++;
		}
	}
	dst.mapXNum++;

	//１データ 16bit最大値 "65535,"(6文字)として文字列入ってきたときの１行サイズ
	int bufferLen = dst.mapXNum * 6 + 1;

	//１行分のバッファ確保
	char* buf = new char[bufferLen];
	char* ptmp = buf;

	//ファイルポインタを先頭に戻し行数を読む
	fseek(fp, 0, SEEK_SET);
	bool firstRow = true;
	while (fgets(buf, bufferLen - 1, fp) != NULL)
	{
		dst.mapYNum++;
	}

	//2次元配列を動的確保する map[row][col] として動的確保する
	dst.mapData = new int* [dst.mapYNum];

	int ix, iy;
	for (iy = 0; iy < dst.mapYNum; iy++)
	{
		dst.mapData[iy] = new int[dst.mapXNum];
	}

	//ファイルポインタを先頭に戻す
	fseek(fp, 0, SEEK_SET);
	// ファイルからデータを配列に移す
	char* readPoint;
	char* firstPoint;
	for (iy = 0; iy < dst.mapYNum; iy++)
	{
		// 1行取り込み
		fgets(buf, bufferLen - 1, fp);
		readPoint = firstPoint = buf;

		for (ix = 0; ix < dst.mapXNum; ix++)
		{
			//カンマまでreadPointの位置を進める
			while (',' != *readPoint && '\n' != *readPoint)
			{
				readPoint++;
			}
			// カンマをNULL文字に置き換え
			*readPoint = '\0';
			//この時点でfirstPoint - readPoint間で文字列が完成するので数値に変換
			dst.mapData[iy][ix] = atoi(firstPoint);

			//次のデータ読み取り位置まで進める
			readPoint++;
			firstPoint = readPoint;
		}
	}

	// ファイルを閉じる
	fclose(fp);	

	// バッファ開放
	delete[] buf;

	return true;
}

void mapUnloader(sMapLayer& dst)
{
	// データの解放
	for (int iy = 0; iy < dst.mapYNum; iy++)
	{
		delete[] dst.mapData[iy];
	}
	delete[] dst.mapData;
	
	// dst初期化
	dst.mapData = NULL;
	dst.mapXNum = 0;
	dst.mapYNum = 0;
}
