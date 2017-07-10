/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * EventManager.cpp
 * �^�X�N(�C�x���g�Ǘ�)�N���X
 * @author SPATZ.
 * @data   2012/04/04 14:48:17
 */
//---------------------------------------------------
#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include "dxlib.h"

//#include "AppLib/Graphic/CharaBase.h"
#include "../Charactor/CharaNPC.h"
#include "AppLib/Resource/CsvAccesser.h"

/*=====================================*
 * define
 *=====================================*/
#define kEVTMGR_ANIM_TYPE_FADE_IN		(0x01)	// �t�F�[�h�C���̃A�j���V���[��
#define kEVTMGR_ANIM_TYPE_FADE_OUT		(0x02)	// �t�F�[�h�A�E�g�̃A�j���V���[��
#define kEVTMGR_ANIM_TYPE_ROTATION		(0x04)	// ��]�̃A�j���V���[��
#define kEVTMGR_ANIM_TYPE_SCALE			(0x08)	// �X�P�[���̃A�j���V���[��

#define EVENT_MANAGER_AFTER_EFFECT_DRAW	(0xFFFFFFFF)

#define kEVTMGR_LAYOUT_NUM				(4)		// EventManager �Ŏg���郌�C�A�E�g�̐�

/*=====================================*
 * struct
 *=====================================*/

/**** �w�b�_ ****/
struct Header
{
	char header[30];	// "Vocaloid Motion Data 0002\0" 30byte // (MMDver2�ȑO��vmd��"Vocaloid Motion Data file\0")
	char modelName[20]; // ���f���� 20byte(MMDver2�ȑO��vmd��10byte) // �I�[ 0x00,�p�f�B���O 0xFD, �����R�[�h�F�V�t�gJIS�i�{�[������\������l�j// ���e���J�����C�Ɩ��̏ꍇ��"�J�����E�Ɩ�\0on Data"�ƂȂ�
};

/**** �J�����L�[�t���[���v�f�f�[�^(61Bytes/�v�f) ****/
struct CameraFrame
{
	DWORD frame;		// �t���[���ԍ�
	float distance;		// �ڕW�_�ƃJ�����̋���(�ڕW�_���J�����O�ʂŃ}�C�i�X)
	float x;			// �ڕW�_��X���ʒu
	float y;			// �ڕW�_��Y���ʒu
	float z;			// �ڕW�_��Z���ʒu
	float rx;			// �J������x����](rad)(MMD���l���͂̃}�C�i�X�l)
	float ry;			// �J������y����](rad)
	float rz;			// �J������z����](rad)
	char bezier[24];	// ��ԃp�����[�^
	DWORD viewAngle;	// ����p(deg)
	char parth;			// �p�[�X�y�N�e�B�u, 0:ON, 1:OFF
};

/*=====================================*
 * class
 *=====================================*/
class EventManager 
{
 public:

	/*=====================================*
	 * struct
	 *=====================================*/
	/**** ���f���A�j���[�V�����\���� ****/
	struct tModelAnimInfo 
	{
		BYTE	mAnimState;
		float	mStartFrame;
		float	mEndFrame;
		float	mRotationSpeed;
		float	mDefaultScale;
	};

	/**** ���C�A�E�g�A�j���[�V�����̊g�k��� ****/
	struct tLayoutAnimScaleInfo
	{
		int		mBaseWidth;
		int		mBaseHeight;
		int		mTargetWidth;
		int		mTargetHeight;
	};

	/**** ���C�A�E�g�A�j���[�V�����\���� ****/
	struct tLayoutAnimInfo
	{
		// ���ʏ��
		BYTE					mAnimState;
		float					mStartFrame;
		float					mEndFrame;
		// �e�A�j���[�V�������
		tLayoutAnimScaleInfo	mLytAnmScaleInfo;
	};

	static EventManager* const GetInstance(void);

	/* �֐� */
	void		Setup(void);
	void		LoadCameraFile(const char* fileName, float scaleCamera=10.0f);
	void		StartEvent(CsvAccesser* pCsvAccesser=NULL, 
						   const VECTOR* pShiftPos=NULL,		// �V�t�g���W�ݒ�(NULL�̏ꍇ�̓I�t�Z�b�g�Ȃ�)
						   bool isVisibleNpc=false,				// NPC�\���t���O
						   bool isVisibleObject=false);			// �A�C�e���\���t���O
						   
	void		SeekEvent(float seekFrame);
	void		Update(void);
	void		EndEvent(void);
	bool		IsEndCamera(void)		{ return mIsEndCamera; }
	bool		IsStartEvent(void)		{ return mIsStartEvent; }
	void		PauseEvent(void)		{ mIsPauseEvent = TRUE; }

	void				EntryModelNum(int num);
	int					GetModelNum(void)			{ return mModelNum; }
	void				SetModelParameter(int modelIdx, const char* strName, int motionIdx, ModelBase* pInsertBeforeModel=NULL, bool isMotionLoop=false);
	CharaNpc* const		GetModelPmd(int modelIdx); // �����ăL���X�g���ēn���Ă܂�

	LayoutBase*			GetLayoutBase(int layoutIdx);
	LayoutBase*			GetSystemLayoutBase(int layoutIdx);

	void				EnableOffsetCamera(float setPosX, float offsetPosY, float setPosZ, float setRot);
	
	// ���Ԏ擾�֐�
	bool				EqualPhaseFrame(float frame);
	bool				BetweenPhaseFrame(float secMin, float secMax);
	
	float				GetPhaseFrame(void)						{ return mFrameCounter; }
	void				SetPhaseFrame(float frame)				{ mFrameCounter = frame; }
	void				GetShiftCameraPosition(VECTOR* pShiftPos)	{ *pShiftPos = mShifCamPos; }

	DWORD				GetNextKey(void)						{ return mNextKey; }
	void				SetNextKey(DWORD nextKey)				{ mNextKey = nextKey; }

	// ���̊֐��͑��̊֐�����Ă΂�鎖�����҂��Ă���֐�
	void				UpdateEventAnimation(void);
	void				SetModelAnimInfoWork(int modelIdx, tModelAnimInfo* pAnimation);
	void				SetLayoutAnimInfoWork(int layoutlIdx, tLayoutAnimInfo* pAnimation);

 private:
	EventManager();
	~EventManager();

	void		getFrameCamPosTar(CameraFrame* pFrame, VECTOR* pPos, VECTOR* pTar);

	/* �ϐ� */
	float		mFrameCounter;
	bool		mIsStartEvent;
	BOOL		mIsPauseEvent;

	char*		mpCameraFrame;
	DWORD		mCameraKeyNum;
	DWORD		mNextKey;
	int			mFileHandle;
	bool		mIsEndCamera;

	bool		mIsOffsetCamera;
	float		mSetCamPosX;
	float		mOffsetCamPosY;
	float		mSetCamPosZ;
	float		mOffsetCamRot;
	float		mCameraScale;

	/* ���f���֘A */
	int			mModelNum;
	CharaNpc*	mpCharaBase;

	/* ���C�A�E�g�֘A */
	LayoutBase*	mpAssignLayout;
	LayoutBase* mpAssignSystemLayout;

	tModelAnimInfo*		mpModelAnimWork;
	tLayoutAnimInfo*	mpLayoutAnimWork;

	int*		mpIndex;

	// �V�t�g�J�����|�W�V���� (Offset�Ƃ͈Ⴄ)
	VECTOR		mShifCamPos;
	CsvAccesser* mpCsvAccesser;

};

#endif //__EVENT_MANAGER_H__

 /**** end of file ****/
