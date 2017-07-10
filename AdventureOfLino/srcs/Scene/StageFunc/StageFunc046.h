/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_046_H__
#define __STAGE_FUNC_046_H__

#include "StageFuncBase.h"

/*=====================================*
  class
 *=====================================*/
class StageFunc046 : public StageFuncBase {
public:
	/* �萔 */
	enum eWaterType {
		eWATER_TYPE_UP,
		eWATER_TYPE_DOWN_GOING,
		eWATER_TYPE_DOWN,
		eWATER_TYPE_UP_GOING,

		eWATER_TYPE_MAX,
	};

	/* �֐� */
	StageFunc046();
	~StageFunc046();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);
	void		Update(int counter);
	void		SetPatternLight(int pattern);
	void		SetPatternObject(int pattern);

	void		SetWaterType(eWaterType type);

private:
	void		destroy(void);

	/* �ϐ� */

	int			mWaterType;

public:

};

#endif // __STAGE_FUNC_046_H__

/**** end of file ****/


