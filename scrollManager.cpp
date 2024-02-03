#include"scrollManager.h"
#include"collision.h"
#pragma warning(disable:4996)
//hedder�ɏ����Ȃ����ƂŃO���[�o�������Ȃ�
float scrollX;		//�X�N���[��X�����l

int screenWidth;	//�X�N���[����
int screenHeight;	//�X�N���[������
float scrollXMax;	//�X�e�[�W�S�̂�X�����ő�l
float scrollXMin;
sHitRect worldScreenRect;//�X�N���[���̃��[���h��ԏ�ł̋�`�͈�

const float scrollSpeed = 0.7f;

void scrollManagerInfo(int _screenWidth, int _screenHeight, const sHitRect& worldRect) {
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;

	scrollXMax = worldRect.worldRX - screenWidth;
	scrollXMin = 0;
	initRect(worldScreenRect, static_cast<float>(_screenWidth), static_cast<float>(_screenHeight));
}
void scrollUpdate(float deltaTime,float px) {
	//�������v���C���[��X���W
	float rightoffset = screenWidth / 3; //��ʕ�1/3
	float leftoffset = screenWidth * 2 / 3;//��ʕ�2/3
	float _px = px - scrollX; //���Έʒu
	if (rightoffset > _px) {
		scrollX = px-rightoffset;
	}
	else if (leftoffset < _px) {
		scrollX = px-leftoffset;
	}
	
	
	// �L�����N�^�[�̈ړ��ɍۂ��āAscrollX��ǉ�
	
	//���[�ɍs�����Ƃ���MIN��
	if (scrollX < scrollXMin) {
		scrollX = scrollXMin;
	}
	//�E�[�ɍs�����Ƃ���MAX��
	if (scrollX > scrollXMax) {
		scrollX = scrollXMax;
	}
	//�X�N���[���̃��[���h���W�X�V

	updateWorldRect(worldScreenRect, scrollX, 0.0f);
}
int scrollGetOffset() {
	return static_cast<int>(scrollX);
}
sHitRect scrollGetWorldScreenRect() {
	return worldScreenRect;
}