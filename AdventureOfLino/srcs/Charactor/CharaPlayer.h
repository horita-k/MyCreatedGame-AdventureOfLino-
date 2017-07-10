/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �v���C���[�N���X
 * @author SPATZ.
 * @data   2012/02/05 21:48:31
 */
//---------------------------------------------------
#ifndef __CHARA_PLAYER_H__
#define __CHARA_PLAYER_H__

#include "CharaBase.h"
#include "Scene/TotalInfo.h"
#include "Object/BladeEffect.h"
#include "Object/ObjectHand.h"
#include "DataDefine/BGDef.h"
#include "Object/ObjectWeapon.h"
#include "AppLib/Graphic/EffekseerManager.h"

enum eBoomerangState {
	eBOOMERANG_STATE_NONE = 0,
	eBOOMERANG_STATE_FOWARD_NO_DAMAGE,
	eBOOMERANG_STATE_FOWARD_DAMAGED,
	eBOOMERANG_STATE_REVERSE_NO_DAMAGE,
	eBOOMERANG_STATE_REVERSE_DAMAGED,
	
	eBOOMERANG_STATE_MAX,
};

// �u�[���������
struct tBoomerangWork {
	//	bool		mIsReverse;
	eBoomerangState		mState;
	VECTOR		mMoveVec;
	VECTOR		mRevVec;
	int			mSndHandle;
};

enum eWingJumpState {
	eWING_JUMP_STATE_NONE = 0,
	eWING_JUMP_STATE_APPEAR,
	eWING_JUMP_STATE_DISAPPEAR,

	eWING_JUMP_STATE_MAX,
};

// ���e�g�p��
#define BOMB_USE_NUM					(4)
#define LIBSHIELD_LIGHT_MESH_IDX		(1)

#define kPLAYER_BOMB_NUM				(4)
#define kPLAYER_ARROW_NUM				(8)

// ���m�̃t���[���ԍ�
static const BYTE kBAG_FRAME_INDEX		= 7;			// �o�b�O
static const BYTE kKNIFE_FRAME_INDEX	= 9;			// �i�C�t
static const BYTE kHAT_FRAME_INDEX		= 18;			// �X�q
static const BYTE kSHEATH_FRAME_INDEX	= 24;			// ��̃C���f�b�N�X
static const BYTE kHILT_FRAME_INDEX		= 25;			// ���̃C���f�b�N�X

/*=====================================*
 * class
 *=====================================*/
class CharaPlayer : public CharaBase {
 public:

	/* ��` */
	enum {
		// 20
		eCHARA_STATE_NOTHING = eCHARA_BASE_STATE_MAX,
		eCHARA_STATE_ATTACK_1,
		eCHARA_STATE_ATTACK_2,
		eCHARA_STATE_ATTACK_3,
		eCHARA_STATE_ATTACK_4,
		// 25
		eCHARA_STATE_ATTACK_THRUST,
		eCHARA_STATE_ATTACK_ROOLING_START,
		eCHARA_STATE_ATTACK_ROOLING,
		eCHARA_STATE_SUBWEAPON_START,	// eCHARA_STATE_HOOK_SHOT_START,		// �t�b�N�V���b�g
		eCHARA_STATE_HOOK_SHOTTING,
		// 30
		eCHARA_STATE_HOOK_SHOT_END,
		eCHARA_STATE_HOOK_MOVING,
		eCHARA_STATE_AWAY_RIGHT,
		eCHARA_STATE_AWAY_LEFT,
		eCHARA_STATE_AWAY_BACK,
		// 35
		eCHARA_STATE_PUT_SWORD,
		eCHARA_STATE_BOOMERANG_START,		// �u�[������
		eCHARA_STATE_AWAY_FRONT,
		eCHARA_STATE_ATTACK_JUMP,
		eCHARA_STATE_SLEEP,
		// 40
		eCHARA_STATE_SMILE,
		eCHARA_STATE_WAIT_RESET_CAMERA,
		eCHARA_STATE_LIB_SHIELD,				// �t�����N�\���o�b�W
		eCHARA_STATE_RAISE_IDLE,				// ���������グ��E�A�C�h��
		eCHARA_STATE_RAISE_TURN,				// ���������グ��E�^�[��
		// 45
		eCHARA_STATE_RAISE_MOVE,				// ���������グ��E�ړ�
		eCHARA_STATE_RAISE_THROW,				// ���𓊂���
		eCHARA_STATE_RAISE_PUT,					// ����u��
		eCHARA_STATE_BOMB_START,				// ���e�J�n
		eCHARA_STATE_ARCHERY_START,				// �|��J�n
		// 50
		eCHARA_STATE_ARCHERY_CHARGE,			// �|����
		eCHARA_STATE_ARCHERY_SHOOT,				// �|���
		eCHARA_STATE_ARCHERY_END,				// �|��I��
		eCHARA_STATE_ATTACK_SMASH_START,			// ���U���J�n
		eCHARA_STATE_ATTACK_SMASH_DOING,			// ���U����
		// 55
		eCHARA_STATE_ATTACK_SMASH_END,			// ���U���I��
		eCHARA_STATE_LOOK_AROUND,				// ����
		eCHARA_STATE_ATTACK_REAR,				// �w�ʎa��
		eCHARA_STATE_ATTACK_ROLING_KICK,		// �񂵏R��
		eCHARA_STATE_GUARD_STAGGER,				// ���ł�낯��
		// 60
		eCHARA_STATE_PIYOPIYO,					// �s���s��
		eCHARA_STATE_GUTS_POSE,					// �K�b�c�|�[�Y
		eCHARA_STATE_MAGIC_THUNDER,				// ���@�E�T���_�[
		eCHARA_STATE_MAGIC_FLAME,				// ���@�E�t���C��
		eCHARA_STATE_MAGIC_TORNADE,				// ���@�E�A�C�X
		// 65
		eCHARA_STATE_PLAY_OCARINA,				// �I�J���i���t
		eCHARA_STATE_COPY_ROD,					// �R�s�[���b�h
		eCHARA_STATE_AFTER_EVENT,				// ��b�E�󔠃C�x���g����

		eCHARA_STATE_MAX,
	};

	enum {
		eATTACH_MODEL_SWORD,
		eATTACH_MODEL_SUB_WEAPON,
		eATTACH_MODEL_ANGEL_WING,
		eATTACH_MODEL_SHIELD,
		
		eATTACH_MODEL_MAX,
	};

	enum {
		eITEM_SWORD_NONE,
		eITEM_SWORD_MASTERSWORD,

		eITEM_SWORD_MAX,
	};

	enum {
		eITEM_CLOTH_NORMAL,
		eITEM_CLOTH_RED,
		eITEM_CLOTH_BLUE,
		eITEM_CLOTH_YELLOW,
		
		eITEM_CLOTH_MAX,
	};

	enum eRequestCamera {
		eREQUEST_CAMERA_NONE = 0,
		eREQUEST_CAMERA_HOOKSHOT,
		eREQUEST_CAMERA_BOOMERANG,

		eREQUEST_CAMERA_MAX,
	};

	// �A�^�b�`�p�^�[��
	enum eAttachPattern {
		eATTACH_PATTERN_NORMAL,				//	����󂫁E���Ə�
		eATTACH_PATTERN_BOOMERANG,			//	�u�[������
		eATTACH_PATTERN_RAISE_HAND,			//	���������Ă���
		eATTACH_PATTERN_ARCHERY,			//	�|
		eATTACH_PATTERN_HOOKSHOT,			//	�t�b�N�V���b�g
		eATTACH_PATTERN_SWORD,				//	���̂�(���@)
		eATTACH_PATTERN_HAND,				//  �f��
		eATTACH_PATTERN_OCARINA,			//  �I�J���i
		eATTACH_PATTERN_COPY_ROD,			//  �R�s�[���b�h
		
		eATTACH_PATTERN_MAX,
	};

	struct tItemWork {
		int			mItemSword;
		eItemKind	mItemSubWeapon;
		int			mItemCloth;
	};

	typedef void (CharaPlayer::*PHASE_STATE)(void);

	/*�֐�*/
	CharaPlayer();
	~CharaPlayer();

	void				ReviseModel(VECTOR* pMoveVec);
	void				SetLoadParameter(int maxHP, int nowHP, UCHAR nowMP);
	void				Setup(bool isDefaultMotioon, int stageIndex);
	void				Draw(void);
	void				Update(void);
	void				ChangeState(int setState);
	void				UpdateAttackChara(void);
	void				ProcessFallDown(float addHeight);
	void				ProcessRecovery(void);
	void				ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType);
	void				ProcessBurstJump(VECTOR* pJumpVec);
	void				ProcessTechniqualJump(VECTOR* pJumpVec);
	void				ProcessRideVehicle(void);
	void				ProcessOperateMagicArmer(void);

	void				PermitAcceptInput(bool isPermit);
	eRequestCamera		GetRequestCamera(void)						{ return mRequestCamera; }
	ModelCollision*		GetLockonChara(void)						{ return mpLockonChara; }
	void				DisableLockonChara(void);
	tItemWork&			ItemWork(void)								{ return mItemWork; }
	void				SetActionLimit(const int actionLimit, eItemKind subWeaponKind=eITEM_KIND_NONE);
	void				GetCameraGeneralVec(VECTOR* pOut);			/* �ėp�J�����x�N�g���擾 */
	void				AddMaxHitpoint(int addValue);
	bool				IsSubWeaponUsing(void);
	tBoomerangWork*		GetBoomerangWork(void);
	void				SetObjectHandModel(ObjectHand* pObjectHand);
	ObjectHand*			GetObjectHandModel(void)					{ return mpObjectHand; }
	void				SetCheckPointPtr(ObjectBase* pObject)		{ mpCheckPointPtr = pObject; }
	ObjectBase* 		GetCheckPointPtr(void)						{ return mpCheckPointPtr; }
	UCHAR&				NowMP(void)									{ return mMagicPoint; }

	ObjectWeapon_Arrow*	GetArrowPtr(int index);
	void				processCancelArchery(void);
	void				SetupAttachAngelWing(void);
	void				RevertCosplay(void);
	BYTE&				CosplayIndex(void);
	int					GetFootstepKind(void);
	void				addPiyoPiyoGauge(SHORT addValue);
	void				GetMovedVec(VECTOR* pMovedVec);
	BOOL				CheckEnableMagic(eItemKind magicItem);
	void				AddNowMagicPoint(SHORT addValue);
	void				ChangeAttachPattern(eAttachPattern pattern);
	void				UnvisibleWeaponArrow(void);
	void				SetUsingRecoverItemKind(eItemKind itemKind);
	
 private:
	void				unprotectAllEnemyDamageFlag(void);
	void				getWeaponTopAndBtmPos(VECTOR* pTopPos, VECTOR* pBtmPos);
	void				updateAttackSmallEnemy(void);
	void				updateAttackRolling(void);
	void				updateAttackJump(void);
	void				updateAttackRollingKick(void);
	void				updateWaterCollision(void);
	void				updateWeaponTargetVecFromInput(VECTOR* pTargetVec, float scaleV, float scaleH, bool isOnMouseMiddle = false);
	ObjectWeapon_Arrow* getUnusedArrowPtr(void);
	void				updateAttachArcheryStringToPlayerFinger(ModelPmd* pArchery);
	void				processWingJump(void);
	void				updateWingJump(void);
	void				updateHookshotAttach(ModelPmd* pHookshotMdl);
	void				changeStateFunc(int charaState, PHASE_STATE phaseEnter, PHASE_STATE phaseUpdate, PHASE_STATE phaseLeave);
	bool				checkShieldHit(VECTOR *pAttackDirVec);
	void				updateLastEarthPos(void);					// mLastEarthPos�X�V����

	/*=====================================*
	 * phaseState
	 *=====================================*/
	void				phaseEnterStateIdle(void);
	void				phaseUpdateStateIdle(void);
	void				phaseLeaveStateIdle(void);
	void				phaseEnterStateTurn(void);
	void				phaseUpdateStateTurn(void);
	void				phaseLeaveStateTurn(void);
	void				phaseEnterStateMove(void);
	void				phaseUpdateStateMove(void);
	void				phaseLeaveStateMove(void);
	void				phaseEnterStateJumpUp(void);
	void				phaseUpdateStateJumpUp(void);
	void				phaseLeaveStateJumpUp(void);
	void				phaseEnterStateJumpDown(void);
	void				phaseUpdateStateJumpDown(void);
	void				phaseLeaveStateJumpDown(void);
	void				phaseEnterStateJumpEnd(void);
	void				phaseUpdateStateJumpEnd(void);
	void				phaseLeaveStateJumpEnd(void);
	void				phaseEnterStateDamage(void);
	void				phaseUpdateStateDamage(void);
	void				phaseLeaveStateDamage(void);
	void				phaseEnterStateRetireStart(void);
	void				phaseUpdateStateRetireStart(void);
	void				phaseLeaveStateRetireStart(void);
	void				phaseEnterStateRetireDoing(void);
	void				phaseUpdateStateRetireDoing(void);
	void				phaseLeaveStateRetireDoing(void);
	void				phaseEnterStateRetireEnd(void);
	void				phaseUpdateStateRetireEnd(void);
	void				phaseLeaveStateRetireEnd(void);
	void				phaseEnterStateSlopeDown(void);
	void				phaseUpdateStateSlopeDown(void);
	void				phaseLeaveStateSlopeDown(void);
	void				phaseEnterStateRefuseInput(void);
	void				phaseUpdateStateRefuseInput(void);
	void				phaseLeaveStateRefuseInput(void);
	void				phaseEnterStateDownStart(void);
	void				phaseUpdateStateDownStart(void);
	void				phaseLeaveStateDownStart(void);
	void				phaseEnterStateDownDoing(void);
	void				phaseUpdateStateDownDoing(void);
	void				phaseLeaveStateDownDoing(void);
	void				phaseEnterStateDownEnd(void);
	void				phaseUpdateStateDownEnd(void);
	void				phaseLeaveStateDownEnd(void);
	void				phaseEnterStateFallDown(void);
	void				phaseUpdateStateFallDown(void);
	void				phaseLeaveStateFallDown(void);

	/* �g���X�e�[�^�X */
	void				phaseEnterStateNothing(void);
	void				phaseUpdateStateNothing(void);
	void				phaseLeaveStateNothing(void);
	void				phaseEnterStateAttack1(void);
	void				phaseUpdateStateAttack1(void);
	void				phaseLeaveStateAttack1(void);
	void				phaseEnterStateAttack2(void);
	void				phaseUpdateStateAttack2(void);
	void				phaseLeaveStateAttack2(void);
	void				phaseEnterStateAttack3(void);
	void				phaseUpdateStateAttack3(void);
	void				phaseLeaveStateAttack3(void);
	void				phaseEnterStateAttack4(void);
	void				phaseUpdateStateAttack4(void);
	void				phaseLeaveStateAttack4(void);
	void				phaseEnterStateAttackThrust(void);
	void				phaseUpdateStateAttackThrust(void);
	void				phaseLeaveStateAttackThrust(void);
	void				phaseEnterStateAttackRollingStart(void);
	void				phaseUpdateStateAttackRollingStart(void);
	void				phaseLeaveStateAttackRollingStart(void);
	void				phaseEnterStateAttackRolling(void);
	void				phaseUpdateStateAttackRolling(void);
	void				phaseLeaveStateAttackRolling(void);
	void				phaseEnterStateHookShotStart(void);
	void				phaseUpdateStateHookShotStart(void);
	void				phaseLeaveStateHookShotStart(void);
	void				phaseEnterStateHookShotting(void);
	void				phaseUpdateStateHookShotting(void);
	void				phaseLeaveStateHookShoting(void);
	void				phaseEnterStateHookShotEnd(void);
	void				phaseUpdateStateHookShotEnd(void);
	void				phaseLeaveStateHookShotEnd(void);
	void				phaseEnterStateHookMoving(void);
	void				phaseUpdateStateHookMoving(void);
	void				phaseLeaveStateHookMoving(void);
	void				phaseEnterStateAwayRight(void);
	void				phaseUpdateStateAwayRight(void);
	void				phaseLeaveStateAwayRight(void);
	void				phaseEnterStateAwayLeft(void);
	void				phaseUpdateStateAwayLeft(void);
	void				phaseLeaveStateAwayLeft(void);
	void				phaseEnterStateAwayBack(void);
	void				phaseUpdateStateAwayBack(void);
	void				phaseLeaveStateAwayBack(void);
	void				phaseEnterStatePutSword(void);
	void				phaseUpdateStatePutSword(void);
	void				phaseLeaveStatePutSword(void);
	void				phaseEnterStateBoomerangStart(void);
	void				phaseUpdateStateBoomerangStart(void);
	void				phaseLeaveStateBoomerangStart(void);
	void				phaseEnterStateAwayFront(void);
	void				phaseUpdateStateAwayFront(void);
	void				phaseLeaveStateAwayFront(void);
	void				phaseEnterStateAttackJump(void);
	void				phaseUpdateStateAttackJump(void);
	void				phaseLeaveStateAttackJump(void);
	void				phaseEnterStateSleep(void);
	void				phaseUpdateStateSleep(void);
	void				phaseLeaveStateSleep(void);
	void				phaseEnterStateSmile(void);
	void				phaseUpdateStateSmile(void);
	void				phaseLeaveStateSmile(void);
	void				phaseEnterStateWaitResetCamera(void);
	void				phaseUpdateStateWaitResetCamera(void);
	void				phaseLeaveStateWaitResetCamera(void);
	void				phaseEnterStateLibShieldStart(void);
	void				phaseUpdateStateLibShieldStart(void);
	void				phaseLeaveStateLibShieldStart(void);
	void				phaseEnterStateRaiseIdle(void);
	void				phaseUpdateStateRaiseIdle(void);
	void				phaseLeaveStateRaiseIdle(void);
	void				phaseEnterStateRaiseTurn(void);
	void				phaseUpdateStateRaiseTurn(void);
	void				phaseLeaveStateRaiseTurn(void);
	void				phaseEnterStateRaiseMove(void);
	void				phaseUpdateStateRaiseMove(void);
	void				phaseLeaveStateRaiseMove(void);
	void				phaseEnterStateRaiseThrow(void);
	void				phaseUpdateStateRaiseThrow(void);
	void				phaseLeaveStateRaiseThrow(void);
	void				phaseEnterStateRaisePut(void);
	void				phaseUpdateStateRaisePut(void);
	void				phaseLeaveStateRaisePut(void);
	void				phaseEnterStateBombStart(void);
	void				phaseUpdateStateBombStart(void);
	void				phaseLeaveStateBombStart(void);
	void				phaseEnterStateArcheryStart(void);
	void				phaseUpdateStateArcheryStart(void);
	void				phaseLeaveStateArcheryStart(void);
	void				phaseEnterStateArcheryCharge(void);
	void				phaseUpdateStateArcheryCharge(void);
	void				phaseLeaveStateArcheryCharge(void);
	void				phaseEnterStateArcheryShoot(void);
	void				phaseUpdateStateArcheryShoot(void);
	void				phaseLeaveStateArcheryShoot(void);
	void				phaseEnterStateArcheryEnd(void);
	void				phaseUpdateStateArcheryEnd(void);
	void				phaseLeaveStateArcheryEnd(void);
	void				phaseEnterStateAttackSmashStart(void);
	void				phaseUpdateStateAttackSmashStart(void);
	void				phaseLeaveStateAttackSmashStart(void);
	void				phaseEnterStateAttackSmashDoing(void);
	void				phaseUpdateStateAttackSmashDoing(void);
	void				phaseLeaveStateAttackSmashDoing(void);
	void				phaseEnterStateAttackSmashEnd(void);
	void				phaseUpdateStateAttackSmashEnd(void);
	void				phaseLeaveStateAttackSmashEnd(void);
	void				phaseEnterStateLookAround(void);
	void				phaseUpdateStateLookAround(void);
	void				phaseLeaveStateLookAround(void);
	void				phaseEnterStateAttackRear(void);
	void				phaseUpdateStateAttackRear(void);
	void				phaseLeaveStateAttackRear(void);
	void				phaseEnterStateAttackRollingKick(void);
	void				phaseUpdateStateAttackRollingKick(void);
	void				phaseLeaveStateAttackRollingKick(void);

	void				phaseEnterStateGuardStagger(void);
	void				phaseUpdateStateGuardStagger(void);
	void				phaseLeaveStateGuardStagger(void);
	void				phaseEnterStatePiyoPiyo(void);
	void				phaseUpdateStatePiyoPiyo(void);
	void				phaseLeaveStatePiyoPiyo(void);
	void				phaseEnterStateGutsPose(void);
	void				phaseUpdateStateGutsPose(void);
	void				phaseLeaveStateGutsPose(void);
	void				phaseEnterStateMagicThunder(void);
	void				phaseUpdateStateMagicThunder(void);
	void				phaseLeaveStateMagicThunder(void);
	void				phaseEnterStateMagicFlame(void);
	void				phaseUpdateStateMagicFlame(void);
	void				phaseLeaveStateMagicFlame(void);
	void				phaseEnterStateMagicTornade(void);
	void				phaseUpdateStateMagicTornade(void);
	void				phaseLeaveStateMagicTornade(void);
	void				phaseEnterStatePlayOcarina(void);
	void				phaseUpdateStatePlayOcarina(void);
	void				phaseLeaveStatePlayOcarina(void);
	void				phaseEnterStateCopyRod(void);
	void				phaseUpdateStateCopyRod(void);
	void				phaseLeaveStateCopyRod(void);
	void				phaseEnterStateAfterEvent(void);
	void				phaseUpdateStateAfterEvent(void);
	void				phaseLeaveStateAfterEvent(void);

	/* �K�[�f�B�A���̏ꍇ�̃X�e�[�^�X */
	void				phaseUpdateStateIdleCaseGuardian(void);
	void				phaseEnterStateMoveCaseGuardian(void);
	void				phaseUpdateStateMoveCaseGuardian(void);
	void				phaseEnterStateAttack1CaseGuardian(void);
	void				phaseUpdateStateAttack1CaseGuardian(void);

	void				subweaponFuncNone(void);
	void				subweaponFuncBoomerang(void);

	/*�ϐ�*/
	static PHASE_STATE mPhaseEnterState[eCHARA_STATE_MAX];
	static PHASE_STATE mPhaseState[eCHARA_STATE_MAX];
	static PHASE_STATE mPhaseLeaveState[eCHARA_STATE_MAX];

	PHASE_STATE			mpSubweaponFunc;

	bool				mIsPermitInput;
	eRequestCamera		mRequestCamera;

	float				mMaxRunSpeed;

	bool				mIsAway;
	bool				mIsSubAttack;			// �T�u�E�F�|���U����
	bool				mIsJumpAttack;
	bool				mIsLeaveWeapon;			// ���������t���O

	ModelCollision*		mpLockonChara;			// ���b�N�I�����Ă���L����(�񃍃b�N�I������NULL)
	tItemWork			mItemWork;
	BladeEffect			mSubweaponBladeEffect;
	VECTOR				mCameraGeneralVec;
	ObjectHand*			mpObjectHand;

	union {
		EFK_HANDLE		uAngelWing;
		EFK_HANDLE		uMagic;
	} mEfkHnd;

	eWingJumpState		mWingJumpState;
	BYTE				mActionLimit;

	BYTE				mCosplayIndex;
	BOOL				mJumpBugCount;

	SHORT				mPiyoPiyoValue;

	// �ėp�J�E���^
	union {
		int				uSleep;
		int				uGutsPose;
		int				uMagic;
	} mCount;

	BYTE				mMagicPoint;

	const tFootstepRelateTableInfo* mpFootstepRelateInfo;

	ObjectBase*			mpCheckPointPtr;
	BOOL				mIsSoakWater;			// ���ɐZ�����Ă��邩�̃t���O
	eItemKind			mUsingRecoverItemKind;	// �g�p���񕜃A�C�e���t���O


}; /* class CharaPlayer */

void DrawLinoFunc(int model_handle);
void DrawKitsuneFunc(int model_handle);
void DrawEldaFunc(int model_handle);
void DrawUsamiFunc(int model_handle);
void DrawDemonFunc(int model_handle);
void DrawSentoFunc(int model_handle);
void DrawRafiaFunc(int model_handle);

#endif // __CHARA_PLAYER_H__

/**** end of file ****/

