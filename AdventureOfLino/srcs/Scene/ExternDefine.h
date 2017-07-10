/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * Extern ���܂Ƃ߂��w�b�_
 * @author SPATZ
 * @data   2013/08/03 10:34:15
 */
//---------------------------------------------------
#ifndef __EXTERN_DEFINE_H__
#define __EXTERN_DEFINE_H__


#include "SceneInfo.h"
#include "SceneBase.h"

#include "AppLib/Resource/CsvAccesser.h"

#include "AppLib/Graphic/ModelPmd.h"
#include "AppLib/Graphic/ModelMap.h"
#include "AppLib/Graphic/LayoutBase.h"

#include "Charactor/CharaPlayer.h"
#include "Charactor/CharaNPC.h"
#include "Charactor/CharaEnemy.h"
#include "Charactor/CharaNavi.h"

#include "Object/ObjectBase.h"
#include "Object/ObjectTresure.h"
#include "Object/ObjectSwitch.h"
#include "Object/ObjectItem.h"

#include "StageFunc/StageFuncBase.h"

/*=====================================*
 * �n���� extern
 *=====================================*/
extern SceneBase*		spScenBase;		// �������g�̃|�C���^
extern CharaPlayer		sPlayer;
extern CharaEnemy**		sppEnemy;
extern CharaNpc*		spNpc;
extern ObjectBase**		sppObjectBase;
extern int				sObjectBaseNum;
extern CharaEnemy*		spDisappearEnemy;
extern ObjectItem		sDropItem[kENEMY_ITEM_NUM];
extern ModelMap			sModelMap;
extern ModelMap*		spSubModelMap;
extern ModelBase		sModelXfile[eMD_MAX];
extern ModelPmd			sGimmick;
extern CharaNavi		sNavi;
//extern LayoutBase		sLayout[eLY_MAX];
extern LayoutBase*		spShopItemLayout;
extern CsvAccesser		sEventCsv;
//extern CsvAccesser		sItemCsv;
extern int				sStageIndex;
extern int				sStagePlaceIndex;
extern int				sIsMessageRange;
extern tBGInfo*			spBGInfo;
extern ActionTaskManager::tTaskInfo sTaskInfo;
extern int				sEventBattleFlag;
extern CHAR				sNextEventNo;			// ���̃C�x���g�ԍ�
// ��������������Ȃ��C�x���g�t���O
extern int				sEventRecoveryFlag;
extern bool				sIsBattle;
extern char*			sStrNaviTalkPtr;
extern int				sLoadStatus;
extern int				sDungeonIndex;
extern BYTE				sDrawFreeFontValue;
extern int				sEventPhase;
extern int				sEventCount;
extern int				sBattleIndex;
extern void*			spStageWorkBuf;	// �X�e�[�W���̏������������o�b�t�@
extern VECTOR			sReviseDiffPos;

extern std::vector<eItemKind>	sMyWeaponList;	// �������X�g
extern std::vector<tStageInfo>	sSubStageInfoList;	// �X�e�[�W�����X�g

extern StageFuncBase*	spStageFunc;
extern int				sNpcNum;

extern float sDissappearLength;

#endif // __EXTERN_DEFINE_H__

/**** end of file ****/

