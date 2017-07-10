/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_048_H__
#define __STAGE_FUNC_048_H__

#include "StageFuncBase.h"

/*=====================================*
  class
 *=====================================*/
class StageFunc048 : public StageFuncBase {
public:
	/* �萔 */

	/* �֐� */
	StageFunc048();
	~StageFunc048();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);
	void		Update(int counter);
	void		SetPatternLight(int pattern);
	void		SetPatternObject(int pattern){}

private:

	void		destroy(void);

	/* �ϐ� */
public:
	//	int		mEffHndTornade;					// 
	int		mEffHndSky;
	int		mEffHndGravityAura;

	//	int		mEffHndCrystal;
	//	int		mEffHndAura;
	
	/*
	DWORD	mDistPlayerToTornadeRate;
	float	mTornadeShapeRate;
	bool	mIsChangeShapeTornade;			// �����̌`��ω��t���O
	WORD	mTornadeBattleCount;			// ������̃J�E���^
	*/
	
	BYTE	mSlapCntPlayer;					// �r���^��
	BYTE	mSlapCntEnemy;					// �r���^��
	CHAR	mRafiaTalkIndex;				// ���t�B�A�g�[�N�ԍ�
	BOOL	mIsSlapBattleWin;				// �r���^��E�����t���O
};

#endif // __STAGE_FUNC_048_H__

/**** end of file ****/


