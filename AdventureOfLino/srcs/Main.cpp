/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ���C���\�[�X
 * @author SPATZ.
 * @data   2015/01/09 00:15:30
 */
//---------------------------------------------------
#ifdef _DEBUG
// ���������[�N���o�֘A
/*
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
*/
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif // _DEBUG
#include "dxlib.h"
#include "Scene/SceneManager.h"
#include "AppLib/Basic/Basic.h"
#include "AppLib/Effect/EffectManager.h"
#include "Scene/TotalInfo.h"
#include "../resource.h"
#include <math.h>

#define __USE_ARCHIVE_SYSTEM	/* dxlib �̃A�[�J�C�u�V�X�e�����g�p���邩�H */

//---------------------------------------------------
/**
 * ���C���֐� 
 */
//---------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
#ifdef _DEBUG
	//<! ���������[�N���o�c�[��
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|
				   _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	
	// �R���t�B�O�f�[�^�̓ǂݍ���
	if (TotalInfo::GetInstance()->LoadConfigData() == false) {
		DxLib_End();
		return 0;
	}

	int reso_type = TotalInfo::GetInstance()->GetConfigVideoSize();
	const tDisplaySetting* p_setting = NULL;
	if (reso_type > (sizeof(sDisplaySetting) / sizeof(tDisplaySetting))) {
		reso_type = 1; // �K����
	}
	p_setting = &sDisplaySetting[reso_type];

	/**** �^�C�g�����ƃA�C�R����ύX ****/
	SetMainWindowText(GAME_TITLE);
	//	SetWindowIconID(IDI_ICON1);
	SetWindowIconID( IDI_ICON2 ); 

#ifdef LIB_VER_3_12e
	/**** �T�E���h�̃{�����[���ύX�ɔ����A���o�[�W�������g�p����t���O ****/
	SetUseOldVolumeCalcFlag(TRUE);
#endif

	/**** ��ʃ��[�h�̕ύX ****/
	if ((p_setting->mWindowSizeX == -1) || (p_setting->mWindowSizeY == -1)) {
		ChangeWindowMode(FALSE);		// �t���X�N���[��
	}
	else {
		SetWindowSize(p_setting->mWindowSizeX, p_setting->mWindowSizeY);
		ChangeWindowMode(TRUE);			// �E�B���h�E���[�h
		SetWindowSizeChangeEnableFlag(TRUE);	// �E�C���h�E�̃T�C�Y�����R�ɕύX�o����悤�ɐݒ肷��
	}
	int res_x, res_y;
	res_x = p_setting->mResolusionX;
	res_y = p_setting->mResolusionY;
	float reso_rate = TotalInfo::GetInstance()->GetConfigResolutionRateFromGraphQuality();
	res_x = (int)((float)res_x * reso_rate);
	res_y = (int)((float)res_y * reso_rate);
	SetGraphMode(res_x, res_y, p_setting->mColorBit);
	SetDrawArea(0, 0, res_x, res_y);

	/**** ���������� ****/
	SRand(65536);
	
	SetUseBackCulling(TRUE);

#ifndef _DEBUG /* only release */
	SetOutApplicationLogValidFlag(FALSE);		// ���O���b�Z�[�W�𖳌��ɂ���	
#else // _DEBUG
	SetOutApplicationLogValidFlag(FALSE);		// ���O���b�Z�[�W�𖳌��ɂ���	
#endif // _DEBUG

	// �t���V�[���A���`�G�C���A�X��ݒ肷��
	if (TotalInfo::GetInstance()->GetConfigIsAntiAlias() == true) {
		SetFullSceneAntiAliasingMode( 4, 2 ) ;
	}

	// �o�b�N�O���E���h�ł����삳����
	if (TotalInfo::GetInstance()->GetConfigIsBackgroundRun()) {
		SetAlwaysRunFlag(true);
	}

	SetUseSoftwareMixingSoundFlag( TRUE ) ;

	
#ifdef LIB_VER_3_12e
	SetUseDirect3DVersion( DX_DIRECT3D_9EX ) ;
#endif // LIB_VER_3_12e

	/**** �c�w���C�u�����̏����� ****/
	if( DxLib_Init() < 0 )
	{
		// �G���[�����������璼���ɏI��
		return -1 ;
	}

	// �E�B���h�E�̃��j���[���X�g�̃Z�b�g�A�b�v
	TotalInfo::GetInstance()->SetupWinMenu();

	/**** SceneManager �̐��� ****/
	SceneManager *const p_sceneman = SceneManager::GetInstance();
	p_sceneman->Create();

#ifdef __USE_ARCHIVE_SYSTEM
	/**** �A�[�J�C�u�V�X�e�����g�p���� ****/
	char pasuwaado[64] = {0};
	
	/*
	 * �Í��������ׁ̈A����J
	 */
	
	SetUseDXArchiveFlag(TRUE);
	SetDXArchiveExtension("spatz");
	SetDXArchiveKeyString(pasuwaado);
	const char* filename = "data.spatz";

#endif // __USE_ARCHIVE_SYSTEM

	// �V�[���}�l�[�W���̃Z�b�g�A�b�v
	p_sceneman->Setup();

	/**** �L�[�{�[�h�̃R���t�B�O�ݒ� ****/
	TotalInfo::GetInstance()->SetupAssignKeyboard();

	/**** �W���C�p�b�h�̃R���t�B�O�ݒ� ****/
	TotalInfo::GetInstance()->SetupAssignJoyInput();

	/**** �A�i���O�̏㉺�E���E���]�r�b�g�̐ݒ� ****/
	TotalInfo::GetInstance()->SetupAnalogReverse();

	/**** �}�E�X���x�̐ݒ� ****/
	TotalInfo::GetInstance()->SetupMouseSensitive();

	/* ���C�����[�v */
	while (p_sceneman->CheckMainLoop()) {
		/* �V�[�����C�����[�v */
		p_sceneman->Update();
	}

	// SceneManager���烁�C���^�X�N��j��
	p_sceneman->RemoveDebugTask();
	// �c�w���C�u�����̌�n��
	DxLib_End() ;

#ifdef _DEBUG
//	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	// �\�t�g�̏I��
	return 0 ;
}

 /**** end of file ****/
