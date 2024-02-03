#pragma once
#include <DxLib.h>
#include "collision.h"
#include"scrollManager.h"

/// <summary>
/// �}�b�v�f�[�^�\����
/// </summary>
struct sMapLayer
{
	int** mapData; // �}�b�v�`�b�v��2�����z��
	int   mapXNum; // �}�b�v�`�b�v���������ɕ���ł��鐔
	int   mapYNum; // �}�b�v�`�b�v���c�����ɕ���ł��鐔
};

/// <summary>
/// �}�b�v������
/// </summary>
void mapInit();

/// <summary>
/// �}�b�v�`��
/// </summary>
void mapDraw();

/// <summary>
/// �}�b�v��n��
/// </summary>
void mapFinalize();

/// <summary>
/// �}�b�v��n��
/// </summary>
/// <param name ="checkRect">�}�b�v�Ɠ������Ă��邩������������`</param>
/// <returns>�}�b�v�ƏՓ˂�������true/false�ŕԂ�</returns>
bool mapHitCalc(sHitRect& checkRect);

/// <summary>
/// �}�b�v�̃C���f�b�N�X�ʒu����u���b�N�ԍ���Ԃ�
/// </summary>
/// <param name ="ix">�}�b�v�̃C���f�b�N�X�ԍ�x</param>
/// <param name ="iy">�}�b�v�̃C���f�b�N�X�ԍ�y</param>
/// <returns>�}�b�v�̃u���b�N�ԍ���Ԃ�</returns>
unsigned char mapGetBlockType(int ix, int iy);

/// <summary>
/// ���W�ʒu����u���b�N�ԍ���Ԃ�
/// </summary>
/// <param name ="ix">x���W</param>
/// <param name ="iy">y���W</param>
/// <returns>�}�b�v�̃u���b�N�ԍ���Ԃ�</returns>
unsigned char mapGetBlockTypeXY(int x, int y);