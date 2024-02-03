#pragma once
#include "DxLib.h"

/// <summary>
/// �����蔻���`
/// </summary>
struct sHitRect
{
	float w;		// �������`���s�N�Z����
	float h;        // �������`�����s�N�Z����
	float worldLX;  // ����X���W
	float worldLY;  // ����Y���W
	float worldRX;  // �E��X���W
	float worldRY;  // �E��Y���W
};

// �u���b�N�Ǐ��
#define BLOCK_LEFT  0x01    // 0001
#define BLOCK_RIGHT 0x02    // 0010
#define BLOCK_UP    0x04    // 0100
#define BLOCK_DOWN  0x08    // 1000

/// <summary>
/// �u���b�N�Ǐ��^
/// </summary>
typedef unsigned char BlockInfo;

/// <summary>
/// �����蔻�菉����
/// </summary>
/// <param name="dstRect">�����������������蔻��ϐ�</param>
/// <param name="w">�����蔻��̕�</param>
/// <param name="h">�����蔻�荂��</param>
void initRect(sHitRect& dstRect, float w, float h);

/// <summary>
/// �����蔻��̈ړ��X�V�����@
/// </summary>
/// <param name="dstRect"> �X�V�����������蔻��ϐ�</param>
/// <param name="x"> �����蔻�荶��x���W</param>
/// <param name="y"> �����蔻�荶��y���W</param>
void updateWorldRect(sHitRect& dstRect,float x, float y);

/// <summary>
/// �����蔻���`�̕`��
/// </summary>
/// <param name="rect">�`�悵���������蔻���`</param>
/// <param name="scrollOffsetX">�X�N���[�������炵��</param>
void drawRect(const sHitRect& rect,int scrollOffsetX);

/// <summary>
/// ��̋�`���Փ˂�������Ԃ�
/// </summary>
/// <param name="rect1">��`1</param>
/// <param name="rect2">��`2</param>
/// <returns>��`�̓���Փ˂�������true/false�ŕԂ�</returns>
bool isHitRect(const sHitRect& rect1, const sHitRect& rect2);


/// <summary>
/// 2�̋�`�̏Փ˂߂荞�ݗʂ��v�Z���AmobableRect�ʒu��ύX����
/// </summary>
/// <param name="movableRect">�ړ��\��`</param>
/// <param name="staticRect">�Î~��`(�����Ȃ��u���b�N�Ȃ�)</param>
void calcFixHitRectPosition(sHitRect& movableRect, const sHitRect& staticRect);

/// <summary>
/// 2�̋�`�̏Փ˂߂荞�ݗʂ��v�Z���AmobableRect�ʒu��ύX����
/// </summary>
/// <param name="movableRect">�ړ��\��`</param>
/// <param name="staticRect">�Î~��`(�����Ȃ��u���b�N�Ȃ�)</param>
/// <param name="blockInfo">�u���b�N���</param>
void calcFixHitRectPosition(sHitRect& movableRect, const sHitRect& staticRect, BlockInfo blockInfo);

