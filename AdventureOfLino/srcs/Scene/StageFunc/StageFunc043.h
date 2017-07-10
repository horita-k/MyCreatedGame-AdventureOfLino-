/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_043_H__
#define __STAGE_FUNC_043_H__

#include "StageFuncBase.h"

/*=====================================*
  class
 *=====================================*/
class StageFunc043 : public StageFuncBase {
public:
	/* �萔 */

	enum {
		eSTAGE_STATE_SHOW_WALL = 0,		// �Ǖ\��
		eSTAGE_STATE_FADEOUT_WALL,		// �ǃt�F�[�h�A�E�g
		eSTAGE_STATE_HIDE_WALL,			// �ǔ�\��
		eSTAGE_STATE_FADEIN_WALL,		// �ǃt�F�[�h�C��

		eSTAGE_STATE_MAX,
	};


	/* �֐� */
	StageFunc043();
	~StageFunc043();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);
	void		Update(int counter);
	void		SetPatternLight(int pattern);
	void		SetPatternObject(int pattern);

	void		StartFadeInWall(void);
	void		StartFadeOutWall(void);

private:

	void		destroy(void);
	void		updateStageState(void);


	/* �ϐ� */
public:
	int		mEffHndCrystal;
	int		mEffHndAura;
	float	mTransH;
	float	mTransV;

	BYTE	mStageState;
	float	mFadeWallAlpha;


};

#endif // __STAGE_FUNC_043_H__

/**** end of file ****/


