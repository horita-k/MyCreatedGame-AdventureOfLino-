/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_016_H__
#define __STAGE_FUNC_016_H__

#include <vector>
#include "StageFuncBase.h"
#include "Object/ObjectSwitch.h"

/*=====================================*
  class
 *=====================================*/
class StageFunc016 : public StageFuncBase {
public:
	/* �萔 */

	/* �֐� */
	StageFunc016();
	~StageFunc016();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);

	void		Update(int counter);

	void		SetPatternLight(int pattern);
	void		SetPatternObject(int pattern);

	void		StartGateTimer(WORD timerCnt);

private:
	void		destroy(void);

	/* �ϐ� */
	WORD			mGateTimerCnt;
	WORD			mGateTimerMax;

	//	ObjectSwitch*	mpGateObject;
	std::vector<ObjectSwitch *>	mListGateObject;

};

#endif // __STAGE_FUNC_016_H__

/**** end of file ****/


