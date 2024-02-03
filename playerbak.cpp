#include "player.h"
#include "collision.h"
#include "map.h"
								      
const int animPatternNum = 3;         // �A�j���[�V�����̃p�^�[����
const int animDown       = 0;         // ������
const int animLeft       = 1;         // ������
const int animRight      = 2;         // �E����
const int animUp         = 3;         // �����
const int animTypeNum    = 4;         // �A�j���[�V�����̎��
const float animationFPS = 6.0f;      // �b�� 6�R�}�̃A�j���[�V����
const float moveSpeed    = 200.0f;    // �ړ����x (�����Ŏw�肵���l���̃s�N�Z��/�b�i��) 
const float maxMoveSpeed = 300.0f;    // �ő剡�ړ����x
const float moveAccell   = 1.2f;      // �ړ������x
								      
const int   imageSizeX   = 64;        // �摜���T�C�Y
const int   imageSizeY   = 96;        // �摜�c�T�C�Y
const int   hitSizeX     = 40;        // �����蔻��T�C�Y��
const int   hitSizeY     = 60;        // �����蔻��T�C�Y����

const float gravity      = 10.0f;     // �d��
const float maxFallSpeed = 300.0f;    // �ő嗎�����x
const float jumpPower    =-1000.0f;   // �W�����v�� 
const int   colliderOffset = 10;      // �R���C�_�[���E���炵��
const float jumpButtonAcceptTime = 0.95f; // �W�����v��������t����

float px, py;                         // �v���C���[�̍��W
float vx, vy;                         // �v���C���[�̑��x
float animTimer;                      // �A�j���[�V�����^�C�}�[
int   animNowType;                    // ���݂̃A�j�����
int   animNowPattern ;                // ���݂̃A�j���p�^�[��
int   animNowIndex;                   // �A�j���[�V�����摜�z��̓Y���� 0�`11�܂ł̐��l
int   drawOffsetX;                    // �����蔻�肩��̉��������炵��
int   drawOffsetY;                    // �����蔻�肩��̏c�������炵��

bool  jumpFlg;                        // �W�����v�t���O
bool  onGround;                       // �n�ʂɗ����Ă��邩
bool  hitHead;                        // �����Ԃ��������H
float jumpTimer;                      // �W�����v���������ԃ^�C�}�[

int playerImg[animPatternNum * animTypeNum]; // �v���C���[�̉摜�n���h��

sHitRect playerHit;                   // �v���C���[�����蔻��
sHitRect playerFootCollider;          // �ڒn�R���C�_�[
sHitRect playerHeadCollider;          // ����R���C�_�[

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

	animNowType = animDown;     // ���ʌ����A�j���[�V�������
	animNowPattern = 0;         // 
	
	animNowIndex = 0;

	// �����蔻�菉����
	initRect(playerHit, hitSizeX, hitSizeY);
	initRect(playerFootCollider, hitSizeX - colliderOffset, 1);
	initRect(playerHeadCollider, hitSizeX - colliderOffset, 1);

	// ��3 �c4 �v12���̉摜�t�@�C���̓ǂݍ���
	LoadDivGraph("data/chara.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, playerImg);

	// �`��ʒu�̃I�t�Z�b�g�l���v�Z
	// ���E���猩�ăZ���^�[�A�㉺�����͒�ӊ�ƂȂ�悤�Ɍv�Z
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

	//�ڒn���Ă�H
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

	// ����ɓ��������H
	if (hitHead)
	{
		vy = -0.5f * vy;
	}

	// �����蔻��ʒu�X�V
	updateWorldRect(playerHit, px, py);
	updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
	updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);
	
	////////////////////////////
	// �A�j���[�V�����v�Z
	////////////////////////////
	if (isMove)
	{
		// animTimer �Ŏ��Ԍo�߂𑪂�A 1.0 / animationFPS �����̃A�j���[�V�����p�^�[���ɐi��ł悢�b����
		// �Ȃ����玟�̃R�}�ɐi�߂�
		animTimer += deltaTime;
		if (animTimer > 1.0f / animationFPS)
		{
			// �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
			animTimer = 0.0f;
			animNowPattern++;
			animNowPattern %= animPatternNum; // �]��Ȃ̂� 0,1,2�̂����ꂩ�ɂȂ�
		}
	}
	else
	{
		animNowPattern = 1; // �Î~���͑������낦���p�^�[���ɋ�������
	}

	// �ŏI�I�ȓY�������v�Z 
	// �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
	animNowIndex = animNowPattern + animNowType * animPatternNum;

}

void playerDraw()
{
	// �`��ʒu��
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
	// ���͏���
	///////////////////////////////////////////

	// �W�����v�{�^���������u�Ԃ��H
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
			// �����]��
			if (vx > 0.0f)
			{
				vx *= 0.95f;
			}
			// �n������x
			vx += -moveAccell;
		}
		else
		{
			//�󒆉����x
			vx += -moveAccell * 0.3f;
		}
		//���x�N���b�v
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
			// �����]��
			if (vx < 0.0f)
			{
				vx *= 0.95f;
			}
			// �n������x
			vx += +moveAccell;
		}
		else
		{
			// �󒆉����x
			vx += +moveAccell * 0.3f;
		}

		// ���x�N���b�v
		if (vx > maxMoveSpeed)
		{
			vx = +maxMoveSpeed;
		}
		animNowType = animRight;
		isMove = true;
	}

	//�W�����v�\�ŃW�����v�L�[�������ꂽ
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

	//�W�����v���Ȃ�d�͂𔭐�
	if (jumpFlg)
	{
		jumpTimer -= deltaTime;
		vy += gravity;
	}

	// �L�[���͂Ȃ��Ƃ��n��ł̌���
	if (!isMove && !jumpFlg)
	{
		vx *= 0.97f;
	}
	// �������x�}��
	if (vy * deltaTime > maxFallSpeed)
	{
		vy = maxFallSpeed;
	}
	// �ʒu�X�V
	px += vx * deltaTime;
	py += vy * deltaTime;

	return isMove;
}
