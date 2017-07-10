/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �R�}���h�֘A�W��
 * @author SPATZ
 * @data   2013/08/03 10:40:29
 */
//---------------------------------------------------
#ifndef __COMMAND_FUNCTION_H__
#define __COMMAND_FUNCTION_H__

#include "AppLib/Resource/CsvAccesser.h"
#include "../DataDefine/EnemyDef.h"
#include "../DataDefine/messagedef.h"
#include "Object/ObjectBase.h"
#include "AppLib/Graphic/ModelMap.h"

/*=====================================*
 * define
 *=====================================*/
// CSV�R�}���h��`
enum {
	eCSV_CMD_SHOW_ITEM,
	eCSV_CMD_VC,
	eCSV_CMD_SND,
	eCSV_CMD_LYT,
	eCSV_CMD_F_IN,
	eCSV_CMD_F_OUT,
	eCSV_CMD_FADE,
	eCSV_CMD_FOG,
	eCSV_CMD_M_IN,
	eCSV_CMD_M_OUT,
	eCSV_CMD_T_IN,
	eCSV_CMD_T_OUT,
	eCSV_CMD_TOON,
	eCSV_CMD_SHAKE,
	eCSV_CMD_EFFECT,
	eCSV_CMD_EFK,
	eCSV_CMD_CAM,
	eCSV_CMD_SHOW,
	eCSV_CMD_POSITION,
	eCSV_CMD_SETPOS,
	eCSV_CMD_SETROT,
	eCSV_CMD_SETSCA,
	eCSV_CMD_POSROT,
	eCSV_CMD_MDLANM,
	eCSV_CMD_LYTANM,
	eCSV_CMD_ATTACH,
	eCSV_CMD_MOTION,
	eCSV_CMD_MUSIC,
	eCSV_CMD_D_MSG,
	eCSV_CMD_L_DIF,
	eCSV_CMD_L_SPE,
	eCSV_CMD_L_AMB,
	eCSV_CMD_FUNC,
	eCSV_CMD_PHASE,
	eCSV_CMD_BLM,
#ifdef _DEBUG
	eCSV_CMD_LOAD,
	eCSV_CMD_SAVE,
#endif // _DEBUG

	eCSV_CMD_MAX,
};

// �g���R�}���h��`
enum {
	eEXCMD_NONE = -1,
	eEXCMD_MSG = 0,
	eEXCMD_MOV,
	eEXCMD_TURN,
	eEXCMD_WAIT,
	eEXCMD_UNTIL,
	eEXCMD_WAITCAM,

	eEXCMD_MAX,
};

// ���R�t�H���g��`�l
enum  {
	eFREE_FONT_NONE = -1,
	eFREE_FONT_APPEAR_SKYSHIP_EVT_0 = 0,
	eFREE_FONT_APPEAR_SKYSHIP_EVT_1,
	eFREE_FONT_APPEAR_SKYSHIP_EVT_2,

	eFREE_FONT_MAX,
};

/*=====================================*
  
 *=====================================*/
typedef void (*EVT_CMD_FUNC)(int);

/*=====================================*
 * struct 
 *=====================================*/
#ifdef _DEBUG
struct tCsvActionDebug {
	USHORT	mCheckLineCnt;
	char	mActiveExCmd;
	SHORT	mActiveExTime;
	float	mFrameCounter;
	DWORD	mNextKey;
	VECTOR	mCamPos;
	VECTOR	mCamTar;
	float	mCamFov;
};
#endif // _DEBUG

/*=====================================*
 * event
 *=====================================*/	
bool			getPlayerPosRotFromBGModel(VECTOR* pOutPos, float* pOutRot, int stagePlaceIndex);
bool			changeSubWeapon(eItemKind itemKind);
void			updateGetItem(eItemKind itemKind);
void			createEnemyWork2(int enemyIndex, eEnemyKind kind, VECTOR* pPos);
void			destroyAllEnemy(void);
void			addMyWeapon(eItemKind itemKind);
void			deleteMyWeapon(eItemKind itemKind);

bool			updateCsvAction(CsvAccesser* pCsvAccesser);
void			startCsvAction2nd(void);
bool			updateCsvAction2nd(CsvAccesser* pCsvAccesser);
#ifdef _DEBUG
void			saveCsvActionDebug(void);
void			loadCsvActionDebug(void);
#endif // _DEBUG
void			DrawFontFunc(void);
void			DrawFreeFontFunc(void);
void			visibleNpcAll(bool isVisible);
void			visibleObjectAll(bool isVisible);
int				getStageIndex(void);
// �N���X�^���o�����o
// crystalColor : 0 Green
//				: 1 Yellow 
//				: ���͖�����
void			processAppearCrystal(int stagePlaceIndex, 
									 ObjectBase* pCrystalObject, 
									 BYTE crystalColor, 
									 BOOL isEfk=TRUE,
									 int* pEffHndAura=NULL,
									 int* pEffHndCrystal=NULL);

// eOBJECT_KIND_XXX ���w�肵�A�ŏ��Ɍ������� ObjectBase��Ԃ��A������Ȃ��ꍇ��NULL��Ԃ�
ObjectBase*		search1stFindObjectKind(int objectKind);
// pModelMap �ɗ\�߃��[�h����Ă��郂�f����j�����A���̗̈�ɐV���ȃ}�b�v���f�������[�h����
void			reloadModelMap(ModelMap* pModelMap, const char* strModelName, bool isCollision);

void			revertVehicle();
void			revertGuardian();
void			setDefaultBloomSetting(void);
void			createDropItem(int objectKind, VECTOR& rSetPos);

void			useBinContents(eItemKind itemKind);
void			addBinContents(eItemKind itemKind);
BOOL			isEmptyBinContents(void);

/*=====================================*
 * CSV�R�}���h�t�@���N�V�����e�[�u��
 *=====================================*/
typedef void (*CSV_CMD_FUNC)(CsvAccesser* pCsvAccesser, int iI);
//static CSV_CMD_FUNC mCsvCmdFunc[eCSV_CMD_MAX];

void funcCsvCmd_SHOW_ITEM(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_VC(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_SND(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_LYT(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_F_IN(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_F_OUT(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_FADE(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_FOG(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_M_IN(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_M_OUT(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_T_IN(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_T_OUT(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_TOON(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_SHAKE(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_EFFECT(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_EFK(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_CAM(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_SHOW(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_POSITION(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_SETPOS(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_SETROT(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_SETSCA(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_POSROT(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_MDLANM(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_LYTANM(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_ATTACH(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_MOTION(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_MUSIC(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_D_MSG(CsvAccesser* pCsvAccesser, int iI);

void funcCsvCmd_L_DIF(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_L_SPE(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_L_AMB(CsvAccesser* pCsvAccesser, int iI);

void funcCsvCmd_FUNC(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_PHASE(CsvAccesser* pCsvAccesser, int iI);

void funcCsvCmd_BLM(CsvAccesser* pCsvAccesser, int iI);
#ifdef _DEBUG
void funcCsvCmd_LOAD(CsvAccesser* pCsvAccesser, int iI);
void funcCsvCmd_SAVE(CsvAccesser* pCsvAccesser, int iI);
#endif // _DEBUG

/*=====================================*
 * �C�x���g�R�}���h�t�@���N�V����
 *=====================================*/
void evtCmdFunc_00(int args);
void evtCmdFunc_01(int args);
void evtCmdFunc_02(int args);
void evtCmdFunc_03(int args);
void evtCmdFunc_04(int args);
void evtCmdFunc_05(int args);
void evtCmdFunc_06(int args);	// �t�H���g�`��֐��̌Ăяo��
void evtCmdFunc_07(int args);	// �f�B���N�V���i�����C�g�ݒ�
void evtCmdFunc_08(int args);	// �a�f�l�t�F�[�h�A�E�g
void evtCmdFunc_09(int args);	// �_���W�����J�n���C�A�E�g�\��
void evtCmdFunc_10(int args);	// �A���j���[�g�o�ꎞ�́A�w�i�ؑ�

#ifdef _DEBUG
void updateGameTestTool(void);
#endif _DEBUG








#endif // __COMMAND_FUNCTION_H__

 /**** end of file ****/
