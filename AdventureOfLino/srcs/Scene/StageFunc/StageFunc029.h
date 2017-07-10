/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_029_H__
#define __STAGE_FUNC_029_H__

#include "StageFuncBase.h"

/*=====================================*
  class
 *=====================================*/
class StageFunc029 : public StageFuncBase {
public:
	/* �萔 */

	/* �֐� */
	StageFunc029();
	~StageFunc029();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);
	void		Update(int counter);
	void		SetPatternLight(int pattern){}
	void		SetPatternObject(int pattern){}
	void		NotifyMovePlaceWhileFadeout(void);

	void		ProcessShowMapBefore(void);
	void		ProcessShowMapAfter(void);

private:

	void		destroy(void);

	/* �ϐ� */
public:
};

#endif // __STAGE_FUNC_029_H__

/**** end of file ****/


