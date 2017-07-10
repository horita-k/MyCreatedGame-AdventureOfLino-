/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ���͏��̒�`�w�b�_�[
 * @author SPATZ.
 * @data   2011/11/21 19:47:51
 */
//---------------------------------------------------
#ifndef __INPUT_INFO_H__
#define __INPUT_INFO_H__

#include "DxLib.h"

//�L�[��`
enum {
	eKEY_BACK,				//�o�b�N�X�y�[�X�L�[
	// eKEY_TAB,			//�^�u�L�[
	eKEY_RETURN,			//�G���^�[�L�[

	eKEY_LSHIFT,			//���V�t�g�L�[
	// eKEY_RSHIFT,			//�E�V�t�g�L�[
	eKEY_LCONTROL,		//���R���g���[���L�[
	eKEY_RCONTROL,		//�E�R���g���[���L�[
	eKEY_ESCAPE,			//�G�X�P�[�v�L�[
	eKEY_SPACE,				//�X�y�[�X�L�[
	eKEY_PGUP,			//�o�������t�o�L�[
	eKEY_PGDN,			//�o�������c�������L�[
	eKEY_END,			//�G���h�L�[
	eKEY_HOME,			//�z�[���L�[
	eKEY_LEFT,				//���L�[
	eKEY_UP,				//��L�[
	eKEY_RIGHT,				//�E�L�[
	eKEY_DOWN,				//���L�[
	eKEY_INSERT,			//�C���T�[�g�L�[
	eKEY_DELETE,			//�f���[�g�L�[

	eKEY_MINUS,			//�|�L�[
	// eKEY_YEN,			//���L�[
	// eKEY_PREVTRACK,		//�O�L�[
	// eKEY_PERIOD,			//�D�L�[
	// eKEY_SLASH,			//�^�L�[
	eKEY_LALT,			//���`�k�s�L�[
	// eKEY_RALT,			//�E�`�k�s�L�[
	// eKEY_SCROLL,			//ScrollLock�L�[
	// eKEY_SEMICOLON,		//�G�L�[
	// eKEY_COLON,			//�F�L�[
	// eKEY_LBRACKET,		//�m�L�[
	// eKEY_RBRACKET,		//�n�L�[
	// eKEY_AT,				//���L�[
	// eKEY_BACKSLASH,		//�_�L�[
	// eKEY_COMMA,			//�C�L�[
	// eKEY_KANJI,			//�����L�[
	// eKEY_CONVERT,		//�ϊ��L�[
	// eKEY_NOCONVERT,		//���ϊ��L�[
	// eKEY_KANA,			//�J�i�L�[
	// eKEY_APPS,			//�A�v���P�[�V�������j���[�L�[
	// eKEY_CAPSLOCK,		//CaspLock�L�[
	// eKEY_SYSRQ,			//PrintScreen�L�[
	// eKEY_PAUSE,			//PauseBreak�L�[
	// eKEY_LWIN,			//���v�����L�[
	// eKEY_RWIN,			//�E�v�����L�[

	// eKEY_NUMLOCK,		//�e���L�[�O
#ifdef _DEBUG
	eKEY_NUMPAD0,		//�e���L�[�O
	eKEY_NUMPAD1,		//�e���L�[�P
	eKEY_NUMPAD2,		//�e���L�[�Q
	eKEY_NUMPAD3,		//�e���L�[�R
	eKEY_NUMPAD4,		//�e���L�[�S
	eKEY_NUMPAD5,		//�e���L�[�T
	eKEY_NUMPAD6,		//�e���L�[�U
	eKEY_NUMPAD7,		//�e���L�[�V
	eKEY_NUMPAD8,		//�e���L�[�W
	eKEY_NUMPAD9,		//�e���L�[�X
#endif // _DEBUG
	// eKEY_MULTIPLY,		//�e���L�[���L�[
	// eKEY_ADD,			//�e���L�[�{�L�[
	// eKEY_SUBTRACT,		//�e���L�[�|�L�[
	// eKEY_DECIMAL,		//�e���L�[�D�L�[
	// eKEY_DIVIDE,			//�e���L�[�^�L�[
	// eKEY_NUMPADENTER,	//�e���L�[�̃G���^�[�L�[

	eKEY_F1,				//�e�P�L�[
	// eKEY_F2,				//�e�Q�L�[
	//	eKEY_F3,				//�e�R�L�[
	eKEY_F4,				//�e�S�L�[
	eKEY_F5,				//�e�T�L�[
	eKEY_F6,				//�e�U�L�[
	eKEY_F7,				//�e�V�L�[
	eKEY_F8,				//�e�W�L�[
	// eKEY_F9,				//�e�X�L�[
	//	eKEY_F10,			//�e�P�O�L�[
	//	eKEY_F11,			//�e�P�P�L�[
	//	eKEY_F12,			//�e�P�Q�L�[

	eKEY_A,					//�`�L�[
	eKEY_B,					//�a�L�[
	eKEY_C,					//�b�L�[
	eKEY_D,					//�c�L�[
	eKEY_E,					//�d�L�[
	eKEY_F,					//�e�L�[
	eKEY_G,					//�f�L�[
	eKEY_H,					//�g�L�[
	eKEY_I,					//�h�L�[
	eKEY_J,					//�i�L�[
	eKEY_K,					//�j�L�[
	eKEY_L,					//�k�L�[
	eKEY_M,					//�l�L�[
	eKEY_N,					//�m�L�[
	eKEY_O,					//�n�L�[
	eKEY_P,					//�o�L�[
	eKEY_Q,					//�p�L�[
	eKEY_R,					//�q�L�[
	eKEY_S,					//�r�L�[
	eKEY_T,					//�s�L�[
	eKEY_U,					//�t�L�[
	eKEY_V,					//�u�L�[
	eKEY_W,					//�v�L�[
	eKEY_X,					//�w�L�[
	eKEY_Y,					//�x�L�[
	eKEY_Z,					//�y�L�[

	eKEY_0,					//�O�L�[
	eKEY_1,					//�P�L�[
	eKEY_2,					//�Q�L�[
	eKEY_3,					//�R�L�[
	eKEY_4,					//�S�L�[
	eKEY_5,					//�T�L�[
	eKEY_6,					//�U�L�[
	eKEY_7,					//�V�L�[
	eKEY_8,					//�W�L�[
	eKEY_9,					//�X�L�[

	eKEY_MAX,

	/*
	// �}�E�X�p��`
	eMOUSE_LCLICK,
	eMOUSE_RCLICK,
	eMOUSE_MIDDLE,
	*/
};

static const unsigned char sKeyConnectDefineTable[eKEY_MAX] = {
	KEY_INPUT_BACK,				//	eKEY_BACK,			//�o�b�N�X�y�[�X�L�[
	// KEY_INPUT_TAB,			//	eKEY_TAB,			//�^�u�L�[
	KEY_INPUT_RETURN,			//	eKEY_RETURN,		//�G���^�[�L�[	

	KEY_INPUT_LSHIFT,		//	eKEY_LSHIFT,		//���V�t�g�L�[	
	// KEY_INPUT_RSHIFT,		//	eKEY_RSHIFT,		//�E�V�t�g�L�[	
	KEY_INPUT_LCONTROL,		//	eKEY_LCONTROL,		//���R���g���[���L�[	
	KEY_INPUT_RCONTROL,		//	eKEY_RCONTROL,		//�E�R���g���[���L�[	
	KEY_INPUT_ESCAPE,			//	eKEY_ESCAPE,		//�G�X�P�[�v�L�[	
	KEY_INPUT_SPACE,			//	eKEY_SPACE,			//�X�y�[�X�L�[
	KEY_INPUT_PGUP,				//	eKEY_PGUP,			//�o�������t�o�L�[
	KEY_INPUT_PGDN,				//	eKEY_PGDN,			//�o�������c�������L�[
	KEY_INPUT_END,				//	eKEY_END,			//�G���h�L�[
	KEY_INPUT_HOME,				//	eKEY_HOME,			//�z�[���L�[
	KEY_INPUT_LEFT,				//	eKEY_LEFT,			//���L�[
	KEY_INPUT_UP,				//	eKEY_UP,			//��L�[
	KEY_INPUT_RIGHT,			//	eKEY_RIGHT,			//�E�L�[
	KEY_INPUT_DOWN,				//	eKEY_DOWN,			//���L�[
	KEY_INPUT_INSERT,			//	eKEY_INSERT,		//�C���T�[�g�L�[	
	KEY_INPUT_DELETE,			//	eKEY_DELETE,		//�f���[�g�L�[	

	KEY_INPUT_MINUS,			//	eKEY_MINUS,			//�|�L�[
	// KEY_INPUT_YEN,			//	eKEY_YEN,			//���L�[
	// KEY_INPUT_PREVTRACK,		//	eKEY_PREVTRACK,		//�O�L�[	
	// KEY_INPUT_PERIOD,		//	eKEY_PERIOD,		//�D�L�[	
	// KEY_INPUT_SLASH,			//	eKEY_SLASH,			//�^�L�[
	KEY_INPUT_LALT,			//	eKEY_LALT,			//���`�k�s�L�[
	// KEY_INPUT_RALT,			//	eKEY_RALT,			//�E�`�k�s�L�[
	// KEY_INPUT_SCROLL,		//	eKEY_SCROLL,		//ScrollLock�L�[	
	// KEY_INPUT_SEMICOLON,		//	eKEY_SEMICOLON,		//�G�L�[	
	// KEY_INPUT_COLON,			//	eKEY_COLON,			//�F�L�[
	// KEY_INPUT_LBRACKET,		//	eKEY_LBRACKET,		//�m�L�[	
	// KEY_INPUT_RBRACKET,		//	eKEY_RBRACKET,		//�n�L�[	
	// KEY_INPUT_AT,			//	eKEY_AT,			//���L�[
	// KEY_INPUT_BACKSLASH,		//	eKEY_BACKSLASH,		//�_�L�[	
	// KEY_INPUT_COMMA,			//	eKEY_COMMA,			//�C�L�[
	// KEY_INPUT_KANJI,			//	eKEY_KANJI,			//�����L�[
	// KEY_INPUT_CONVERT,		//	eKEY_CONVERT,		//�ϊ��L�[	
	// KEY_INPUT_NOCONVERT,		//	eKEY_NOCONVERT,		//���ϊ��L�[	
	// KEY_INPUT_KANA,			//	eKEY_KANA,			//�J�i�L�[
	// KEY_INPUT_APPS,			//	eKEY_APPS,			//�A�v���P�[�V�������j���[�L�[
	// KEY_INPUT_CAPSLOCK,		//	eKEY_CAPSLOCK,		//CaspLock�L�[	
	// KEY_INPUT_SYSRQ,			//	eKEY_SYSRQ,			//PrintScreen�L�[
	// KEY_INPUT_PAUSE,			//	eKEY_PAUSE,			//PauseBreak�L�[
	// KEY_INPUT_LWIN,			//	eKEY_LWIN,			//���v�����L�[
	// KEY_INPUT_RWIN,			//	eKEY_RWIN,			//�E�v�����L�[

	// KEY_INPUT_NUMLOCK,		//	eKEY_NUMLOCK,		//�e���L�[�O	
#ifdef _DEBUG
	KEY_INPUT_NUMPAD0,		//	eKEY_NUMPAD0,		//�e���L�[�O	
	KEY_INPUT_NUMPAD1,		//	eKEY_NUMPAD1,		//�e���L�[�P	
	KEY_INPUT_NUMPAD2,		//	eKEY_NUMPAD2,		//�e���L�[�Q	
	KEY_INPUT_NUMPAD3,		//	eKEY_NUMPAD3,		//�e���L�[�R	
	KEY_INPUT_NUMPAD4,		//	eKEY_NUMPAD4,		//�e���L�[�S	
	KEY_INPUT_NUMPAD5,		//	eKEY_NUMPAD5,		//�e���L�[�T	
	KEY_INPUT_NUMPAD6,		//	eKEY_NUMPAD6,		//�e���L�[�U	
	KEY_INPUT_NUMPAD7,		//	eKEY_NUMPAD7,		//�e���L�[�V	
	KEY_INPUT_NUMPAD8,		//	eKEY_NUMPAD8,		//�e���L�[�W	
	KEY_INPUT_NUMPAD9,		//	eKEY_NUMPAD9,		//�e���L�[�X	
#endif // _DEBUG
	// KEY_INPUT_MULTIPLY,		//	eKEY_MULTIPLY,		//�e���L�[���L�[	
	// KEY_INPUT_ADD,			//	eKEY_ADD,			//�e���L�[�{�L�[
	// KEY_INPUT_SUBTRACT,		//	eKEY_SUBTRACT,		//�e���L�[�|�L�[	
	// KEY_INPUT_DECIMAL,		//	eKEY_DECIMAL,		//�e���L�[�D�L�[	
	// KEY_INPUT_DIVIDE,		//	eKEY_DIVIDE,		//�e���L�[�^�L�[	
	// KEY_INPUT_NUMPADENTER,	//	eKEY_NUMPADENTER,	//�e���L�[�̃G���^�[�L�[		

	KEY_INPUT_F1,			//	eKEY_F1,			//�e�P�L�[
	// KEY_INPUT_F2,			//	eKEY_F2,			//�e�Q�L�[
	//	KEY_INPUT_F3,			//	eKEY_F3,			//�e�R�L�[
	KEY_INPUT_F4,			//	eKEY_F4,			//�e�S�L�[
	KEY_INPUT_F5,			//	eKEY_F5,			//�e�T�L�[
	KEY_INPUT_F6,			//	eKEY_F6,			//�e�U�L�[
	KEY_INPUT_F7,			//	eKEY_F7,			//�e�V�L�[
	KEY_INPUT_F8,			//	eKEY_F8,			//�e�W�L�[
	// KEY_INPUT_F9,			//	eKEY_F9,			//�e�X�L�[
	//	KEY_INPUT_F10,			//	eKEY_F10,			//�e�P�O�L�[
	//	KEY_INPUT_F11,			//	eKEY_F11,			//�e�P�P�L�[	
	// KEY_INPUT_F12,			//	eKEY_F12,			//�e�P�Q�L�[	

	KEY_INPUT_A,				//	eKEY_A,				//�`�L�[
	KEY_INPUT_B,				//	eKEY_B,				//�a�L�[
	KEY_INPUT_C,				//	eKEY_C,				//�b�L�[
	KEY_INPUT_D,				//	eKEY_D,				//�c�L�[
	KEY_INPUT_E,				//	eKEY_E,				//�d�L�[
	KEY_INPUT_F,				//	eKEY_F,				//�e�L�[
	KEY_INPUT_G,				//	eKEY_G,				//�f�L�[
	KEY_INPUT_H,				//	eKEY_H,				//�g�L�[
	KEY_INPUT_I,				//	eKEY_I,				//�h�L�[
	KEY_INPUT_J,				//	eKEY_J,				//�i�L�[
	KEY_INPUT_K,				//	eKEY_K,				//�j�L�[
	KEY_INPUT_L,				//	eKEY_L,				//�k�L�[
	KEY_INPUT_M,				//	eKEY_M,				//�l�L�[
	KEY_INPUT_N,				//	eKEY_N,				//�m�L�[
	KEY_INPUT_O,				//	eKEY_O,				//�n�L�[
	KEY_INPUT_P,				//	eKEY_P,				//�o�L�[
	KEY_INPUT_Q,				//	eKEY_Q,				//�p�L�[
	KEY_INPUT_R,				//	eKEY_R,				//�q�L�[
	KEY_INPUT_S,				//	eKEY_S,				//�r�L�[
	KEY_INPUT_T,				//	eKEY_T,				//�s�L�[
	KEY_INPUT_U,				//	eKEY_U,				//�t�L�[
	KEY_INPUT_V,				//	eKEY_V,				//�u�L�[
	KEY_INPUT_W,				//	eKEY_W,				//�v�L�[
	KEY_INPUT_X,				//	eKEY_X,				//�w�L�[
	KEY_INPUT_Y,				//	eKEY_Y,				//�x�L�[
	KEY_INPUT_Z,				//	eKEY_Z,				//�y�L�[

	KEY_INPUT_0,				//	eKEY_0,				//�O�L�[
	KEY_INPUT_1,				//	eKEY_1,				//�P�L�[
	KEY_INPUT_2,				//	eKEY_2,				//�Q�L�[
	KEY_INPUT_3,				//	eKEY_3,				//�R�L�[
	KEY_INPUT_4,				//	eKEY_4,				//�S�L�[
	KEY_INPUT_5,				//	eKEY_5,				//�T�L�[
	KEY_INPUT_6,				//	eKEY_6,				//�U�L�[
	KEY_INPUT_7,				//	eKEY_7,				//�V�L�[
	KEY_INPUT_8,				//	eKEY_8,				//�W�L�[
	KEY_INPUT_9,				//	eKEY_9,				//�X�L�[
};

// �p�b�h���͒�`
#define kJOY_DOWN								(0x00000001)	// ���`�F�b�N�}�X�N
#define kJOY_LEFT								(0x00000002)	// ���`�F�b�N�}�X�N
#define kJOY_RIGHT								(0x00000004)	// ���`�F�b�N�}�X�N
#define kJOY_UP									(0x00000008)	// ���`�F�b�N�}�X�N
#define kJOY_A									(0x00000010)	// �`�{�^���`�F�b�N�}�X�N
#define kJOY_B									(0x00000020)	// �a�{�^���`�F�b�N�}�X�N
#define kJOY_C									(0x00000040)	// �b�{�^���`�F�b�N�}�X�N
#define kJOY_X									(0x00000080)	// �w�{�^���`�F�b�N�}�X�N
#define kJOY_Y									(0x00000100)	// �x�{�^���`�F�b�N�}�X�N
#define kJOY_Z									(0x00000200)	// �y�{�^���`�F�b�N�}�X�N
#define kJOY_L									(0x00000400)	// �k�{�^���`�F�b�N�}�X�N
#define kJOY_R									(0x00000800)	// �q�{�^���`�F�b�N�}�X�N
#define kJOY_START								(0x00001000)	// �r�s�`�q�s�{�^���`�F�b�N�}�X�N
#define kJOY_M									(0x00002000)	// �l�{�^���`�F�b�N�}�X�N

#define kJOY_NUM								(14)

#define kJOY_D									(0x00004000)
#define kJOY_F									(0x00008000)
#define kJOY_G									(0x00010000)
#define kJOY_H									(0x00020000)
#define kJOY_I									(0x00040000)
#define kJOY_J									(0x00080000)
#define kJOY_K									(0x00100000)
#define kJOY_LL									(0x00200000)
#define kJOY_N									(0x00400000)
#define kJOY_O									(0x00800000)
#define kJOY_P									(0x01000000)
#define kJOY_RR									(0x02000000)
#define kJOY_S									(0x04000000)
#define kJOY_T									(0x08000000)
#define kJOY_U									(0x10000000)
#define kJOY_V									(0x20000000)
#define kJOY_W									(0x40000000)
#define kJOY_XX									(0x80000000)

#endif // __INPUT_INFO_H__

/**** end of file ****/
