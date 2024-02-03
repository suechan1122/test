#include "player.h"
#include "collision.h"
#include "map.h"
								      
const int animPatternNum = 3;         // アニメーションのパターン数
const int animDown       = 0;         // 下方向
const int animLeft       = 1;         // 左方向
const int animRight      = 2;         // 右方向
const int animUp         = 3;         // 上方向
const int animTypeNum    = 4;         // アニメーションの種類
const float animationFPS = 6.0f;      // 秒間 6コマのアニメーション
const float moveSpeed    = 200.0f;    // 移動速度 (ここで指定した値分のピクセル/秒進む) 
const float maxMoveSpeed = 300.0f;    // 最大横移動速度
const float moveAccell   = 1.2f;      // 移動加速度
								      
const int   imageSizeX   = 64;        // 画像横サイズ
const int   imageSizeY   = 96;        // 画像縦サイズ
const int   hitSizeX     = 40;        // 当たり判定サイズ幅
const int   hitSizeY     = 60;        // 当たり判定サイズ高さ

const float gravity      = 10.0f;     // 重力
const float maxFallSpeed = 300.0f;    // 最大落下速度
const float jumpPower    =-1000.0f;   // ジャンプ力 
const int   colliderOffset = 10;      // コライダー左右ずらし量
const float jumpButtonAcceptTime = 0.95f; // ジャンプ長押し受付時間

float px, py;                         // プレイヤーの座標
float vx, vy;                         // プレイヤーの速度
float animTimer;                      // アニメーションタイマー
int   animNowType;                    // 現在のアニメ種類
int   animNowPattern ;                // 現在のアニメパターン
int   animNowIndex;                   // アニメーション画像配列の添え字 0～11までの数値
int   drawOffsetX;                    // 当たり判定からの横方向ずらし量
int   drawOffsetY;                    // 当たり判定からの縦方向ずらし量

bool  jumpFlg;                        // ジャンプフラグ
bool  onGround;                       // 地面に立っているか
bool  hitHead;                        // 頭がぶつかったか？
float jumpTimer;                      // ジャンプ長押し時間タイマー

int playerImg[animPatternNum * animTypeNum]; // プレイヤーの画像ハンドル

sHitRect playerHit;                   // プレイヤー当たり判定
sHitRect playerFootCollider;          // 接地コライダー
sHitRect playerHeadCollider;          // 頭上コライダー

bool prevJumpButton;
bool isJumpPush;

bool inputMove(float deltaTime);

void playerInit()
{
	px = 100.0f;
	py = 100.0f;
	vx = 0.0f;
	vy = 0.0f;
	animTimer = 0.0f;
	jumpFlg = false;

	animNowType = animDown;     // 正面向きアニメーション種類
	animNowPattern = 0;         // 
	
	animNowIndex = 0;

	// 当たり判定初期化
	initRect(playerHit, hitSizeX, hitSizeY);
	initRect(playerFootCollider, hitSizeX - colliderOffset, 1);
	initRect(playerHeadCollider, hitSizeX - colliderOffset, 1);

	// 横3 縦4 計12枚の画像ファイルの読み込み
	LoadDivGraph("data/chara.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, playerImg);

	// 描画位置のオフセット値を計算
	// 左右から見てセンター、上下方向は底辺基準となるように計算
	drawOffsetX = (hitSizeX - imageSizeX) / 2;
	drawOffsetY = (hitSizeY - imageSizeY);

	onGround = false;
	hitHead = false;
	jumpTimer = jumpButtonAcceptTime;

	isJumpPush = false;
	prevJumpButton = false;
}

void playerUpdate(float deltaTime)
{
	bool isMove = inputMove(deltaTime);

	//接地してる？
	if (onGround)
	{
		jumpFlg = false;
		vy = 0.0f;
		jumpTimer = jumpButtonAcceptTime;
	}
	else
	{
		jumpFlg = true;
	}

	// 頭上に当たった？
	if (hitHead)
	{
		vy = -0.5f * vy;
	}

	// 当たり判定位置更新
	updateWorldRect(playerHit, px, py);
	updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
	updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);
	
	////////////////////////////
	// アニメーション計算
	////////////////////////////
	if (isMove)
	{
		// animTimer で時間経過を測り、 1.0 / animationFPS →次のアニメーションパターンに進んでよい秒数に
		// なったら次のコマに進める
		animTimer += deltaTime;
		if (animTimer > 1.0f / animationFPS)
		{
			// タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
			animTimer = 0.0f;
			animNowPattern++;
			animNowPattern %= animPatternNum; // 余りなので 0,1,2のいずれかになる
		}
	}
	else
	{
		animNowPattern = 1; // 静止時は足をそろえたパターンに強制する
	}

	// 最終的な添え字を計算 
	// 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
	animNowIndex = animNowPattern + animNowType * animPatternNum;

}

void playerDraw()
{
	// 描画位置は
	DrawGraph(static_cast<int>(px) + drawOffsetX, static_cast<int>(py) + drawOffsetY, playerImg[animNowIndex], TRUE);
	drawRect(playerHit);
	drawRect(playerFootCollider);
	drawRect(playerHeadCollider);
}


void playerFinalize()
{
	if (playerImg[0] != -1)
	{
		for (int i = 0; i < animPatternNum * animTypeNum; i++)
		{
			DeleteGraph(playerImg[i]);
		}
	}
}

sHitRect getPlayerHitRect()
{
	return playerHit;
}

void playerfixColPosition(sHitRect& hitRect)
{
	px = hitRect.worldLX;
	py = hitRect.worldLY;

	playerHit = hitRect;
}

void playerSetGroundFlg(bool groundFlg)
{
	onGround = groundFlg;
}

void playerSetHeadHitFlg(bool headHitFlg)
{
	hitHead = headHitFlg;
}

sHitRect playerGetGroundCollider()
{
	return playerFootCollider;
}

sHitRect playerGetHeadCollider()
{
	return playerHeadCollider;
}

bool inputMove(float deltaTime)
{
	///////////////////////////////////////////
	// 入力処理
	///////////////////////////////////////////

	// ジャンプボタン押した瞬間か？
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (!prevJumpButton)
		{
			isJumpPush = true;
		}
		else
		{
			isJumpPush = false;
		}
		prevJumpButton = true;
	}
	else
	{
		prevJumpButton = false;
	}

	bool isMove = false;
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		if (!jumpFlg)
		{
			// 方向転換
			if (vx > 0.0f)
			{
				vx *= 0.95f;
			}
			// 地上加速度
			vx += -moveAccell;
		}
		else
		{
			//空中加速度
			vx += -moveAccell * 0.3f;
		}
		//速度クリップ
		if (vx < -maxMoveSpeed)
		{
			vx = -maxMoveSpeed;
		}
		animNowType = animLeft;
		isMove = true;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (!jumpFlg)
		{
			// 方向転換
			if (vx < 0.0f)
			{
				vx *= 0.95f;
			}
			// 地上加速度
			vx += +moveAccell;
		}
		else
		{
			// 空中加速度
			vx += +moveAccell * 0.3f;
		}

		// 速度クリップ
		if (vx > maxMoveSpeed)
		{
			vx = +maxMoveSpeed;
		}
		animNowType = animRight;
		isMove = true;
	}

	//ジャンプ可能でジャンプキーが押された
	if (isJumpPush && !jumpFlg && jumpTimer > 0.0f)
	{
		vy -= 350.0f;
		jumpFlg = true;
		onGround = false;
	}

	if (prevJumpButton && jumpTimer > 0.0f)
	{
		vy -= 8.0f;
	}

	//ジャンプ中なら重力を発生
	if (jumpFlg)
	{
		jumpTimer -= deltaTime;
		vy += gravity;
	}

	// キー入力ないとき地上での減速
	if (!isMove && !jumpFlg)
	{
		vx *= 0.97f;
	}
	// 落下速度抑制
	if (vy * deltaTime > maxFallSpeed)
	{
		vy = maxFallSpeed;
	}
	// 位置更新
	px += vx * deltaTime;
	py += vy * deltaTime;

	return isMove;
}
