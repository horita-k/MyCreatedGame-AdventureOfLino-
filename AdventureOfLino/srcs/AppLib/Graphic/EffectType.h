/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �G�t�F�N�g�t�@�C���̒�`	
 * @author SPATZ.
 * @data   2012/05/03 01:59:45
 */
//---------------------------------------------------
#ifndef __EFFECT_TYPE_H__
#define __EFFECT_TYPE_H__


#include <d3dx9.h>

/*=====================================*
 * �\����
 *=====================================*/
struct MY_VERTEX
{
	// �ϊ��ςݒ��_
	FLOAT X, Y, Z, RHW;
	// �f�B�t���[�Y�F
	D3DCOLOR Diffuse;
	// �e�N�X�`�����W
	FLOAT U0, V0;
	FLOAT U1, V1;
};
// FVF
#define D3DFVF_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX2|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1))

/*=====================================*
 * typedef
 *=====================================*/
typedef	D3DXHANDLE		EFFHND_TECH;	// �e�N�j�b�N�p��`
typedef	D3DXHANDLE		EFFHND_PASS;	// �p�X�p��`
typedef	D3DXHANDLE		EFFHND_PARM;	// �p�����[�^�p��`
typedef	D3DXHANDLE		EFFHND_ANNO;	// �A�m�e�[�V�����p��`

/*=====================================*
 * �Q�ƃt�@�C��
 *
 * E:\project\DXLibTest\�݌v�p\�G�t�F�N�g�w�K.xls
 * �Z�}���e�B�b�N��` sheet 
 *
 * ��`��ǉ�����ꍇ�͏�̃V�[�g��p���Ēǉ����邱�ƁI�I
 *=====================================*/
/*=====================================*
 * �Z�}���e�B�b�N��`
 *=====================================*/
/**** �Z�}���e�B�b�N���ʒ�` ****/	
enum eSmtKind {
    eSMT_TIME,						    // 0 : 0�t���[���ڂ���̍Đ����Ԃ�\��
    eSMT_WORLD,                         // 1 : ���[���h�ϊ��s��
    eSMT_VIEW,                          // 2 : �r���[�ϊ��s��
    eSMT_VIEWINVERSE,                   // 3 : �r���[�ϊ��s��@�t�s��
    eSMT_VIEWPROJECTION,                // 4 : �r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��
    eSMT_VIEWPROJECTIONINVERSE,         // 5 : �r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��@�t�s��
    eSMT_PROJECTION,                    // 6 : �v���W�F�N�V�����ϊ��s��
    eSMT_PROJECTIONINVERSE,             // 7 : �v���W�F�N�V�����ϊ��s��@����k�s��
    eSMT_WORLDVIEWPROJECTION,           // 8 : ���[���h�ϊ��s��~�r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��
    eSMT_WORLDVIEWPROJECTIONINVERSE,    // 9 : ���[���h�ϊ��s��~�r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��@�t�s��
    eSMT_WORLDINVERSETRANSPOSE,         // 10 : ���[���h�ϊ��s��@�t�|�u�s��
    eSMT_WORLDVIEW,                     // 11 : ���[���h�ϊ��s��~�r���[�ϊ��s��
    eSMT_VIEWINVERSETRANSPOSE,          // 12 : �r���[�ϊ��s��@�t�|�u�s��
    eSMT_WORLDINVERSE,                  // 13 : ���[���h�ϊ��s��@�t�s��
    eSMT_WORLDVIEWTRANSPOSE,            // 14 : ���[���h�ϊ��s��~�r���[�ϊ��s��@�|�u�s��
    eSMT_WORLDVIEWINVERSE,              // 15 : ���[���h�ϊ��s��~�r���[�ϊ��s��@�t�s��
    eSMT_VIEWTRANSPOSE,                 // 16 : �r���[�ϊ��s��@�|�u�s��
    eSMT_POSITION,                      // 17 : ���W�x�N�g��
    eSMT_DIRECTION,                     // 18 : �����x�N�g��
    eSMT_MODELDATA,                     // 19 : ���f���f�[�^�w��
    eSMT_DIFFUSE,                       // 20 : �f�B�t���[�Y�F�i�g�U���j
    eSMT_AMBIENT,                       // 21 : �A���r�G���g�F�i�����j
    eSMT_EMISSIVE,                      // 22 : �G�~�b�V�����F�i���ˌ��j
    eSMT_SPECULAR,                      // 23 : �X�y�L�����F�i���ʌ��j
    eSMT_SPECULARPOWER,                 // 24 : �X�y�L�����̋��x
    eSMT_TOONCOLOR,                     // 25 : �g�D�[���F
    eSMT_EDGECOLOR,                     // 26 : �֊s�F
    eSMT_GROUNDSHADOWCOLOR,             // 27 : �n�ʉe�F
    eSMT_ELAPSEDTIME,                   // 28 : �O��̕`�悩��̌o�ߎ��Ԃ�\��
    eSMT_CONTROLOBJECT,                 // 29 : �w�肵���I�u�W�F�N�g�́A���W�⃏�[���h�ϊ��s����擾����B
    eSMT_RENDERCOLORTARGET,             // 30 : �����_�����O�^�[�Q�b�g�Ɏw��\�ȃT�[�t�F�C�X�𐶐�����B
    eSMT_RENDERDEPTHSTENCILTARGET,      // 31 : �[�x�X�e���V���T�[�t�F�C�X�i������Z�o�b�t�@�j�𐶐�����B
    eSMT_TEXTUREVALUE,                  // 32 : �w�肵���e�N�X�`���́A�e�N�Z�������擾���Ĕz��Ɋi�[����B
    eSMT_STANDARDSGLOBAL,               // 33 : SAS�iStandard Annotations and Semantics�j�̃o�[�W�������w�肷��B
    eSMT_OFFSCREENRENDERTARGET,         // 34 : �I�t�X�N���[�������_�[�^�[�Q�b�g�𐶐����܂��B
    eSMT_VIEWPORTPIXELSIZE,				// 35 : �r���[�|�[�g�s�N�Z���T�C�Y
    eSMT_DUMMY,                         // 36 : �_�~�[
    eSMT_MAX,                           // 37 : �v�f��
};


/**** �Z�}���e�B�b�N������ ****/	
static const char* strSmtName[eSMT_MAX] = {
    "TIME",								// 0 : 0�t���[���ڂ���̍Đ����Ԃ�\��
    "WORLD",                            // 1 : ���[���h�ϊ��s��
    "VIEW",                             // 2 : �r���[�ϊ��s��
    "VIEWINVERSE",                      // 3 : �r���[�ϊ��s��@�t�s��
    "VIEWPROJECTION",                   // 4 : �r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��
    "VIEWPROJECTIONINVERSE",            // 5 : �r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��@�t�s��
    "PROJECTION",                       // 6 : �v���W�F�N�V�����ϊ��s��
    "PROJECTIONINVERSE",                // 7 : �v���W�F�N�V�����ϊ��s��@����k�s��
    "WORLDVIEWPROJECTION",              // 8 : ���[���h�ϊ��s��~�r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��
    "WORLDVIEWPROJECTIONINVERSE",       // 9 : ���[���h�ϊ��s��~�r���[�ϊ��s��~�v���W�F�N�V�����ϊ��s��@�t�s��
    "WORLDINVERSETRANSPOSE",            // 10 : ���[���h�ϊ��s��@�t�|�u�s��
    "WORLDVIEW",                        // 11 : ���[���h�ϊ��s��~�r���[�ϊ��s��
    "VIEWINVERSETRANSPOSE",             // 12 : �r���[�ϊ��s��@�t�|�u�s��
    "WORLDINVERSE",                     // 13 : ���[���h�ϊ��s��@�t�s��
    "WORLDVIEWTRANSPOSE",               // 14 : ���[���h�ϊ��s��~�r���[�ϊ��s��@�|�u�s��
    "WORLDVIEWINVERSE",                 // 15 : ���[���h�ϊ��s��~�r���[�ϊ��s��@�t�s��
    "VIEWTRANSPOSE",                    // 16 : �r���[�ϊ��s��@�|�u�s��
    "POSITION",							// 17 : ���W�x�N�g��
    "DIRECTION",						// 18 : �����x�N�g��
    "MODELDATA",						// 19 : ���f���f�[�^�w��
    "DIFFUSE",							// 20 : �f�B�t���[�Y�F�i�g�U���j
    "AMBIENT",							// 21 : �A���r�G���g�F�i�����j
    "EMISSIVE",							// 22 : �G�~�b�V�����F�i���ˌ��j
    "SPECULAR",							// 23 : �X�y�L�����F�i���ʌ��j
    "SPECULARPOWER",					// 24 : �X�y�L�����̋��x
    "TOONCOLOR",						// 25 : �g�D�[���F
    "EDGECOLOR",						// 26 : �֊s�F
    "GROUNDSHADOWCOLOR",				// 27 : �n�ʉe�F
    "ELAPSEDTIME",						// 28 : �O��̕`�悩��̌o�ߎ��Ԃ�\��
    "CONTROLOBJECT",					// 29 : �w�肵���I�u�W�F�N�g�́A���W�⃏�[���h�ϊ��s����擾����B
    "RENDERCOLORTARGET",				// 30 : �����_�����O�^�[�Q�b�g�Ɏw��\�ȃT�[�t�F�C�X�𐶐�����B
    "RENDERDEPTHSTENCILTARGET",			// 31 : �[�x�X�e���V���T�[�t�F�C�X�i������Z�o�b�t�@�j�𐶐�����B
    "TEXTUREVALUE",						// 32 : �w�肵���e�N�X�`���́A�e�N�Z�������擾���Ĕz��Ɋi�[����B
    "STANDARDSGLOBAL",					// 33 : SAS�iStandard Annotations and Semantics�j�̃o�[�W�������w�肷��B
    "OFFSCREENRENDERTARGET",			// 34 : �I�t�X�N���[�������_�[�^�[�Q�b�g�𐶐����܂��B
    "VIEWPORTPIXELSIZE",				// 35 : �r���[�|�[�g�s�N�Z���T�C�Y
    "DUMMY",							// 36 : �_�~�[
};	


/*=====================================*
 * �X�N���v�g��`
 *=====================================*/
/**** �X�N���v�g���ʒ�` ****/	
enum eScriptKind {
	eSCRIPT_RenderColorTarget,			// 0 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	eSCRIPT_RenderColorTarget0,         // 1 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	eSCRIPT_RenderColorTarget1,         // 2 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	eSCRIPT_RenderColorTarget2,         // 3 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	eSCRIPT_RenderColorTarget3,         // 4 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	eSCRIPT_RenderDepthStencilTarget,   // 5 : �[�x�X�e���V���T�[�t�F�C�X��ݒ肵�܂��B
	eSCRIPT_ClearSetColor,              // 6 : �����_�����O�^�[�Q�b�g���N���A����F��ݒ肷��B
	eSCRIPT_ClearSetDepth,              // 7 : �[�x�X�e���V���T�[�t�F�C�X���N���A����Z�l��ݒ肷��B
	eSCRIPT_Clear,                      // 8 : (=Color):�����_�����O�^�[�Q�b�g���N���A���܂��B, (=Depth):�[�x�X�e���V���T�[�t�F�C�X���N���A���܂��B
	eSCRIPT_ScriptExternal,             // 9 : ���̃I�u�W�F�N�g��`�悵�܂��B
	eSCRIPT_Pass,                       // 10 : �w�肵���p�X�����s���܂�
	eSCRIPT_LoopByCount,                // 11 : �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v���܂��B
	eSCRIPT_LoopEnd,                    // 12 : �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v���܂��B
	eSCRIPT_LoopGetIndex,               // 13 : ���[�v���̃��[�v�J�E���^�̒l���A�w�肵���p�����[�^�ɐݒ肵�܂��B
	eSCRIPT_Draw,                       // 14 : (=Geometory):�I�u�W�F�N�g��`�悵�܂��B (=Buffer):�����_�����O�^�[�Q�b�g�̃X�N���[���ƈ�v����T�C�Y�́A�����`�̃|���S����`�悵�܂��B
	eSCRIPT_MAX,                        // 15 : �v�f��
};

/**** �X�N���v�g������ ****/	
static const char* strScriptName[eSCRIPT_MAX] = {
	"RenderColorTarget",				// 0 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	"RenderColorTarget0",           	// 1 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	"RenderColorTarget1",           	// 2 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	"RenderColorTarget2",           	// 3 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	"RenderColorTarget3",           	// 4 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
	"RenderDepthStencilTarget",     	// 5 : �[�x�X�e���V���T�[�t�F�C�X��ݒ肵�܂��B
	"ClearSetColor",                	// 6 : �����_�����O�^�[�Q�b�g���N���A����F��ݒ肷��B
	"ClearSetDepth",                	// 7 : �[�x�X�e���V���T�[�t�F�C�X���N���A����Z�l��ݒ肷��B
	"Clear",                        	// 8 : (=Color):�����_�����O�^�[�Q�b�g���N���A���܂��B, (=Depth):�[�x�X�e���V���T�[�t�F�C�X���N���A���܂��B
	"ScriptExternal",               	// 9 : ���̃I�u�W�F�N�g��`�悵�܂��B
	"Pass",                         	// 10 : �w�肵���p�X�����s���܂�
	"LoopByCount",                  	// 11 : �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v���܂��B
	"LoopEnd",                      	// 12 : �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v���܂��B
	"LoopGetIndex",                 	// 13 : ���[�v���̃��[�v�J�E���^�̒l���A�w�肵���p�����[�^�ɐݒ肵�܂��B
	"Draw",                         	// 14 : (=Geometory):�I�u�W�F�N�g��`�悵�܂��B (=Buffer):�����_�����O�^�[�Q�b�g�̃X�N���[���ƈ�v����T�C�Y�́A�����`�̃|���S����`�悵�܂��B
};

#endif // __EFFECT_TYPE_H__

 /**** end of file ****/

