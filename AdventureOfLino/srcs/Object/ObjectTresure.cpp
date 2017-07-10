/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �I�u�W�F�N�g�x�[�X�N���X
 * @author SPATZ.
 * @data   2012/04/14 19:35:52
 */
//---------------------------------------------------
#include "ObjectTresure.h"
#include "../Charactor/CharaPlayer.h"
#include "AppLib/Basic/Basic.h"
#include "DataDefine/MotionDef.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Collision/CollisionManager.h"
#include "AppLib/Resource/ResourceModel.h"
#include "AppLib/Sound/SoundManager.h"
#include "Scene/ActionTaskManager.h"

#include "Utility/Utility.h"
#include "DataDefine/VoiceDef.h"

#include "../Scene/CommandFunctions.h"

//#include "CharaInfo.h"

/*=====================================*
 * class ObjectTresure
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ObjectTresure::ObjectTresure()
{
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ObjectTresure::~ObjectTresure()
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ObjectTresure::Setup(int objectIdx, eItemKind itemIdx, VECTOR* pSetPos, int rotDeg)
{
	ObjectBase::Setup(objectIdx, itemIdx, pSetPos, rotDeg);
	mObjectType = eOBJECT_TYPE_TREASURE;

	// ���f���̑}�����h���N���X�Őݒ肳����
	mpEntryTargetModel = CollisionManager::GetInstance()->GetEarthModel();
	this->InsertDrawModelSelf();

	CollisionManager::GetInstance()->EntryBodyCollision(this);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ObjectTresure::Update(void)
{
	ObjectBase::Update();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
ObjectTresure::CheckDamageCollNormal(tCheckDamageWork* pParam)
{
	/* �󔠂͍U��������󂯕t���Ȃ����߁A���� */
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ObjectTresure::ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType)
{
	/* �󔠂͍U��������󂯕t���Ȃ����߁A���� */
	return;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ObjectTresure::SetUsed(void)
{
	this->StartMotion(eMT_TRESURE_EVENT_SHORT, false, 0.5f);

	mIsUsed = true;
	ObjectBase::RemoveActiveObjectList(this);

	// �������̕�̏ꍇ�A��\���ɂ���
	if (getTreasureKindFromObjectKind(mObjectIndex) == eTREASURE_KIND_FALLEN) {
		
		CollisionManager::GetInstance()->RemoveBodyCollision(this);

		PRINT_CONSOLE("[%d] ReserveRemoveModel \n", __LINE__);
		GraphicManager::GetInstance()->RemoveDrawModel(this);
		ModelPmd::DeleteModel();
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
//ObjectTresure::StartGetItemEvent(void)
void
ObjectTresure::StartCheckEvent(void)
{
	ActionTaskManager::tTaskInfo info;
	info.mTaskType	= ActionTaskManager::eTASK_GETITEM;
	info.mTaskValueA= (long)this;

	// �W���[�J�[�̏ꍇ
	if (mObjectIndex == eOBJECT_KIND_CARD_JK) {
		info.mTaskType		= ActionTaskManager::eTASK_EVENT;
		info.mTaskValueA	= eEVENT_29;
		info.mTaskValueB	= eEVENT_29_APPEAR_WIZARD;
	}
	// ����ȊO
	else {
		// ���ʂ��I�u�W�F�N�g����擾���A�A�C�e���擾�̃C�x���g�����{
		info.mTaskValueB = getTreasureKindFromObjectKind(mObjectIndex);
	}

	//	mIsUsed = true;
	ActionTaskManager::GetInstance()->NotifyAction(&info);
}
	
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
ObjectTresure::getTreasureKindFromObjectKind(int objectKind)
{
	int treasure_kind = -1;

	switch (objectKind) {
	case eOBJECT_KIND_TRESURE:
		treasure_kind = eTREASURE_KIND_BIG;
		break;

	case eOBJECT_KIND_SMALL_TRESURE:
		treasure_kind= eTREASURE_KIND_SMALL;
		break;

	case eOBJECT_KIND_CARD_K:
	case eOBJECT_KIND_CARD_Q:
	case eOBJECT_KIND_CARD_J:
	case eOBJECT_KIND_BOOK:
	case eOBJECT_KIND_CARD_JK:
		treasure_kind = eTREASURE_KIND_FALLEN;
		break;

	case eOBJECT_KIND_CRYSTAL:
		treasure_kind = eTREASURE_KIND_CRYSTAL;
		break;
	};

	return treasure_kind;
}

/*=====================================*
 * class ObjectTresureCrystal
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ObjectTresureCrystal::ObjectTresureCrystal()
{
	mTaskValue = 0;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ObjectTresureCrystal::~ObjectTresureCrystal()
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ObjectTresureCrystal::StartCheckEvent(void)
{
	ActionTaskManager::tTaskInfo info;
	info.mTaskType	= ActionTaskManager::eTASK_GETITEM;
	info.mTaskValueA= (long)this;

	int event_def =	ConvertPlaceIndex_TO_EventDefine(getStageIndex());
	switch (event_def) {
	case eEVENT_28:
		info.mTaskValueB= eTREASURE_KIND_CRYSTAL;
		this->SetVisible(false);
		this->Scale() = ZERO_VEC;
		break;

	case eEVENT_38:
		info.mTaskType = ActionTaskManager::eTASK_EVENT;
		info.mTaskValueA = eEVENT_38;
		info.mTaskValueB = mTaskValue;
		break;

	case eEVENT_43:
		info.mTaskValueB= eTREASURE_KIND_CRYSTAL;
		this->SetVisible(false);
		this->Scale() = ZERO_VEC;
		break;

	default:
		APP_ASSERT_PRINT(NULL, "Not Support crystal event\n");
		break;
	};

	//	mIsUsed = true;
	ActionTaskManager::GetInstance()->NotifyAction(&info);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ObjectTresureCrystal::SetCrystalEvent(ULONG taskValue)
{
	mTaskValue = taskValue;
}

/**** end of file ****/

