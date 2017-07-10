/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_028_H__
#define __STAGE_FUNC_028_H__

#include "StageFuncBase.h"

/*=====================================*
  class
 *=====================================*/
class StageFunc028 : public StageFuncBase {
public:
	/* �萔 */

	/* �֐� */
	StageFunc028();
	~StageFunc028();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);
	void		Update(int counter);
	void		SetPatternLight(int pattern){}
	void		SetPatternObject(int pattern){}

private:

	void		destroy(void);

	/* �ϐ� */
public:
	int		mEffHndCrystal;
	int		mEffHndAura;
};

#endif // __STAGE_FUNC_028_H__

/**** end of file ****/


