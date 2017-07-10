/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �M�~�b�N�x�[�X
 * @author SPATZ.
 * @data   2015/01/09 00:15:30
 */
//---------------------------------------------------
#ifndef __GIMMICK_BASE_H__
#define __GIMMICK_BASE_H__

#include "AppLib/Graphic/ModelCollision.h"
#include "GimmickBase.h"
//#include "AppLib/Resource/CsvAccesser.h"

/*=====================================*
 * class
 *=====================================*/

/* �M�~�b�N��ʒ�` */
enum eGimmickKind {
	eGIMMICK_KIND_LIFT = 0,
	eGIMMICK_KIND_SHIP,
	eGIMMICK_KIND_ROLLMOGU,
	eGIMMICK_KIND_KEY,
	eGIMMICK_KIND_TROLLEY,
	eGIMMICK_KIND_ELEVATOR,
	eGIMMICK_KIND_TOWER,
	eGIMMICK_KIND_ELEVATOR_B,
	eGIMMICK_KIND_BABEL_SWITCH,				// �o�x���̓��̃X�C�b�`
	eGIMMICK_KIND_HOLE,						// ���e��
	eGIMMICK_KIND_WHEEL,					// ����
	eGIMMICK_KIND_EYE,						// �����
	eGIMMICK_KIND_CANNON,					// ��C
	eGIMMICK_KIND_ELEVATOR_C,
	eGIMMICK_KIND_LEAN_FLOOR,				// �X����
	eGIMMICK_KIND_AIRSHIP,					// ��s�D
	eGIMMICK_KIND_LIFT_AUTO,				// �����Ń��[�v�ړ����郊�t�g
	eGIMMICK_KIND_CLOCK,					// �U��q���v
	eGIMMICK_KIND_SWITCH,					// �ėp�X�C�b�`
	eGIMMICK_KIND_ELEVATOR_MOGU,			// ���O�[���G���x�[�^
	eGIMMICK_KIND_FOOT_SWITCH,				// ���ރX�C�b�`
	eGIMMICK_KIND_TETO,						// �哮��
	eGIMMICK_KIND_TROLLEY_EX,				// �g���b�R����
	eGIMMICK_KIND_PLIGHT,					// �|�C���g���C�g
	eGIMMICK_KIND_BARRIER,					// �o���A
	eGIMMICK_KIND_FLOAT_BLOCK,				// �����u���b�N
	eGIMMICK_KIND_MAGIC_CUBE,				// ���@�̃L���[�u
	eGIMMICK_KIND_PRESS_WALL,				// �����Ԃ���
	eGIMMICK_KIND_LEAN_PASSAGE,				// �X���ʘH
	eGIMMICK_KIND_ROLLING_PIPE,				// ��]���铛
	eGIMMICK_KIND_SCREW,					// �v���y��
	eGIMMICK_KIND_LASER_BEAM,				// ���[�U�[�r�[��
	eGIMMICK_KIND_PRESS_WALL_HARD,			// �����Ԃ���(���Ȃ��^�C�v)
	eGIMMICK_KIND_BOAT,

	eGIMMICK_KIND_MAX,
};

class GimmickBase : public ModelCollision {
 public:
	struct tOnNodeInfo {
		ModelCollision*		mpOnModel;
		VECTOR				mAddVec;
		VECTOR				mPrevRot;
	};

	static const BYTE	kON_NODE_MODEL_NUM = 8;
	static const char*	strCsvRootPath;
	static VECTOR		sRevisePos;
	static BOOL			sPauseAttach;

	enum eCsvLine {
		eCSVLINE_NONE = 0,

		eCSVLINE_ORG_POS_X,
		eCSVLINE_ORG_POS_Y,
		eCSVLINE_ORG_POS_Z,
		eCSVLINE_ORG_ROT_X,
		eCSVLINE_ORG_ROT_Y,
		eCSVLINE_ORG_ROT_Z,
		eCSVLINE_ORG_SCA_X,
		eCSVLINE_ORG_SCA_Y,
		eCSVLINE_ORG_SCA_Z,
		eCSVLINE_RESERVE_9,

		eCSVLINE_MAX,
	};

	// �M�~�b�N�̏�ɏ���Ă��邩�̔��ʂ̎w��
	enum eGimmickNodeType {
		eGMK_NODE_TYPE_PROGRAM = 0,
		eGMK_NODE_TYPE_DATA,

		eGMK_NODE_TYPE_MAX,
	};

	/* �M�~�b�N�p�����[�^�\���� */
	struct tGimmickBaseParam {
		const char*			mResourceName;
		const char*			mCsvKeyword;
	};

	/* �֐� */
	GimmickBase();
	~GimmickBase();

	// �M�~�b�N�֘A
	static bool			LoadAllGimmick(int stageIndex);
	static void			DeleteAllGimmick(void);
	static void			SetupAllGimmick(void);

	static int			GetGimmickBaseNum(void);
	static GimmickBase*	GetGimmickBase(int gimmickIndex);
	static GimmickBase*	GetGimmickBaseFromEarthCollIndex(INT8 onCollisionIndex);
	static GimmickBase*	GetGimmickBaseFromWallCollIndex(INT8 onCollisionIndex);
	static void			ReloadInformationAll(int stageNo);
	static void			ReloadInformationAllFallDown(int stageNo);

	static void			ReviseAllGimmickOnPosition(VECTOR& rMoveVec);

	static CHAR			GetEmptyOnNodeListIndex(void);
	static tOnNodeInfo*	GetOnNodeInfo(CHAR nodeIndex);
	static GimmickBase*	Search1stFindGimmickKind(eGimmickKind kind);
	static void			SetGimmickPauseAttach(BOOL isAttach);
	static void			SetVisibleAll(bool isVisible);
	
	void				DeleteModel(void);
	void				LoadParamCsv(int stageIndex, BYTE csvNo, BYTE extendBufNum, BYTE gimmickKind);
	void				ReloadInformation(int stageIndex);

	BYTE				GetExtendBufferNum(void)						{ return mExtendBufferNum; }
	float				GetGimmickExtParamValue(BYTE extParamIndex);
	void				SetGimmickExtParamValue(BYTE extParamIndex, float paramValue);
	eGimmickKind		GetKind(void)									{ return (eGimmickKind)mGimmickKind; }
	BYTE				GetState(void)									{ return mState; }
	void				SetState(BYTE state)							{ mState = state; }
	void				SetNodeType(eGimmickNodeType nodeType)			{ mGimmickNodeType = nodeType; }
	bool				CheckEnableAttack(void)							{ return true; }

	void				Setup(void);
	void				GetBodyCapsule(ShapeCapsule* pCapsule);
	void				SetNowGravity(float gravity);

	virtual void		PreDraw(void);
	virtual void		PreUpdate(void);
	virtual void		Update(void);

	void				SetCharaPositionToFrameMatrix(ModelCollision* pModelCollision, int frameIndex);

	virtual void		ReviseOnPosition(VECTOR& rMoveVec);
	virtual int			GetWakeupEventValue(void);
	void				GetGimmickMoveVec(VECTOR& rMoveVec);
	void				ResetOnNodeInfo(ModelCollision* pOnModel);

	void				getOriginalPosition(VECTOR* pPos);
	void				getOriginalRotation(VECTOR* pRot);
	void				getOriginalScale(VECTOR* pScale);

protected:
	void				searchNearesetNode(VECTOR* pPosition, int frameIndex);
	void				commonSetup(void);
	virtual	void		extendSetup(void)=0;
	virtual	void		extendUpdate(void)=0;
	bool				isOnNodeModel(void);

	int					getSoundVolume(float maxHearingDist);

	/*�ϐ�*/
protected:

	BYTE				mGimmickKind;			// �M�~�b�N��� (eGimmickKind �ɃL���X�g���Ďg�p)

	VECTOR				mOnPosition;
	char				mOnNodeName[10];

	// �L���ȃm�[�h�����i�[����r�b�g
	BYTE				mOnNodeInfoHandleBit;

	BYTE				mGimmickNodeType;
	BYTE				mState;
	WORD				mCounter;
	BYTE				mCsvNo;
	CHAR				mWakeupEvent;			// �M�~�b�N�N���C�x���g
	BYTE				mExtendBufferNum;
	float*				mpExtendBuffer;			// �g���o�b�t�@


};


#endif // __GIMMICK_BASE_H__

 /**** end of file ****/

