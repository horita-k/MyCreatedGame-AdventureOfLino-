/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_047_H__
#define __STAGE_FUNC_047_H__

#include "StageFuncBase.h"
#include "AppLib\Resource\CsvAccesser.h"

/*=====================================*
  class
 *=====================================*/
class StageFunc047 : public StageFuncBase {
public:
	/* �萔 */

	/* �֐� */
	StageFunc047();
	~StageFunc047();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);
	void		Update(int counter);
	void		SetPatternLight(int pattern){}
	void		SetPatternObject(int pattern){}
	void		DestroyBuf(void);

private:

	void		destroy(void);

	/* �ϐ� */
public:
	/*
	USHORT	mGroupCount;
	SHORT	mScrollX;
	SHORT	mScrollY;
	BYTE	mIsReverse;
	int		mRollStringColor;
	*/
	LayoutBase*		mpTitleLog;
	/*
	BOOL	mIsEndScroll;
	*/

	/* �X�^�b�t���[���֘A */
	struct tStaffRollWork {
		CsvAccesser*	mpCsv;					// CSV�t�@�C��
		BYTE			mCurCsvFileIndex;		// ���ݓǂݍ���ł���csv�t�@�C���C���f�b�N�X
		BOOL			mIsEndReadCsv;			// CSV�t�@�C���ǂݍ��݊����t���O

		short			mCurCsvRowIndex;		// ���ݓǂݍ��ݒ��̍s��
		short			mTextPosX;				// �e�L�X�g�ʒuX
		short			mShiftTextPosY;			// �V�t�g�̃e�L�X�g�ʒuY
		int				mTextColor;				// �e�L�X�g�F

	} mStaffroll;

	int		mEffHndSky;
};

#endif // __STAGE_FUNC_047_H__

/**** end of file ****/


