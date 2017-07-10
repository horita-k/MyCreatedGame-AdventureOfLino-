/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#ifndef __STAGE_FUNC_COMMON_H__
#define __STAGE_FUNC_COMMON_H__

#include "StageFuncBase.h"

#ifdef _DEBUG
#include "Charactor/CharaGuardian.h"
#endif // _DEBUG

/*=====================================*
  class
 *=====================================*/
class StageFuncCommon : public StageFuncBase {
public:
	/* �萔 */

	/* �֐� */
	StageFuncCommon();
	~StageFuncCommon();

	void		SetupBeforeLoad(void);
	void		SetupAfterLoad(int& rPhase);

	void		Update(int counter);

	void		SetPatternLight(int pattern);
	void		SetPatternObject(int pattern);
	void		DestroyBuf(void);

private:

	void		destroy(void);

	/* �ϐ� */
#ifdef _DEBUG
	CharaGuardian mGuardian;
#endif // _DEBUG
};

#endif // __STAGE_FUNC_COMMON_H__

/**** end of file ****/


