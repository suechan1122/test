#include"scrollManager.h"
#include"collision.h"
#pragma warning(disable:4996)
//hedderに書かないことでグローバル化しない
float scrollX;		//スクロールX方向値

int screenWidth;	//スクロール幅
int screenHeight;	//スクリーン高さ
float scrollXMax;	//ステージ全体のX方向最大値
float scrollXMin;
sHitRect worldScreenRect;//スクリーンのワールド空間上での矩形範囲

const float scrollSpeed = 0.7f;

void scrollManagerInfo(int _screenWidth, int _screenHeight, const sHitRect& worldRect) {
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;

	scrollXMax = worldRect.worldRX - screenWidth;
	scrollXMin = 0;
	initRect(worldScreenRect, static_cast<float>(_screenWidth), static_cast<float>(_screenHeight));
}
void scrollUpdate(float deltaTime,float px) {
	//引数をプレイヤーのX座標
	float rightoffset = screenWidth / 3; //画面幅1/3
	float leftoffset = screenWidth * 2 / 3;//画面幅2/3
	float _px = px - scrollX; //相対位置
	if (rightoffset > _px) {
		scrollX = px-rightoffset;
	}
	else if (leftoffset < _px) {
		scrollX = px-leftoffset;
	}
	
	
	// キャラクターの移動に際して、scrollXを追加
	
	//左端に行ったときにMINに
	if (scrollX < scrollXMin) {
		scrollX = scrollXMin;
	}
	//右端に行ったときにMAXに
	if (scrollX > scrollXMax) {
		scrollX = scrollXMax;
	}
	//スクリーンのワールド座標更新

	updateWorldRect(worldScreenRect, scrollX, 0.0f);
}
int scrollGetOffset() {
	return static_cast<int>(scrollX);
}
sHitRect scrollGetWorldScreenRect() {
	return worldScreenRect;
}