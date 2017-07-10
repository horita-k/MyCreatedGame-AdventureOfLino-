/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * アイテムメニュー選択
 * @author SPATZ.
 * @data   2015/07/13 23:38:22
 */
//---------------------------------------------------
#ifndef __ITEM_MENU_SELECT_H__
#define __ITEM_MENU_SELECT_H__

#include "AppLib/Graphic/ModelBase.h"
#include "SceneBase.h"
#include "SceneInfo.h"
#include "AppLib/Graphic/LayoutBase.h"
#include "AppLib/Graphic/ModelMap.h"
#include "AppLib/Graphic/LayoutNumber.h"

/*=====================================*
  class
 *=====================================*/
class SubSceneItemMenu {
public:

	static const BYTE kPLATE_LAYOUT_NUM = 3;
	/* 定数 */
	enum {
		eSTATE_NONE = 0,

		eSTATE_ITEM_ENTER,
		eSTATE_ITEM_DOING,
		eSTATE_ITEM_LEVE,

		eSTATE_COSPLAY_ENTER,
		eSTATE_COSPLAY_DOING,
		eSTATE_COSPLAY_LEVE,
		
		eSTATE_KEY_ENTER,
		eSTATE_KEY_DOING,
		eSTATE_KEY_LEVE,

		eSTATE_FINISH,

		eSTATE_MAX,
	};

	enum {
		eMENU_KIND_ITEM = 0,
		eMENU_KIND_COSPLAY,
		eMENU_KIND_KEY,

		eMENU_KIND_MAX,
	};

	/* 関数 */
	SubSceneItemMenu();
	~SubSceneItemMenu();

	void		Create(BYTE defMenuKind, int lyt_num);
	bool		Update(void);
	void		Destroy(BYTE& rDefMenuKind);
	eItemKind	GetSelectWeapon(void)			{ return mSelectWeapon; }

private:

	/*=====================================*
	 * phase
	 *=====================================*/
	typedef void (SubSceneItemMenu::*STATE_FUNC)(void);
	static STATE_FUNC mStateFunc[eSTATE_MAX];

	void		stateNone(void);
	void		stateItemEnter(void);
	void		stateItemDoing(void);
	void		stateItemLeave(void);
	void		stateCosplayEnter(void);
	void		stateCosplayDoing(void);
	void		stateCosplayLeave(void);
	void		stateKeyEnter(void);
	void		stateKeyDoing(void);
	void		stateKeyLeave(void);

	void		stateFinish(void);

	void		updateSelect(void);

	
	/* 変数 */
	int			mRingLytNum;

	BYTE		mState;
	BYTE		mItemNum;
	int			mNowItemIndex;
	int			mNextItemIndex;
	int			mRotCnt;
	bool		mIsRight;
	float		mIconDist;

	BYTE		mMenuKind;
	BOOL		mIsAlertMessage;

	// アイテム選択時のアイテム数レイアウト
	LayoutNumber*	mpSelectItemNumLayoutList;

	LayoutBase	mInfoLayout;
	LayoutBase  mPlateLayout[kPLATE_LAYOUT_NUM];
	LayoutBase	mSquareLayout;
	LayoutBase	mMsgLayout;
	eItemKind	mSelectWeapon;

};

#endif // __ITEM_MENU_SELECT_H__

/**** end of file ****/


