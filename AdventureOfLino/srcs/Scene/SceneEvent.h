/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ���C�u�����̐F�X�Z�������e�X�g
 * @author SPATZ.
 * @data   2011/11/21 00:55:18
 */
//---------------------------------------------------
#ifndef __SCENE_EVENT_H__
#define __SCENE_EVENT_H__

#include "AppLib/Graphic/ModelBase.h"
#include "SceneBase.h"
#include "SceneInfo.h"
#include "AppLib/Graphic/LayoutBase.h"
#include "AppLib/Graphic/ModelMap.h"

/*=====================================*
  class
 *=====================================*/
class SceneEvent : public SceneBase {
public:
	/* �萔 */
	enum {
		ePHASE_LOAD_BEFORE,
		ePHASE_LOADING,
		ePHASE_SETUP,
		ePHASE_EVENT_BEFORE,
		ePHASE_EVENT,

		ePHASE_MAX,
	};

	/* �֐� */
	SceneEvent();
	~SceneEvent();

	/*=====================================*
	 * useful method
	 *=====================================*/
	void		Create(int debugArg);
	void		Update(void);

	int			GetLoadingPercent(void);

private:

	void		destroy(void);

	/*=====================================*
	 * phase
	 *=====================================*/
	typedef void (SceneEvent::*PHASE_FUNC)(void);
	static PHASE_FUNC mPhaseFunc[ePHASE_MAX];

	void		phaseLoadBefore(void);
	void		phaseLoading(void);
	void		phaseSetup(void);
	void		phaseEventBefore(void);
	void		phaseEvent(void);
	
	/* �ϐ� */
	LayoutBase	mLayoutBack;
	LayoutBase	mLayoutMain;
	LayoutBase	mLayoutPressAnyKey;
	LayoutBase	mLayoutSpatz;
	ModelMap	mModelMap;

};

#endif // __SCENE_EVENT_H__

/**** end of file ****/


