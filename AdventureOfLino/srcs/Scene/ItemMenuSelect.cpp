/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * アイテムメニュー選択
 * @author SPATZ.
 * @data   2015/07/13 23:38:22
 */
//---------------------------------------------------
#include "SceneManager.h"
#include "SceneMergeTest.h"
#include "SubSceneItemMenu.h"
#include "CommandFunctions.h"

#include "AppLib/Basic/Basic.h"
#include "AppLib/Basic/Counter.h"
#include "AppLib/Input/InputManager.h"

#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Sound/SoundManager.h"
#include "AppLib/Resource/ResourceManager.h"
#include "AppLib/Resource/ResourceLayout.h"

#include "DataDefine/VoiceDef.h"

#include "ExternDefine.h"		//<! Extern 宣言は全てここに集約する

#include "dxlib.h"
#include "Utility/Utility.h"

/*=====================================*
 * フェーズ関数
 *=====================================*/
/* static */SubSceneItemMenu::STATE_FUNC
SubSceneItemMenu::mStateFunc[SubSceneItemMenu::eSTATE_MAX] = {
	&SubSceneItemMenu::stateNone,
	&SubSceneItemMenu::stateItemEnter,
	&SubSceneItemMenu::stateItemDoing,
	&SubSceneItemMenu::stateItemLeave,
	&SubSceneItemMenu::stateCosplayEnter,
	&SubSceneItemMenu::stateCosplayDoing,
	&SubSceneItemMenu::stateCosplayLeave,
	&SubSceneItemMenu::stateKeyEnter,
	&SubSceneItemMenu::stateKeyDoing,
	&SubSceneItemMenu::stateKeyLeave,
};

static Vec2 center = Vec2(320, 180);

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
SubSceneItemMenu::SubSceneItemMenu()
{
	mRingLytNum		= 0;

	mState			= eSTATE_NONE;
	mItemNum		= 0;

	mNowItemIndex	= 0;
	mNextItemIndex	= 0;
	mRotCnt			= 0;
	mIsRight		= true;
	mIconDist		= 0;

	mpSelectItemNumLayoutList = NULL;

	mMenuKind		= eMENU_KIND_ITEM;
	mIsAlertMessage	= FALSE;
	mSelectWeapon	= (eItemKind)0;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
SubSceneItemMenu::~SubSceneItemMenu()
{

}

//---------------------------------------------------
/**
   作成
 */
//---------------------------------------------------
void
SubSceneItemMenu::Create(BYTE defMenuKind, int lyt_num)
{
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_COSPLAY_LINO);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_COSPLAY_PRONAMA);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_COSPLAY_DABAH);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_COSPLAY_LAPAN);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_COSPLAY_MIKU);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_COSPLAY_FRES);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_COSPLAY_TDA_MIKU);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_CARD_K);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_CARD_Q);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_CARD_J);

	mMenuKind	= defMenuKind;
	mItemNum	= lyt_num;

	center.x = 320.0f / GraphicManager::GetInstance()->GetAspectRate();
	center.y = 180.0f;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
SubSceneItemMenu::Update(void)
{
	(this->*mStateFunc[mState])();

	if (mState == eSTATE_FINISH) {
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::Destroy(BYTE& rDefMenuKind)
{
	// 呼びW出し側にメニュー種別を更新させる
	rDefMenuKind = mMenuKind;

	InputManager::GetInstance()->ResetNowJoyState();

	GraphicManager::GetInstance()->RemoveSystemLayout(&mInfoLayout);
	GraphicManager::GetInstance()->RemoveSystemLayout(&mSquareLayout);
	GraphicManager::GetInstance()->RemoveSystemLayout(&mMsgLayout);

	for (int iI = 0; iI < kPLATE_LAYOUT_NUM; iI ++) {
		GraphicManager::GetInstance()->RemoveSystemLayout(&mPlateLayout[iI]);
	}

	for (int iI = 0; iI < mRingLytNum; iI ++) {
		GraphicManager::GetInstance()->RemoveSystemLayout(&spShopItemLayout[iI]);
	}
	if (spShopItemLayout != NULL) {
		delete[] spShopItemLayout;
		spShopItemLayout = NULL;
	}

	for (int iI = 0; iI < 2; iI ++) {
		GraphicManager::GetInstance()->RemoveSystemLayout(&mpSelectItemNumLayoutList[iI]);
	}
	if (mpSelectItemNumLayoutList != NULL) {
		delete[] mpSelectItemNumLayoutList;
		mpSelectItemNumLayoutList = NULL;
	}
		
	//	MSG_FADE_OUT(0.3f);
	mMsgLayout.StartFade(0, 0.3f);

	// キャラクターのアップデートを再開する
	GraphicManager::GetInstance()->SetStopModeUpdate(false);

	FADE_BOARD()->StartFade(0, 0.25f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateNone(void)
{
	LayoutBase* p_layout	= NULL;

	sPlayer.PermitAcceptInput(false);

	FADE_BOARD()->StartFade(100, 0.25f);

	/*=====================================*
	 *
	 *=====================================*/
	mInfoLayout.LinkLayout(RES_LYT_INFO_ITEM);
	mInfoLayout.SetSize(192, 96);
	mInfoLayout.SetPositionByCentering(110, 40);
	mInfoLayout.SetAlpha(0);
	GraphicManager::GetInstance()->EntrySystemLayout(&mInfoLayout);

	mSquareLayout.LinkLayout(RES_LYT_INFO_SQUARE);
	mSquareLayout.SetSize(94, 94);
	mSquareLayout.SetPositionByCentering(-1, -1);
	mSquareLayout.SetAlpha(0);

	{
		LayoutBase* p_lyt_balloon;
		p_lyt_balloon = TotalInfo::GetInstance()->GetSystemLayout(TotalInfo::eLY_BALLOON);

		int x, y;
		mMsgLayout.LinkLayout(RES_LYT_BALLOON);
		p_lyt_balloon->GetSize(&x, &y);		
		mMsgLayout.SetSize(x, y);
		p_lyt_balloon->GetPositionByCentering(&x, &y);
		mMsgLayout.SetPositionByCentering(x, y);
		mMsgLayout.SetAlpha(0);
		GraphicManager::GetInstance()->EntrySystemLayout(&mMsgLayout,
														 GraphicManager::eSYS_LAYOUT_PRIORITY_FADE_AFTER);
	}

	/* 選択アイテム数レイアウト */
	if (mpSelectItemNumLayoutList == NULL) {
		mpSelectItemNumLayoutList = new LayoutNumber[2];
		APP_ASSERT((mpSelectItemNumLayoutList != NULL),
				   "mpSelectItemNumLayoutList is NULL \n");
	}
	for (int iI = 0; iI < 10; iI ++) {
		mpSelectItemNumLayoutList[0].LinkLayout(iI, TotalInfo::GetInstance()->GetLinkLayoutName(iI) );
		mpSelectItemNumLayoutList[1].LinkLayout(iI, TotalInfo::GetInstance()->GetLinkLayoutName(iI) );
	}
	mpSelectItemNumLayoutList[0].SetPositionByCentering(354, 104);
	mpSelectItemNumLayoutList[1].SetPositionByCentering(338, 104);
	mpSelectItemNumLayoutList[0].SetSize(20, 20);
	mpSelectItemNumLayoutList[1].SetSize(20, 20);
	mpSelectItemNumLayoutList[0].SetAlpha(0);
	mpSelectItemNumLayoutList[1].SetAlpha(0);
	if (GraphicManager::GetInstance()->CheckEntrySystemLayout(&mpSelectItemNumLayoutList[0]) == false) {
		GraphicManager::GetInstance()->EntrySystemLayout(&mpSelectItemNumLayoutList[0]);
	}
	if (GraphicManager::GetInstance()->CheckEntrySystemLayout(&mpSelectItemNumLayoutList[1]) == false) {
		GraphicManager::GetInstance()->EntrySystemLayout(&mpSelectItemNumLayoutList[1]);
	}

	/*=====================================*
	 *
	 *=====================================*/
	//	MSG_TYPE_TELOP();
	//	MSG_FADE_IN(0.3f);
	mMsgLayout.StartFade(186, 0.3f);

	mRotCnt = 0;
	mIconDist = 300.0f;
	SoundManager::GetSndComponent()->PlaySound(eSE_ITEM_APPEAR);

	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);

	// キャラクターのアップデートをとめる
	GraphicManager::GetInstance()->SetStopModeUpdate(true);
	
	switch (mMenuKind) {
	case eMENU_KIND_ITEM:
		mState = eSTATE_ITEM_ENTER;
		break;
	case eMENU_KIND_COSPLAY:
		mState = eSTATE_COSPLAY_ENTER;
		break;
	case eMENU_KIND_KEY:
		mState = eSTATE_KEY_ENTER;
		break;

	default:
		APP_ASSERT(NULL, "Invalid mMenuKind \n");
		break;
	};
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateItemEnter(void)
{
	LayoutBase* p_layout = NULL;

	for (int iI = 0; iI < mRingLytNum; iI ++) {
		p_layout = &spShopItemLayout[iI];
		GraphicManager::GetInstance()->RemoveSystemLayout(p_layout);
	}
	if (spShopItemLayout != NULL) {
		delete[] spShopItemLayout;
		spShopItemLayout = NULL;
	}

	mRingLytNum = mItemNum;

	spShopItemLayout = new LayoutBase[mRingLytNum];
	for (int iI = 0; iI < mRingLytNum; iI ++) {
		const tItemDetailInfo * p_item_detail = GetItemDetailInfo(sMyWeaponList.at(iI));
		p_layout = &spShopItemLayout[iI];
		p_layout->LinkLayout(p_item_detail->mLayoutName);
		p_layout->SetPositionByCentering(-100, -100);
		p_layout->SetSize(80, 80);
		p_layout->SetAlpha(255);
		GraphicManager::GetInstance()->EntrySystemLayout(p_layout, 
														 GraphicManager::eSYS_LAYOUT_PRIORITY_FADE_AFTER);
	}

	int search_index = 0;
	for (int iI = 0; iI < mItemNum; iI ++) {
		if (sMyWeaponList.at(iI) == sPlayer.ItemWork().mItemSubWeapon) {
			search_index = iI;
		}
	}
	mNextItemIndex = search_index;
	mNowItemIndex = ((mNextItemIndex + (mItemNum / 2)) % mItemNum);

	mInfoLayout.LinkLayout(RES_LYT_INFO_ITEM);
	mInfoLayout.SetAlpha(0);
	mInfoLayout.StartFade(255, 0.3f);

	mIconDist = 300.0f;

	mSquareLayout.SetAlpha(0);

	mState = eSTATE_ITEM_DOING;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateItemDoing(void)
{
	updateSelect();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateItemLeave(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateCosplayEnter(void)
{
	LayoutBase* p_layout	= NULL;

	for (int iI = 0; iI < mRingLytNum; iI ++) {
		p_layout = &spShopItemLayout[iI];
		GraphicManager::GetInstance()->RemoveSystemLayout(p_layout);
	}
	if (spShopItemLayout != NULL) {
		delete[] spShopItemLayout;
		spShopItemLayout = NULL;
	}

	BYTE cosplay_num = 0;
	cosplay_num = TotalInfo::GetInstance()->GetCosplayNum();

	mRingLytNum = cosplay_num;

	spShopItemLayout = new LayoutBase[mRingLytNum];
	
	mNextItemIndex = 0;	// fail safe

	DWORD cosplay_bit = TotalInfo::GetInstance()->GetCosplayBit();
	int cosplay_cnt = 0;
	for (int iI = 0; iI < eWINMENU_COSPLAY_MAX; iI ++) {
		if (cosplay_bit & (1 << iI) ) {
			TotalInfo::tWinMenuCosplayInfo* p_cosplay_info = TotalInfo::GetInstance()->GetCosplayInfo(iI);
			p_layout = &spShopItemLayout[cosplay_cnt];
			p_layout->LinkLayout(p_cosplay_info->mLayoutName);
			p_layout->SetPositionByCentering(-100, -100);
			p_layout->SetSize(80, 80);
			p_layout->SetAlpha(255);
			GraphicManager::GetInstance()->EntrySystemLayout(p_layout, 
															 GraphicManager::eSYS_LAYOUT_PRIORITY_FADE_AFTER);

			if (sPlayer.CosplayIndex() == iI) {
				mNextItemIndex = cosplay_cnt;
			}
			cosplay_cnt ++;
		}
	}
	mNowItemIndex = ((mNextItemIndex + (mRingLytNum / 2)) % mRingLytNum);

	mInfoLayout.LinkLayout(RES_LYT_INFO_COSPLAY);
	mInfoLayout.SetAlpha(0);
	mInfoLayout.StartFade(255, 0.3f);

	mpSelectItemNumLayoutList[0].SetAlpha(0);
	mpSelectItemNumLayoutList[1].SetAlpha(0);

	mIconDist = 300.0f;

	mSquareLayout.SetAlpha(0);

	mIsAlertMessage = FALSE;

	mState = eSTATE_COSPLAY_DOING;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateCosplayDoing(void)
{
	updateSelect();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateCosplayLeave(void)
{
	return;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateKeyEnter(void)
{
	LayoutBase* p_layout = NULL;

	for (int iI = 0; iI < mRingLytNum; iI ++) {
		p_layout = &spShopItemLayout[iI];
		GraphicManager::GetInstance()->RemoveSystemLayout(p_layout);
	}
	if (spShopItemLayout != NULL) {
		delete[] spShopItemLayout;
		spShopItemLayout = NULL;
	}
	
	mRingLytNum = eKEYITEM_MAX;

	/* プレートレイアウト */
	static const BYTE kCOLUMN_MAX = 8;
	int column_num = 0;

	float left_pos;
	if (GraphicManager::GetInstance()->GetAspectRate() == 1.0f) {
		left_pos = 44.0f;
	}
	else {
		left_pos = 156.0f;
	}

	for (int iI = 0; iI < kPLATE_LAYOUT_NUM; iI ++) {
		p_layout = &mPlateLayout[iI];
		p_layout->LinkLayout(RES_LYT_PLATE_COMMON);
		p_layout->SetPosition(left_pos, 108+(84*iI) );
		p_layout->SetSize(76.0f, 76.0f);
		p_layout->SetAlpha(0);
		p_layout->StartFade(128, 0.16f);

		if ( ((iI + 1) * kCOLUMN_MAX) <= eKEYITEM_MAX) {
			column_num = kCOLUMN_MAX;
		}
		else {
			column_num = eKEYITEM_MAX - (kCOLUMN_MAX * iI);
			if (column_num < 0) {
				column_num = 0;
			}
		}

		//		p_layout->SettingContinuDraw(column_num, 64, 0);
		p_layout->SettingContinuDraw(column_num, 78, 0);
		GraphicManager::GetInstance()->EntrySystemLayout(p_layout, 
														 GraphicManager::eSYS_LAYOUT_PRIORITY_FADE_AFTER);
	}

	/* キーアイテムのレイアウト */
	spShopItemLayout = new LayoutBase[mRingLytNum];
	for (int iI = 0; iI < mRingLytNum; iI ++) {
		eItemKind item_kind = GetKeyitemItemKind( (iI % eKEYITEM_MAX) );
		const tItemDetailInfo * p_item_detail = GetItemDetailInfo(item_kind);

		p_layout = &spShopItemLayout[iI];
		p_layout->LinkLayout(p_item_detail->mLayoutName);
		p_layout->SetPosition(left_pos + (78*(iI%8)), (108 + (84*(iI/8))) );
		p_layout->SetSize(48, 48);
		p_layout->SetAlpha(0);
		p_layout->StartFade(220, 0.16f);

		if (TotalInfo::GetInstance()->IsKeyItem((USHORT)iI)) {
			GraphicManager::GetInstance()->EntrySystemLayout(p_layout, 
															 GraphicManager::eSYS_LAYOUT_PRIORITY_LAST);
		}
	}

	mNowItemIndex = 0;

	mInfoLayout.LinkLayout(RES_LYT_INFO_KEYITEM);
	mInfoLayout.SetAlpha(0);
	mInfoLayout.StartFade(255, 0.3f);

	mpSelectItemNumLayoutList[0].SetAlpha(0);
	mpSelectItemNumLayoutList[1].SetAlpha(0);


	GraphicManager::GetInstance()->RemoveSystemLayout(&mSquareLayout);
	GraphicManager::GetInstance()->EntrySystemLayout(&mSquareLayout,
													 GraphicManager::eSYS_LAYOUT_PRIORITY_LAST);
	mSquareLayout.SetAlpha(255);

	mState = eSTATE_KEY_DOING;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateKeyDoing(void)
{
	LayoutBase* p_layout = NULL;

	if (CHECK_TRIG_JOY(JOY_INPUT_LEFT)) {
		mNowItemIndex = SELECT_ROTATION_SUB(mNowItemIndex, mRingLytNum);
		SoundManager::GetSndComponent()->PlaySound(eSE_SELECT_KEY);
	}
	else if (CHECK_TRIG_JOY(JOY_INPUT_RIGHT)) {
		mNowItemIndex = SELECT_ROTATION_ADD(mNowItemIndex, mRingLytNum);
		SoundManager::GetSndComponent()->PlaySound(eSE_SELECT_KEY);
	}


	eItemKind item_kind = GetKeyitemItemKind( (mNowItemIndex % eKEYITEM_MAX) );
	const tItemDetailInfo * p_item_detail = GetItemDetailInfo(item_kind);

	//	if (TotalInfo::GetInstance()->IsKeyItem(item_kind)) {
	if (TotalInfo::GetInstance()->IsKeyItem(mNowItemIndex)) {
		RENDER_MESSAGE_COL("", 
						   p_item_detail->mDispName,
						   p_item_detail->mExplain0,
						   p_item_detail->mExplain1,
						   p_item_detail->mExplain2,
						   GetColor(0, 0, 0));
	} 
	else {
		RENDER_MESSAGE_COL("", "", "", "", "", GetColor(0, 0, 0));
	}


	for (int iI = 0; iI < mRingLytNum; iI ++) {
		p_layout = &spShopItemLayout[iI];
		//		p_layout->SetSize(64, 64);
		//		p_layout->SetSize(52, 52);
		//		p_layout->SetSize(78, 78);
		p_layout->SetSize(70, 70);
	}
	//	spShopItemLayout[mNowItemIndex].SetSize(78, 78);
	int pos_x, pos_y;
	spShopItemLayout[mNowItemIndex].GetPositionByCentering(&pos_x, &pos_y);
	mSquareLayout.SetPositionByCentering(pos_x, pos_y);

	/*=====================================*
	 * 
	 *=====================================*/
	bool is_change_kind = false;
	if (CHECK_TRIG_JOY(JOY_INPUT_UP)) {
		is_change_kind = true;
		mMenuKind = SELECT_ROTATION_SUB(mMenuKind, eMENU_KIND_MAX);
	}
	else if (CHECK_TRIG_JOY(JOY_INPUT_DOWN)) {
		is_change_kind = true;
		mMenuKind = SELECT_ROTATION_ADD(mMenuKind, eMENU_KIND_MAX);
	}
	if (is_change_kind) {
		SoundManager::GetSndComponent()->PlaySound(eSE_ITEM_APPEAR);
		switch (mMenuKind) {
		case eMENU_KIND_ITEM:
			mState = eSTATE_ITEM_ENTER;
			break;
		case eMENU_KIND_COSPLAY:
			mState = eSTATE_COSPLAY_ENTER;
			break;
		case eMENU_KIND_KEY:
			mState = eSTATE_KEY_ENTER;
			break;
		default:
			APP_ASSERT(NULL, "Invalid mMenuKind \n");
			break;
		};

		for (int iI = 0; iI < kPLATE_LAYOUT_NUM; iI ++) {
			p_layout = &mPlateLayout[iI];
			GraphicManager::GetInstance()->RemoveSystemLayout(p_layout);
		}
		return;
	}
	
	/**** 終了処理 ****/
	if ((CHECK_TRIG_JOY(JOY_INPUT_JUMP)) || 
		(CHECK_TRIG_JOY(JOY_INPUT_SELECT_SUB)) || 
		(CHECK_TRIG_JOY(JOY_INPUT_AWAY)) ||
		(CHECK_TRIG_JOY(JOY_INPUT_ATTACK))) {

		mState = eSTATE_FINISH;
	}

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateKeyLeave(void)
{
	return;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::stateFinish(void)
{
	return;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SubSceneItemMenu::updateSelect(void)
{
	static const int kROT_COUNT = 30;

	float theta;
	LayoutBase* p_layout	= NULL;

	/**** アップデート処理 ****/
	/**** ここからは選択時間の処理 ****/

	mIconDist -= 20.0f;
	if (mIconDist < 100.0f) {
		mIconDist = 100.0f;
	}

	// 回転が止まっている時の処理
	if (mRotCnt == 0) {
		if (CHECK_TRIG_JOY(JOY_INPUT_LEFT)) {
			mIsAlertMessage = FALSE;
			mIsRight = false;
			mNextItemIndex = SELECT_ROTATION_ADD(mNextItemIndex, mRingLytNum);
			SoundManager::GetSndComponent()->PlaySound(eSE_SELECT_KEY);
		}
		else if (CHECK_TRIG_JOY(JOY_INPUT_RIGHT)) {
			mIsAlertMessage = FALSE;
			mIsRight = true;
			mNextItemIndex = SELECT_ROTATION_SUB(mNextItemIndex, mRingLytNum);
			SoundManager::GetSndComponent()->PlaySound(eSE_SELECT_KEY);
		}

		bool is_change_kind = false;
		if (CHECK_TRIG_JOY(JOY_INPUT_UP)) {
			is_change_kind = true;
			mMenuKind = SELECT_ROTATION_SUB(mMenuKind, eMENU_KIND_MAX);
		}
		else if (CHECK_TRIG_JOY(JOY_INPUT_DOWN)) {
			is_change_kind = true;
			mMenuKind = SELECT_ROTATION_ADD(mMenuKind, eMENU_KIND_MAX);
		}
		if (is_change_kind) {
			mIsAlertMessage = FALSE;

			SoundManager::GetSndComponent()->PlaySound(eSE_ITEM_APPEAR);
			switch (mMenuKind) {
			case eMENU_KIND_ITEM:
				mState = eSTATE_ITEM_ENTER;
				break;
			case eMENU_KIND_COSPLAY:
				mState = eSTATE_COSPLAY_ENTER;
				break;
			case eMENU_KIND_KEY:
				mState = eSTATE_KEY_ENTER;
				break;
			default:
				APP_ASSERT(NULL, "Invalid mMenuKind \n");
				break;
			};

			return;
		}
	}

	// レイアウトの位置の制御
	float div_rad = Utility::DegToRad(360.0f / (float)mRingLytNum);
	if (mNowItemIndex == mNextItemIndex) {
		for (int iI = 0; iI < mRingLytNum; iI ++) {
			p_layout = &spShopItemLayout[iI];
			theta = (iI - mNowItemIndex) * div_rad;
			p_layout->SetPosition(center.x - (int)(sin(theta) * mIconDist * 1.5f),
								  center.y - (int)(cos(theta) * mIconDist));

		}
		mRotCnt = 0;
	}
	else {
		for (int iI = 0; iI < mRingLytNum; iI ++) {
			p_layout = &spShopItemLayout[iI];

			float now_theta, next_theta;
			now_theta = (iI - mNowItemIndex) * div_rad;
			next_theta = (iI - mNextItemIndex) * div_rad;
			float offset_theta = next_theta - now_theta;
			if ((mIsRight == false) && (offset_theta > 0)) {
				offset_theta += -Utility::DegToRad(360.0f);
			}
			if ((mIsRight == true) && (offset_theta < 0)) {
				offset_theta += Utility::DegToRad(360.0f);
			}
			theta = now_theta + (offset_theta / kROT_COUNT) * mRotCnt;
			p_layout->SetPosition(center.x - (int)(sin(theta) * mIconDist * 1.5f),
								  center.y - (int)(cos(theta) * mIconDist));



			mRotCnt ++;
			if (mRotCnt >= kROT_COUNT) {
				mNowItemIndex = mNextItemIndex;
			}
		}
	}

	switch (mMenuKind) {
	case eMENU_KIND_ITEM:
		{
			eItemKind set_weapon = sMyWeaponList.at(mNowItemIndex);

			/* 選択アイテムに応じて保持数を表示する */
			if (mNowItemIndex == mNextItemIndex) {
				CHAR item_num = -1;
				switch (set_weapon) {
				case eITEM_KIND_BOMB:	// 爆弾
					item_num = TotalInfo::GetInstance()->GetItemNum(eSUBWEAPON_NUM_BOMB);
					break;
				case eITEM_KIND_ARCHERY:// 弓矢
					item_num = TotalInfo::GetInstance()->GetItemNum(eSUBWEAPON_NUM_ARROW);
					break;
				case eITEM_KIND_BIN_RED:// 赤いクスリ
					item_num = TotalInfo::GetInstance()->GetBinContentsKindNum(eBIN_CONTENTS_KIND_RED_MEDICINE);
					break;
				default:
					break;
				}
				if (item_num == -1) {
					mpSelectItemNumLayoutList[0].SetAlpha(0);
					mpSelectItemNumLayoutList[1].SetAlpha(0);
				}
				else {
					mpSelectItemNumLayoutList[0].SetNumber( (item_num / 1)  % 10 );
					mpSelectItemNumLayoutList[1].SetNumber( (item_num / 10) % 10 );
					mpSelectItemNumLayoutList[0].SetAlpha(255);
					mpSelectItemNumLayoutList[1].SetAlpha(255);
				}
			}

			const tItemDetailInfo * p_item_detail = GetItemDetailInfo( set_weapon );
			RENDER_MESSAGE_COL("", 
							   p_item_detail->mDispName,
							   p_item_detail->mExplain1,
							   p_item_detail->mExplain2,
							   "", GetColor(0, 0, 0));
	
			/* 武器の切り替え処理 */
			if ((CHECK_TRIG_JOY(JOY_INPUT_AWAY)) ||
				(CHECK_TRIG_JOY(JOY_INPUT_ATTACK))) {

				if (sPlayer.ItemWork().mItemSubWeapon != set_weapon) {
					// アイテム切り替え処理
					if (changeSubWeapon(set_weapon) == false) {
						return;
					}
					mSelectWeapon = set_weapon;
					SoundManager::GetSndComponent()->PlaySound(eSE_HOOK_START);
				}
			}
		}
		break;

	case eMENU_KIND_COSPLAY:
		{
			DWORD cosplay_index = 0;
			DWORD cosplay_cnt = 0;
			DWORD cosplay_bit = TotalInfo::GetInstance()->GetCosplayBit();
			for (int iI = 0; iI < eWINMENU_COSPLAY_MAX; iI ++) {
				if (cosplay_bit & (1 << iI)) {
					if (mNextItemIndex == cosplay_cnt) {
						cosplay_index = iI;
						break;
					}
					cosplay_cnt ++;
				}
			}
		
			TotalInfo::tWinMenuCosplayInfo* p_cosplay_info = TotalInfo::GetInstance()->GetCosplayInfo(cosplay_index);
			if (mIsAlertMessage == TRUE) {
				RENDER_MESSAGE_COL("",(char *)strNaviTalk_CannotCosplay, "", "", "", COLOR_BLACK);
			} 
			else {
				RENDER_MESSAGE_COL("",
								   (char *)p_cosplay_info->mMessage,
								   "の姿に変装するぞ",
								   "",
								   "",
								   COLOR_BLACK);
			}

			/* 武器の切り替え処理 */
			if ((CHECK_TRIG_JOY(JOY_INPUT_AWAY)) ||
				(CHECK_TRIG_JOY(JOY_INPUT_ATTACK))) {

				// 水の上にいる場合は選択できない様にする
				if (sPlayer.GetFootstepKind() == eFOOTSTEP_KIND_WATER) {
					SoundManager::GetSndComponent()->PlaySound(eSE_NG);
					mIsAlertMessage = TRUE;
					return;
				}
				// 水の上にいない場合は選択処理を実施させる
				else {
					SoundManager::GetSndComponent()->PlaySound(eSE_HOOK_START);
					if (sPlayer.CosplayIndex() != cosplay_index) {
						sPlayer.CosplayIndex() = cosplay_index;
						SceneMergeTest::ChangePlayerModel(p_cosplay_info->mModelName);
					}
				}

			}
		}
		break;

	default:
		break;

	}


	/**** 終了処理 ****/
	if ((CHECK_TRIG_JOY(JOY_INPUT_JUMP)) || 
		(CHECK_TRIG_JOY(JOY_INPUT_SELECT_SUB)) || 
		(CHECK_TRIG_JOY(JOY_INPUT_AWAY)) ||
		(CHECK_TRIG_JOY(JOY_INPUT_ATTACK))) {

		mState = eSTATE_FINISH;
	}
}


/**** end of file ****/


