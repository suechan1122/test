#pragma once
#include "collision.h"

/// <summary>
/// �X�N���[���}�l�[�W���[
/// </summary>
/// <param name="screenWidth">�E�B���h�E�̕��h�b�g��</param>
/// <param name="screenHeight">�E�B���h�E�̍����h�b�g��</param>
/// <param name="worldRect">�X�e�[�W�S�̂̋�`</param>
void scrollManagerInfo(int _screenWidth, int _screenHeight, const sHitRect& worldRect);

/// <summary>
/// �X�N���[���̍X�V����
/// </summary>
/// <param name="deltaTime">�E�B���h�E�X�V����</param>
void scrollUpdate(float deltaTime,float px);

/// <summary>
/// �X�N���[���̕`��I�t�Z�b�gX���擾
/// </summary>]
/// <returns>�����I�t�Z�b�g</returns>
int scrollGetOffset();

/// <summary>
/// ���[���h���W�n�ł̃X�N���[����`��Ԃ�
/// </summary>
/// <returns>���[���h���W�n�ł̃X�N���[����`</returns>
sHitRect scrollGetWorldScreenRect();
