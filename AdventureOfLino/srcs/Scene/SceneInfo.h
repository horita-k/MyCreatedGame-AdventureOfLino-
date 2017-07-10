/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �V�[���ɂ��Ă̏����L��
 * @author SPATZ.
 * @data   2011/11/21 00:50:29
 */
//---------------------------------------------------
#ifndef __SCENE_INFO_H__
#define __SCENE_INFO_H__

#include "Scene/ActionTaskManager.h"
#include "DataDefine/MessageDef.h"
#include "DataDefine/EnemyDef.h"

/*=====================================*
 * define
 *=====================================*/
#define _MENU_MERGE_TEST

// �I�𒆂̃��j���[
enum {
	eMENU_MERGE,
	eMENU_EFFECT_TEST,
	eMENU_TITLE,
	eMENU_SELECT_FILE,
	eMENU_EVENT,

	eMENU_MAX,
};

static const char* sDebugMenuName[eMENU_MAX] = {
	"Merge Test",
	"EffectTest",
	"Merge TItle",
	"Select File",
	"Event",
};

enum {
	eACTION_OK = 0,
	eACTION_NG,
	/*
	eACTION_LIMIT_ATTACK_NORMAL,
	eACTION_LIMIT_ATTACK_SUB,
	*/

	eACTION_LIMIT_MAX,
};

#if 0
struct tNpcWork {
	BYTE				mNpcNum;
	tNpcInfo*			mpNpcInfo;
};
#endif // 0

// ���C�A�E�g�ݒ���
struct tLayoutSetting {
	const char* mStrLayoutName;
	USHORT		mPosX;
	USHORT		mPosY;
	USHORT		mSizeX;
	USHORT		mSizeY;
	BYTE		mAlpha;
};

/*=====================================*
 * BG�̏��
 *=====================================*/
struct tBGInfo {
	BOOL				mIsExtData;
	BYTE				mFileIndex;
	float				mScale;
	const char*			mMusicName;
	eEnemyTblIdx		mEnemyTblIdx;
	int					mActionLimit;
};

struct tStageInfo {
	//	const char* mStageName;
	BYTE		mFileIndex;
	bool		mIsCollision;
};

/*=====================================*
 * macro function
 *=====================================*/
/* ���b�Z�[�W�֘A */
#define MSG_TYPE_TELOP()		TotalInfo::GetInstance()->MsgTypeTelop()
#define MSG_TYPE_MESSAGE()		TotalInfo::GetInstance()->MsgTypeMessage()

//		sLayout[eLY_BALLOON].StartFade(176, sec);
//		sLayout[eLY_BALLOON].StartFade(210, sec);
#define MSG_FADE_IN(sec)		TotalInfo::GetInstance()->MsgFadeIn(sec)
#define MSG_FADE_OUT(sec)		TotalInfo::GetInstance()->MsgFadeOut(sec)
#define MSG_IS_FADE()			TotalInfo::GetInstance()->MsgIsFade()

#define MSG_FADE_IN_PATTERN()	TotalInfo::GetInstance()->MsgFadeIn();	\
	SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_IN);
#define MSG_FADE_OUT_PATTERN()	TotalInfo::GetInstance()->MsgFadeOut();	\
	SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_OUT);

#define RENDER_MESSAGE_COL(name, msg1, msg2, msg3, msg4, col)			\
	{																	\
		char* tmpmassgearrayABC[4];										\
		memset(tmpmassgearrayABC, 0x00, sizeof(tmpmassgearrayABC));		\
		tmpmassgearrayABC[0] = msg1;									\
		tmpmassgearrayABC[1] = msg2;									\
		tmpmassgearrayABC[2] = msg3;									\
		tmpmassgearrayABC[3] = msg4;									\
		GraphicManager::GetInstance()->RenderMessage(name,				\
													 tmpmassgearrayABC,	\
													 col);				\
	}
#define RENDER_MESSAGE(name, msg1, msg2, msg3, msg4)	RENDER_MESSAGE_COL(name, msg1, msg2, msg3, msg4, GetColor(0, 0, 0))

/* �A�C�R���֘A */
#define	VISIBLE_BATTLE_ICON()	TotalInfo::GetInstance()->VisibleBattleIcon()
#define HIDE_BATTLE_ICON()		TotalInfo::GetInstance()->HideBattleIcon()

/* �o�g���t���O���^�X�N�֘A */
#define TASKVALUE_TO_BATTLEFLAG(big, little)	(big * 32 + little)
#define BATTLEFLAG_TO_TASKVALUE_A(battleflag)	(battleflag >> 5)
#define BATTLEFLAG_TO_TASKVALUE_B(battleflag)	(battleflag & 0x1F)
#define SET_EVENT_END_FLAG()					(TotalInfo::GetInstance()->SetEventFlag(sTaskInfo.mTaskValueA, sTaskInfo.mTaskValueB))
// �C�x���g�I������
#define IS_END_EVT(evtA, evtB)					TotalInfo::GetInstance()->GetEventFlagEnable(evtA, evtB)

/* �G���擾�}�N�� */
//#define GET_ENEMY_TBL_INFO()			(&sEnemyArrayTable[spBGInfo->mEnemyTblIdx])
#define GET_ENEMY_TBL_INFO()			(GetEnemyTableInfo(spBGInfo->mEnemyTblIdx))
#define GET_ENEMY_PATTERN_NUM()			(GET_ENEMY_TBL_INFO()->mEnemyPatternNum)
#define GET_ENEMY_KIND(pattern, index)	(*(GET_ENEMY_TBL_INFO()->mpEnemyKind + (pattern * kENEMY_ONCE_MAX) + index))

/*=====================================*
 * define
 *=====================================*/
static const float sEvt24_WaterCheckHeight	= -10.0f;
static const float sEvt24_WaterStopHeight	= -100.0f;
static const int kENEMY_ITEM_NUM			= 5;

// ���f�����W�X�V����
static const float ALL_MODEL_REVISE_DIST	= 50000.0f;
// �X�J�C�h�[���̃f�t�H���g�X�P�[��
static const float kSKYDOME_DEF_SCALE		= 980.0f;

#define EVENT29_DISABLE_OFFSET_X			(20000.0f)

/*=====================================*
 * �e�I�u�W�F�N�g�̒�`
 *=====================================*/
enum {
	eMD_SKYDOME = 0,
	eMD_SWORD,
	eMD_GETTED_ITEM,
	eMD_LOCKON,
	eMD_SHIELD,

	eMD_MAX,
};

/*
enum {
	eLY_LINO_FACE = 0,
	eLY_SUB_WEAPON,

	eLY_GAUGE_LINE,
	eLY_GAUGE_FRAME,

	eLY_ENEMY_FACE,

	eLY_MAP_ARROW,
	eLY_MAP_POSITION,

	eLY_BALLOON,
	eLY_INFO,

	eLY_DEFAULT_DRAW_MAX,					// �ŏ�����\������Ă��郌�C�A�E�g�̐�

	// �ȉ��̃��C�A�E�g���ŏ��͕\������Ă��Ȃ����C�A�E�g
	eLY_HAS_MAP = eLY_DEFAULT_DRAW_MAX,		// �}�b�v�������Ă��邩�H
	eLY_HAS_KEY,							// ���������Ă��邩�H

	eLY_MAX,
};
*/

#endif // __SCENE_INFO_H__

/**** end of file ****/

