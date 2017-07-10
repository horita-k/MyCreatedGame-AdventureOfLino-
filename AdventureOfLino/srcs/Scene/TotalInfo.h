/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * TotalInfo.cpp
 * �^�X�N(�C�x���g�Ǘ�)�N���X
 * @author SPATZ.
 * @data   2012/04/04 14:48:17
 */
//---------------------------------------------------
#ifndef __TOTAL_INFO_H__
#define __TOTAL_INFO_H__

#include "dxlib.h"
#include "DataDefine/ItemDef.h"
#include "../Charactor/CharaInfo.h"
#include "EventFunctions.h"
#include "AppLib/Graphic/LayoutBase.h"

#include <time.h>

/*=====================================*
 * define
 *=====================================*/
#define GAME_TITLE		("AdventureOfLino Ver2.0.1")

/*=====================================*
 * �ėp��`�l
 *=====================================*/
static const float	kBASIC_FRAME_COUNTER= 1.0f;
static const BYTE	kWEAPON_FLAG_BYTE	= 3;

#ifdef _DEBUG

#define DBG_KEY_DUMP_STATUS		(eKEY_NUMPAD1)
#define DBG_KEY_SKYAWAY			(eKEY_NUMPAD2)
#define DBG_KEY_WARP_LASTPOS	(eKEY_NUMPAD3)
#define DBG_KEY_EVENT_CLEAR		(eKEY_NUMPAD4)
#define DBG_KEY_CURE_PLAYER		(eKEY_NUMPAD5)
#define DBG_KEY_HEAL_ENEMY		(eKEY_NUMPAD6)
#define DBG_KEY_VISIBLE_EARTH	(eKEY_NUMPAD7)
#define DBG_KEY_VISIBLE_WALL	(eKEY_NUMPAD8)
#define DBG_KEY_START_EVENT		(eKEY_NUMPAD9)

#endif // _DEBUG

// �T�u�E�F�|�����Ǘ�
enum eSubweaponNum {
	eSUBWEAPON_NUM_BOMB = 0,
	eSUBWEAPON_NUM_ARROW,
	eSUBWEAPON_NUM_HEART_PIECE,

	eSUBWEAPON_NUM_BIN_EMPTY,
	eSUBWEAPON_NUM_BIN_MAGENTA,
	eSUBWEAPON_NUM_BIN_CYAN,
	eSUBWEAPON_NUM_BIN_YELLOW,
	eSUBWEAPON_NUM_BIN_FAILY,

	eSUBWEAPON_NUM_MAX,
};

static const BYTE kEMPTY_BIN_NUM = 4;


// ���Ԋ֘A
enum eTimeKind {
	eTIME_KIND_SEC = 0,
	eTIME_KIND_MIN,
	eTIME_KIND_HOUR,

	eTIME_KIND_MAX,
};

/*=====================================*
 * �Z�[�u�֘A
 * 2015/05/08 MajorVersion�� 1 => 2�ɍX�V *
 *=====================================*/
/* �e�X���b�g���Ƃ̃Z�[�u�p�\���� */
/* �� �����ɗv�f��ǉ�����ꍇ�� SaveData, UpdateLoadDataFromBuffer, ClearSaveInfo, ConvSaveData_101_to_201 �ɂ��������K�v */
struct tSaveWork 
{
	unsigned char	mSavePlaceIndex;
	unsigned char	mSaveMaxHP;
	unsigned char	mSaveNowHP;
	unsigned short	mSaveMoney;
	unsigned long	mSaveEventFlag[eEVENT_MAX];
	unsigned long	mSaveObjectFlag[eEVENT_MAX];
	unsigned long	mSaveNaviTalkFlag[eEVENT_MAX];
	unsigned long	mSaveFieldInfoFlag[eFIELD_MAX];
	//	ULONGLONG		mSaveWeaponFlag;
	ULONG			mSaveWeaponFlag[kWEAPON_FLAG_BYTE];
	unsigned char	mSaveNowWeapon;
	UCHAR			mSaveSubWeaponNum[eSUBWEAPON_NUM_MAX];
	UCHAR			mSavePlayTime[eTIME_KIND_MAX];
	DWORD			mSaveCosplayBit;
	UCHAR			mSaveNowCosplayIndex;						// ���݂̃R�X�v��
	UCHAR			mSaveColorRevision;							// �F�ʕ␳�l
	UCHAR			mSaveSubPlaceIndex;
	DWORD			mSaveKeyItemBit;
	UCHAR			mSaveNowMP;									// �}�W�b�N�|�C���g
	UCHAR			mSaveSongBit;								// �S(�I�J���i)�r�b�g
};
#define kSAVE_DATA_HEADER				("AOLS")
static const char* saveFileName			= "Save.dat";
/* �Q�[���o�[�W���� Ver 2.0.1 ���Ŕz�z */
/* �Q�[���o�[�W���� Ver 2.0.2 ���ŕs��C��*/
static const char  kSAVE_DATA_MAJOR_VER			= 2;
static const char  kSAVE_DATA_MINOR_VER			= 1;

/* ------------------------------------------------------------------------ */
/**** �Q�[���z�z 1.1�ł̎��̃Z�[�u�f�[�^ ****/
enum {
	eVER101_EVENT_00 = 0,
 	eVER101_EVENT_01,
	eVER101_EVENT_05,
	eVER101_EVENT_10,
	eVER101_EVENT_13,
	eVER101_EVENT_21,
	eVER101_EVENT_22,
	eVER101_EVENT_24,			// �n�����H
	eVER101_EVENT_25,			// �n���S
	eVER101_EVENT_26,			// �n���
	eVER101_EVENT_27,			// �󔠃X�e�[�W

	eVER101_EVENT_MAX,
};
enum {
	eVER101_FIELD_HOME = 0,
	eVER101_FIELD_REBERNIAN,
	eVER101_FIELD_WILDERNESS,
	eVER101_FIELD_WATERWAY,
	eVER101_FIELD_PRISM,

	eVER101_FIELD_MAX,
};
struct tVer101_SaveWork 
{
	unsigned char	mSavePlaceIndex;
	unsigned char	mSaveMaxHP;
	unsigned char	mSaveNowHP;
	unsigned short	mSaveMoney;
	unsigned long	mSaveEventFlag[eVER101_EVENT_MAX];
	unsigned long	mSaveObjectFlag[eVER101_EVENT_MAX];
	unsigned long	mSaveNaviTalkFlag[eVER101_EVENT_MAX];
	unsigned long	mSaveFieldInfoFlag[eVER101_FIELD_MAX];
	unsigned long	mSaveWeaponFlag;
};
/**** �Q�[���z�z 1.1�ł̎��̃Z�[�u�f�[�^ ****/
/* ------------------------------------------------------------------------ */


/*=====================================*
 * �R���t�B�O�֘A�̏��
 *=====================================*/
static const char*			kCONFIG_HEADER_STRING	= "LOLS";
/* �R���t�B�O�o�[�W���� Ver 2.1 ���Ŕz�z		*/
/* �R���t�B�O�o�[�W���� Ver 2.2 ���ŕs��C��	*/
static const unsigned short kCONFIG_MAJAR_VER	= 2;
static const unsigned short kCONFIG_MINOR_VER	= 2;

enum eConfigParam {
	/*
	eCONFIG_VIDEO_SIZE,                // ��ʃT�C�Y
	eCONFIG_GRAPH_QUALITY,		       // �掿�N�I���e�B
	eCONFIG_IS_ANTIALIAS,              // �A���`�G���A�X
	eCONFIG_EFF_QUALITY,				// �G�t�F�N�g�N�I���e�B
	eCONFIG_JOY_1,                     // joy �ʏ�U��
	eCONFIG_JOY_2,                     // joy �T�u�E�F�|���g�p
	eCONFIG_JOY_3,                     // joy �W�����v
	eCONFIG_JOY_4,                     // joy ���
	eCONFIG_JOY_5,                     // joy ���b�N�I��
	eCONFIG_JOY_6,                     // joy �T�u�E�F�|���ؑ�
	eCONFIG_JOY_7,                     // joy �}�b�v�\��
	eCONFIG_JOY_8,                     // joy �|�[�Y/�X�L�b�v
	eCONFIG_JOY_9,                     // joy �J�����ؑ�

	eCONFIG_CAP_1,		// �����s�v�f�[�^�����A�������̈׃o�b�t�@�ɕۑ��E�E�E
	eCONFIG_CAP_2,
	eCONFIG_CAP_3,
	eCONFIG_CAP_4,
	eCONFIG_CAP_5,
	eCONFIG_CAP_6,
	eCONFIG_CAP_7,
	eCONFIG_CAP_8,
	eCONFIG_CAP_9,
	eCONFIG_CAP_10,
	eCONFIG_CAP_11,
	eCONFIG_CAP_12,

	// Ver1.0.1�ǉ���
	eCONFIG_JOY_10,						// joy �J�������Z�b�g
	eCONFIG_ANALOG_REVERSE,				// �A�i���O�L�[���]
		
	eCONFIG_MAX,
	*/
	// Graphic
	eCONFIG_SCREEN_SIZE,       // ��ʃT�C�Y
	eCONFIG_GRAPHIC_QUALITY,   // �O���t�B�b�N�i��
	eCONFIG_ANTIALIAS,         // �A���`�G�C���A�X
	eCONFIG_BACKGROUND_RUN,    // �o�b�N�O���E���h�N��
	// Pad
	eCONFIG_JOY_1,                     //
	eCONFIG_JOY_2,                     //
	eCONFIG_JOY_3,                     //
	eCONFIG_JOY_4,                     //
	eCONFIG_JOY_5,                     //
	eCONFIG_JOY_6,                     //
	eCONFIG_JOY_7,                     //
	eCONFIG_JOY_8,                     //
	eCONFIG_JOY_9,                     //
	eCONFIG_JOY_10,
	eCONFIG_CAP_1,
	eCONFIG_CAP_2,
	eCONFIG_CAP_3,
	eCONFIG_CAP_4,
	eCONFIG_CAP_5,
	eCONFIG_CAP_6,
	eCONFIG_CAP_7,
	eCONFIG_CAP_8,
	eCONFIG_CAP_9,
	eCONFIG_CAP_10,
	eCONFIG_CAP_11,
	eCONFIG_CAP_12,
	// Analog
	eCONFIG_LEFT_ANALOG_HORIZON,
	eCONFIG_LEFT_ANALOG_VERTICAL,
	eCONFIG_LEFT_ANALOG_AXIS,
	eCONFIG_RIGHT_ANALOG_HORIZON,
	eCONFIG_RIGHT_ANALOG_VERTICAL,
	eCONFIG_RIGHT_ANALOG_AXIS,
	// KeCONFIG_yboard
	eCONFIG_KEY_UP,
	eCONFIG_KEY_DOWN,
	eCONFIG_KEY_RIGHT,
	eCONFIG_KEY_LEFT,

	eCONFIG_KEY_CAM_UP,
	eCONFIG_KEY_CAM_DOWN,
	eCONFIG_KEY_CAM_RIGHT,
	eCONFIG_KEY_CAM_LEFT,
	eCONFIG_KEY_RESET_CAMERA,
	eCONFIG_KEY_CHANGE_CAMERA,

	eCONFIG_KEY_ATTACK,
	eCONFIG_KEY_USE_ITEM,
	eCONFIG_KEY_JUMP,
	eCONFIG_KEY_AWAY,
	eCONFIG_KEY_LOCKON,
	eCONFIG_KEY_SELECT_ITEM,
	eCONFIG_KEY_SHOW_MAP,
	eCONFIG_KEY_SKIP_EVENT,

	eCONFIG_MOUSE_SCROLL_VAL,

	eCONFIG_MAX,
};

/*=====================================*
 * �E�B���h�E�Y���j���[�ꗗ��`
 *=====================================*/
enum {
	// �t�@�C��
	eWINMENU_MENU_ROOT = 0,
	eWINMENU_MENU_TO_TITLE,
	eWINMENU_MENU_EXIT,

	// �\��
	eWINMENU_VISIBLE_ROOT,
	eWINMENU_VISIBLE_FPS,

	// �L�[�R���t�B�O
	eWINMENU_KEYCONFIG_ROOT,
	eWINMENU_KEYCONFIG_KEYCONFIG,

	// �e�̕\��
	eWINMENU_SHADOW_ROOT,
	eWINMENU_SHADOW_CIRCLE,
	eWINMENU_SHADOW_SHADOWMAP_MODEL,
	
	// �F�ʕ␳
	eWINMENU_COLOR_REVISION_ROOT,
	eWINMENU_COLOR_REVISION_DISABLE,
	eWINMENU_COLOR_REVISION_ENABLE,

	eWINMENU_MAX,

	// �g���R�X�v�����
	eWINMENU_COSPLAY_ROOT = eWINMENU_MAX,
};
/*=====================================*
 * �R�X�v�����
 *=====================================*/
enum {
	// �R�X�v��
	eWINMENU_COSPLAY_NONE = 0,					// ���m(�R�X�v���Ȃ�)
	eWINMENU_COSPLAY_FRES,						// �t���X�x���N
	eWINMENU_COSPLAY_DABAH,						// ���΂�
	eWINMENU_COSPLAY_SUNOPHONE,					// �X�m�t�H��
	eWINMENU_COSPLAY_MASK,						// �}�X�N
	eWINMENU_COSPLAY_BAT,						// ��������
	eWINMENU_COSPLAY_TDA_MIKU,					// �~�N
	eWINMENU_COSPLAY_PRONAMA,					// �v���������
	eWINMENU_COSPLAY_LAPAN,						// ���p��
	eWINMENU_COSPLAY_CHILNO,					// �`���m
	eWINMENU_COSPLAY_HONGKONG,					// ���`�f��P
	eWINMENU_COSPLAY_LOANA,						// ���A�i
	eWINMENU_COSPLAY_YUKARI,					// �����䂩��
	eWINMENU_COSPLAY_NAKANO,					// ��{�J�m

	// ��蕨�e�B�^�j�X��ԕ��ɂ�����
	eWINMENU_COSPLAY_TITANIS,					// �e�B�^�j�X
	eWINMENU_COSPLAY_GUARDIAN,					// �K�[�f�B�A��
	
	eWINMENU_COSPLAY_MAX,
};

/* �s�v�E�E�E�H
enum eConfigLoadState {
	eCONFIG_LOAD_OK = 0,
	eCONFIG_LOAD_ERR_HEADER,	// �w�b�_�[�̑���G���[
	eCONFIG_LOAD_ERR_VERSION,	// �o�[�W��������G���[
};
*/

struct tDisplaySetting {
	int mResolusionX;
	int mResolusionY;
	int mColorBit;
	int mWindowSizeX;
	int mWindowSizeY;
};
static const tDisplaySetting sDisplaySetting[] = {
	/*
	  {1680,	1050,	32,		-1,		-1, },	// �t���X�N���[��
	  {1024,	768,	32,		640,	480,},	// ����L���v�`���p���
	  {800,	600,	32,		800,	600,},	// 
	  {1024,	768,	32,		1024,	768,},	// ���掿�Q�[���p���
	  {960,	720,	32,		640,	480,},	// 
	  {640,	480,	32,		640,	480,},	// 
	  {512,	384,	32,		512,	384,},	// 
	  {2048,	1536,	32,		1024,	768,},	// 
	*/
	{320, 	240,	32,		320, 	240, },
	{640, 	360,    32,    	640, 	360, },
	{640, 	480,    32,    	640, 	480, },
	{800, 	600,    32,    	800, 	600, },
	{1024,	768,    32,    	1024,	768, },
	{1280,	720,    32,    	1280,	720, },
	{1280,	960,    32,    	1280,	960, },
	{1600,	900,    32,    	1600,	900, },
	{1600,	1200,   32,    	1600,	1200,},
	{1920,	1080,   32,    	1920,	1080,},
	{2048,	1536,   32,    	2048,	1536,},
	//<! �f�o�b�O�p�ɋ}������
	{1024,	768,	32,		640,	480,},	// ����L���v�`���p���
};

/*=====================================*
 * class
 *=====================================*/
class TotalInfo {
	
 public:
	static TotalInfo* const GetInstance(void);

	// ���[�h��
	enum eLoadStatus {
		eLOAD_FAILED = 0,
		eLOAD_SUCEESS,
		eLOAD_REQUEST_RELOAD,		// �ă��[�h�˗�
	};

	// ���C�A�E�g���
	enum {
		eLY_MONEY = 0,
		eLY_COMPASS,
		eLY_LOCKON,
		eLY_HEART_00,
		eLY_HEART_01,
		eLY_HEART_NOW,
		eLY_PIYOPIYO_GAUGE_FRAME,
		eLY_PIYOPIYO_GAUGE_LINE,
		eLY_PIYOPIYO_GAUGE_ICON,
		eLY_MAGIC_GAUGE_FRAME,
		eLY_MAGIC_GAUGE_LINE,
		eLY_MAGIC_GAUGE_ICON,
		eLY_LINO_FACE,
		eLY_RING_FLAME,
		eLY_SUB_WEAPON,
		eLY_GAUGE_LINE,
		eLY_GAUGE_FRAME,
		eLY_MAP_ARROW,
		eLY_MAP_POSITION,
		eLY_BALLOON,
		// �ȉ��̃��C�A�E�g���ŏ��͕\������Ă��Ȃ����C�A�E�g
		eLY_HAS_MAP,							// �}�b�v�������Ă��邩�H
		eLY_HAS_KEY,							// ���������Ă��邩�H

		eLY_MAX,
	};

	//	static int sDefaultMotionModelHandle;
	struct tLoadWork {
		unsigned char		mPlaceIndex;
		unsigned char		mMaxHP;
		unsigned char		mNowHP;
		unsigned char		mNowWeapon;
		UCHAR				mNowCosplayIndex;
		UCHAR				mSubPlaceIndex;
		UCHAR				mNowMP;
	};

	struct tWinMenuCosplayInfo {
		const char* mMessage;
		const char* mModelName;
		const char* mLayoutName;
	};

	/* �֐� */
	void		Setup(void);
	void		Update(void);

	/*=====================================*
	 * �R���t�B�O�f�[�^�֘A
	 *=====================================*/
	bool		LoadConfigData(void);
	bool		SetupConfigData(char* pBuf, int size);

	void		SetupAssignKeyboard(void);
	void		SetupAssignJoyInput(void);
	void		SetupAnalogReverse(void);
	void		SetupMouseSensitive(void);

	int			GetConfigVideoSize(void);
	float		GetConfigResolutionRateFromGraphQuality(void);
	bool		GetConfigIsAntiAlias(void);
	bool		GetConfigIsBackgroundRun(void);
	int			GetConfigEffectQuality(void);

	float		GetConfigMusicVolumeScale(void)	{ return mMusicVolumeScale; }
	float		GetConfigSoundVolumeScale(void)	{ return mSoundVolumeScale; }
	float		GetConfigVoiceVolumeScale(void)	{ return mVoiceVolumeScale; }

	/*=====================================*
	 * �Z�[�u�֘A
	 *=====================================*/
	bool		SaveData(int slotID, int stageIndex, int maxHP, int nowHP, int nowWeapon, BYTE cosplayIndex, BYTE subPlaceIndex, UCHAR nowMP);

	bool		saveDataWork(int slotID, tSaveWork& rSaveWork, bool isConv=false);

	//	eLoadStatus	LoadData(int slotID);
	eLoadStatus	LoadDataToBuffer(tSaveWork* p3LoadWork);
	void		UpdateLoadDataFromBuffer(tSaveWork& rLoadWork);
	void		SetPlayintSaveSlot(int slotIndex)						{ mPlayingSaveSlot = slotIndex; }

	void		GetLoadWork(TotalInfo::tLoadWork* pLoadWork)			{ *pLoadWork = mLoadWork; }
	void		ClearSaveInfo(tSaveWork& rSaveWork);
	void		ConvSaveData_101_to_201(tSaveWork& rSaveWork);
	//	void		GetTotalPlayTime(UCHAR* pPlayTimeArray);

	/*=====================================*
	 * Event flags �֘A
	 *=====================================*/
	void		SetEventFlag(unsigned long kind, unsigned long flag);
	bool		GetEventFlagEnable(unsigned long kind, unsigned long flag);
	ULONG		GetEventFlagValue(unsigned long kind);
	void		SetEventFlagValue(unsigned long kind, unsigned long value);
	void		DisableEventFlag(unsigned long kind, unsigned long flag);
	void		ResetAllEventFlag(void);

	/*=====================================*
	 * �i�r�H�g�[�N �֘A
	 *=====================================*/
	void		SetNaviTalkFlag(unsigned long kind, unsigned long flag);
	bool		GetNaviTalkFlagEnable(unsigned long kind, unsigned long flag);

	/*=====================================*
	 * Object Flags �֘A
	 *=====================================*/
	void			SetObjectFlagBit(unsigned long kind, unsigned long bit);
	unsigned long	GetObjectFlagBit(unsigned long kind);

	/*=====================================*
	 * �T�u�E�F�|���t���O
	 *=====================================*/
	void		SetWeaponFlag(eItemKind item);
	void		DisableWeaponFlag(eItemKind item);
	bool		IsWeaponFlag(eItemKind item);

	/*=====================================*
	 * Item flags �֘A
	 *=====================================*/
	void		EnableItemFlag(eItemKind kind);
	bool		IsItemFlag(eItemKind kind);

	/*=====================================*
	 * �t�B�[���h���Ƃ�flags �֘A
	 *=====================================*/
	void		SetEnableFieldInfoFlag(int stageIndex, unsigned long flag);
	void		SetDisableFieldInfoFlag(int stageIndex, unsigned long flag);
	bool		GetHasFieldMap(int stageIndex);
	bool		GetHasFieldKey(int stageIndex);

	bool		GetIsFieldStartNoConvert(int fieldIndex);
	unsigned long GetFieldInfoFlagBit(int stageIndex);
	void		ResetFieldInfoFlag(void);

	/*=====================================*
	 * ���C�A�E�g�֘A
	 *=====================================*/
	void		LoadTotalInfoLayout(void);

	void		VisibleBattleIcon(void);
	void		HideBattleIcon(void);
	void		UpdateCompass(void);
	void		CrazyCompass(void);
	void		SetCommapssForShowMap(void);

	void		EnableLockonLayout(VECTOR* pWorldPos, BOOL isHighlight=TRUE);
	void		DisableLockonLayout(void);
	void		SetActiveLockonEnemyName(BOOL isActive);

	void		UpdateHeartLayout(int maxHP, int nowHP);
	void		ShowHeartLayout(int maxHP, int nowHP, BOOL isTitle=FALSE);

	LayoutBase*	GetSystemLayout(int index);

	const char*	GetLinkLayoutName(int numIndex);
	void		UpdatePiyoPiyoGaugeByRate(float rate, float magicRate);

	void		FadeinLockonEnemyLayout(int gaugeSize);
	void		FadeoutLockonEnemyLayout(void);
	void		ChangePlayerFaceLayout(bool isEncount);

	void		MsgTypeTelop(void);
	void		MsgTypeMessage(void);
	void		MsgFadeIn(float sec=0.3f);
	void		MsgFadeOut(float sec=0.3f);
	bool		MsgIsFade(void);

	void		setupLayoutSettingInfo(int lytIndex);
	void		setLayoutPosPadRight(int lytIndex);

	/*=====================================*
	 * ���֘A
	 *=====================================*/
	int			GetNowMoney(void);
	void		AddMoney(int value);

	void		AddItemNum(eSubweaponNum subWeaponNum, int value, bool isUpdateDraw=true);
	UCHAR		GetItemNum(eSubweaponNum subWeaponNum);

	/*=====================================*
	 * �E�B���h�E���j���[�֘A
	 *=====================================*/
	void		SetupWinMenu(void);
	void		SetEnableWinMenu(BOOL isEnable);
	BYTE&		IsVisibleFPS(void)						{ return mIsVisibleFPS; }

	// �F�ʕ␳�L���E�����t���O (�������A���j���[�ݒ肪�D��)
	void		SetEnableColorRevision(BOOL isEnable);

	/*=====================================*
	 * ���Ԋ֘A
	 *=====================================*/
	void		StartPlayingTime(void);
	void		UpdatePlayingTime(void);

	/*=====================================*
	 * �R�X�v���֌W
	 *=====================================*/
	BYTE		GetCosplayNum(void);
	DWORD		GetCosplayBit(void)						{ return mCosplayBit; }
	void		AddCosplay(int cosplayIndex);
	tWinMenuCosplayInfo* GetCosplayInfo(int cosplayKind);
	
	/*=====================================*
	 * �L�[�A�C�e���֘A
	 *=====================================*/
	DWORD		GetKeyItemBit(void);
	void		AddKeyItem(USHORT keyItemIndex);
	BOOL		IsKeyItem(USHORT keyItemIndex);

	/*=====================================*
	 * �S(�I�J���i)�֘A
	 *=====================================*/
	void		AddSongType(UCHAR songType);
	BOOL		IsSongType(UCHAR songType);

	// 
	//	static unsigned long sInputAttack[2];
	//	unsigned long&	InputAttack(int index)		{ return mInputAttack[index]; }

 private:
	TotalInfo();
	~TotalInfo();

	/* �ϐ� */
	tLoadWork			mLoadWork;

	ULONG				mEventFlag[eEVENT_MAX];
	ULONG				mObjectFlag[eEVENT_MAX];
	ULONG				mNaviTalkFlag[eEVENT_MAX];
	ULONG				mFieldInfoFlag[eFIELD_MAX];
	//	ULONGLONG			mWeaponFlag;
	ULONG				mWeaponFlag[kWEAPON_FLAG_BYTE];

	ULONG				mItemFlag;

	//<! �ȉ��̓Z�[�u���Ɉړ�����\��
	float				mMusicVolumeScale;
	float				mSoundVolumeScale;
	float				mVoiceVolumeScale;

	UCHAR				mSubWeaponNum[eSUBWEAPON_NUM_MAX];
	UCHAR				mTotalPlayTime[eTIME_KIND_MAX];

	time_t				mPlayStartTime;
	DWORD				mCosplayBit;
	DWORD				mKeyItemBit;
	UCHAR				mSongBit;

	/*=====================================*
	 * ���֘A
	 *=====================================*/
	int					mNowMoney;

	/*=====================================*
	 * ���j���[�ŕύX�ł��鍀��
	 *=====================================*/
	BYTE				mIsVisibleFPS;
	CHAR				mPlayingSaveSlot;

	//	ULONG		mInputAttack[2];

};

// �E�B���h�E���j���[�X�V�ʒm�R�[���o�b�N�֐�
void CallbackMenuSelect(const TCHAR *pItemName, int nItemID);

#endif //__TOTAL_INFO_H__

 /**** end of file ****/
