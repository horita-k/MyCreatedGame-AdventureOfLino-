/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <d3dx9math.h>
#include "StageFunc029.h"
#include "AppLib/Effect/EffectManager.h"
#include "AppLib/Sound/SoundManager.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Resource/ResourceManager.h"
#include "AppLib/Resource/ResourceModel.h"
#include "AppLib/Resource/ResourceLayout.h"

#include "AppLib/Graphic/RenderBloom.h"

#include "DataDefine/EffectDef.h"
#include "Gimmick/GimmickBase.h"
#include "Gimmick/GimmickExtend.h"
#include "../CommandFunctions.h"
#include "DataDefine/VoiceDef.h"

#include "../ExternDefine.h"		//<! Extern �錾�͑S�Ă����ɏW�񂷂�
#include "../SceneMainGame.h"

/*=====================================*
 * static
 *=====================================*/

/*=====================================*
 * public
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
StageFunc029::StageFunc029()
{
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
StageFunc029::~StageFunc029()
{
	destroy();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc029::Update(int counter)
{
	/* �R���p�X�̕\�� */
	TotalInfo::GetInstance()->UpdateCompass();

	// ���������Ɍ��̒��ɓ������ꍇ�A���̌����f���𔭍s������
	if (TotalInfo::GetInstance()->IsKeyItem(eKEYITEM_LIB_SHIELD)) {

	//	if (sPlayer.ItemWork().mItemSubWeapon == eITEM_KIND_LIB_SHIELD) {
		ModelPmd* p_attach = (ModelPmd *)sPlayer.GetAttachAddr(CharaPlayer::eATTACH_MODEL_SHIELD)->mpAttachedModel;

		if (sPlayer.EarthMaterialIndex() == 0) {
			// �����f����\������
			MV1SetMeshVisible(p_attach->GetModelHandle(), LIBSHIELD_LIGHT_MESH_IDX, TRUE);

			if (!(IS_END_EVT(eEVENT_29, eEVENT_29_APPEAR_TRUMP))) {
				VECTOR vec_start, vec_end, hitpos, normal;
				int frame_index; BYTE mat_index;
				p_attach->GetFramePosFromIndex(&vec_start, 0);
				p_attach->GetFramePosFromIndex(&vec_end, 2);
				// ���̌��Ƃ̓����蔻��
				if (CollisionManager::GetInstance()->CheckEarthCollision(vec_start, vec_end, &hitpos, &normal, &frame_index, &mat_index)) {
					if (frame_index == (eBG_TYPE_GIMMICK + 12)) {
						ActionTaskManager::tTaskInfo param = {ActionTaskManager::eTASK_EVENT, eEVENT_29, eEVENT_29_APPEAR_TRUMP};
						ActionTaskManager::GetInstance()->NotifyAction(&param);
					}
				}
			}			

		}
		else {
			// �����f�����\������
			//			MV1SetMeshVisible(p_attach->GetModelHandle(), 2, FALSE);
			MV1SetMeshVisible(p_attach->GetModelHandle(), LIBSHIELD_LIGHT_MESH_IDX, FALSE);
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc029::SetupBeforeLoad(void)
{
	tStageInfo info = {0};

	info.mFileIndex = eRES_STG_CASTLE_EARTH;
	info.mIsCollision = true;
	sSubStageInfoList.push_back(info);

	info.mFileIndex = eRES_STG_REVELNEAR2ND_EARTH;
	info.mIsCollision = true;
	sSubStageInfoList.push_back(info);

	// �ۂ��۔��݂𕜊������Ă��邩�H
	if (IS_END_EVT(eEVENT_29, eEVENT_29_REVIVAL_POPPO)) {
		//<! NPC�̃Z���t��ύX
		CharaNpc::SetCsvSubNo(2);
	}
	//<! �P�ʂ��N���A�������𔻒�
	else if (TotalInfo::GetInstance()->GetEventFlagEnable(eEVENT_29, eEVENT_29_COME_FRES)) {
		//<! NPC�̃Z���t��ύX
		CharaNpc::SetCsvSubNo(1);
	}

	/**** �x�[�X�N���X�֐������N���X������ɃR�[�� ****/
	StageFuncBase::SetupBeforeLoad();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc029::SetupAfterLoad(int& rPhase)
{
	unsigned long event_kind;
	unsigned long event_flag;
	int model_handle = sModelMap.GetModelHandle();

	/**** �x�[�X�N���X�֐������N���X�����O�ɃR�[�� ****/
	StageFuncBase::SetupAfterLoad(rPhase);

	event_kind = eEVENT_29;
	event_flag = eEVENT_29_WELCOME_CASTLE;
	if (TotalInfo::GetInstance()->GetEventFlagEnable(event_kind, event_flag) == false) {
		sTaskInfo.mTaskType		= ActionTaskManager::eTASK_EVENT;
		sTaskInfo.mTaskValueA	= (BYTE)event_kind;
		sTaskInfo.mTaskValueB	= (BYTE)event_flag;
		rPhase = SceneMainGame::ePHASE_EVENT_BEFORE;
	}

	//		mStageFunc = &SceneMainGame::stageFunc29;

	// �W���[�J�[�g�����v��\��
	if (TotalInfo::GetInstance()->GetEventFlagEnable(eEVENT_29, eEVENT_29_APPEAR_WIZARD) == true) {
		sModelMap.SetAlpha((eBG_TYPE_GIMMICK + 1), 0.0f);
	}

	// �g�����v�̃J���N�����I����Ă��邩�H
	if (TotalInfo::GetInstance()->GetEventFlagEnable(eEVENT_29, eEVENT_29_GATE_SECURITY) == true) {
		sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 2);
		sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 3);
		sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 4);
		sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 5);

		sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 0);
	}
	else  {

		// �o���A�̒ǉ�
		CollisionManager::GetInstance()->EntryWallCollision(model_handle, (eBG_TYPE_GIMMICK + 0));
		sModelMap.EntryBarrier((eBG_TYPE_GIMMICK + 0));

		// ����̃R���W�����ǉ�
		CollisionManager::GetInstance()->EntryEarthCollision(model_handle, (eBG_TYPE_GIMMICK + 2));

		unsigned long field_bit = TotalInfo::GetInstance()->GetFieldInfoFlagBit(sStageIndex);
		if (!(field_bit & kFIELD_INFO_29_TRUMP_PUT_K)) {
			sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 3);
		}
		if (!(field_bit & kFIELD_INFO_29_TRUMP_PUT_Q)) {
			sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 4);
		}
		if (!(field_bit & kFIELD_INFO_29_TRUMP_PUT_J)) {
			sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 5);
		}
	}

	// ���O�C�x���g
	if (IS_END_EVT(eEVENT_29, eEVENT_29_BIG_KEY)) {
		sModelMap.SetVisible(false, eBG_TYPE_GIMMICK + 10);
	}
	else {
		sModelMap.SetVisible(true, eBG_TYPE_GIMMICK + 10);
		CollisionManager::GetInstance()->EntryWallCollision(model_handle, eGIMMICK_29_BIG_KEY);
	}


	if (IS_END_EVT(eEVENT_29, eEVENT_29_COME_FRES) ) {
		spBGInfo->mActionLimit = eACTION_NG;
		sPlayer.SetActionLimit(spBGInfo->mActionLimit,
							   sPlayer.ItemWork().mItemSubWeapon);
		
		CollisionManager::GetInstance()->RemoveWallCollision(model_handle, (eBG_TYPE_GIMMICK + 11));
		MV1SetFrameVisible(model_handle, (eBG_TYPE_GIMMICK + 11), FALSE);
	}
	else {
		// �o���A�̒ǉ�
		CollisionManager::GetInstance()->EntryWallCollision(model_handle, (eBG_TYPE_GIMMICK + 11));
		sModelMap.EntryBarrier((eBG_TYPE_GIMMICK + 11));
	}

	// ���z�̃��f���ƌ��̂����蔻��p
	CollisionManager::GetInstance()->EntryEarthCollision(model_handle, (eBG_TYPE_GIMMICK + 12));

	if (IS_END_EVT(eEVENT_29, eEVENT_29_COME_FRES) == false) {
		spBGInfo->mMusicName = BGM_CASTLE;
	}
	else {
		spBGInfo->mMusicName = BGM_CASTLE_2;
	}

	// �ړ����̃t�F�[�h�A�E�g���ɌĂ΂��n���h��
	NotifyMovePlaceWhileFadeout();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc029::NotifyMovePlaceWhileFadeout(void)
{
	if ((sStagePlaceIndex == 17) ||
		(sStagePlaceIndex == 40)) {
		// �u���[���ݒ�
		RenderBloom::SetBloomType(RenderBloom::eBLOOM_TYPE_BRIGHT_LESS);
		RenderBloom::SetBloomParam(12, 135, 140);
		RenderBloom::tBloomBrightParam param = {0};
		param.mClipParam = 230;
		RenderBloom::BloomBrightParam() = param;
	}
	else {
		setDefaultBloomSetting();
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc029::ProcessShowMapBefore(void)
{
	for (int iI = 0; iI < (int)sSubStageInfoList.size(); iI ++) {
		spSubModelMap[iI].SetVisible(false);
	}			
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc029::ProcessShowMapAfter(void)
{
	for (int iI = 0; iI < (int)sSubStageInfoList.size(); iI ++) {
		spSubModelMap[iI].SetVisible(true);
	}			
}

/*=====================================*
 * private
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc029::destroy(void)
{
	return;
}

/**** end of file ****/


