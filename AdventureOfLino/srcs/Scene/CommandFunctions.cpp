/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �R�}���h�֘A�W��
 * @author SPATZ
 * @data   2013/08/03 10:40:29
 */
//---------------------------------------------------
/*=====================================*
 * include
 *=====================================*/
#include <windows.h>
#include "dxlib.h"

#include "AppLib/Basic/Basic.h"

#include "AppLib/Input/InputManager.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Graphic/RenderBloom.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Graphic/EffekseerManager.h"
#include "AppLib/Sound/SoundManager.h"
#include "AppLib/Collision/CollisionManager.h"
#include "AppLib/Effect/EffectManager.h"

#include "AppLib/Resource/ResourceManager.h"
#include "AppLib/Resource/ResourceModel.h"
#include "AppLib/Resource/ResourceLayout.h"

#include "SceneManager.h"
#include "Scene/ActionTaskManager.h"
#include "Scene/EventManager.h"
#include "Scene/SceneMainGame.h"

#include "DataDefine/VoiceDef.h"
#include "DataDefine/MessageDef.h"
#include "DataDefine/MotionDef.h"
#include "DataDefine/BGDef.h"
#include "DataDefine/EffectDef.h"
#include "DataDefine/EnemyDef.h"

#include "Gimmick/GimmickBase.h"

#include "ExternDefine.h"		//<! Extern �錾�͑S�Ă����ɏW�񂷂�
#include "CommandFunctions.h"
#include "StageFunc/StageFuncDef.h"

#include "Utility/Utility.h"

#ifdef _DEBUG
#include "External/PIC/PIC.h"
#endif // _DEBUG


/*=====================================*
 * �C�x���g�R�}���h�֐�
 *=====================================*/
static const BYTE kEVT_CMD_FUNC_NUM = 11;
static const EVT_CMD_FUNC mEvtCmdFunc[kEVT_CMD_FUNC_NUM] = {
	&evtCmdFunc_00,
	&evtCmdFunc_01,
	&evtCmdFunc_02,
	&evtCmdFunc_03,
	&evtCmdFunc_04,
	&evtCmdFunc_05,
	&evtCmdFunc_06,
	&evtCmdFunc_07,
	&evtCmdFunc_08,
	&evtCmdFunc_09,
	&evtCmdFunc_10,
};
/*=====================================*
 * CSV�R�}���h�֐�
 *=====================================*/
/* static */CSV_CMD_FUNC
mCsvCmdFunc[eCSV_CMD_MAX] = {
	&funcCsvCmd_SHOW_ITEM,
	&funcCsvCmd_VC,
	&funcCsvCmd_SND,
	&funcCsvCmd_LYT,
	&funcCsvCmd_F_IN,
	&funcCsvCmd_F_OUT,
	&funcCsvCmd_FADE,
	&funcCsvCmd_FOG,
	&funcCsvCmd_M_IN,
	&funcCsvCmd_M_OUT,
	&funcCsvCmd_T_IN,
	&funcCsvCmd_T_OUT,
	&funcCsvCmd_TOON,
	&funcCsvCmd_SHAKE,
	&funcCsvCmd_EFFECT,
	&funcCsvCmd_EFK,
	&funcCsvCmd_CAM,
	&funcCsvCmd_SHOW,
	&funcCsvCmd_POSITION,
	&funcCsvCmd_SETPOS,
	&funcCsvCmd_SETROT,
	&funcCsvCmd_SETSCA,
	&funcCsvCmd_POSROT,
	&funcCsvCmd_MDLANM,
	&funcCsvCmd_LYTANM,
	&funcCsvCmd_ATTACH,
	&funcCsvCmd_MOTION,
	&funcCsvCmd_MUSIC,
	&funcCsvCmd_D_MSG,

	&funcCsvCmd_L_DIF,
	&funcCsvCmd_L_SPE,
	&funcCsvCmd_L_AMB,
	&funcCsvCmd_FUNC,
	&funcCsvCmd_PHASE,

	&funcCsvCmd_BLM,
#ifdef _DEBUG
	&funcCsvCmd_LOAD,
	&funcCsvCmd_SAVE,
#endif // _DEBUG
};
static const char* sCsvCmdStringTable[eCSV_CMD_MAX] = {
	"SHOW_ITEM",
	"VC",
	"SND",
	"LYT",
	"F_IN",
	"F_OUT",
	"FADE",
	"FOG",
	"M_IN",
	"M_OUT",
	"T_IN",
	"T_OUT",
	"TOON",
	"SHAKE",
	"EFFECT",
	"EFK",
	"CAM",
	"SHOW",
	"POSITION",
	"SETPOS",
	"SETROT",
	"SETSCA",
	"POSROT",
	"MDLANM",
	"LYTANM",
	"ATTACH",
	"MOTION",
	"MUSIC",
	"D_MSG",
	"L_DIF",
	"L_SPE",
	"L_AMB",
	"FUNC",
	"PHASE",

	"BLM",
#ifdef _DEBUG
	"LOAD",
	"SAVE",
#endif // _DEBUG
};

/*=====================================*
 * Font�ݒ�\����
 *=====================================*/
struct tDrawFontWork {
	char* mMessage[4];
	int mSetX;
	int mSetY;
	int mFontSize;
};

/*=====================================*
 * CsvAction �֘A��`
 *=====================================*/
static USHORT	sCheckLineCnt	= 0;
static char		sActiveExCmd	= (char)-1;
static SHORT	sActiveExTime	= 0;

#ifdef _DEBUG
static tCsvActionDebug sCsvActionDebug = {0};
#endif // _DEBUG

//---------------------------------------------------
/**
   BG���f������v���C���[�̍��W�E�p�x���擾����
*/
//---------------------------------------------------
/*static*/bool
getPlayerPosRotFromBGModel(VECTOR* pOutPos, float* pOutRot, int stagePlaceIndex)
{
	if (stagePlaceIndex == -1) return false;

	int model_handle = -1;
	int model_num = (sSubStageInfoList.size() + 1);

	for (int iI = 0; iI < model_num; iI ++) {

		if (iI == 0) {
			model_handle = sModelMap.GetModelHandle();
		} else {
			model_handle = spSubModelMap[iI - 1].GetModelHandle();
		}

		int mat_num = MV1GetMaterialNum(model_handle);
		int frame_num = MV1GetFrameNum(model_handle);
		if (frame_num <= eBG_TYPE_PLACE) {
			return false;
		}
	
		int mesh_num = MV1GetFrameMeshNum(model_handle, eBG_TYPE_PLACE);

		int material_idx;
		int mesh_index = 0;
		for (int iJ = 0; iJ < mesh_num; iJ ++) {
			mesh_index = MV1GetFrameMesh(model_handle, eBG_TYPE_PLACE, iJ);

			material_idx = MV1GetMeshMaterial(model_handle, mesh_index);

			// �}�e���A�������擾����
			const char* mat_name = MV1GetMaterialName(model_handle, material_idx);
			// �����K���Ƃ��āA �wPlaceXXX_YYY�x �Ƃ��� (XXX �͏ꏊ�C���f�b�N�X YYY �͊p�x)
			// ���O�̒����� 12 �����͂͂���
			if (strlen(mat_name) < 12) {
				continue;
			}
			char label[5+1];
			char place_index[3+1];
			char rotation[3+1];
			memcpy(label,		&mat_name[0], sizeof(label));
			label[5] = '\0';
			memcpy(place_index, &mat_name[5], sizeof(place_index));
			place_index[3] = '\0';
			memcpy(rotation,	&mat_name[9], sizeof(rotation));
			rotation[3] = '\0';

			if (strcmp(label, "Place") != 0) {
				continue;
			}
			int nPlaceIndex, nRotation;
			nPlaceIndex = atoi(place_index);
			nRotation = atoi(rotation);

			if (stagePlaceIndex == nPlaceIndex) {
				VECTOR world = MV1GetFramePosition(model_handle, eBG_TYPE_PLACE);
				VECTOR local = MV1GetMeshMaxPosition(model_handle, mesh_index);
				local = VScale(local, spBGInfo->mScale);
				VECTOR local_2 = MV1GetMeshMinPosition(model_handle, mesh_index);
				local_2 = VScale(local_2, spBGInfo->mScale);
				local = VScale(VAdd(local, local_2), 0.5f);
				*pOutPos = VAdd(world, local);

				*pOutRot = Utility::DegToRad((float)nRotation);
				return true;
			}
		}
	}
	
	PRINT_ASSERT(false, "Failed getPlayerPosRotFromBGModel(), stagePlaceIndex : %d \n", stagePlaceIndex); 

	// �Y���̔ԍ��Ȃ�
	return false;
}

//---------------------------------------------------
/**
 * ����ɍ��킹���ύX����
 */
//---------------------------------------------------
bool changeSubWeapon(eItemKind itemKind)
{
	PRINT_CONSOLE("Trace : changeSubWeapon() \n");

	int action_limit;
	action_limit = spBGInfo->mActionLimit;
	if (sIsBattle) {
		action_limit = eACTION_OK;
	}

	const tItemDetailInfo * p_item_detail = GetItemDetailInfo(itemKind);
	
	const char* model_name = p_item_detail->mModelName;

	/* */
	if ((itemKind == eITEM_KIND_PORTION) ||
		(itemKind == eITEM_KIND_BIN_MAGENTA) ||
		(itemKind == eITEM_KIND_BIN_CYAN) ||
		(itemKind == eITEM_KIND_BIN_YELLOW) ||
		(itemKind == eITEM_KIND_MAGIC_THUNDER) ||
		(itemKind == eITEM_KIND_MAGIC_FLAME) ||
		(itemKind == eITEM_KIND_MAGIC_TORNADE)) {
		// �������� SceneMainGame::phaseSelectWeaponEnd() �Ŏ��{
	}
	/* ��r�� */
	else if (itemKind == eITEM_KIND_BIN_EMPTY) {
		SoundManager::GetSndComponent()->PlaySound(eSE_NG);

		return false;
	}
	/* �d�� */
	else if (itemKind == eITEM_KIND_BIN_FAIRY) {
		sPlayer.AddHitPoint(16);//4x4
		SoundManager::GetSndComponent()->PlaySound(eSE_HOLY);
		EffekseerManager::GetInstance()->Play(eEFK_HEAL, &sPlayer.Position(), 30.0f);
		
		useBinContents(eITEM_KIND_BIN_FAIRY);
	}
	/* ���̑��A�C�e�� */
	else {
		// ���C�A�E�g��ύX
		LayoutBase* p_lyt_weapon = TotalInfo::GetInstance()->GetSystemLayout(TotalInfo::eLY_SUB_WEAPON);
		p_lyt_weapon->LinkLayout(p_item_detail->mLayoutName);

		// sGimmick �I�u�W�F�N�g�̍Đݒ�
		sGimmick.DeleteModel();
		if (itemKind == eITEM_KIND_NOWEAPON) {
			// �_�~�[�Ƃ��ĕʃ��f���������N�����Ă����A��\���ɂ�����
			sGimmick.LinkModel(GetItemDetailInfo(eITEM_KIND_LONGHOOK)->mModelName);
			sGimmick.SetVisible(false);
		}
		else {
			sGimmick.LinkModel(model_name);
			sGimmick.SetVisible(true);
		}
		sGimmick.Setup();
		sGimmick.StartMotion(0, true, 0.5f);
		GraphicManager::GetInstance()->EntryDrawModel(&sGimmick);	// �`��G���g���[

		/* �T�u�E�F�|�����A�^�b�` */
		int node_index = sPlayer.GetFrameIndexFromName("�E���w�P");
		sPlayer.ChangeAttachModelFrameIndex(CharaPlayer::eATTACH_MODEL_SUB_WEAPON, node_index);

		sPlayer.SetActionLimit(action_limit, itemKind);

		// ���C�A�E�g���̍X�V
		TotalInfo::GetInstance()->VisibleBattleIcon();
	}

	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
useBinContents(eItemKind itemKind)
{
	eSubweaponNum sub_weapon;
	// �ϊ�
	switch (itemKind) {
	case eITEM_KIND_BIN_MAGENTA:	sub_weapon = eSUBWEAPON_NUM_BIN_MAGENTA;	break;
	case eITEM_KIND_BIN_CYAN:		sub_weapon = eSUBWEAPON_NUM_BIN_CYAN;		break;
	case eITEM_KIND_BIN_YELLOW:		sub_weapon = eSUBWEAPON_NUM_BIN_YELLOW;		break;
	case eITEM_KIND_BIN_FAIRY:		sub_weapon = eSUBWEAPON_NUM_BIN_FAILY;		break;
	default:
		APP_ASSERT_PRINT((NULL), "Invalid itemKind %d \n", itemKind);
		break;
	};

	TotalInfo::GetInstance()->AddItemNum(sub_weapon, -1, false);
	if (TotalInfo::GetInstance()->GetItemNum(sub_weapon) == 0) {
		deleteMyWeapon(itemKind);
	}
	TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_BIN_EMPTY, 1, true);
	addMyWeapon(eITEM_KIND_BIN_EMPTY);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
addBinContents(eItemKind itemKind)
{
	eSubweaponNum sub_weapon;
	// �ϊ�
	switch (itemKind) {
	case eITEM_KIND_BIN_MAGENTA:	sub_weapon = eSUBWEAPON_NUM_BIN_MAGENTA;	break;
	case eITEM_KIND_BIN_CYAN:		sub_weapon = eSUBWEAPON_NUM_BIN_CYAN;		break;
	case eITEM_KIND_BIN_YELLOW:		sub_weapon = eSUBWEAPON_NUM_BIN_YELLOW;		break;
	case eITEM_KIND_BIN_FAIRY:		sub_weapon = eSUBWEAPON_NUM_BIN_FAILY;		break;
	default:
		APP_ASSERT_PRINT((NULL), "Invalid itemKind %d \n", itemKind);
		break;
	};

	TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_BIN_EMPTY, -1, false);
	if (TotalInfo::GetInstance()->GetItemNum(eSUBWEAPON_NUM_BIN_EMPTY) == 0) {
		deleteMyWeapon(eITEM_KIND_BIN_EMPTY);
	}
	TotalInfo::GetInstance()->AddItemNum(sub_weapon, 1, true);
	addMyWeapon(itemKind);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
BOOL
isEmptyBinContents(void)
{
	if (!(TotalInfo::GetInstance()->IsWeaponFlag(eITEM_KIND_BIN_EMPTY)))  {
		// �󂫃r�����Ȃ��ꍇ
		return FALSE;
	}
	return TRUE;
}

//---------------------------------------------------
/**
 * �A�C�e���擾�X�V����
 */
//---------------------------------------------------
/*static*/void
updateGetItem(eItemKind itemKind)
{
	// �}�X�^�[�\�[�h�̕\����߂�
	//	switch (p_item->GetItemKind()) {
	switch (itemKind) {
	case eITEM_KIND_MASTERSWORD:
		sPlayer.SetMeshVisible(true, 4);
		sPlayer.SetMeshVisible(true, 19);
		sPlayer.SetMeshVisible(true, 20);
		
		/*===================================== *
		 * NPC �̊��蓖�Ă�ύX������
		 *===================================== */
		spNpc = CharaNpc::RecreateNpcInstance(1, spNpc, sNpcNum, sStageIndex, spBGInfo->mScale);
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_MASTERSWORD);

		break;
	case eITEM_KIND_MONEY_1:
		TotalInfo::GetInstance()->AddMoney(1);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_MONEY_5:
		TotalInfo::GetInstance()->AddMoney(5);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_MONEY_10:
		TotalInfo::GetInstance()->AddMoney(10);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_MONEY_20:
		TotalInfo::GetInstance()->AddMoney(20);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_MONEY_50:
		TotalInfo::GetInstance()->AddMoney(50);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_MONEY_100:
		TotalInfo::GetInstance()->AddMoney(100);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_MONEY_200:
		TotalInfo::GetInstance()->AddMoney(200);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_MONEY_400:
		TotalInfo::GetInstance()->AddMoney(400);
		SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		break;
	case eITEM_KIND_HEART:
		sPlayer.AddHitPoint(4);
		SoundManager::GetSndComponent()->PlaySound(eSE_COMICAL);
		break;
	case eITEM_KIND_BOOMERANG:
		addMyWeapon(eITEM_KIND_BOOMERANG);
		break;
	case eITEM_KIND_LONGHOOK:
		addMyWeapon(eITEM_KIND_LONGHOOK);
		break;
	case eITEM_KIND_HEART_CASE:
		sPlayer.AddMaxHitpoint(4);
		sPlayer.AddHitPoint(9999); // �S��
		sPlayer.AddNowMagicPoint(9999);
		sPlayer.addPiyoPiyoGauge(9999);
		SoundManager::GetSndComponent()->PlaySound(eSE_HOLY);
		break;

	case eITEM_KIND_HEART_PIECE:
		{
			TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_HEART_PIECE, 1, false);
			if ( (TotalInfo::GetInstance()->GetItemNum(eSUBWEAPON_NUM_HEART_PIECE) % 4) == 0) {
				sPlayer.AddMaxHitpoint(4);
				sPlayer.AddHitPoint(9999); // �S��
				sPlayer.AddNowMagicPoint(9999);
				sPlayer.addPiyoPiyoGauge(9999);
				SoundManager::GetSndComponent()->PlaySound(eSE_HOLY);
				//				TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_HEART_PIECE, -4, false);
			}
		}
		break;

	case eITEM_KIND_MAP:
		{
			// ���݂̃_���W�����̃}�b�v��ێ����Ă��邱�Ƃɂ���
			TotalInfo::GetInstance()->SetEnableFieldInfoFlag(sStageIndex, kFIELD_INFO_MAP_BIT);
			//			LayoutBase* p_lyt_map = TotalInfo::GetInstance()->GetSystemLayout(TotalInfo::eLY_HAS_MAP);
			//			p_lyt_map->SetAlpha(220);
		}
		break;

	case eITEM_KIND_BOSSKEY:
		{
			// ���݂̃_���W�����̌���ێ����Ă��邱�Ƃɂ���
			TotalInfo::GetInstance()->SetEnableFieldInfoFlag(sStageIndex, kFIELD_INFO_KEY_BIT);
			//			LayoutBase* p_lyt_key = TotalInfo::GetInstance()->GetSystemLayout(TotalInfo::eLY_HAS_KEY);
			//			p_lyt_key->SetAlpha(220);
		}
		break;

	case eITEM_KIND_PORTION:
		addMyWeapon(eITEM_KIND_PORTION);
		break;

	case eITEM_KIND_SMILE_0:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_SMILE_0);
		break;
	case eITEM_KIND_SMILE_1:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_SMILE_1);
		break;
	case eITEM_KIND_SMILE_2:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_SMILE_2);
		break;
	case eITEM_KIND_SMILE_3:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_SMILE_3);
		break;
	case eITEM_KIND_SMILE_4:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_SMILE_4);
		break;
	case eITEM_KIND_SMILE_5:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_SMILE_5);
		break;

	case eITEM_KIND_DANCE:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_DANCE);
		break;

	case eITEM_KIND_LIB_SHIELD:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_LIB_SHIELD);
		break;

	case eITEM_KIND_CARD_K:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_CARD_K);
		break;

	case eITEM_KIND_CARD_Q:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_CARD_Q);
		break;

	case eITEM_KIND_CARD_J:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_CARD_J);
		break;

	case eITEM_KIND_BOMB:
		addMyWeapon(eITEM_KIND_BOMB);
		TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_BOMB, 10, false);
		break;

	case eITEM_KIND_CRYSTAL_G:
	case eITEM_KIND_CRYSTAL_Y:
		break;

	case eITEM_KIND_ARCHERY:
		addMyWeapon(eITEM_KIND_ARCHERY);
		TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_ARROW, 50, false);
		break;

	case eITEM_KIND_ANGEL_WING:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_ANGEL_WING);
		break;

	case eITEM_KIND_COSPLAY_PRONAMA:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_PRONAMA);
		break;
	case eITEM_KIND_COSPLAY_DABAH:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_DABAH);
		break;
	case eITEM_KIND_COSPLAY_TDA_MIKU:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_TDA_MIKU);
		break;
	case eITEM_KIND_COSPLAY_LAPAN:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_LAPAN);
		break;
	case eITEM_KIND_COSPLAY_FRES:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_FRES);
		break;
	case eITEM_KIND_COSPLAY_MASK:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_MASK);
		break;
	case eITEM_KIND_COSPLAY_SUNOPHONE:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_SUNOPHONE);
		break;
	case eITEM_KIND_COSPLAY_BAT:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_BAT);
		break;
	case eITEM_KIND_COSPLAY_CHILNO:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_CHILNO);
		break;
	case eITEM_KIND_COSPLAY_HONGKONG:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_HONGKONG);
		break;
	case eITEM_KIND_COSPLAY_LOANA:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_LOANA);
		break;
	case eITEM_KIND_COSPLAY_YUKARI:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_YUKARI);
		break;
	case eITEM_KIND_COSPLAY_NAKANO:
		TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_NAKANO);
		break;

	case eITEM_KIND_PLATE_ARROW:
		{
			bool is_update_draw = false;
			if (sPlayer.ItemWork().mItemSubWeapon == eITEM_KIND_ARCHERY) {
				is_update_draw = true;
			}
			TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_ARROW, 10, is_update_draw);
			SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		}
		break;

	case eITEM_KIND_PLATE_BOMB:
		{
			bool is_update_draw = false;
			if (sPlayer.ItemWork().mItemSubWeapon == eITEM_KIND_BOMB) {
				is_update_draw = true;
			}
			TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_BOMB, 3, is_update_draw);
			SoundManager::GetSndComponent()->PlaySound(eSE_GET_MONEY);
		}
		break;

	case eITEM_KIND_BIN_EMPTY:
		addMyWeapon(eITEM_KIND_BIN_EMPTY);
		TotalInfo::GetInstance()->AddItemNum(eSUBWEAPON_NUM_BIN_EMPTY, 1, true);
		break;

	case eITEM_KIND_BIN_MAGENTA:
		addBinContents(eITEM_KIND_BIN_MAGENTA);
		break;

	case eITEM_KIND_BIN_CYAN:
		addBinContents(eITEM_KIND_BIN_CYAN);
		break;

	case eITEM_KIND_BIN_YELLOW:
		addBinContents(eITEM_KIND_BIN_YELLOW);
		break;

	case eITEM_KIND_BIN_FAIRY:
		addBinContents(eITEM_KIND_BIN_FAIRY);
		break;

	case eITEM_KIND_WALLET_A:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_WALLET_A);
		break;
	case eITEM_KIND_WALLET_B:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_WALLET_B);
		break;
	case eITEM_KIND_WALLET_C:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_WALLET_C);
		break;
	case eITEM_KIND_SEAL_KEY:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_SEAL_KEY);
		break;

	case eITEM_KIND_TROPHY:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_TROPHY);
		break;

	case eITEM_KIND_BOOK:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_BOOK);
		break;

	case eITEM_KIND_FLAME_RING:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_FLAME_RING);
		break;
	case eITEM_KIND_CHICK_PIERCE:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_CHICK_PIERCE);
		break;
	case eITEM_KIND_FAIRY_BRACELET:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_FAIRY_BRACELET);
		break;

	case eITEM_KIND_NAVI_LIGHT:
		TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_NAVI_LIGHT);
		break;

	case eITEM_KIND_MAGIC_THUNDER:
		addMyWeapon(eITEM_KIND_MAGIC_THUNDER);
		break;
	case eITEM_KIND_MAGIC_FLAME:
		addMyWeapon(eITEM_KIND_MAGIC_FLAME);
		break;
	case eITEM_KIND_MAGIC_TORNADE:
		addMyWeapon(eITEM_KIND_MAGIC_TORNADE);
		break;
	case eITEM_KIND_OCARINA:
		addMyWeapon(eITEM_KIND_OCARINA);
		break;
	case eITEM_KIND_COPY_ROD:
		addMyWeapon(eITEM_KIND_COPY_ROD);
		break;

	default:
		APP_ASSERT_PRINT(NULL,
						 "Invalid itemKind  : %d \n", itemKind);
		break;
	};
}

//---------------------------------------------------
/**
   pPos == NULL : �K���ɔz�u
   pPos != NULL : �w��̈ʒu�ɔz�u
*/
//---------------------------------------------------
void
createEnemyWork2(int enemyIndex, eEnemyKind kind, VECTOR* pPos)
{
	VECTOR vec;
	float rad;

	tEnemyWork* p_enemy_work;
	APP_ASSERT_PRINT(enemyIndex < kENEMY_ONCE_MAX,
					 "enemyIndex %d is Invalid Value\n", enemyIndex);

	p_enemy_work = (tEnemyWork *)GetEnemyWorkTable(kind);

	if (sppEnemy[enemyIndex] != NULL) {

		GraphicManager::GetInstance()->RemoveDrawModel(sppEnemy[enemyIndex]);	// �`��G���g���[
		CollisionManager::GetInstance()->RemoveBodyCollision(sppEnemy[enemyIndex]);	// �����蔻��G���g���[
		CollisionManager::GetInstance()->RemoveAttackCollision(sppEnemy[enemyIndex]);	// �����蔻��G���g���[
		delete sppEnemy[enemyIndex];
		sppEnemy[enemyIndex] = NULL;
	}

	// ��ʂɉ������C���X�^���X����
	sppEnemy[enemyIndex] = CharaEnemy::CreateInstance(kind);
	APP_ASSERT((sppEnemy[enemyIndex] != NULL),
			   "Failed CharaEnemy::CreateInstance() \n");

	CharaEnemy* p_enemy = sppEnemy[enemyIndex];

	p_enemy->LinkCharaResource(p_enemy_work->mResourceName);
	rad = sPlayer.Rotation().y + Utility::DegToRad(180.0f);
	float amp = 0.05f + ((float)enemyIndex * 0.001f);
	rad += ( ((float)GetRand(10) * amp) - (amp * 0.5f));
	
	p_enemy->Position() = *pPos;
	vec = VSub(sPlayer.Position(), p_enemy->Position());

	p_enemy->Rotation().y	= atan2(vec.x, vec.z) + Utility::DegToRad(180.0f);
	Utility::CorrectAngleFrom000To360(&p_enemy->Rotation().y);

	p_enemy->Scale()		= VGet(p_enemy_work->mScale, p_enemy_work->mScale, p_enemy_work->mScale);
	p_enemy->SetMoveSpeed(p_enemy_work->mMoveSpeed);

	p_enemy->Setup(p_enemy_work->mResourceName, 
				   p_enemy_work->mHitPoint, 
				   //				   p_enemy_work->mLayoutName);
				   p_enemy_work->mEnemyName);

	p_enemy->BodySize()	= p_enemy_work->mBodySize;
	p_enemy->BodyHeight()	= p_enemy_work->mBodyHeight;

	p_enemy->SetItemInfo(p_enemy_work->mGetItemKind , p_enemy_work->mGetItemPercent);

	GraphicManager::GetInstance()->EntryDrawModel(sppEnemy[enemyIndex]);	// �`��G���g���[
	CollisionManager::GetInstance()->EntryBodyCollision(sppEnemy[enemyIndex]);	// �����蔻��G���g���[
	CollisionManager::GetInstance()->EntryAttackCollision(sppEnemy[enemyIndex]);	// �����蔻��G���g���[

	p_enemy->SetPositionOnCollision();

	// �G�o���֐�
	p_enemy->AppearEnemy(enemyIndex * ((USHORT)kBASIC_FRAME_COUNTER * 30));

	// ���[�h��̃}�b�v���̃Z�b�g�A�b�v
	spStageFunc->NotifyCreateEnemy(enemyIndex);

	sIsBattle = true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
destroyAllEnemy(void)
{
	for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
		if (sppEnemy[iI]->GetIsActive() == true) {
			sppEnemy[iI]->ChangeState(CharaBase::eCHARA_STATE_RETIRE_END);
		}
	}
}

//---------------------------------------------------
/**
 * �����ǉ�
 */
//---------------------------------------------------
/*static*/void
addMyWeapon(eItemKind itemKind)
{
	std::vector<eItemKind>::iterator ite;
	for (ite = sMyWeaponList.begin(); ite != sMyWeaponList.end(); ite ++) {
		if (*ite == itemKind) {
			return;
		}
	}
	sMyWeaponList.push_back(itemKind);

	// �A�C�e���t���O�𗧂Ă�
	TotalInfo::GetInstance()->SetWeaponFlag(itemKind);
}

//---------------------------------------------------
/**
 * ������폜
 */
//---------------------------------------------------
/*static*/void
deleteMyWeapon(eItemKind itemKind)
{
	std::vector<eItemKind>::iterator ite;
	for (ite = sMyWeaponList.begin(); ite != sMyWeaponList.end(); ite ++) {
		if (*ite == itemKind) {
			sMyWeaponList.erase(ite);

			// �A�C�e���t���O�𗧂Ă�
			TotalInfo::GetInstance()->DisableWeaponFlag(itemKind);

			return;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
updateCsvAction(CsvAccesser* pCsvAccesser)
{
	if (CHECK_TRIG_JOY(JOY_INPUT_START)) {
		SoundManager::GetSndComponent()->PlaySound(eSE_PRESS_KEY);
		return true;
	}

	float start_time, end_time;
	char *chr_name, *msg1, *msg2, *msg3, *msg4;
	int colR, colG, colB;

	char* cmd = NULL;
	float check_frame = 0;

	int line_cnt = (pCsvAccesser->GetInt(0, 0) + 1);
	for (; line_cnt < (int)pCsvAccesser->GetRowNum(); line_cnt ++) {

		cmd			= pCsvAccesser->GetString(line_cnt, 0);
		check_frame = pCsvAccesser->GetFloat(line_cnt, 1);

		if (strcmp(cmd, "MSG") == 0) {
			start_time	= check_frame;
			end_time	= pCsvAccesser->GetFloat(line_cnt, 2);
			if (EventManager::GetInstance()->BetweenPhaseFrame(start_time, end_time)) {
				chr_name= pCsvAccesser->GetString(line_cnt, 3);
				msg1	= pCsvAccesser->GetString(line_cnt, 4);
				msg2	= pCsvAccesser->GetString(line_cnt, 5);
				msg3	= pCsvAccesser->GetString(line_cnt, 6);
				msg4	= pCsvAccesser->GetString(line_cnt, 7);
				colR	= pCsvAccesser->GetInt(line_cnt, 8);
				colG	= pCsvAccesser->GetInt(line_cnt, 9);
				colB	= pCsvAccesser->GetInt(line_cnt, 10);
				RENDER_MESSAGE_COL(chr_name, msg1, msg2, msg3, msg4, GetColor(colR, colG, colB));
				//	break;
			}
		} else if (strcmp(cmd, "MOV") == 0) {
			start_time	= check_frame;
			end_time	= pCsvAccesser->GetFloat(line_cnt, 2);
			if (EventManager::GetInstance()->BetweenPhaseFrame(start_time, end_time)) {
				int chr_index;
				VECTOR posA, posB, vec;
				float rate, now, length;
				float scale = pCsvAccesser->GetFloat(line_cnt, 6);

				chr_index = pCsvAccesser->GetInt(line_cnt, 3);
				posA = pCsvAccesser->GetVector(line_cnt, 4);
				posA = VScale(posA, scale);

				posB = pCsvAccesser->GetVector(line_cnt, 5);
				posB = VScale(posB, scale);

				vec = VSub(posB, posA);

				now = EventManager::GetInstance()->GetPhaseFrame();
				rate = (now - start_time) / (end_time - start_time);
				length = VSize(vec) * rate;
				
				ModelPmd* p_model = NULL;
				p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(chr_index);
				p_model->Position() = VAdd(VScale(VNorm(vec), length), posA);
			}
		}
		else if (strcmp(cmd, "TURN") == 0) {

			start_time	= check_frame;
			end_time	= pCsvAccesser->GetFloat(line_cnt, 2);
			if (EventManager::GetInstance()->BetweenPhaseFrame(start_time, end_time)) {

				static const float kTURN_MOVE_ROT = 10.0f * PHI_F / 180.0f;

				int chr_index	= pCsvAccesser->GetInt(line_cnt, 3);
				float targetrot = Utility::DegToRad(pCsvAccesser->GetFloat(line_cnt, 4));

				ModelPmd* p_model = NULL;
				p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(chr_index);

				/* �L�����N�^�[�̌������������� */
				Utility::CorrectAngleFrom000To360(&p_model->Rotation().y);

				float diff_rot = targetrot - p_model->Rotation().y;
	
				if (kTURN_MOVE_ROT >= abs(diff_rot)) {
					p_model->Rotation().y = targetrot;

				}
				else {
					bool is_plus = (diff_rot >= 0.0f) ? true : false;
					if (abs(diff_rot) > Utility::DegToRad(180.0f)) {
						is_plus ^= true;
					}
					p_model->Rotation().y += (is_plus) ? kTURN_MOVE_ROT : -kTURN_MOVE_ROT;
				}
			}
		}
		else if (strcmp(cmd, "END") == 0) {
			if (EventManager::GetInstance()->EqualPhaseFrame(check_frame)) {
				FADE_COL_BLACK();
				return true;
			}
		}
		else if (EventManager::GetInstance()->EqualPhaseFrame(check_frame)) {
			for (int cmd_cnt = 0; cmd_cnt < eCSV_CMD_MAX; cmd_cnt ++) {
				if (strcmp(cmd, sCsvCmdStringTable[cmd_cnt]) == 0) {
					(mCsvCmdFunc[cmd_cnt])(pCsvAccesser, line_cnt);
					break;
				}
			}
		}

		// �w��̃t���[���ڍs�̏ꍇ�͏I��
		if (EventManager::GetInstance()->EqualPhaseFrame(check_frame - 1.0f)) {
			// �b�r�u���ł̌������I������
			break;
		}
	}

	// ���̊֐��͂�������Ă΂�邱�Ƃ����҂��Ă���֐�
	EventManager::GetInstance()->UpdateEventAnimation();

	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
startCsvAction2nd(void)
{
	sCheckLineCnt = 0;
	sActiveExCmd  = (char)-1;
	sActiveExTime = 0;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
updateCsvAction2nd(CsvAccesser* pCsvAccesser)
{
	char* cmd = NULL;
	
	if (CHECK_TRIG_JOY(JOY_INPUT_START)) {
		SoundManager::GetSndComponent()->PlaySound(eSE_PRESS_KEY);
		sCheckLineCnt = 0;
		return true;
	}

	PRINT_SCREEN(GetColor(255, 255, 0), "sCheckLineCnt = %d", sCheckLineCnt);

	// ����R�}���h���{���̏ꍇ�́A���ݎ��s���̃R�}���h�̏������s��
	switch (sActiveExCmd) {
	case eEXCMD_MSG:
		{
			char *chr_name, *msg1, *msg2, *msg3, *msg4;
			int colR, colG, colB;
			chr_name= pCsvAccesser->GetString(sCheckLineCnt, 3);
			msg1	= pCsvAccesser->GetString(sCheckLineCnt, 4);
			msg2	= pCsvAccesser->GetString(sCheckLineCnt, 5);
			msg3	= pCsvAccesser->GetString(sCheckLineCnt, 6);
			msg4	= pCsvAccesser->GetString(sCheckLineCnt, 7);
			colR	= pCsvAccesser->GetInt(sCheckLineCnt, 8);
			colG	= pCsvAccesser->GetInt(sCheckLineCnt, 9);
			colB	= pCsvAccesser->GetInt(sCheckLineCnt, 10);
			RENDER_MESSAGE_COL(chr_name, msg1, msg2, msg3, msg4, GetColor(colR, colG, colB));
		}
		break;
	case eEXCMD_MOV:
		{
			int chr_index;
			VECTOR posA, posB, vec;
			float rate, length;
			float scale = pCsvAccesser->GetFloat(sCheckLineCnt, 6);

			chr_index = pCsvAccesser->GetInt(sCheckLineCnt, 3);
			posA = pCsvAccesser->GetVector(sCheckLineCnt, 4);
			posA = VScale(posA, scale);
			posB = pCsvAccesser->GetVector(sCheckLineCnt, 5);
			posB = VScale(posB, scale);
			vec = VSub(posB, posA);
			float total, remain;
			total = (SHORT)pCsvAccesser->GetFloat(sCheckLineCnt, 2);
			remain = sActiveExTime;
			rate = (total - remain) / total;

			length = VSize(vec) * rate;
				
			ModelPmd* p_model = NULL;
			p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(chr_index);
			p_model->Position() = VAdd(VScale(VNorm(vec), length), posA);
		}
		break;
	case eEXCMD_TURN:
		{
			static const float kTURN_MOVE_ROT = 10.0f * PHI_F / 180.0f;
			int chr_index	= pCsvAccesser->GetInt(sCheckLineCnt, 3);
			float targetrot = Utility::DegToRad(pCsvAccesser->GetFloat(sCheckLineCnt, 4));
			ModelPmd* p_model = NULL;
			p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(chr_index);
			/* �L�����N�^�[�̌������������� */
			Utility::CorrectAngleFrom000To360(&p_model->Rotation().y);
			float diff_rot = targetrot - p_model->Rotation().y;
			if (kTURN_MOVE_ROT >= abs(diff_rot)) {
				p_model->Rotation().y = targetrot;
			} else {
				bool is_plus = (diff_rot >= 0.0f) ? true : false;
				if (abs(diff_rot) > Utility::DegToRad(180.0f)) {
					is_plus ^= true;
				}
				p_model->Rotation().y += (is_plus) ? kTURN_MOVE_ROT : -kTURN_MOVE_ROT;
			}
		}
		break;
	case eEXCMD_WAIT:
		// ���������Ђ�����҂�
		break;
	case eEXCMD_UNTIL:
#ifdef _DEBUG
		APP_ASSERT((EventManager::GetInstance()->GetPhaseFrame() <= sActiveExTime),
				   "eEXCMD_UNTIL, over count frame \n");
#endif // _DEBUG
		// �w��t���[���܂ł܂�
		if (EventManager::GetInstance()->EqualPhaseFrame(sActiveExTime)) {
			sActiveExCmd = (char)eEXCMD_NONE;
			sActiveExTime = 0;
			sCheckLineCnt ++;
		}
		break;
	case eEXCMD_WAITCAM:
		// �w��̃J�����ʒu�ɂȂ�܂ő҂�
		if (EventManager::GetInstance()->GetNextKey() == sActiveExTime) {
			sActiveExCmd = (char)eEXCMD_NONE;
			sActiveExTime = 0;
			sCheckLineCnt ++;
		}
		break;

	case eEXCMD_NONE:
		// �f�t�H���g��ʂ����߂̒�`
		break;
	default:
		APP_ASSERT(NULL, "Not Support ExCommand function \n");
		break;
	};
	// ����R�}���h�̃J�E���^���Z�����ƁA���̏I������
	if (( sActiveExCmd != eEXCMD_NONE) && ( sActiveExCmd != eEXCMD_UNTIL) && (sActiveExCmd != eEXCMD_WAITCAM)) {
		if ( (sActiveExTime - 1) <= 0) {
			sActiveExCmd = (char)eEXCMD_NONE;
			sActiveExTime = 0;
			sCheckLineCnt ++;
		} else {
			sActiveExTime --;
		}
	}


	//�@CSV�t�@�C���̍s�����ꂼ�ꏈ�����Ă����A�Y���̃R�}���h���`�F�b�N���ď������Ă���
	if (sActiveExCmd == eEXCMD_NONE) {
		for(;;) {
			cmd = pCsvAccesser->GetString(sCheckLineCnt, 0);
			// �ʏ�R�}���h�̃`�F�b�N�Ǝ��{
			for (int cmd_cnt = 0; cmd_cnt < eCSV_CMD_MAX; cmd_cnt++) {
				if (strcmp(cmd, sCsvCmdStringTable[cmd_cnt]) == 0 ){
					(mCsvCmdFunc[cmd_cnt])(pCsvAccesser, sCheckLineCnt);
					break;
				}
			}
			// ����R�}���h�̃`�F�b�N�Ǝ��{
			if (strcmp(cmd, "MSG") == 0) {
				sActiveExCmd = eEXCMD_MSG;
				sActiveExTime = (SHORT)pCsvAccesser->GetFloat(sCheckLineCnt, 2);
#ifdef _DEBUG
				PRINT_CONSOLE("Evt MSG log: line=%d, frame=%.1f, msg=%s\n", 
							  sCheckLineCnt, 
							  EventManager::GetInstance()->GetPhaseFrame(), 
							  pCsvAccesser->GetString(sCheckLineCnt, 4) );
#endif // _DEBUG
				break;
			} else if (strcmp(cmd, "MOV") == 0) {
				sActiveExCmd = eEXCMD_MOV;
				sActiveExTime = (SHORT)pCsvAccesser->GetFloat(sCheckLineCnt, 2);
				break;
			} else if (strcmp(cmd, "TURN") == 0) {
				sActiveExCmd = eEXCMD_TURN;
				sActiveExTime = (SHORT)pCsvAccesser->GetFloat(sCheckLineCnt, 2);
				break;
			} else if (strcmp(cmd, "WAIT") == 0) {
				sActiveExCmd = eEXCMD_WAIT;
				sActiveExTime = (SHORT)pCsvAccesser->GetFloat(sCheckLineCnt, 2);
				break;
			} else if (strcmp(cmd, "UNTIL") == 0) {
				sActiveExCmd = eEXCMD_UNTIL;
				sActiveExTime = (SHORT)pCsvAccesser->GetFloat(sCheckLineCnt, 2);
				break;
			} else if (strcmp(cmd, "WAITCAM") == 0) {
				sActiveExCmd = eEXCMD_WAITCAM;
				sActiveExTime = (SHORT)pCsvAccesser->GetFloat(sCheckLineCnt, 2);
				break;
			} else if (strcmp(cmd, "END") == 0) {
				FADE_COL_BLACK();
				sCheckLineCnt = 0;
				return true;
			}
			sCheckLineCnt ++;
		}
	}

	// ���̊֐��͂�������Ă΂�邱�Ƃ����҂��Ă���֐�
	EventManager::GetInstance()->UpdateEventAnimation();

	return false;
}

#ifdef _DEBUG
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
saveCsvActionDebug(void)
{
	sCsvActionDebug.mCheckLineCnt	= sCheckLineCnt;
	sCsvActionDebug.mActiveExCmd	= sActiveExCmd;
	sCsvActionDebug.mActiveExTime	= sActiveExTime;

	sCsvActionDebug.mFrameCounter	= EventManager::GetInstance()->GetPhaseFrame();
	sCsvActionDebug.mNextKey		= EventManager::GetInstance()->GetNextKey();

	sCsvActionDebug.mCamPos			=  CameraManager::GetInstance()->Position();
	sCsvActionDebug.mCamTar			=  CameraManager::GetInstance()->Target();
	sCsvActionDebug.mCamFov			=  GetCameraFov();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
loadCsvActionDebug(void)
{
	if (sCsvActionDebug.mCheckLineCnt == 0) {
		PRINT_CONSOLE("not save sCsvActionDebug info \n");
		return;
	}

	sCheckLineCnt	= sCsvActionDebug.mCheckLineCnt;
	sActiveExCmd	= sCsvActionDebug.mActiveExCmd;
	sActiveExTime	= sCsvActionDebug.mActiveExTime;
	
	EventManager::GetInstance()->SetPhaseFrame(sCsvActionDebug.mFrameCounter);
	EventManager::GetInstance()->SetNextKey(sCsvActionDebug.mNextKey);

	CameraManager::GetInstance()->Position()= sCsvActionDebug.mCamPos;
	CameraManager::GetInstance()->Target()	= sCsvActionDebug.mCamTar;
	CameraManager::GetInstance()->SetPerspectiveCamera(sCsvActionDebug.mCamFov);
}
#endif // _DEBUG

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void DrawFontFunc(void)
{
	int color = GetColor(255, 255, 255);

	static const tDrawFontWork sDrawFontWork[] = {
		{{"STAGE�P�@���x���j�A���� �n�����H",	"�n�����H�̈���",				"",""}, 320, 140, 30},
		{{"STAGE�Q�@���x���j�A���� ����",		"���x���j�A�̔��ƈł̎i��",	"",""}, 320, 140, 30},
		{{"STAGE�R�@�Y�p�̐X",					"�X�ɖ���N���X�^��",			"",""}, 320, 140, 30},
		{{"STAGE�S�@�X�m���g�z�R",				"�Ñ�s�s�ւƑ������ƈł̓�",	"",""}, 320, 140, 30},
		{{"STAGE�T�@�Ñ�s�s�n���b�p",			"���mVS�Ñ㕶��",				"",""}, 320, 140, 30},
		{{"STAGE�U�@���˂������s�s_-_Lazuline_City", "���m�Ɛ��̏���",			"",""}, 320, 140, 30},
		{{"STAGE�V�@�󒆒뉀",					"���l�����Z�ލ�"	,			"",""}, 320, 140, 30},
		{{"STAGE�W�@���x���j�A���� ��w��",		"�łт䂭���x���j�A",			"",""}, 320, 140, 30},
		{{"         STAGE �X�@����̊Ԃ̐��E",	"�Ō�̊�]",					"",""}, 320, 140, 30},
	};
	APP_ASSERT_PRINT((sDungeonIndex < (sizeof(sDrawFontWork) / sizeof(tDrawFontWork))),
					 "Invalid Access %d to sDrawFontWork[] \n", sDungeonIndex);

	const tDrawFontWork* p_work = &sDrawFontWork[sDungeonIndex];

	ChangeFont("HG�ۺ޼��M-PRO");
	GraphicManager::GetInstance()->DrawFontStringCenter(p_work->mSetX, p_work->mSetY,
														p_work->mMessage[0], color, p_work->mFontSize);
	GraphicManager::GetInstance()->DrawFontStringCenter(p_work->mSetX, p_work->mSetY + (int)(p_work->mFontSize*1.2f),	
														p_work->mMessage[1], color, p_work->mFontSize);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void DrawFreeFontFunc(void)
{
	//	int color = GetColor(255, 255, 255);
	int color = GetColor(0, 0, 0);

	static const tDrawFontWork sDrawFontWork[eFREE_FONT_MAX] = {
		{{"�u�͂���E�E�E�v", "" ,"",""},				260, 64,	30},		// eFREE_FONT_APPEAR_SKYSHIP_EVT_0,
		{{"�u���̐�Ɍ������đ���I�I�v", "" ,"",""},	260, 64,	30},		// eFREE_FONT_APPEAR_SKYSHIP_EVT_1,
		{{"���{�^���@���肤��", "" ,"",""},				64,  220,	30},		// eFREE_FONT_APPEAR_SKYSHIP_EVT_2,
	};
	APP_ASSERT_PRINT((sDrawFreeFontValue < eFREE_FONT_MAX),
					  "Invalid Access %d to sDrawFontWork[] \n", sDrawFontWork);
	const tDrawFontWork* p_work = &sDrawFontWork[sDrawFreeFontValue];

	ChangeFont("HG�ۺ޼��M-PRO");
	GraphicManager::GetInstance()->DrawFontString(p_work->mSetX, p_work->mSetY,
												  p_work->mMessage[0], color, p_work->mFontSize);
	GraphicManager::GetInstance()->DrawFontString(p_work->mSetX, p_work->mSetY + (int)(p_work->mFontSize*1.2f),	
												  p_work->mMessage[1], color, p_work->mFontSize);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
visibleNpcAll(bool isVisible)
{
	if (spNpc == NULL) {
		return;
	}

	for (int iI = 0; iI < sNpcNum; iI ++) {
		spNpc[iI].SetVisible(isVisible);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
visibleObjectAll(bool isVisible)
{
	if (sppObjectBase == NULL) {
		return;
	}

	for (int iI = 0; iI < sObjectBaseNum; iI ++) {
		sppObjectBase[iI]->SetVisible(isVisible);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
getStageIndex(void)
{
	return sStageIndex;
}

//---------------------------------------------------
/**
// �N���X�^���o�����o
// crystalColor : 0 Green
//				: 1 Yellow 
//				: ���͖�����
*/
//---------------------------------------------------
void processAppearCrystal(int stagePlaceIndex, ObjectBase* pCrystalObject, BYTE crystalColor,
						  BOOL isEfk/*=TRUE*/, int* pEffHndAura/*=NULL*/, int* pEffHndCrystal/*=NULL*/)
{
	VECTOR eff_pos;
	float scale = 0.0f;
	float rot;
	int graph_handle = -1;
	int eff_hnd_tmp = -1;

	getPlayerPosRotFromBGModel(&eff_pos, &rot, stagePlaceIndex);
	eff_hnd_tmp = EffectManager::GetInstance()->Play(eEF_AURA, &eff_pos, &VGet(370.0f, 50.0f, 370.0f) );
	if (pEffHndAura != NULL) {
		*pEffHndAura = eff_hnd_tmp;		// �Ăяo�����Ƀn���h�����Ǘ�������
	}

	eff_pos.y += 190.0f;
	switch (crystalColor) {
	case 0: /* Green */
		eff_hnd_tmp = EffectManager::GetInstance()->Play(eEF_GREEN, &eff_pos, 140.0f);
		graph_handle = ResourceLayout::GetInstance()->GetHandle(RES_LYT_CRYSTAL_GREEN);
		MV1SetTextureGraphHandle(pCrystalObject->GetModelHandle(), 0, graph_handle, FALSE);
		break;
	case 1: /* Yellow */
		eff_hnd_tmp = EffectManager::GetInstance()->Play(eEF_MARUI, &eff_pos, 140.0f);
		graph_handle = ResourceLayout::GetInstance()->GetHandle(RES_LYT_CRYSTAL_YELLOW);
		MV1SetTextureGraphHandle(pCrystalObject->GetModelHandle(), 0, graph_handle, FALSE);
		break;
	default:
		APP_ASSERT(NULL, "Not Support crystal color \n");
		break;
	};
	if (pEffHndCrystal != NULL) {
		*pEffHndCrystal = eff_hnd_tmp;		// �Ăяo�����Ƀn���h�����Ǘ�������
	}

	if (isEfk) {
		EffekseerManager::GetInstance()->Play(eEFK_GETKEYITEM, &eff_pos, 10.0f);
	}

	scale = ObjectBase::GetObjectBaseParam(eOBJECT_KIND_CRYSTAL)->mScale;
	pCrystalObject->Scale() = VGet(scale, scale, scale);

	COLOR_F material = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	pCrystalObject->SetEnableEmissive(true);
	pCrystalObject->SetDiffuse(material);
	pCrystalObject->SetEmissive(material);
	pCrystalObject->SetAmbient(material);
	pCrystalObject->SetSpecular(material);
}

//---------------------------------------------------
/**
   eOBJECT_KIND_XXX ���w�肵�A�ŏ��Ɍ������� ObjectBase��Ԃ��A������Ȃ��ꍇ��NULL��Ԃ�
 */
//---------------------------------------------------
ObjectBase*	search1stFindObjectKind(int objectKind)
{
	ObjectBase* p_return = NULL;

	for (int iI = 0; iI < sObjectBaseNum; iI ++) {
		if (sppObjectBase[iI]->GetObjectKind() == objectKind) {
			return sppObjectBase[iI];
		}
	}

	return p_return;
}

//---------------------------------------------------
/**
   pModelMap �ɗ\�߃��[�h����Ă��郂�f����j�����A���̗̈�ɐV���ȃ}�b�v���f�������[�h����
 */
//---------------------------------------------------
void reloadModelMap(ModelMap* pModelMap, const char* strModelName, bool isCollision)
{
	// �\�߃��[�h�̃��f����j��
	GraphicManager::GetInstance()->RemoveDrawMapModel(pModelMap);
	CollisionManager::GetInstance()->RemoveEarthCollision(pModelMap->GetModelHandle(), eBG_TYPE_EARTH);
	CollisionManager::GetInstance()->RemoveWallCollision(pModelMap->GetModelHandle(), eBG_TYPE_WALL);
	CollisionManager::GetInstance()->RemoveEarthCollision(pModelMap->GetModelHandle(), eBG_TYPE_ALPHA_EARTH);
	CollisionManager::GetInstance()->RemoveWallCollision(pModelMap->GetModelHandle(), eBG_TYPE_ALPHA_WALL);

	pModelMap->DeleteModel();		

	// ���� pModelMap �̗̈�� strModelName �̃}�b�v���i�[����
	pModelMap->LoadModel(strModelName);
	pModelMap->Setup(spBGInfo->mScale);
	pModelMap->Position()	= ZERO_VEC;
	pModelMap->Scale()	= VGet(spBGInfo->mScale, spBGInfo->mScale, spBGInfo->mScale);
	GraphicManager::GetInstance()->EntryDrawMapModel(pModelMap);

	if (isCollision) {
		CollisionManager::GetInstance()->EntryEarthCollision(pModelMap->GetModelHandle(), eBG_TYPE_EARTH);
		CollisionManager::GetInstance()->EntryWallCollision(pModelMap->GetModelHandle(), eBG_TYPE_WALL);
		
		CollisionManager::GetInstance()->EntryEarthCollision(pModelMap->GetModelHandle(), eBG_TYPE_ALPHA_EARTH);
		CollisionManager::GetInstance()->EntryWallCollision(pModelMap->GetModelHandle(), eBG_TYPE_ALPHA_WALL);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void revertVehicle()
{
	for (int iI = 0; iI < sNpcNum; iI ++) {
		if (strcmp(CharaNpc::GetNpcResourceName(iI), NAME_Titanis) == 0) {
			spNpc[iI].Scale() = CHARA_DEF_SCALE;
			spNpc[iI].Position() = sPlayer.Position();
			spNpc[iI].SetPositionOnCollision();
			spNpc[iI].Position().y += 1.0f;

			spNpc[iI].Rotation().y = sPlayer.Rotation().y;
			
			spNpc[iI].EnableShadow();
			spNpc[iI].SetupOnceDrawShadow();
			break;
		}
	}
	
	sPlayer.Position().y += 100.0f;

	SoundManager::GetInstance()->PlayBGM(spBGInfo->mMusicName);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void revertGuardian()
{
	CharaGuardian* p_guardian = spStageFunc->GetGuardianPtr();
	APP_ASSERT((p_guardian != NULL),
			   "p_guardin is NULL \n");
	
	// �}�W�b�N�A�[�}�[�̕\�������ɖ߂�
	p_guardian->DeleteModel();
	p_guardian->LinkModel(NAME_Guardian);
	p_guardian->Setup();
	p_guardian->StartMotion(0, true, 0.5f);
	p_guardian->SetPositionOnCollision();
	p_guardian->Scale() = CHARA_DEF_SCALE;

	p_guardian->ChangeState(CharaGuardian::eCHARA_STATE_BOOTDOWN);

	// �X�t�B�A�̐ݒ�̕ύX
	MV1SetMaterialSphereMapBlendTypeAll(sPlayer.GetModelHandle(),
										DX_MATERIAL_BLENDTYPE_ADDITIVE); // ���Z

	// �Q�̃��f���̈ʒu����������
	VECTOR tmp_pos = sPlayer.Position();
	sPlayer.Position() = p_guardian->Position();
	p_guardian->Position() = tmp_pos;

	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
	CameraManager::GetInstance()->ResetCameraPosY();
	CameraManager::GetInstance()->StartDeleyCamera(CameraManager::eDELAY_SPEED_LOW, true);

	// �Q�̃��f���̉�]����������
	float tmp_rot = sPlayer.Rotation().y;
	sPlayer.Rotation().y = p_guardian->Rotation().y;
	p_guardian->Rotation().y = tmp_rot;
		
	// �T�u�E�F�|����\��������
	ModelPmd* p_attach_mdl = NULL;
	p_attach_mdl = (ModelPmd *)sPlayer.GetAttachAddr(CharaPlayer::eATTACH_MODEL_SUB_WEAPON)->mpAttachedModel;
	GraphicManager::GetInstance()->ReserveEntryModel(p_attach_mdl);	// �`��G���g���[�\��

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void setDefaultBloomSetting(void)
{
	RenderBloom::SetBloomType(RenderBloom::eBLOOM_TYPE_HSB_RELATIVE);
	RenderBloom::SetBloomParam(11, 195, 10);
	RenderBloom::tBloomHSBParam param = {0};
	param.mHue = 10;param.mSaturation = 200;param.mBright = 90;
	RenderBloom::BloomHSBParam() = param;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void createDropItem(int objectKind, VECTOR& rSetPos)
{
	for (int iI = 0; iI < kENEMY_ITEM_NUM; iI ++) {
		if (sDropItem[iI].GetIsActive() == false) {
			ObjectItem* p_item = &sDropItem[iI];
			p_item->Setup(objectKind, (eItemKind)0, &rSetPos, 0, true);
			p_item->SetPositionOnCollision();
			p_item->SetRotationToTarget(&sPlayer.Position());
			break;
		}
	}
	

}

/*=====================================*

 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
funcCsvCmd_SHOW_ITEM(CsvAccesser* pCsvAccesser, int iI)
{
	ModelBase* p_model = NULL;
	p_model = &sModelXfile[eMD_GETTED_ITEM];
	//				p_model->LinkModel(RES_MDL_MASTERSWORD);
	p_model->LinkModel(pCsvAccesser->GetString(iI, 4));
	p_model->DisableToonShader();
	GraphicManager::GetInstance()->EntryDrawModel(p_model);
	p_model->Position() = sPlayer.Position();
	p_model->Position().y += (sPlayer.BodyHeight() * 1.0f);
	p_model->Scale() = sPlayer.Scale();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
funcCsvCmd_VC(CsvAccesser* pCsvAccesser, int iI)
{
	char* chr_name;
	int vc_index;

	chr_name		= pCsvAccesser->GetString(iI, 2);
	vc_index		= pCsvAccesser->GetInt(iI, 3);
	SoundManager::GetVcComponent(chr_name)->PlaySound(vc_index);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
funcCsvCmd_SND(CsvAccesser* pCsvAccesser, int iI)
{
	int vc_index;
	vc_index		= pCsvAccesser->GetInt(iI, 2);
	SoundManager::GetSndComponent()->PlaySound(vc_index);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
funcCsvCmd_LYT(CsvAccesser* pCsvAccesser, int iI)
{
	char* file_name;
	int fade_alpha, start_alpha;
	float fade_sec;
	int pos_x, pos_y, size_x, size_y;
	int layerr;
	int set_size_x, set_size_y;

	start_alpha	= pCsvAccesser->GetInt(iI, 2);
	fade_alpha	= pCsvAccesser->GetInt(iI, 3);
	fade_sec	= pCsvAccesser->GetFloat(iI, 4);
	file_name	= pCsvAccesser->GetString(iI, 5);
	pos_x		= pCsvAccesser->GetInt(iI, 6);
	pos_y		= pCsvAccesser->GetInt(iI, 7);
	size_x		= pCsvAccesser->GetInt(iI, 8);
	size_y		= pCsvAccesser->GetInt(iI, 9);
	layerr		= pCsvAccesser->GetInt(iI, 10);

	LayoutBase* p_layout = EventManager::GetInstance()->GetLayoutBase(layerr);

	p_layout->LinkLayout(file_name);
	p_layout->SetPositionByCentering(pos_x, pos_y);

	p_layout->GetSize(&set_size_x, &set_size_y);
	if (size_x != -1) {	// �ݒ�l������ꍇ�̂ݍX�V����
		set_size_x = size_x;
	}
	if (size_y != -1) {	// �ݒ�l������ꍇ�̂ݍX�V����
		set_size_y = size_y;
	}	
	p_layout->SetSize(set_size_x, set_size_y);
	p_layout->SetAlpha(start_alpha);
	p_layout->StartFade(fade_alpha, fade_sec);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
funcCsvCmd_F_IN(CsvAccesser* pCsvAccesser, int iI)
{
	float fade_time;
	fade_time	= pCsvAccesser->GetFloat(iI, 2);
	if (strcmp(pCsvAccesser->GetString(iI, 3), "WHITE") == 0) {
		FADE_COL_WHITE();
	} else if (strcmp(pCsvAccesser->GetString(iI, 3), "BLUE") == 0) {
		FADE_COL_BLUE();
	} else {
		FADE_COL_BLACK();
	}
	FADE_IN(fade_time);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_F_OUT(CsvAccesser* pCsvAccesser, int iI)
{
	float fade_time;
	fade_time	= pCsvAccesser->GetFloat(iI, 2);
	if (strcmp(pCsvAccesser->GetString(iI, 3), "WHITE") == 0) {
		FADE_COL_WHITE();
	} else if (strcmp(pCsvAccesser->GetString(iI, 3), "BLUE") == 0) {
		FADE_COL_BLUE();
	} else {
		FADE_COL_BLACK();
	}
	FADE_OUT(fade_time);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_FADE(CsvAccesser* pCsvAccesser, int iI)
{
	float fade_time;
	fade_time	= pCsvAccesser->GetFloat(iI, 2);
	int fade_val = pCsvAccesser->GetInt(iI, 4);
	if (strcmp(pCsvAccesser->GetString(iI, 3), "WHITE") == 0) {
		FADE_COL_WHITE();
	} else if (strcmp(pCsvAccesser->GetString(iI, 3), "BLUE") == 0) {
		FADE_COL_BLUE();
	} else {
		FADE_COL_BLACK();
	}
	//				FADE_OUT(fade_time);
	FADE_VAL(fade_time, fade_val);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_FOG(CsvAccesser* pCsvAccesser, int iI)
{
	int r, g, b;
	float fnear, ffar, sec;
	r	= pCsvAccesser->GetInt(iI, 2);
	g	= pCsvAccesser->GetInt(iI, 3);
	b	= pCsvAccesser->GetInt(iI, 4);
	fnear= pCsvAccesser->GetFloat(iI, 5);
	ffar	= pCsvAccesser->GetFloat(iI, 6);
	sec = pCsvAccesser->GetFloat(iI, 7);

	if (fnear == ffar) {
		// �t�H�O����
		GraphicManager::GetInstance()->RevertFogParam(sec);
	} else {
		GraphicManager::tFogParam fogParam;
		fogParam.mIsEnable = TRUE;
		fogParam.mIsActive = TRUE;
		fogParam.mColR = r;
		fogParam.mColG = g;
		fogParam.mColB = b;
		fogParam.mNear = fnear;
		fogParam.mFar = ffar;
		GraphicManager::GetInstance()->ChangeFogParam(sec, &fogParam);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_M_IN(CsvAccesser* pCsvAccesser, int iI)
{
	SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_IN);
	MSG_TYPE_MESSAGE();
	MSG_FADE_IN(0.3f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_M_OUT(CsvAccesser* pCsvAccesser, int iI)
{
	SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_OUT);
	MSG_TYPE_MESSAGE();
	MSG_FADE_OUT(0.4f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_T_IN(CsvAccesser* pCsvAccesser, int iI)
{
	MSG_TYPE_TELOP();
	MSG_FADE_IN(0.3f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_T_OUT(CsvAccesser* pCsvAccesser, int iI)
{
	MSG_TYPE_TELOP();
	MSG_FADE_OUT(0.4f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_TOON(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd* p_model;
	p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(0);
	p_model->EnableToonShader(pCsvAccesser->GetFloat(iI, 2),
							  pCsvAccesser->GetFloat(iI, 3));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_SHAKE(CsvAccesser* pCsvAccesser, int iI)
{
	float sec = pCsvAccesser->GetFloat(iI, 2);
	float width = pCsvAccesser->GetFloat(iI, 3);
	float speed = pCsvAccesser->GetFloat(iI, 4);
	CameraManager::GetInstance()->StartShakeCamera(sec, width, speed);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_EFFECT(CsvAccesser* pCsvAccesser, int iI)
{
	int effect_index = pCsvAccesser->GetInt(iI, 2);
	VECTOR eff_pos;
	int model_index = pCsvAccesser->GetInt(iI, 3);
	char* frame_name = pCsvAccesser->GetString(iI, 4);
	float size = pCsvAccesser->GetFloat(iI, 5);
	float sec = pCsvAccesser->GetFloat(iI, 6);
	ModelPmd* p_model = NULL;
	if (model_index == -1) {
		p_model = (ModelPmd *)&sPlayer;
		p_model->GetFramePosFromName(&eff_pos, frame_name);
	} else if (model_index == -2) {
		eff_pos = pCsvAccesser->GetVector(iI, 7); // Pos�w��̏ꍇ�̂ݑ�V�p�����[�^�L��
	} else {
		p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);
		p_model->GetFramePosFromName(&eff_pos, frame_name);
	}
	int eff_handle = EffectManager::GetInstance()->Play(effect_index,
														&eff_pos,
														&VGet(size, size, size),
														0.0f);
	EffectManager::GetInstance()->Stop(eff_handle, sec);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_EFK(CsvAccesser* pCsvAccesser, int iI)
{
	int effect_index = pCsvAccesser->GetInt(iI, 2);
	VECTOR eff_pos;
	int model_index = pCsvAccesser->GetInt(iI, 3);
	char* frame_name = pCsvAccesser->GetString(iI, 4);
	float size = pCsvAccesser->GetFloat(iI, 5);
	float sec = pCsvAccesser->GetFloat(iI, 6);
	ModelPmd* p_model = NULL;
	if (model_index == -1) {
		p_model = (ModelPmd *)&sPlayer;
	} else {
		p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);
	}
	p_model->GetFramePosFromName(&eff_pos, frame_name);

	EFK_HANDLE handle;
	handle = EffekseerManager::GetInstance()->Play(effect_index,
												   &eff_pos, 
												   &VGet(size, size, size));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_CAM(CsvAccesser* pCsvAccesser, int iI)
{
	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);

	VECTOR pos, tar;
	pos.x = pCsvAccesser->GetFloat(iI, 2);
	pos.y = pCsvAccesser->GetFloat(iI, 3);
	pos.z = pCsvAccesser->GetFloat(iI, 4);
	tar.x = pCsvAccesser->GetFloat(iI, 5);
	tar.y = pCsvAccesser->GetFloat(iI, 6);
	tar.z = pCsvAccesser->GetFloat(iI, 7);

	CameraManager::GetInstance()->Position()= pos;
	CameraManager::GetInstance()->Target()	= tar;
}

//---------------------------------------------------
/**
 * ���f���̕\���E��\���̐ݒ�ƁA���f���̃��l��ݒ�
 */
//---------------------------------------------------
void funcCsvCmd_SHOW(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd* p_model = NULL;
	int		model_index = pCsvAccesser->GetInt(iI, 2);
	bool	is_visible	= pCsvAccesser->GetBool(iI, 3);
	float	model_alpha = pCsvAccesser->GetFloat(iI, 4);
	bool	is_shadow	= pCsvAccesser->GetBool(iI, 5);

	if (model_index == -1) {

		sModelMap.SetVisible(is_visible);
		for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
			spSubModelMap[iI].SetVisible(is_visible);
		}
		sModelXfile[eMD_SKYDOME].SetVisible(is_visible);
		
	} else {
		p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);
		
		// �\���ݒ�
		p_model->SetVisible(is_visible);
		
		// ���f���̃��l��ݒ�
		p_model->SetAlpha(model_alpha);

		// �e�̕\��
		if (is_shadow) {
			((CharaNpc *)p_model)->EnableShadow();
		} else {
			((CharaNpc *)p_model)->DisableShadow();
		}
	}
}

//---------------------------------------------------
/**
 * //<! ���W�Ɋւ��Ă� spBGInfo->mScale����������̂Œ��ӁI
 */
//---------------------------------------------------
void funcCsvCmd_POSITION(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd* p_model = NULL;
	int model_index = pCsvAccesser->GetInt(iI, 2);
	VECTOR shift_pos;

	p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);
	
	p_model->Position() = VScale(VGet(pCsvAccesser->GetFloat(iI, 3),
									  pCsvAccesser->GetFloat(iI, 4),
									  pCsvAccesser->GetFloat(iI, 5)),
								 spBGInfo->mScale);								//<! BG�̃X�P�[���l���������Ă���̂Œ��ӁI
	p_model->Rotation() = VGet(Utility::DegToRad(pCsvAccesser->GetFloat(iI, 6)),
							   Utility::DegToRad(pCsvAccesser->GetFloat(iI, 7)),
							   Utility::DegToRad(pCsvAccesser->GetFloat(iI, 8)));

	// �C�x���g�}�l�[�W������V�t�g���W�����߂ċ������V�t�g
	EventManager::GetInstance()->GetShiftCameraPosition(&shift_pos);
	p_model->Position() = VAdd(p_model->Position(), shift_pos);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_SETPOS(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd* p_model = NULL;
	VECTOR setpos = ZERO_VEC;
	int model_index = pCsvAccesser->GetInt(iI, 2);
	char* frame_name = pCsvAccesser->GetString(iI, 3);
	p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);

	if (strcmp(frame_name, "Pos") == 0) {
		setpos = pCsvAccesser->GetVector(iI, 4);
	} else {
		p_model->GetFramePosFromName(&setpos, frame_name);
	}
	p_model->Position() = setpos;

	p_model->SetPositionOnCollision();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_SETROT(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd *p_mine, *p_your;
	int mine_index = pCsvAccesser->GetInt(iI, 2);
	int your_index = pCsvAccesser->GetInt(iI, 3);
	p_mine = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(mine_index);
	if (your_index != -1) {
		p_your = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(your_index);
		p_mine->SetRotationToTarget(&p_your->Position());
	} else {
		float set_rot = pCsvAccesser->GetFloat(iI, 4);
		p_mine->Rotation().y = Utility::DegToRad(set_rot);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_SETSCA(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd *p_model;
	int model_index = pCsvAccesser->GetInt(iI, 2);
	float set_scale =  pCsvAccesser->GetFloat(iI, 3);

	p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);
	if (set_scale == 0.0f) {
		((CharaNpc *)p_model)->DisableShadow();
	} else {
		((CharaNpc *)p_model)->EnableShadow();
	}
	p_model->Scale() = VGet(set_scale, set_scale, set_scale);
}

//---------------------------------------------------
/**
   No  : 0,      1,     2,     3,      4,       5,      6
   �Ӗ�: cmd,    frame, mdlno, posX,   posY,    posZ,   rotY
   --------------------------------------------------
   ��  : POSROT, 0,     3,     227.46, 1813.05, 784.36, -67.6,
 */
//---------------------------------------------------
void funcCsvCmd_POSROT(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd* p_model = NULL;
	VECTOR setpos = ZERO_VEC;
	int model_index = -1;
	float setrot = 0.0f;
	bool is_disable_on_collision = false;

	model_index = pCsvAccesser->GetInt(iI, 2);
	setpos.x	= pCsvAccesser->GetFloat(iI, 3);
	setpos.y	= pCsvAccesser->GetFloat(iI, 4);
	setpos.z	= pCsvAccesser->GetFloat(iI, 5);

	setrot		= pCsvAccesser->GetFloat(iI, 6);
	is_disable_on_collision = pCsvAccesser->GetBool(iI, 7);

	p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);

	// MMD�̐ݒ�����̂܂ܐݒ�ł���l�ɕ␳��������
	setpos = VScale(setpos, 10.0f);						//<! �S�Ẵ��f��(�L������)��10�{����̂ŁA10�{������
	p_model->Position() = setpos;

	// MMD�̐ݒ�����̂܂ܐݒ�ł���l�ɕ␳��������
	setrot *= -1.0f;
	p_model->Rotation().y = Utility::DegToRad(setrot);

	// ��7������ false (�܂��͎w��Ȃ�)�̏ꍇ�� SetPositionOnCollision()�ɂĕ␳���s��
	if (is_disable_on_collision == false) {
		p_model->SetPositionOnCollision();
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_MDLANM(CsvAccesser* pCsvAccesser, int iI)
{
	int			model_index;
	char*		sub_cmd;

	model_index			= pCsvAccesser->GetInt(iI, 2);
	sub_cmd				= pCsvAccesser->GetString(iI, 4);

	EventManager::tModelAnimInfo work = {0};
	
	if (strcmp(sub_cmd, "F_IN") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_FADE_IN;
	} else if (strcmp(sub_cmd, "F_OUT") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_FADE_OUT;
	} else if (strcmp(sub_cmd, "ROT") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_ROTATION;
	} else if (strcmp(sub_cmd, "SCA") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_SCALE;
	} else if (strcmp(sub_cmd, "ROTSCA") == 0) {
		work.mAnimState = (kEVTMGR_ANIM_TYPE_ROTATION|kEVTMGR_ANIM_TYPE_SCALE);
	} else {
		return;
	}
	work.mStartFrame	= pCsvAccesser->GetFloat(iI, 1);
	work.mEndFrame		= pCsvAccesser->GetFloat(iI, 3);
	work.mRotationSpeed	= pCsvAccesser->GetFloat(iI, 5);
	work.mDefaultScale	= pCsvAccesser->GetFloat(iI, 6);
	EventManager::GetInstance()->SetModelAnimInfoWork(model_index, &work);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_LYTANM(CsvAccesser* pCsvAccesser, int iI)
{
	int			layout_index;
	char*		sub_cmd;

	layout_index		= pCsvAccesser->GetInt(iI, 2);
	sub_cmd				= pCsvAccesser->GetString(iI, 4);

	EventManager::tLayoutAnimInfo work = {0};
	if (strcmp(sub_cmd, "F_IN") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_FADE_IN;
	} else if (strcmp(sub_cmd, "F_OUT") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_FADE_OUT;
	} else if (strcmp(sub_cmd, "ROT") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_ROTATION;
	} else if (strcmp(sub_cmd, "SCA") == 0) {
		work.mAnimState = kEVTMGR_ANIM_TYPE_SCALE;
	} else if (strcmp(sub_cmd, "ROTSCA") == 0) {
		work.mAnimState = (kEVTMGR_ANIM_TYPE_ROTATION|kEVTMGR_ANIM_TYPE_SCALE);
	} else {
		return;
	}
	work.mStartFrame	= pCsvAccesser->GetFloat(iI, 1);
	work.mEndFrame		= pCsvAccesser->GetFloat(iI, 3);
	work.mLytAnmScaleInfo.mBaseWidth	= pCsvAccesser->GetInt(iI, 5);
	work.mLytAnmScaleInfo.mBaseHeight	= pCsvAccesser->GetInt(iI, 6);
	work.mLytAnmScaleInfo.mTargetWidth	= pCsvAccesser->GetInt(iI, 7);
	work.mLytAnmScaleInfo.mTargetHeight	= pCsvAccesser->GetInt(iI, 8);

	EventManager::GetInstance()->SetLayoutAnimInfoWork(layout_index, &work);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_ATTACH(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd* p_model;
	ModelPmd* p_attach;
	int model_index  = pCsvAccesser->GetInt(iI, 2);
	int attach_index = pCsvAccesser->GetInt(iI, 3);
	char* frame_name = pCsvAccesser->GetString(iI, 4);

	int frame_index = -1;

	p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);
	p_attach = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(attach_index);

	frame_index = p_model->GetFrameIndexFromName(frame_name);

	p_model->AttachModel((ModelBase *)p_attach, 
						 ModelPmd::eATTACHED_MODEL_NORMAL,
						 frame_index);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_MOTION(CsvAccesser* pCsvAccesser, int iI)
{
	ModelPmd* p_model = NULL;
	int model_index = pCsvAccesser->GetInt(iI, 2);
	int motion_index = pCsvAccesser->GetInt(iI, 3);
	float motion_speed = pCsvAccesser->GetFloat(iI, 4);
	bool is_disable_loop = pCsvAccesser->GetBool(iI, 5);

	p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(model_index);
	p_model->StartMotion(motion_index, !(is_disable_loop), motion_speed);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_MUSIC(CsvAccesser* pCsvAccesser, int iI)
{
	char* music_name = pCsvAccesser->GetString(iI, 4);
	if ((music_name == "") || (music_name == NULL)) {
		SoundManager::GetInstance()->StopBGM();
	}
	else {
		SoundManager::GetInstance()->PlayBGM(music_name);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_D_MSG(CsvAccesser* pCsvAccesser, int iI)
{
	sDungeonIndex = pCsvAccesser->GetInt(iI, 2);
	if (sDungeonIndex == -1) {
		GraphicManager::GetInstance()->EntryDrawFontFunc(NULL);
	} else {
		GraphicManager::GetInstance()->EntryDrawFontFunc((FUNC_DRAW_FONT)DrawFontFunc);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_L_DIF(CsvAccesser* pCsvAccesser, int iI)
{
	COLOR_F col;
	col = GetColorF(pCsvAccesser->GetFloat(iI, 2),
					pCsvAccesser->GetFloat(iI, 3),
					pCsvAccesser->GetFloat(iI, 4),
					pCsvAccesser->GetFloat(iI, 5));
	SetLightDifColor(col);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_L_SPE(CsvAccesser* pCsvAccesser, int iI)
{
	COLOR_F col;
	col = GetColorF(pCsvAccesser->GetFloat(iI, 2),
					pCsvAccesser->GetFloat(iI, 3),
					pCsvAccesser->GetFloat(iI, 4),
					pCsvAccesser->GetFloat(iI, 5));
	SetLightSpcColor(col);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_L_AMB(CsvAccesser* pCsvAccesser, int iI)
{
	COLOR_F col;
	col = GetColorF(pCsvAccesser->GetFloat(iI, 2),
					pCsvAccesser->GetFloat(iI, 3),
					pCsvAccesser->GetFloat(iI, 4),
					pCsvAccesser->GetFloat(iI, 5));
	SetLightAmbColor(col);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_FUNC(CsvAccesser* pCsvAccesser, int iI)
{
	int func_index = pCsvAccesser->GetInt(iI, 2);
	int func_args = pCsvAccesser->GetInt(iI, 3);

	mEvtCmdFunc[func_index](func_args);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_PHASE(CsvAccesser* pCsvAccesser, int iI)
{
	sEventPhase = pCsvAccesser->GetInt(iI, 2);
	sEventCount = pCsvAccesser->GetInt(iI, 3);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_BLM(CsvAccesser* pCsvAccesser, int iI)
{
	RenderBloom::SetBloomType((RenderBloom::eBloomType)pCsvAccesser->GetInt(iI, 2));
	RenderBloom::SetBloomParam(pCsvAccesser->GetInt(iI, 3),
							   pCsvAccesser->GetInt(iI, 4),
							   pCsvAccesser->GetInt(iI, 5));
	RenderBloom::ChangeBloomParam(pCsvAccesser->GetFloat(iI, 6),
								  pCsvAccesser->GetInt(iI, 7),
								  pCsvAccesser->GetInt(iI, 8));
}

#ifdef _DEBUG
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_LOAD(CsvAccesser* pCsvAccesser, int iI)
{
	PRINT_CONSOLE("=== EVENT LOAD: line = %d : frame = %.1f ==== \n", 
				  sCheckLineCnt, 
				  EventManager::GetInstance()->GetPhaseFrame() );

	loadCsvActionDebug();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void funcCsvCmd_SAVE(CsvAccesser* pCsvAccesser, int iI)
{
	PRINT_CONSOLE("=== EVENT SAVE: line = %d : frame = %.1f ==== \n", 
				  sCheckLineCnt, 
				  EventManager::GetInstance()->GetPhaseFrame() );

	saveCsvActionDebug();
}
#endif // _DEBUG

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void evtCmdFunc_00(int args)
{
	/* ���C�g�̐؂�ւ� */
	StageFunc036* p_stage = (StageFunc036 *)spStageFunc;
	p_stage->SetPatternLight(StageFunc036::eLIGHT_PATTEN_NAVI_LIGHT);

	// Navi �̍��W���擾
	ModelPmd* p_model = (ModelPmd *)EventManager::GetInstance()->GetModelPmd(1);
	VECTOR light_pos = p_model->Position();
	light_pos.y += (19.406f * p_model->Scale().x);
	GraphicManager::GetInstance()->SetPLightPosition(&light_pos);
	eventGeneral_NaviLuminusBigLight();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void evtCmdFunc_01(int args)
{
	int model_handle = sModelMap.GetModelHandle();

	// �����ȏ��ǉ�
	CollisionManager::GetInstance()->RemoveEarthCollision(model_handle, (eBG_TYPE_GIMMICK + 3));
	MV1SetFrameVisible(model_handle, (eBG_TYPE_GIMMICK + 3), FALSE);

	// ���E��
	CollisionManager::GetInstance()->RemoveEarthCollision(model_handle, (eBG_TYPE_GIMMICK + 4));
	MV1SetFrameVisible(model_handle, (eBG_TYPE_GIMMICK + 4), FALSE);
	// ���E��
	CollisionManager::GetInstance()->RemoveWallCollision(model_handle, (eBG_TYPE_GIMMICK + 5));
	MV1SetFrameVisible(model_handle, (eBG_TYPE_GIMMICK + 5), FALSE);

	// �Ԃa�̏���
	CollisionManager::GetInstance()->RemoveWallCollision(model_handle, (eBG_TYPE_GIMMICK + 6));
	MV1SetFrameVisible(model_handle, (eBG_TYPE_GIMMICK + 6), FALSE);

	// �����ȏ��E�{�X�O�ǉ�
	CollisionManager::GetInstance()->EntryEarthCollision(model_handle, (eBG_TYPE_GIMMICK + 7));
	MV1SetFrameVisible(sModelMap.GetModelHandle(), (eBG_TYPE_GIMMICK + 7), TRUE);

	// ���[���̏���
	CollisionManager::GetInstance()->RemoveWallCollision(model_handle, (eBG_TYPE_GIMMICK + 8));
	MV1SetFrameVisible(model_handle, (eBG_TYPE_GIMMICK + 8), FALSE);

	// �����̏���
	CollisionManager::GetInstance()->RemoveWallCollision(model_handle, (eBG_TYPE_GIMMICK + 9));
	MV1SetFrameVisible(model_handle, (eBG_TYPE_GIMMICK + 9), FALSE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void evtCmdFunc_02(int args)
{
	ObjectBase* p_object = NULL;
	int place_index = 0;
	int crystal_color = 0;

	switch (args) {
	
	case 0:	// �X�̃N���X�^��
		place_index = 3;
		crystal_color = 0;
		p_object = sppObjectBase[0];
		break;
	case 1:	// �y�̃N���X�^��
		place_index = 4;
		crystal_color = 1;
		p_object = sppObjectBase[1];
		break;
	default:
		APP_ASSERT(NULL, "Not Support this argument \n");
		break;
	};
	APP_ASSERT( (p_object != NULL), "Not Found eOBJECT_KIND_CRYSTAL \n");

	processAppearCrystal(place_index,		// stagePlaceIndex
						 p_object,			// pCrystalObject
						 crystal_color);	// crystalColor : 0 : Green, 1 : Yellow
}

//---------------------------------------------------
/**
 * �n���b�p�̃s���s���̃T�E���h�ƃG�t�F�N�g���~����̂Ɏg�p
 * ���̑��v�f�ɂ��g������
 // �{�C�X�̑S��~
 // Effekseer�̑S��~
 */
//---------------------------------------------------
void evtCmdFunc_03(int args)
{
	SoundManager::GetSndComponent()->StopAll();
	EffekseerManager::GetInstance()->StopAll();
}

//---------------------------------------------------
/**
 * ���G�t�F�N�g���Ӑ}�I�ɏo�͂�����(PG�ˑ�)
 */
//---------------------------------------------------
void evtCmdFunc_04(int args)
{
	VECTOR pos; float rot;
	int tmp = 0;

	getPlayerPosRotFromBGModel(&pos, &rot, args);
	
	tmp = EffectManager::GetInstance()->Play(eEF_DUST, 
											 &pos,
											 &VScale(VGet(10.0f, 5.0f, 10.0f), 1.1f),
											 0.0f);
	EffectManager::GetInstance()->Rotation(tmp).y = (float)sEventCount;
	EffectManager::GetInstance()->SetFloat(tmp, "scale",				20.0f);
	EffectManager::GetInstance()->SetFloat(tmp, "particleSystemShape",	0.8f);
	EffectManager::GetInstance()->SetFloat(tmp, "gravity",				-0.100f);
	EffectManager::GetInstance()->Stop(tmp, 1.0f);

	pos.y += -5000.0f;

	EFK_HANDLE handle;
	handle = EffekseerManager::GetInstance()->Play(eEFK_BREAK_BARRIER,
												   &pos, 
												   100.0f);
	EffekseerManager::GetInstance()->SetSpeed(handle, 3.0f);

	SoundManager::GetSndComponent()->PlaySound(eSE_BURST);
	SoundManager::GetSndComponent()->PlaySound(eSE_EXPLOSION);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void evtCmdFunc_05(int args)
{
	GraphicManager::GetInstance()->SetEnablePLight(TRUE);
	GraphicManager::GetInstance()->SetPLightPosition(&VGet(0.000f, 8300.000f, 0.000f) );
	GraphicManager::GetInstance()->SetPLightRangeAtten(30800.000f, 0.1f, (float)1E-05, (float)4E-10);
	GraphicManager::GetInstance()->SetPLightDiffuse(0.360780f, 0.580390f, 0.533330f, 0.000000f);
	GraphicManager::GetInstance()->SetPLightSpecular(0.003920f, 0.074510f, 0.074510f, 0.000000f);
	GraphicManager::GetInstance()->SetPLightAmbient(0.090200f, 0.196080f, 0.149020f, 0.000000f);

	RenderBloom::SetBloomType(RenderBloom::eBLOOM_TYPE_BRIGHT_LESS);
	RenderBloom::SetBloomParam(12, 45, 200);
	RenderBloom::tBloomBrightParam param = {0};
	param.mClipParam = 170;
	RenderBloom::BloomBrightParam() = param;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void evtCmdFunc_06(int args)
{
	if (args == -1) {
		GraphicManager::GetInstance()->EntryDrawFontFunc(NULL);
		sDrawFreeFontValue = eFREE_FONT_NONE;
	} else {
		GraphicManager::GetInstance()->EntryDrawFontFunc((FUNC_DRAW_FONT)DrawFreeFontFunc);
		sDrawFreeFontValue = args;
	}
}

//---------------------------------------------------
/**
 * �f�B���N�V���i�����C�g�ݒ�	
 */
//---------------------------------------------------
void evtCmdFunc_07(int args)
{
	switch (args) {
	case 0:
		// ���C�g����
		GraphicManager::GetInstance()->SetEnableDirectionLight(FALSE);
		break;

	case 1:
		// ���C�g�L���E�W��
		GraphicManager::GetInstance()->SetEnableDirectionLight(TRUE);
		GraphicManager::GetInstance()->SetDefaultDirectionLight();
		break;

	case 2:
		// ���C�g�L���E�C�x���g���L
		GraphicManager::GetInstance()->SetEnableDirectionLight(TRUE);
		SetLightDifColor(GetColorF(0.860000f, 0.811760f, 0.600000f, 0.000000f) );
		SetLightSpcColor(GetColorF(0.098040f, 0.098040f, 0.098040f, 0.000000f) );
		SetLightAmbColor(GetColorF(0.090000f, 0.105880f, 0.278430f, 0.000000f) );
		break;

	default:
		break;
	};
}

//---------------------------------------------------
/**
 * BGM�t�F�[�h�A�E�g
 */
//---------------------------------------------------
void evtCmdFunc_08(int args)
{
	SoundManager::GetInstance()->StartFadeVolumeBGM(0, 6.0f);
}

//---------------------------------------------------
/**
 * �_���W�����J�n���C�A�E�g�\��
 */
//---------------------------------------------------
void evtCmdFunc_09(int args)
{
	static const tLayoutSetting sDuengeionLayoutTable[] = {
		{RES_LYT_INFO_STAGE,		160, 220,	144, 72,		255,},
		{RES_LYT_INFO_1,			240, 220,	72,	 72,		255,},
		{RES_LYT_INFO_MAP_1,		400, 220,	288, 72,		255,},
		{RES_LYT_INFO_MAP_SUB_1,	320, 276,	384, 48,		255,},		// �\�����e�ɂ����X���������ŕύX���Ă���
	};

	LayoutBase* p_layout = NULL;
	int dungeon_index = args;

	float aspect_rate = GraphicManager::GetInstance()->GetAspectRate();
	const tLayoutSetting* p_setting = NULL;
	for (int iI = 0; iI < 4; iI ++) {
		p_layout	= EventManager::GetInstance()->GetSystemLayoutBase(iI);
		p_setting	= &sDuengeionLayoutTable[iI];
		
		p_layout->SetPositionByCentering(p_setting->mPosX, p_setting->mPosY);
		p_layout->SetSize((int)((float)p_setting->mSizeX / aspect_rate),
						  (int)((float)p_setting->mSizeY / aspect_rate) );
		p_layout->SetAlpha(p_setting->mAlpha);
		//			p_layout->StartFade(255, 0.4f);
		//		GraphicManager::GetInstance()->EntrySystemLayout(p_layout);
	}

	// -1 �̏ꍇ�͔�\���ɂ���
	if (args == -1) {
		for (int iI = 0; iI < 4; iI ++) {
			p_layout	= EventManager::GetInstance()->GetSystemLayoutBase(iI);
			p_layout->SetAlpha(255);
			p_layout->StartFade(0, 0.5f);
		}
	}
	// �_���W�����ԍ��ɉ��������C�A�E�g��\��������
	else {
		for (int iI = 0; iI < 4; iI ++) {
			p_layout	= EventManager::GetInstance()->GetSystemLayoutBase(iI);
			p_setting	= &sDuengeionLayoutTable[iI];

			p_layout->LinkLayout(p_setting->mStrLayoutName);
			p_layout->SetAlpha(0);
			p_layout->StartFade(255, 0.5f);
		}

		// �_���W�����ɉ����ă��C�A�E�g����ύX����
		p_layout = EventManager::GetInstance()->GetSystemLayoutBase(1);
		p_layout->LinkLayout(sResourceLayoutNameTable[eRES_LYT_INFO_1			+ dungeon_index]);

		p_layout = EventManager::GetInstance()->GetSystemLayoutBase(2);
		p_layout->LinkLayout(sResourceLayoutNameTable[eRES_LYT_INFO_MAP_1		+ dungeon_index]);

		p_layout = EventManager::GetInstance()->GetSystemLayoutBase(3);
		p_layout->LinkLayout(sResourceLayoutNameTable[eRES_LYT_INFO_MAP_SUB_1	+ dungeon_index]);
	}
}

//---------------------------------------------------
/**
 * BGM�t�F�[�h�A�E�g
 */
//---------------------------------------------------
void evtCmdFunc_10(int args)
{
	((StageFunc043 *)spStageFunc)->StartFadeOutWall();
}

#ifdef _DEBUG
/*=====================================*
 * �f�[�^�]������
 *=====================================*/
enum eDataTransDir {
	eINVALID,	// ����
	eSEND,		// �N���C�A���g�փf�[�^���M�v��
	eRECV,		// �N���C�A���g����f�[�^��M�v��
	eSEND_RECV, // �N���C�A���g�փf�[�^���M����M�v��
	
	eMAX,
};

/*=====================================*
 * �R�}���h���
 *=====================================*/
/* �R�}���h��� */
enum eCmdType {
	eCMD_NONE,              // �R�}���h�Ȃ�
	eCMD_SET_PLAY_SOUND,    // �����Đ�
	eCMD_GET_CLIP_VALUE,    // NearFarClip�擾
	eCMD_SET_CLIP_VALUE,    // NearFarClip�ݒ�
	eCMD_GET_MAP_INFO,      // �}�b�v���擾
	eCMD_SET_SKYDOME,       // �X�J�C�h�[���ݒ�
	eCMD_SET_MAPSCALE,      // �}�b�v�X�P�[���ݒ�
	eCMD_SET_FOG,           // �t�H�O�ݒ�
	eCMD_SET_SHAKE_CAM,     // �J�����U��
	eCMD_SET_VIBE_CAM,      // �J�����o�C�u
	eCMD_GET_CHARA_INFO,    // �L�������̎擾
	eCMD_SET_DBG_VALUE,     // �f�o�b�O�l�擾
	eCMD_SET_EVENT,         // �C�x���g�ԍ��ݒ�
	eCMD_SET_CHR_POS,       // �L�����̍��W�ݒ�,
	// Light
	// ���C�g�̗L���E����
	eCMD_GET_ENABLE_LIGHT,
	eCMD_ENABLE_LIGHT_DIRECTION,  // 
	eCMD_ENABLE_LIGHT_POINT,      // 
	eCMD_ENABLE_LIGHT_SPOT,       // 
	eCMD_ENABLE_MAP_EMMISIVE,
	eCMD_ENABLE_USER_SHADER,		
	// �f�B���N�V�������C�g�֘A
	eCMD_GET_DLIGHT_INFO,     
	eCMD_SET_DLIGHT_DIR,
	eCMD_SET_DLIGHT_DIF,       
	eCMD_SET_DLIGHT_SPC,       
	eCMD_SET_DLIGHT_AMB,       
	// �|�C���g���C�g�֘A
	eCMD_GET_PLIGHT_INFO,
	eCMD_SET_PLIGHT_POSITION,  
	eCMD_SET_PLIGHT_RANGE_ATTEN,
	eCMD_SET_PLIGHT_DIF,     
	eCMD_SET_PLIGHT_SPC,     
	eCMD_SET_PLIGHT_AMB,     
	eCMD_GET_FRAME_VISIBLE,
	eCMD_SET_FRAME_VISIBLE,
	eCMD_EVENTTYPE_START,
	eCMD_SET_OUTLINE_WIDTH,
	eCMD_SET_OUTLINE_DOT,
	// �M�~�b�N�֘A
	eCMD_GET_GIMMICK_NUM,           // �M�~�b�N���擾
	eCMD_SET_GIMMICK_NO,            // �M�~�b�N�ԍ��w��
	eCMD_UPDATE_GIMMICK_INFO,       // �M�~�b�N���̓ǂݍ���
	eCMD_SET_GIMMICK_POS,           // ���W�ݒ�
	eCMD_SET_GIMMICK_ROT,           // ��]�ݒ�
	eCMD_SET_GIMMICK_SCA,           // �g�k�ݒ�
	eCMD_SET_GIMMICK_PLAYER_POS,    // �M�~�b�N���v���C���[���W�ɐݒ�
	eCMD_SET_GIMMICK_ONCOLLISION,   // �v���C���[��n�ʂɐݒu������
	eCMD_LOAD_GIMMICK_INFO,         // �M�~�b�N���̃��[�h
	/*
	eCMD_GET_GIMMICK_EXT_DATA_A,    // �M�~�b�N�g���f�[�^�擾A
	eCMD_GET_GIMMICK_EXT_DATA_B,    // �M�~�b�N�g���f�[�^�擾B
	eCMD_GET_GIMMICK_EXT_DATA_C,    // �M�~�b�N�g���f�[�^�擾C
	eCMD_SET_GIMMICK_EXT_A,         // �M�~�b�N�g���f�[�^A
	eCMD_SET_GIMMICK_EXT_B,         // �M�~�b�N�g���f�[�^B
	eCMD_SET_GIMMICK_EXT_C,         // �M�~�b�N�g���f�[�^C
	*/
	eCMD_GET_GIMMICK_EXT_DATA_A,    // �M�~�b�N�g���f�[�^�擾 �O���[�vA
	eCMD_GET_GIMMICK_EXT_DATA_B,    // �M�~�b�N�g���f�[�^�擾 �O���[�vB
	eCMD_SET_GIMMICK_EXT_DATA,      // �M�~�b�N�g���f�[�^A

	// �u���[���֘A
	eCMD_GET_BLOOM_INFO,            // �u���[�����擾
	eCMD_SET_BLOOM_TYPE,            // �u���[���^�C�v�w��
	eCMD_SET_BLOOM_PARAM,           // �u���[���p�����[�^�ݒ�
	eCMD_SET_BLOOM_BRIGT_PARAM,     // �u���[�����邳���ݒ�
	eCMD_SET_BLOOM_HSB_PARAM,       // �u���[���F���E�ʓx�E���x�ݒ�
	// NPC�֘A
	eCMD_GET_NPC_NUM,           // NPC�����擾
	eCMD_SET_NPC_NO,            // NPC�ԍ��w��
	eCMD_UPDATE_NPC_INFO,       // NPC���̎擾�E�X�V
	eCMD_SET_NPC_POS_X,           // ���W�ݒ�
	eCMD_SET_NPC_POS_Z,           // ���W�ݒ�
	eCMD_SET_NPC_ROT_Y,           // ��]�ݒ�
	eCMD_SET_NPC_PLAYER_POS,    // NPC���v���C���[���W�ɐݒ�
	eCMD_SET_NPC_ONCOLLISION,   // �v���C���[��n�ʂɐݒu������
	eCMD_LOAD_NPC_INFO,         // NPC���̃��[�h
	eCMD_SET_PLAYER_BG_POS,     // �w��̍��W�ֈړ�������

	eCMD_GET_EVENT_FLAG,        // �C�x���g�t���O�擾
	eCMD_SET_EVENT_FLAG,        // �C�x���g�t���O�ݒ�

	eCMD_GET_NEAREST_GIMMICK_NO,// ��ԋ߂��M�~�b�N�̏ꏊ���擾

	eCMD_PLAY_EFFECT = eCMD_EVENTTYPE_START,       // �G�t�F�N�g�Đ�

	eGTT_MAX,
};

/*=====================================*
 * 
 *=====================================*/
#define GET_BUF(ValueName)										\
	type_size = sizeof(ValueName);								\
	memcpy(&ValueName, &p_cmdbuf[sBufIndex], type_size);		\
	sBufIndex += type_size;

static BYTE sBufIndex = 0;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void updateGameTestTool(void)
{
	static BYTE sGimmickNo = 0;
	static BYTE sNpcNo = 0;

	bool ret = false;

	GraphicManager * const p_graphic = GraphicManager::GetInstance();

	BYTE type_size = 0;
	sBufIndex = 0;

	// �R�}���h�擾�v��
	char tmp_buf[1] = {0};
	tmp_buf[0] = (char)0xAC;
	ret = PIC_LIB::SendCommand(tmp_buf, sizeof(tmp_buf));

	// �R�}���h��ʁE�T�C�Y���擾
	BYTE recv_buf[3] = {0};
	ret = PIC_LIB::RecvCommand((char *)recv_buf, sizeof(recv_buf));
	BYTE cmd_type = recv_buf[0];
	BYTE cmd_size = recv_buf[1];
	BYTE cmd_dir = recv_buf[2];
	if (cmd_size == 0) {
		// �s���ȃp�P�b�g���󂯂��̂ŏ����������^�[������
		return;
	}

	BYTE* p_cmdbuf = new BYTE[cmd_size];

	//	APP_ASSERT(cmd_dir != eINVALID,
	//			   "Get Command direction is invalid \n");

	/**** ���M�f�[�^�̏ꍇ ****/
	if ( (cmd_dir == eSEND) ||
		 (cmd_dir == eSEND_RECV) ) {
		// �R�}���h�ڍ׏����擾
		ret = PIC_LIB::RecvCommand((char *)p_cmdbuf, cmd_size);
	}

	if (cmd_type != eCMD_NONE) {
		PRINT_CONSOLE("%s Command  type : %d (%d) \n", (cmd_dir == eRECV ? "Send" : "Recv"), cmd_type, cmd_size);
	}

	// �R�}���h�ɉ��������������{
	switch (cmd_type)
		{
		case eCMD_NONE:
			break;
		case eCMD_SET_PLAY_SOUND:
			SoundManager::GetSndComponent()->PlaySound(p_cmdbuf[0]);
			break;
		case eCMD_GET_CLIP_VALUE:
			{
				float outdata[2];
				outdata[0] = CameraManager::GetInstance()->GetNearClip();
				outdata[1] = CameraManager::GetInstance()->GetFarClip();

				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;
		case eCMD_SET_CLIP_VALUE:
			{	
				float tmp;
				memcpy(&tmp, &p_cmdbuf[0], sizeof(float));
				CameraManager::GetInstance()->SetNearClip(tmp);
				memcpy(&tmp, &p_cmdbuf[4], sizeof(float));
				CameraManager::GetInstance()->SetFarClip(tmp);
			}
			break;

		case eCMD_GET_MAP_INFO:
			{
				float outdata[2];
				outdata[0] = sModelXfile[eMD_SKYDOME].Scale().x;
				outdata[1] = sModelMap.Scale().x;

				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_SET_SKYDOME:
			{
				float indata[1];
				memcpy(&indata[0], &p_cmdbuf[0], sizeof(float));
				sModelXfile[eMD_SKYDOME].Scale() = VGet(indata[0], indata[0], indata[0]);
			}
			break;

		case eCMD_SET_MAPSCALE:
			{
				float indata[1];
				memcpy(&indata[0], &p_cmdbuf[0], sizeof(float));
				sModelMap.Scale() = VGet(indata[0], indata[0], indata[0]);
			}
			break;

		case eCMD_SET_FOG:
			{
				float indata[5];
				memcpy(&indata[0], &p_cmdbuf[0], sizeof(float) * 5);

				GraphicManager::tFogParam fogParam;
				fogParam.mIsEnable = TRUE;
				fogParam.mIsActive = TRUE;
				fogParam.mColR = (int)indata[2];
				fogParam.mColG = (int)indata[3];
				fogParam.mColB = (int)indata[4];
				fogParam.mNear = indata[0];
				fogParam.mFar = indata[1];
				p_graphic->ChangeFogParam(1.0, &fogParam);

			}
			break;

		case eCMD_SET_SHAKE_CAM:
			{
				float indata[3];
				memcpy(&indata[0], &p_cmdbuf[0], sizeof(float) * 3);
				CameraManager::GetInstance()->StartShakeCamera(indata[0], indata[1], indata[2]);
			}
			break;

		case eCMD_SET_VIBE_CAM:
			{
				float indata[3];
				memcpy(&indata[0], &p_cmdbuf[0], sizeof(float) * 3);
				CameraManager::GetInstance()->StartVibeCamera(indata[0], indata[1], indata[2]);
			}
			break;

		case eCMD_GET_CHARA_INFO:
			{
				float outdata[3];
				outdata[0] = (float)sNpcNum;
				outdata[1] = (float)sObjectBaseNum;
				outdata[2] = (float)MV1GetMeshNum(sPlayer.GetModelHandle());
				
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_SET_DBG_VALUE:
			{
				memcpy(Basic::sDbgToolValue, &p_cmdbuf[0], sizeof(float) * 12);
			}
			break;

		case eCMD_SET_EVENT:
			{
				BYTE indata[3];
				memcpy(&indata[0], &p_cmdbuf[0], sizeof(BYTE) * 3);

				// �Ƃ肠�����C�x���g�t���O��������
				if (indata[2] == TRUE){
					TotalInfo::GetInstance()->ResetAllEventFlag();
				}

				ActionTaskManager::tTaskInfo info;
				info.mTaskType = ActionTaskManager::eTASK_EVENT;
				info.mTaskValueA = indata[0];
				info.mTaskValueB = indata[1];
				ActionTaskManager::GetInstance()->NotifyAction(&info);
			}
			break;

		case eCMD_SET_CHR_POS:
			{
				BYTE inbyte;
				float infloat[3];
				memcpy(&inbyte, &p_cmdbuf[0], sizeof(BYTE) * 1);
				memcpy(&infloat, &p_cmdbuf[1], sizeof(float) * 3);

				sPlayer.Position() = VGet(infloat[0], infloat[1], infloat[2]);

			}
			break;

		case eCMD_GET_ENABLE_LIGHT:
			{
				byte outdata[4];
				outdata[0] = (p_graphic->GetEnableDirectionLight() == false ? 0 : 1);
				outdata[1] = (p_graphic->GetEnablePLight() == false ? 0 : 1);
				outdata[2] = 0;// �X�|�b�g���C�g�͌��󖢎g�p
				outdata[3] = (p_graphic->GetEnableOriginalShader() == false ? 0 : 1);
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

			// ChangeLightTypeDir ���s
		case eCMD_SET_DLIGHT_DIR:
			{
				float x, y, z;
				GET_BUF(x);
				GET_BUF(y);
				GET_BUF(z);
				ChangeLightTypeDir(VGet(x, y, z));
			}
			break;

			// SetLightDifColor ���s
		case eCMD_SET_DLIGHT_DIF:
			{
				float r, g, b, a;
				GET_BUF(r);
				GET_BUF(g);
				GET_BUF(b);
				GET_BUF(a);
				SetLightDifColor(GetColorF(r, g, b, a));
			}
			break;

			// SetLightSpcColor ���s
		case eCMD_SET_DLIGHT_SPC:
			{
				float r, g, b, a;
				GET_BUF(r);
				GET_BUF(g);
				GET_BUF(b);
				GET_BUF(a);
				SetLightSpcColor(GetColorF(r, g, b, a));
			}
			break;

			// SetLightAmbColor ���s
		case eCMD_SET_DLIGHT_AMB:
			{
				float r, g, b, a;
				GET_BUF(r);
				GET_BUF(g);
				GET_BUF(b);
				GET_BUF(a);
				SetLightAmbColor(GetColorF(r, g, b, a));
			}
			break;

		case eCMD_GET_DLIGHT_INFO:
			{
				float outdata[4*3+3];
				COLOR_F color;
				VECTOR dir;

				color = GetLightDifColor();
				outdata[0] = color.r;
				outdata[1] = color.g;
				outdata[2] = color.b;
				outdata[3] = color.a;

				color = GetLightSpcColor();
				outdata[0+4] = color.r;
				outdata[1+4] = color.g;
				outdata[2+4] = color.b;
				outdata[3+4] = color.a;

				color = GetLightAmbColor();
				outdata[0+8] = color.r;
				outdata[1+8] = color.g;
				outdata[2+8] = color.b;
				outdata[3+8] = color.a;

				dir = GetLightDirection();
				outdata[0+12]= dir.x;
				outdata[1+12]= dir.y;
				outdata[2+12]= dir.z;

				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

			// �|�C���g���C�g�����擾
		case eCMD_GET_PLIGHT_INFO:
				float outdata[19];
				COLOR_F color;
				VECTOR vec;
				float range;

				p_graphic->GetPLightDiffuse(&color);
				outdata[0] = color.r;
				outdata[1] = color.g;
				outdata[2] = color.b;
				outdata[3] = color.a;

				p_graphic->GetPLightSpecular(&color);
				outdata[0+4] = color.r;
				outdata[1+4] = color.g;
				outdata[2+4] = color.b;
				outdata[3+4] = color.a;

				p_graphic->GetPLightAmbient(&color);
				outdata[0+8] = color.r;
				outdata[1+8] = color.g;
				outdata[2+8] = color.b;
				outdata[3+8] = color.a;

				p_graphic->GetPLightPosition(&vec);
				outdata[0+12]= vec.x;
				outdata[1+12]= vec.y;
				outdata[2+12]= vec.z;

				p_graphic->GetPLightRangeAtten(&range, &vec.x, &vec.y, &vec.z);
				outdata[0+15]= range;
				outdata[0+16]= vec.x;
				outdata[1+16]= vec.y;
				outdata[2+16]= vec.z;

				memcpy(p_cmdbuf, outdata, cmd_size);


			break;

		case eCMD_SET_PLIGHT_POSITION:
			{
				VECTOR position;
				GET_BUF(position.x);
				GET_BUF(position.y);
				GET_BUF(position.z);
				//				SetLightPosition(position);
				p_graphic->SetPLightPosition(&position);
				PRINT_CONSOLE("eCMD_SET_LIGHT_POSITION\n");
			}
			break;

		case eCMD_SET_PLIGHT_RANGE_ATTEN:
			{
				float range;
				float atten[3];
				GET_BUF(range);
				GET_BUF(atten[0]);
				GET_BUF(atten[1]);
				GET_BUF(atten[2]);
				//				SetLightRangeAtten(range, atten[0], atten[1], atten[2]);
				p_graphic->SetPLightRangeAtten(range, atten[0], atten[1], atten[2]);
				PRINT_CONSOLE("eCMD_SET_LIGHT_RANGE_ATTEN\n");
			}
			break;

		case eCMD_ENABLE_LIGHT_DIRECTION:
			{
				byte flag;
				GET_BUF(flag);
				p_graphic->SetEnableDirectionLight((flag == 0 ? FALSE : TRUE));
				PRINT_CONSOLE("eCMD_SET_LIGHT_TYPE_DIRECTION\n");
			}
			break;

		case eCMD_ENABLE_LIGHT_POINT:
			{
				byte flag;
				GET_BUF(flag);

				p_graphic->SetEnablePLight((flag == 0 ? FALSE : TRUE));
				PRINT_CONSOLE("eCMD_SET_LIGHT_TYPE_POINT\n");
			}
			break;

		case eCMD_ENABLE_LIGHT_SPOT:
			//<! ������
			PRINT_CONSOLE("eCMD_SET_LIGHT_TYPE_SPOT\n");
			break;

		case eCMD_ENABLE_MAP_EMMISIVE:
			{
				byte flag;
				GET_BUF(flag);
				sModelMap.SetEnableEmissive((flag == 0 ? false : true));

				sPlayer.SetEnableEmissive((flag == 0 ? false : true));
				

			}
			break;

		case eCMD_ENABLE_USER_SHADER:
			{
				byte flag;
				GET_BUF(flag);
				p_graphic->SetEnableOriginalShader((flag == 0 ? false : true));
			}
			break;

		case eCMD_SET_PLIGHT_DIF:
			{
				float r, g, b, a;
				GET_BUF(r);
				GET_BUF(g);
				GET_BUF(b);
				GET_BUF(a);
				p_graphic->SetPLightDiffuse(r, g, b, a);
				PRINT_CONSOLE("eCMD_SET_P_LIGHT_DIF_COLOR\n");
			}
			break;

		case eCMD_SET_PLIGHT_SPC:
			{
				float r, g, b, a;
				GET_BUF(r);
				GET_BUF(g);
				GET_BUF(b);
				GET_BUF(a);
				p_graphic->SetPLightSpecular(r, g, b, a);
				PRINT_CONSOLE("eCMD_SET_P_LIGHT_SPC_COLOR\n");
			}
			break;

		case eCMD_SET_PLIGHT_AMB:
			{
				float r, g, b, a;
				GET_BUF(r);
				GET_BUF(g);
				GET_BUF(b);
				GET_BUF(a);
				p_graphic->SetPLightAmbient(r, g, b, a);
				PRINT_CONSOLE("eCMD_SET_P_LIGHT_AMB_COLOR\n");
			}
			break;

		case eCMD_GET_FRAME_VISIBLE:
			{
				// nop;
			}
			break;

		case eCMD_SET_FRAME_VISIBLE:
			{
				BYTE frameno, visible;
				GET_BUF(frameno);
				GET_BUF(visible);
				MV1SetMeshVisible(sPlayer.GetModelHandle(), frameno, (visible == 0) ? FALSE : TRUE);

			}
			break;

		case eCMD_SET_OUTLINE_WIDTH:
			{
				float width;
				GET_BUF(width);
				int model_handle = sPlayer.GetModelHandle();
				int material_num = MV1GetMaterialNum(model_handle);
				for (int iI = 0; iI < material_num; iI ++) {
					MV1SetMaterialOutLineWidth(model_handle, iI, width);
				}
			}
			break;

		case eCMD_SET_OUTLINE_DOT:
			{
				float width;
				GET_BUF(width);
				int model_handle = sPlayer.GetModelHandle();
				int material_num = MV1GetMaterialNum(model_handle);
				for (int iI = 0; iI < material_num; iI ++) {
					MV1SetMaterialOutLineDotWidth(model_handle, iI, width);
				}
			}
			break;

			// �M�~�b�N���擾
		case eCMD_GET_GIMMICK_NUM:
			{
				float outdata[1];
				outdata[0] = (float)GimmickBase::GetGimmickBaseNum();
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

			//<! ���g�p�R�}���h
		case eCMD_SET_GIMMICK_NO:
			{
				APP_ASSERT(NULL, "Not Support this command \n");
			}
			break;

		case eCMD_UPDATE_GIMMICK_INFO:         // �M�~�b�N���̓ǂݍ���
			{
				BYTE gimmick_no;
				GET_BUF(gimmick_no);
				sGimmickNo = gimmick_no;

				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				float outdata[6];
				outdata[0] = (float)sStageIndex;
				outdata[1] = p_gimmick->Position().x + sReviseDiffPos.x;
				outdata[2] = p_gimmick->Position().y + sReviseDiffPos.y;
				outdata[3] = p_gimmick->Position().z + sReviseDiffPos.z;
				outdata[4] = Utility::RadToDeg(p_gimmick->Rotation().y);
				outdata[5] = p_gimmick->Scale().y;
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_SET_GIMMICK_POS:           // ���W�ݒ�
			{
				float x, y, z;
				GET_BUF(x);
				GET_BUF(y);
				GET_BUF(z);

				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				p_gimmick->Position() = VSub(VGet(x, y, z), sReviseDiffPos);
			}
			break;

		case eCMD_SET_GIMMICK_ROT:           // ��]�ݒ�
			{
				float deg;
				GET_BUF(deg);

				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				p_gimmick->Rotation().y = Utility::DegToRad(deg);
			}
			break;

		case eCMD_SET_GIMMICK_SCA:           // �g�k�ݒ�
			{
				float scale;
				GET_BUF(scale);

				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				p_gimmick->Scale() = VGet(scale, scale, scale);
			}
			break;

		case eCMD_SET_GIMMICK_PLAYER_POS:    // �M�~�b�N���v���C���[���W�ɐݒ�
			{
				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				p_gimmick->Position() = sPlayer.Position();
				p_gimmick->Position().y += (sPlayer.BodyHeight() * 0.5f);
				p_gimmick->Rotation().y = sPlayer.Rotation().y;
			}
			break;

		case eCMD_SET_GIMMICK_ONCOLLISION:   // �v���C���[��n�ʂɐݒu������
			{
				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				p_gimmick->SetPositionOnCollision();
			}
			break;

		case eCMD_LOAD_GIMMICK_INFO:         // �M�~�b�N���̃��[�h
			{
				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				//				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				//				p_gimmick->ReloadInformation(sStageIndex);
				GimmickBase::ReloadInformationAll(sStageIndex);
			}
			break;

		case eCMD_GET_GIMMICK_EXT_DATA_A:    // �M�~�b�N�g���f�[�^�擾
			{
				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				float outdata[5];
				BYTE ext_buf_num = p_gimmick->GetExtendBufferNum();
				for (int iI = 0; iI < 5; iI ++) {
					if (ext_buf_num > iI) {
						outdata[iI] = p_gimmick->GetGimmickExtParamValue(iI+0);
					}
					else {
						outdata[iI] = -1;
					}
				}
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_GET_GIMMICK_EXT_DATA_B:    // �M�~�b�N�g���f�[�^�擾
			{
				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				float outdata[5];
				BYTE ext_buf_num = p_gimmick->GetExtendBufferNum();
				for (int iI = 0; iI < 5; iI ++) {
					if (ext_buf_num > iI + 5) {
						outdata[iI] = p_gimmick->GetGimmickExtParamValue(iI+5);
					}
					else {
						outdata[iI] = -1;
					}
				}
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_SET_GIMMICK_EXT_DATA:         // �M�~�b�N�g���f�[�^
			{
				if (GimmickBase::GetGimmickBaseNum() <= sGimmickNo) {
					SYSTEM_MESSAGE("warning", "Select gimmick no is invalid value");
					break;
				}

				BYTE index;
				float value;
				GET_BUF(index);
				GET_BUF(value);
				GimmickBase* p_gimmick = GimmickBase::GetGimmickBase(sGimmickNo);
				p_gimmick->SetGimmickExtParamValue(index, value);
			}
			break;

		case eCMD_GET_BLOOM_INFO:            // �u���[�����擾
			{
				int index = 0;
				float outdata[12];
				outdata[index++] = (float)RenderBloom::GetBloomType();
				
				BYTE blend_mode;
				WORD blend_value, gauss_value;
				RenderBloom::GetBloomParam(blend_mode, blend_value, gauss_value);
				outdata[index++] = (float)blend_mode;
				outdata[index++] = (float)blend_value;
				outdata[index++] = (float)gauss_value;

				outdata[index++] = (float)RenderBloom::BloomBrightParam().mClipParam;
				outdata[index++] = COL_BYTE_2_FLOAT(RenderBloom::BloomBrightParam().mPixelColorR);
				outdata[index++] = COL_BYTE_2_FLOAT(RenderBloom::BloomBrightParam().mPixelColorG);
				outdata[index++] = COL_BYTE_2_FLOAT(RenderBloom::BloomBrightParam().mPixelColorB);
				outdata[index++] = 0.0f;

				outdata[index++] = (float)RenderBloom::BloomHSBParam().mHue;
				outdata[index++] = (float)RenderBloom::BloomHSBParam().mSaturation;
				outdata[index++] = (float)RenderBloom::BloomHSBParam().mBright;

				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;


		case eCMD_SET_BLOOM_TYPE:            // �u���[���^�C�v�w��
			{
				float bloom_type;
				GET_BUF(bloom_type);
				DWORD tmp;
				tmp = (DWORD)bloom_type;
				RenderBloom::SetBloomType((RenderBloom::eBloomType)tmp);
				{
					float blend_mode, blend_value, gauss_value;
					GET_BUF(blend_mode);
					GET_BUF(blend_value);
					GET_BUF(gauss_value);
					RenderBloom::SetBloomParam((BYTE)blend_mode, (WORD)blend_value, (WORD)gauss_value);
				}
				{
					float clip_param, r, g, b;
					GET_BUF(clip_param);
					GET_BUF(r);
					GET_BUF(g);
					GET_BUF(b);
					RenderBloom::tBloomBrightParam param;
					param.mClipParam = (SHORT)clip_param;
					param.mPixelColorR = (BYTE)r;
					param.mPixelColorG = (BYTE)g;
					param.mPixelColorB = (BYTE)b;
					RenderBloom::BloomBrightParam() = param;
				}
				{
					float hue, saturation, bright;
					GET_BUF(hue);
					GET_BUF(saturation);
					GET_BUF(bright);
					RenderBloom::tBloomHSBParam param;
					param.mHue = (SHORT)hue;
					param.mSaturation = (SHORT)saturation;
					param.mBright = (SHORT)bright;
					RenderBloom::BloomHSBParam() = param;
				}
			}
			break;

		case eCMD_SET_BLOOM_PARAM:           // �u���[���p�����[�^�ݒ�
			{
				float blend_mode, blend_value, gauss_value;
				GET_BUF(blend_mode);
				GET_BUF(blend_value);
				GET_BUF(gauss_value);
				RenderBloom::SetBloomParam((BYTE)blend_mode, (WORD)blend_value, (WORD)gauss_value);
			}
			break;

		case eCMD_SET_BLOOM_BRIGT_PARAM:     // �u���[�����邳���ݒ�
			{
				float clip_param, r, g, b;
				GET_BUF(clip_param);
				GET_BUF(r);
				GET_BUF(g);
				GET_BUF(b);

				RenderBloom::tBloomBrightParam param;
				param.mClipParam = (SHORT)clip_param;
				param.mPixelColorR = (BYTE)r;
				param.mPixelColorG = (BYTE)g;
				param.mPixelColorB = (BYTE)b;
				RenderBloom::BloomBrightParam() = param;
			}
			break;

		case eCMD_SET_BLOOM_HSB_PARAM:       // �u���[���F���E�ʓx�E���x�ݒ�
			{
				float hue, saturation, bright;
				GET_BUF(hue);
				GET_BUF(saturation);
				GET_BUF(bright);

				RenderBloom::tBloomHSBParam param;
				param.mHue = (SHORT)hue;
				param.mSaturation = (SHORT)saturation;
				param.mBright = (SHORT)bright;
				RenderBloom::BloomHSBParam() = param;
			}
			break;
			
		case eCMD_GET_NPC_NUM:           // NPC�����擾
			{
				float outdata[1];
				outdata[0] = (float)CharaNpc::GetNpcResourceNum();
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_SET_NPC_NO:            // NPC�ԍ��w��
			{
				float npc_no = 0;
				GET_BUF(npc_no);
				sNpcNo = (BYTE)npc_no;
			}
			break;

		case eCMD_UPDATE_NPC_INFO:       // NPC���̎擾�E�X�V
			{
				float outdata[6];
				outdata[0] = (float)sStageIndex;
				outdata[1] = (sReviseDiffPos.x + spNpc[sNpcNo].Position().x) / spBGInfo->mScale;
				outdata[2] = (sReviseDiffPos.y + spNpc[sNpcNo].Position().y) / spBGInfo->mScale;
				outdata[3] = (sReviseDiffPos.z + spNpc[sNpcNo].Position().z) / spBGInfo->mScale;
				outdata[4] = Utility::RadToDeg(spNpc[sNpcNo].Rotation().y);
				outdata[5] = (float)CharaNpc::GetCsvSubNo();
				
				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_SET_NPC_POS_X:           // ���W�ݒ�
			{
				float add_pos;
				GET_BUF(add_pos);
				spNpc[sNpcNo].Position().x += add_pos;
			}
			break;

		case eCMD_SET_NPC_POS_Z:           // ���W�ݒ�
			{
				float add_pos;
				GET_BUF(add_pos);
				spNpc[sNpcNo].Position().z += add_pos;
			}
			break;

		case eCMD_SET_NPC_ROT_Y:           // ��]�ݒ�
			{
				float set_rot;
				GET_BUF(set_rot);
				spNpc[sNpcNo].Rotation().y = Utility::DegToRad(set_rot);
			}
			break;

		case eCMD_SET_NPC_PLAYER_POS:    // NPC���v���C���[���W�ɐݒ�
			{
				spNpc[sNpcNo].Position() = sPlayer.Position();
				spNpc[sNpcNo].Position().y += 1.0f;
			}
			break;

		case eCMD_SET_NPC_ONCOLLISION:   // �v���C���[��n�ʂɐݒu������
			{
				spNpc[sNpcNo].SetPositionOnCollision();
			}
			break;

		case eCMD_LOAD_NPC_INFO:         // NPC���̃��[�h
			{
				spNpc[sNpcNo].ReloadInformation(sStageIndex, sReviseDiffPos);
			}
			break;

		case eCMD_SET_PLAYER_BG_POS:
			{
				float pos_no;
				GET_BUF(pos_no);
				getPlayerPosRotFromBGModel(&sPlayer.Position(), &sPlayer.Rotation().y, (int)pos_no);
			}
			break;

		case eCMD_GET_EVENT_FLAG:        // �C�x���g�t���O�擾
			{
				ULONG value = 0;
				ULONG tmp = 0;

				int place_index = ConvertPlaceIndex_TO_EventDefine(sStageIndex);
				if (place_index != eEVENT_ERROR) {
					value = TotalInfo::GetInstance()->GetEventFlagValue(place_index);
				}

				tmp = (value & 0x000000FF) >> 0;
				outdata[0] = (float)tmp;
				tmp = (value & 0x0000FF00) >> 8;
				outdata[1] = (float)tmp;
				tmp = (value & 0x00FF0000) >> 16;
				outdata[2] = (float)tmp;
				tmp = (value & 0xFF000000) >> 24;
				outdata[3] = (float)tmp;

				memcpy(p_cmdbuf, outdata, cmd_size);
			}
			break;

		case eCMD_SET_EVENT_FLAG:        // �C�x���g�t���O�ݒ�
			{
				DWORD value = 0;
				float tmpA, tmpB, tmpC, tmpD;
				GET_BUF(tmpA);
				GET_BUF(tmpB);
				GET_BUF(tmpC);
				GET_BUF(tmpD);
				value = ((DWORD)tmpA << 0) + ((DWORD)tmpB << 8) + ((DWORD)tmpC << 16) + ((DWORD)tmpD << 24);
				
				int place_index = ConvertPlaceIndex_TO_EventDefine(sStageIndex);
				if (place_index != eEVENT_ERROR) {
					TotalInfo::GetInstance()->SetEventFlagValue(place_index, value);
				}
			}
			break;

		case eCMD_GET_NEAREST_GIMMICK_NO:
			{
				float	nearest_dist = 99999.9f;
				float	dist = 0.0f;
				VECTOR	ch_pos = sPlayer.Position();
				int		nearest_index = -1;

				GimmickBase* p_gimmick = NULL;
				for (int iI = 0; iI < GimmickBase::GetGimmickBaseNum(); iI ++) {
					p_gimmick = (GimmickBase *)GimmickBase::GetGimmickBase(iI);
					dist = VSize(VSub(p_gimmick->Position(), ch_pos));
					if (nearest_dist > dist) {
						nearest_dist = dist;
						nearest_index= iI;
					}
				}

				outdata[0] = (float)nearest_index;
				memcpy(p_cmdbuf, outdata, cmd_size);				
			}
			break;

		case eCMD_PLAY_EFFECT:
			{
				BYTE eff_no, chr_no, node_name;
				float size, second;
				
				GET_BUF(eff_no);
				GET_BUF(chr_no);
				GET_BUF(node_name);
				GET_BUF(size);
				GET_BUF(second);

				VECTOR eff_pos;
				eff_pos = sPlayer.Position();

				int eff_handle = EffectManager::GetInstance()->Play(eff_no,
																	&eff_pos,
																	&VGet(size, size, size),
																	0.0f);
				EffectManager::GetInstance()->Stop(eff_handle, second);

			}
			break;

		default:
			break;
		};
	/**** ��M�f�[�^�̏ꍇ ****/
	if( cmd_dir == eRECV) {
		// �R�}���h�ڍ׏����擾
		ret = PIC_LIB::SendCommand((char *)p_cmdbuf, cmd_size);
	} else if( cmd_dir == eSEND_RECV) {
		// �R�}���h�ڍ׏����擾
		ret = PIC_LIB::SendCommand((char *)p_cmdbuf, cmd_size);
	}

	delete[] p_cmdbuf;
}
#endif // _DEBUG

/**** end of file ****/
