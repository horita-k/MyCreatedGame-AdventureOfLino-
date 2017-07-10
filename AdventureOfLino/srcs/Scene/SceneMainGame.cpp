/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ���C�u�����̐F�X�Z�������e�X�g
 * @author SPATZ.
 * @data   2011/11/21 00:58:44
 */
//---------------------------------------------------
#include <math.h>
#include <vector>

#include <d3dx9math.h>

#include "SceneManager.h"
#include "SceneMainGame.h"

#include "AppLib/Basic/Basic.h"
#include "AppLib/Basic/Counter.h"
#include "AppLib/Basic/Path.h"
#include "AppLib/Input/InputManager.h"

#include "Scene/ActionTaskManager.h"
#include "Scene/TotalInfo.h"
#include "Scene/EventManager.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Graphic/ShadowMap.h"
#include "AppLib/Graphic/RenderBloom.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Graphic/ModelPmd.h"
#include "AppLib/Graphic/ModelMap.h"
#include "AppLib/Effect/EffectManager.h"
#include "AppLib/Graphic/EffekseerManager.h"
#include "AppLib/Sound/SoundManager.h"
#include "AppLib/Collision/CollisionManager.h"
#include "DataDefine/VoiceDef.h"
#include "DataDefine/MessageDef.h"
#include "DataDefine/MotionDef.h"
#include "DataDefine/BGDef.h"
#include "DataDefine/EffectDef.h"
#include "DataDefine/CharaDef.h"
#include "Charactor/CharaInfo.h"
#include "Object/ObjectBase.h"
#include "Object/ObjectTresure.h"
#include "Object/ObjectSwitch.h"
#include "Object/ObjectItem.h"
#include "Object/ObjectHand.h"
#include "Object/ObjectSeesaw.h"
#include "Gimmick/GimmickBase.h"
#include "Gimmick/GimmickExtend.h"
#include "Charactor/CharaPlayer.h"
#include "Charactor/CharaEnemy.h"
#include "Charactor/CharaNPC.h"
#include "Charactor/CharaNavi.h"
#include "AppLib/Resource/ResourceManager.h"
#include "AppLib/Resource/ResourceModel.h"
#include "AppLib/Resource/ResourceLayout.h"
#include "dxlib.h"
#include "Utility/Utility.h"
#include "AppLib/Graphic/LayoutNumber.h"

#include "EventFunctions.h"
#include "CommandFunctions.h"

/* SubScene */
#include "SubSceneItemMenu.h"
#include "SubSceneFlyingMove.h"
#include "SubSceneGetItem.h"
#include "SubScenePlayOcarina.h"

// eBG_TYPE_ITEM �̃A�C�e���^�̕�����
#define	BG_TYPE_ITEM_MATERIAL_NAME_LEN		(16)
#define BG_TYPE_ITEM_KEYWORD				("Tresu")
#define LABEL_TYPE_NAME_LEN					(5)
#define LABEL_OBJECT_NAME_LEN				(3)
#define LABEL_ITEM_NAME_LEN					(3)
#define LABEL_ROT_NAME_LEN					(3)


static const USHORT	 kCHECK_USING_MEMORY_ATTENTION_SIZE	= 1300;					// �g�p�������x���T�C�Y

static VECTOR	sReviseShadowPos	= ZERO_VEC;
static USHORT	sCheckObjectIndex	= 0;
static USHORT	sCheckNpcIndex		= 0;
static BYTE		sItemMenuDefault	= (BYTE)SubSceneItemMenu::eMENU_KIND_ITEM;

/*=====================================*
 * �t�F�[�Y�֐�
 *=====================================*/
/* static */SceneMainGame::PHASE_FUNC
SceneMainGame::mBasePhaseFunc[SceneMainGame::ePHASE_MAX] = {
	&SceneMainGame::phaseLoadBefore,
	&SceneMainGame::phaseLoading,
	&SceneMainGame::phaseSetup,
	&SceneMainGame::phaseGameBefore,
	&SceneMainGame::phaseGame,
	&SceneMainGame::phaseShowMap,
	&SceneMainGame::phaseChangeStage,
	&SceneMainGame::phaseFallDown,
	&SceneMainGame::phaseRecovery,
	&SceneMainGame::phaseMessageBefore,
	&SceneMainGame::phaseMessage,
	&SceneMainGame::phaseEventBefore,
	&SceneMainGame::phaseEvent,
	&SceneMainGame::phaseGetItemBefore,
	&SceneMainGame::phaseGetItem,
	&SceneMainGame::phaseGameOverBefore,
	&SceneMainGame::phaseGameOver,
	&SceneMainGame::phaseStageclear,
	&SceneMainGame::phaseShop,
	&SceneMainGame::phaseSelectWeapon,
	&SceneMainGame::phaseSelectWeaponEnd,
	&SceneMainGame::phaseStartBattle,
	&SceneMainGame::phaseNaviTalk,
	&SceneMainGame::phaseNaviTalkEnd,
	&SceneMainGame::phaseSave,
	&SceneMainGame::phaseMovePlace,
	&SceneMainGame::phaseFlyingMove,
	&SceneMainGame::phasePlayOcarina,
	&SceneMainGame::phaseTestConnect,
};

static tBGInfo sBGInfoTable[] = {
	{TRUE,  eRES_STG_SKYLAND,                      15.0f,   BGM_SKYLAND,           eENE_TBL_IDX_NONE,       eACTION_OK,}, // 0 �󒆒뉀
	{TRUE,  eRES_STG_AIR_RUINS,                    15.0f,   BGM_DUNGEON_B,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 1 ��t�H�[�X
	{TRUE,  eRES_STG_ROVININ_VILLAGE,              1.5f,    BGM_SKYLAND,           eENE_TBL_IDX_NONE,       eACTION_OK,}, // 2 
	{FALSE, eRES_STG_NONE,                         14.0f,   BGM_ZELDA,             eENE_TBL_IDX_001,        eACTION_OK,}, // 3 (���g�p)
	{FALSE, eRES_STG_NONE,                         14.0f,   BGM_ZELDA,             eENE_TBL_IDX_001,        eACTION_OK,}, // 4 (���g�p)
	{FALSE, eRES_STG_NONE,                         14.0f,   BGM_ZELDA,             eENE_TBL_IDX_003,        eACTION_OK,}, // 5 (���g�p)
	{FALSE, eRES_STG_FORTRESS_CITY,                10.0f,   BGM_TOWN,              eENE_TBL_IDX_001,        eACTION_OK,}, // 6 
	{FALSE, eRES_STG_FORTRESS_FIELD,               10.0f,   BGM_ZELDA,             eENE_TBL_IDX_002,        eACTION_OK,}, // 7 
	{FALSE, eRES_STG_JAPAN_1,                      1.65f,   BGM_JAPAN,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 8 ���̍�1
	{TRUE,  eRES_STG_JAPAN_2,                      1.65f,   BGM_JAPAN,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 9 ���̍�2
	{FALSE, eRES_STG_REVELNEAR2ND_EARTH,           10.0f,   BGM_FIELD,             eENE_TBL_IDX_010,        eACTION_OK,}, // 10 ���x�E��n
	{FALSE, eRES_STG_REVELNEAR2ND_HORBOR,          10.0f,   BGM_HARBOR,            eENE_TBL_IDX_NONE,       eACTION_NG,}, // 11 ���x�E�`
	{FALSE, eRES_STG_FRESS_ROOM,                   10.0f,   BGM_CASTLE_2,          eENE_TBL_IDX_NONE,       eACTION_NG,}, // 12 �t���X�̕���
	{FALSE, eRES_STG_REVELNEAR2ND_RICHCITY,        10.0f,   BGM_LIBELUNIA,         eENE_TBL_IDX_NONE,       eACTION_NG,}, // 13 ���x�E�M���X
	{FALSE, eRES_STG_VUAL_LIBRARY,                 1.0f,    BGM_COLLEGE,           eENE_TBL_IDX_NONE,       eACTION_NG,}, // 14 �_�~�[
	{TRUE,  eRES_STG_MARISA_ROOM,                  1.5f,    BGM_LIBELUNIA,         eENE_TBL_IDX_NONE,       eACTION_NG,}, // 15 �_�~�[
	{FALSE, eRES_STG_FOREST,                       24.0f,   BGM_DUNGEON_E,         eENE_TBL_IDX_016,        eACTION_OK,}, // 16 �Y�p�̐X
	{TRUE,  eRES_STG_OLD_PALACE,                   30.0f,   BGM_FORCE_PLACE,       eENE_TBL_IDX_TEST,       eACTION_OK,}, // 17 �Â��{�a
	{TRUE,  eRES_STG_DEPTH_SEA,                    10.0f,   BGM_FORCE_PLACE,       eENE_TBL_IDX_TEST,       eACTION_OK,}, // 18 �_�~�[
	{FALSE, eRES_STG_FOREST_CAVE,                  10.0f,   BGM_DUNGEON_E,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 19 �X�̕�ꏊ
	{TRUE,  eRES_STG_GRAVEYARD,                    2.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_OK,}, // 20 ��n
	{FALSE, eRES_STG_REVELNEAR2ND_MERGE,           10.0f,   BGM_LIBELUNIA,         eENE_TBL_IDX_NONE,       eACTION_NG,}, // 21 ���x���j�A�X
	{FALSE, eRES_STG_KITCHEN,                      1.2f,    BGM_LINO_THEMA,        eENE_TBL_IDX_NONE,       eACTION_NG,}, // 22 ���m�̉�
	{TRUE,  eRES_STG_LIBRARY,                      10.0f,   BGM_LIBELUNIA,         eENE_TBL_IDX_NONE,       eACTION_NG,}, // 23 �}����
	{FALSE, eRES_STG_WATERWAY,                     14.0f,   BGM_DUNGEON_A,         eENE_TBL_IDX_024,        eACTION_OK,}, // 24 �n�����H
	{FALSE, eRES_STG_PRISON,                       2.0f,    BGM_ELDA_THEMA,        eENE_TBL_IDX_NONE,       eACTION_NG,}, // 25 �n���S
	{FALSE, eRES_STG_UNDERGROUND_LAKE,             50.0f,   BGM_FORCE_PLACE,       eENE_TBL_IDX_026,        eACTION_OK,}, // 26 �n���
	{FALSE, eRES_STG_TREASURE_ROOM,                10.0f,   BGM_DUNGEON_A,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 27 �󕔉�
	{FALSE, eRES_STG_CRYSTAL_CAVE,                 50.0f,   BGM_FORCE_PLACE,       eENE_TBL_IDX_028,        eACTION_OK,}, // 28 �N���X�^������
	{FALSE, eRES_STG_CASTLE_KINGDOM,               10.0f,   BGM_CASTLE,            eENE_TBL_IDX_029,        eACTION_OK,}, // 29 ���x���j�A����
	{TRUE,  eRES_STG_SAILLING_SHIP,                16.0f,   BGM_SHIP,              eENE_TBL_IDX_NONE,       eACTION_OK,}, // 30 ���D
	{TRUE,  eRES_STG_WATER_CAVE,                   20.0f,   BGM_DUNGEON_B,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 31 �C�t�H�[�X
	{TRUE,  eRES_STG_BIG_SHIP,                     3.0f,    BGM_DUNGEON_A,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 32 �_�~�[
	{FALSE, eRES_STG_LINO_HOUSE,                   2.0f,    BGM_LINO_THEMA,        eENE_TBL_IDX_NONE,       eACTION_OK,}, // 33 ���m�̉�
	{FALSE,	eRES_STG_LOST_FOREST,                  2.0f,	BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_OK,}, // 34 �����̐X
	{FALSE, eRES_STG_VILLAGE,                      1.3f,    BGM_VILLAGE,           eENE_TBL_IDX_NONE,       eACTION_NG,}, // 35 �W���g��
	{FALSE, eRES_STG_SULT_MANE,                    4.5f,    BGM_DUNGEON_F,         eENE_TBL_IDX_036,        eACTION_OK,}, // 36 �X���g�z�R
	{FALSE, eRES_STG_SULT_MANE_TO_HARAPPA,         4.5f,    BGM_DUMMY,             eENE_TBL_IDX_036,        eACTION_OK,}, // 37 �X���g�z�R�E(�p�~)
	{FALSE, eRES_STG_CRYSTAL_ROOM,                 7.0f,    BGM_CRYSTAL_PLACE,     eENE_TBL_IDX_NONE,       eACTION_NG,}, // 38 �N���X�^�����[��
	{FALSE, eRES_STG_SULT_MANE_TROLLEY,            4.5f,    BGM_DUNGEON_F_2,       eENE_TBL_IDX_039,        eACTION_OK,}, // 39 �X���g�z�R�E�g���b�R
	{FALSE, eRES_STG_HARAPPA_SUB,                  7.5f,    BGM_DUNGEON_C,         eENE_TBL_IDX_040,        eACTION_OK,}, // 40 �n���b�p
	{FALSE, eRES_STG_PARANOIA,                     4.5f,    BGM_DUNGEON_G,         eENE_TBL_IDX_041,        eACTION_OK,}, // 41 �o�x���E
	{FALSE, eRES_STG_PARANOIA_CORE,                2.9f,    BGM_DUNGEON_G,         eENE_TBL_IDX_042,        eACTION_OK,}, // 42 �o�x���E�R�A
	{FALSE, eRES_STG_ELEVATOR,                     10.0f,   BGM_FORCE_PLACE,       eENE_TBL_IDX_NONE,       eACTION_OK,}, // 43 �o�x���E�{�X��
	{FALSE, eRES_STG_PARANOIA_SPIRAL,              16.0f,   BGM_DUNGEON_G,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 44 �o�x���E�K�i
	{FALSE, eRES_STG_AIRSHIP,                      1.5f,    BGM_SKYSHIP,           eENE_TBL_IDX_NONE,       eACTION_OK,}, // 45 ���D�_���X(�d��?)
	{FALSE, eRES_STG_BABEL_PRISON,                 18.0f,   BGM_DUNGEON_D,         eENE_TBL_IDX_046,		eACTION_OK,}, // 46 �o�x���E�S��
	{FALSE, eRES_STG_AIRSHIP,                      1.5f,    BGM_SKYSHIP,           eENE_TBL_IDX_NONE,       eACTION_OK,}, // 47 ���D�_���X
	{FALSE, eRES_STG_HARAPPA_MAIN,                 7.5f,    BGM_DUNGEON_D,         eENE_TBL_IDX_040,        eACTION_OK,}, // 48 �n���b�p�E���C��
	{FALSE, eRES_STG_HARAPPA_CAVE,                 1.5f,    BGM_DUNGEON_C,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 49 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 50 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 51 �_�~�[
	{FALSE, eRES_STG_PARANOIA_2F,                  10.0f,   BGM_DUNGEON_D,         eENE_TBL_IDX_041,		eACTION_OK,}, // 52 �o�x���̓�2F
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 53 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 54 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 55 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 56 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 57 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 58 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 59 �_�~�[
	{TRUE,  eRES_STG_KARUN,                        20.0f,   BGM_DUNGEON_Z,         eENE_TBL_KARUN_A,        eACTION_OK,}, // 60 
	{TRUE,  eRES_STG_KARUN_THRONE,                 20.0f,   BGM_DUNGEON_Z,         eENE_TBL_KARUN_BOSS_A,   eACTION_OK,}, // 61 
	{TRUE,  eRES_STG_KARUN_BARADGULARAN,           200.0f,  BGM_DUNGEON_Z,         eENE_TBL_KARUN_BOSS_A,   eACTION_OK,}, // 62 
	{TRUE,  eRES_STG_KARUN_CASTLE,                 20.0f,   BGM_DUNGEON_Z,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 63 
	{TRUE,  eRES_STG_KARUN_GATE,                   20.0f,   BGM_DUNGEON_Z,         eENE_TBL_KARUN_A,        eACTION_OK,}, // 64 
	{TRUE,  eRES_STG_KARUN_TOWN,                   20.0f,   BGM_DUNGEON_Z,         eENE_TBL_IDX_NONE,       eACTION_OK,}, // 65 
	{TRUE,  eRES_STG_KARUN_FORGE,                  20.0f,   BGM_DUNGEON_Z,         eENE_TBL_KARUN_A,        eACTION_OK,}, // 66 
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 67 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 68 �_�~�[
	{FALSE, eRES_STG_NONE,                         0.0f,    BGM_DUMMY,             eENE_TBL_IDX_NONE,       eACTION_NG,}, // 69 �_�~�[
#ifdef _DEBUG
	/**** ��������ʂ͈ړ��֎~ ****/
	// 70
	{TRUE , eRES_STG_PITAGORA,						15.0f, 	BGM_BOSS_W, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//������
	{TRUE , eRES_STG_BAKUGEKI_CASTLE,				15.0f, 	BGM_DUNGEON_B, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//������
	{TRUE , eRES_STG_NEO_CITY,						15.0f, 	BGM_DUNGEON_B, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//Neo-City
	{TRUE , eRES_STG_SHINGEKI_NO_KABE,				50.0f, 	BGM_DUNGEON_B, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//������
	{TRUE , eRES_STG_BLUE_DEVIL,					200.0f, BGM_DUNGEON_B, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//�S
	{TRUE , eRES_STG_VALLEY,						5000.0f,BGM_DUNGEON_B, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_ASH_STAGE,						50.0f,	BGM_DUNGEON_B, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_DEVIL_CASTLE,					15.0f, 	BGM_DUNGEON_B, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//�������ۂ���
	{FALSE, eRES_STG_KINGDOM_GARDEN,				10.0f, 	BGM_DUNGEON_B,			 	eENE_TBL_IDX_NONE, 		eACTION_OK, },//�����x���j�A����
	{FALSE, eRES_STG_HAKUGYOKURO,					2.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//���ʘO
	// 80   
	{TRUE,  eRES_STG_MY_FOREST,						10.0f, 	BGM_DUNGEON_3,	 			eENE_TBL_IDX_TEST, 		eACTION_OK, },
	{TRUE,  eRES_STG_BLACK_BOX,						10.0f, 	BGM_DUMMY, 					eENE_TBL_IDX_TEST, 		eACTION_OK, },
	//	{TRUE,  eRES_STG_BLACK_BOX,						4.5f, 	BGM_DUMMY, 					eENE_TBL_IDX_TEST, 		eACTION_OK, },
	
	{TRUE,  eRES_STG_INN,							16.0f, 	BGM_TOWN,		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },

	{FALSE, eRES_STG_CHIREIDEN_HONDEN,				20.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//�n��a
	{FALSE, eRES_STG_CHIREIDEN_ROBY,				20.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE,  eRES_STG_SIN_REI_CHO,					3.0f, 	BGM_TOWN,		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE,  eRES_STG_SHIRA_TAMA_ROW,				3.0f, 	BGM_TOWN,		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },

	{FALSE, eRES_STG_ARENA,							10.0f, 	BGM_BOSS_B, 	 			eENE_TBL_IDX_ARENA,		eACTION_OK, },
	{TRUE,  eRES_STG_BIG_WOOD,						10.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE,  eRES_STG_MIKAGETSU,						10.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_OUTSET_ISLAND,					10.0f, 	BGM_ZELDA, 					eENE_TBL_IDX_NONE, 		eACTION_OK, },

	{TRUE ,eRES_STG_MOB_HARBOR,						22.0f, 	BGM_DUNGEON_G, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//
	{TRUE ,eRES_STG_STEEMPANK_FACTORY,				22.0f, 	BGM_DUNGEON_G, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//

	{TRUE ,eRES_STG_TEMPLE_OF_FOREST,				22.0f, 	BGM_DUNGEON_G, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//
	{TRUE ,eRES_STG_FIRST_PLACE,					22.0f, 	BGM_DUNGEON_G, 	 			eENE_TBL_IDX_NONE, 		eACTION_OK, },//

	{TRUE , eRES_STG_NIGHTS_BAR,					12.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	// 90   
	{FALSE, eRES_STG_KAIPELINA,						6.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_NEO_VENICE,					1.4f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{FALSE ,eRES_STG_HARRAPPA_TEST,					7.5f, 	BGM_TOWN,					eENE_TBL_IDX_NONE, 		eACTION_OK, },//�n���b�p
	{TRUE , eRES_STG_COOL_COOL_MOUNTAIN,			20.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_KAGITABA_FIELD_01,				1.4f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_KAGITABA_FIELD_02,				1.4f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_JOSAITOSHI,					24.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_TOY_BOX_STAGE,					3.0f, BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_STAR_CHART_STAGE,				1.0f, BGM_DUNGEON_3, 		 		eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_SKY_COLOR_CITY,				140.0f, BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	// 100
	{TRUE , eRES_STG_GARAGE,						2.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },

	{TRUE , eRES_STG_CAVE,							30.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_CAVE_LOAD,						150.0f,	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },

	{FALSE ,eRES_STG_PARANOIA_PIPE,					4.5f, 	BGM_DUNGEON_G, 	 			eENE_TBL_IDX_041, 		eACTION_OK, },//�o�x���E

	{TRUE , eRES_STG_UTOPIA,						4.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_SAN_MARUKO_PLAZA,				1.4f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_US_CONSTITUTION,				16.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_VILLAGE_077,					1.5f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_HARBOR,						1.5f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_TANI,							1.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_MEIRO,							2.0f, 	BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_DRMITORY,						100.0f, BGM_TOWN, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_SAIDAN_MQO,					20.0f, 	BGM_ZELDA, 		 			eENE_TBL_KARUN_A, 		eACTION_OK, },
	{TRUE , eRES_STG_SAIDAN_X,						200.0f, BGM_ZELDA, 		 			eENE_TBL_KARUN_A, 		eACTION_OK, },
	{TRUE , eRES_STG_ACTION,						10.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_BALL_COASTER,					200.0f, BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_PLANETS_CORE,					80.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_002, 		eACTION_OK, },
	{TRUE , eRES_STG_CAMOME_CITY,					35.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_NG, },
	{TRUE , eRES_STG_BOMB_BATTLE_FIELD,				20.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_REVELNEAR,						20.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_TMP_STAGE,						10.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_AREA_C,						500.0f, BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_BOSS_STAGE,					100.0f, BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_WARM_ISLAND,					10.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },
	{TRUE , eRES_STG_HALLOWEEN_TOWN,				20.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_004, 		eACTION_OK, },
	{TRUE , eRES_STG_GAREKI_CITY,					14.0f, 	BGM_ZELDA, 		 			eENE_TBL_IDX_NONE, 		eACTION_OK, },

#endif // _DEBUG
};
static const unsigned short kBG_INFO_NUM = sizeof(sBGInfoTable) / sizeof(tBGInfo);

/*=====================================*
 * �G�t�F�N�g�֘A
 *=====================================*/
// �G�t�F�N�g���\�[�X���
struct tEffectResourceInfo {
	const char* mShaderName;	// fx��
	const char* mModelName;		// ���f����
};
// �g�p�G�t�F�N�g���X�g
static const tEffectResourceInfo sEffectResourceTable[eEF_MAX] = {
	{"Flame/Flame.fx",				"FlameMdlCmn.x"},			//	eEF_FLAME
	{"HitP/HitP.fx",				"DustMdlCmn.x"},			//	eEF_HIT
	{"Thunder/Thunder.fx",			"LightMdlCmn.x"},			//	eEF_THUNDER
	{"Aura/Aura.fx",				"WindMdlCmn.x"},			//	eEF_AURA
	{"Wind/Wind.fx",				"WindMdlCmn.x"},			//	eEF_WIND
	{"DustP/DustP.fx",				"DustMdlCmn.x"},			//	eEF_DUST
	{"Study/Study.fx",				NULL},						//	eEF_MY_TAIL
	{"BlackDustP/BlackDustP.fx",	"DustMdlCmn.x"},			//	eEF_BLACKDUST
	{"WaterDustP/WaterDustP.fx",	"DustMdlCmn.x"},			//	eEF_WATERDUST
	{"LightningEx/LightningEx.fx",	"LightMdlCmn.x"},			//	eEF_LIGHTNING_EX
	{"Lightning/Lightning.fx",		"LightMdlCmn.x"},			//	eEF_LIGHTNING
	{"Navi/Navi.fx",				"ParticleMdlCmn.x"},		//	eEF_NAVI
	{"EnergyBall/EnergyBall.fx",	"ParticleMdlCmn.x"},		//	eEF_ENERGY_BALL
	{"Navi/Navi.fx",				"ParticleMdlCmn.x"},		//	eEF_PROTECT
	{"WindFire/WindFire.fx",		"WindMdlCmn.x"},			//	eEF_WINDFIRE
	{"EnergyBall/EnergyBall.fx",	"ParticleMdlCmn.x"},		//	eEF_ENERGY_BALL_R
	{"EnergyBall/EnergyBall.fx",	"ParticleMdlCmn.x"},		//	eEF_ENERGY_BALL_G
	{"EnergyBall/EnergyBall.fx",	"ParticleMdlCmn.x"},		//	eEF_ENERGY_BALL_B
	{"Marui/Marui.fx",				"ParticleMdlCmn.x"},		//	eEF_MARUI
	{"FlameBomb/FlameBomb.fx",		"SmokeMdlCmn.x"},			//	eEF_FLAME_BOMB
	{"ChargeP/ChargeP.fx",			"DustMdlCmn.x"},			//	eEF_CHARGE
	{"Forest/Forest.fx",			"FlameMdlCmn.x"},			//	eEF_FOREST
	{"Arrow/Arrow.fx",				"FlameMdlCmn.x"},			//	eEF_ARROW
	{"Green/Navi.fx",				"ParticleMdlCmn.x"},		//	eEF_GREEN
	{"RealFire/RealFire.fx",		"TailMdlCmn.x"},			//	eEF_REALFIRE
	{"AutoSmoke2/AutoSmoke2.fx",	"SmokeMdlCmn.x"},			//	eEF_TORNADE
	{"RealFire/WhiteTail.fx",		"TailMdlCmn.x"},			//	eEF_WHITE_TAIL
	{"Sky/sky.fx",					NULL},						//	eEF_SKY
};

// Effekseer�̃G�t�F�N�g�ꗗ
static char* sEffekseerResourceName[eEFK_MAX] = {
	"MagicSquare.efk",				  // eEFK_MAGIC_SQUARE
	"MagicShield.efk",                // eEFK_SHIELD
	"CorrectItem3.efk",               // eEFK_MAGIC_BALL
	"Simple_Sprite_BillBoard.efk",    // eEFK_SPREAD
	"Simple_Sprite_FixedYAxis.efk",   // eEFK_DISAPPER

	"MagicHeal2.efk",                 // eEFK_PROTECT_START
	"MagicHeal1.efk",                 // eEFK_HEAL
	"Aya_Attack_grass.efk",           // eEFK_BREAK_BARRIER
	"Explosion.efk",                  // eEFK_EXPLOSION_BOMB
	"Aya_Attack_jar.efk",             // eEFK_BREAK_JAR

	"Boss_Death.efk",                 // eEFK_BOSS_DEATH
	"Shock.efk",                      // eEFK_SHOCK
	"Piyopiyo.efk",                   // eEFK_PIYOPIYO
	"ShockThin.efk",                  // eEFK_SHOCK_THIN
	"BreakLock.efk",                  // eEFK_BREAK_LOCK

	"Telekinesis.efk",                // eEFK_TELEKINESIS
	"GetKeyItem2.efk",                // eEFK_GETKEYITEM
	"Laser.efk",                      // eEFK_LASER
	"MekaDragonLaserShoot.efk",       // eEFK_CANNON_LASER_SHOOT
	"sonickwave.efk",                 // eEFK_SONICK_WAVE

	"wind.efk",                       // eEFK_WIND
	"remilia_heart_bullet.efk",       // eEFK_THRUST
	"HealPotion2.efk",                // eEFK_PORTION
	"MekaDragonLaserCharge.efk",      // eEFK_CANNON_LASER_CHARGE
	"utsuho_sun_area.efk",            // eEFK_AURA_FLAME

	"patch_stElmo_area.efk",          // eEFK_HELL_FLAME
	"RealFlame.efk",                  // eEFK_REAL_FLAME
	"TitleEffect.efk",                // eEFK_STAGE_CLEAR
	"LightningStrike.efk",            // eEFK_LIGHTNING_STRIKE
	"aya_senpu_area.efk",             // eEFK_SENPU_AREA

	"magma_effect.efk",               // eEFK_MAGMA
	"Thunder2.efk",                   // eEFK_HIT_LIGHTNING
	"MAKING.efk",					// eEFK_HIT_LIGHTNING

#ifdef _DEBUG
	"",								  // eEFK_DRAG_DROP_SAMPLE,
#endif // _DEBUG
};


/*=====================================*
 * �}�b�v�\����
 *=====================================*/
struct tMapIconWork {
	LayoutBase		mpLayout[8];
	int				mLayoutNum;
};
struct tShowMapWork {
	BOOL			mIsZoom;
	float			mCamDist;
	VECTOR			mCamCenterPos;
};
static tShowMapWork	sShowMapWork;
static tMapIconWork	sMapIconMove;

/*=====================================*
 * static values
 *=====================================*/
SceneBase*		spScenBase;		// �������g�̃|�C���^
CharaPlayer		sPlayer;
CharaEnemy**	sppEnemy				= NULL;
CharaNpc*		spNpc					= NULL;
ObjectBase**	sppObjectBase			= NULL;
int				sObjectBaseNum			= 0;
CharaEnemy*		spDisappearEnemy		= NULL;
ObjectItem		sDropItem[kENEMY_ITEM_NUM];
ModelMap		sModelMap;
ModelMap*		spSubModelMap			= NULL;
ModelBase		sModelXfile[eMD_MAX];
ModelPmd		sGimmick;

CharaNavi		sNavi;
LayoutBase*		spShopItemLayout		= NULL;

CsvAccesser		sEventCsv;
int				sStageIndex;
int				sStagePlaceIndex;
int				sIsMessageRange			= -1;
tBGInfo*		spBGInfo				= NULL;
ActionTaskManager::tTaskInfo sTaskInfo;
int				sEventBattleFlag		= -1;
CHAR			sNextEventNo			= -1;	// ���̃C�x���g�ԍ�
int				sEventRecoveryFlag		= -1;	// ��������������Ȃ��C�x���g�t���O
bool			sIsBattle				= false;
char*			sStrNaviTalkPtr			= NULL;
int				sLoadStatus				= 0;
int				sDungeonIndex			= 0;
BYTE			sDrawFreeFontValue		= eFREE_FONT_NONE;
int				sEventPhase				= 0;
int				sEventCount				= 0;
int				sBattleIndex			= -1;
VECTOR			sReviseDiffPos			= ZERO_VEC;
std::vector<eItemKind>	sMyWeaponList;			// �������X�g
std::vector<tStageInfo> sSubStageInfoList;		// �X�e�[�W�����X�g
LayoutBase*		spStageClearLayoutList	= NULL;	// �X�e�[�W�N���A�\�����̃��C�A�E�g
//static SubSceneItemMenu* spSubSceneItemMenu	= NULL;
static SubSceneBase* spSubSceneInst		= NULL;
StageFuncBase*  spStageFunc				= NULL;
int				sNpcNum;
float			sDissappearLength		= 12000.0f;
float			kGRAVITY_SPEED			= -0.98f;
BYTE			sOcarinaSongSelect		= 0;	// �I�J���i�I�ȃC���f�b�N�X


/* ���[�h���� */
enum {
	eLOAD_PLAYER = 0,
	eLOAD_ENEMY,
	eLOAD_NPC,
	eLOAD_SKYDOME,
	eLOAD_MAP,
	eLOAD_SUB_MAP,
	eLOAD_SHOW_A_MAP,
	eLOAD_OBJECT,
	eLOAD_TEXTURE,
	eLOAD_SOUND,
	eLOAD_EFFECT,

	eLOAD_MAX,
};

#ifdef _DEBUG
static const char* strLoadStateName[] = {
	"eLOAD_PLAYER ",	// eLOAD_PLAYER "
	"eLOAD_ENEMY",		// eLOAD_ENEMY"
	"eLOAD_NPC",		// eLOAD_NPC"
	"eLOAD_SKYDOME",	// eLOAD_SKYDOME"
	"eLOAD_MAP",		// eLOAD_MAP"
	"eLOAD_SUB_MAP",	// eLOAD_SUB_MAP"
	"eLOAD_SHOW_A_MAP",	// eLOAD_SHOW_A_MAP"
	"eLOAD_OBJECT",		// eLOAD_OBJECT"
	"eLOAD_TEXTURE",	// eLOAD_TEXTURE"
	"eLOAD_SOUND",		// eLOAD_SOUND"
	"eLOAD_EFFECT",		// eLOAD_EFFECT"
};
#endif // _DEBUG


//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
SceneMainGame::SceneMainGame()
{
	sStageIndex			= -1;
	sStagePlaceIndex	= -1;

#ifdef _DEBUG
	// �N���X�̃T�C�Y�𒲂ׂ�ׂ̃t���O
	static BYTE sClassDumpFlag = FALSE;

	if (sClassDumpFlag == FALSE) {

		PRINT_CONSOLE("  * ActionTaskManager \t%08d \n", sizeof(ActionTaskManager ));
		PRINT_CONSOLE("  * Basic \t%08d \n", sizeof(Basic ));
		PRINT_CONSOLE("  * BillboardBase \t%08d \n", sizeof(BillboardBase ));
		PRINT_CONSOLE("  * BladeEffect\t%08d \n", sizeof(BladeEffect));
//		PRINT_CONSOLE("  * BookSystem \t%08d \n", sizeof(BookSystem ));
		PRINT_CONSOLE("  * CameraManager\t%08d \n", sizeof(CameraManager));
		PRINT_CONSOLE("  * CharaBase\t%08d \n", sizeof(CharaBase));
		PRINT_CONSOLE("  * CharaEnemy\t%08d \n", sizeof(CharaEnemy));
		PRINT_CONSOLE("  * CharaNavi\t%08d \n", sizeof(CharaNavi));
		PRINT_CONSOLE("  * CharaNpc\t%08d \n", sizeof(CharaNpc));
		PRINT_CONSOLE("  * CharaPlayer\t%08d \n", sizeof(CharaPlayer));
		PRINT_CONSOLE("  * CollisionManager\t%08d \n", sizeof(CollisionManager));
		PRINT_CONSOLE("  * Counter\t%08d \n", sizeof(Counter));
		PRINT_CONSOLE("  * CsvAccesser \t%08d \n", sizeof(CsvAccesser ));
		PRINT_CONSOLE("  * EffectManager \t%08d \n", sizeof(EffectManager ));
		PRINT_CONSOLE("  * EffectNode \t%08d \n", sizeof(EffectNode ));
		PRINT_CONSOLE("  * EffectResource \t%08d \n", sizeof(EffectResource ));
//		PRINT_CONSOLE("  * EffectTest\t%08d \n", sizeof(EffectTest));
		PRINT_CONSOLE("  * EffekseerManager\t%08d \n", sizeof(EffekseerManager));
		PRINT_CONSOLE("  * EventManager \t%08d \n", sizeof(EventManager ));
		PRINT_CONSOLE("  * GimmickBase\t%08d \n", sizeof(GimmickBase));
		PRINT_CONSOLE("  * GimmickExtend_Hole\t%08d \n", sizeof(GimmickExtend_Hole));
		PRINT_CONSOLE("  * GimmickExtend_Lift\t%08d \n", sizeof(GimmickExtend_Lift));
		PRINT_CONSOLE("  * GimmickExtend_Motion\t%08d \n", sizeof(GimmickExtend_Motion));
		PRINT_CONSOLE("  * GimmickExtend_Roll\t%08d \n", sizeof(GimmickExtend_Roll));
		PRINT_CONSOLE("  * GimmickExtend_Ship\t%08d \n", sizeof(GimmickExtend_Ship));
		PRINT_CONSOLE("  * GimmickExtend_Tower\t%08d \n", sizeof(GimmickExtend_Tower));
		PRINT_CONSOLE("  * GimmickExtend_Trolley\t%08d \n", sizeof(GimmickExtend_Trolley));
		PRINT_CONSOLE("  * GimmickExtend_Wheel\t%08d \n", sizeof(GimmickExtend_Wheel));
		PRINT_CONSOLE("  * GraphicManager\t%08d \n", sizeof(GraphicManager));
		PRINT_CONSOLE("  * InputManager\t%08d \n", sizeof(InputManager));
		PRINT_CONSOLE("  * LayoutBase\t%08d \n", sizeof(LayoutBase));
//		PRINT_CONSOLE("  * LayoutNumber\t%08d \n", sizeof(LayoutNumber));
		PRINT_CONSOLE("  * ModelBase\t%08d \n", sizeof(ModelBase));
		PRINT_CONSOLE("  * ModelCollision\t%08d \n", sizeof(ModelCollision));
		PRINT_CONSOLE("  * ModelMap\t%08d \n", sizeof(ModelMap));
		PRINT_CONSOLE("  * ModelPmd\t%08d \n", sizeof(ModelPmd));
		PRINT_CONSOLE("  * ObjectBase\t%08d \n", sizeof(ObjectBase));
		PRINT_CONSOLE("  * ObjectHand\t%08d \n", sizeof(ObjectHand));
		PRINT_CONSOLE("  * ObjectHand_Bomb\t%08d \n", sizeof(ObjectHand_Bomb));
		PRINT_CONSOLE("  * ObjectHand_Jar\t%08d \n", sizeof(ObjectHand_Jar));
		PRINT_CONSOLE("  * ObjectHand_Lock\t%08d \n", sizeof(ObjectHand_Lock));
		PRINT_CONSOLE("  * ObjectHand_Stone\t%08d \n", sizeof(ObjectHand_Stone));
		PRINT_CONSOLE("  * ObjectHand_Usachu\t%08d \n", sizeof(ObjectHand_Usachu));
		PRINT_CONSOLE("  * ObjectItem\t%08d \n", sizeof(ObjectItem));
		PRINT_CONSOLE("  * ObjectSeesaw\t%08d \n", sizeof(ObjectSeesaw));
		PRINT_CONSOLE("  * ObjectSwitch\t%08d \n", sizeof(ObjectSwitch));
		PRINT_CONSOLE("  * ObjectTresure\t%08d \n", sizeof(ObjectTresure));
		PRINT_CONSOLE("  * ObjectWeapon\t%08d \n", sizeof(ObjectWeapon));
		PRINT_CONSOLE("  * ObjectWeapon_Arrow\t%08d \n", sizeof(ObjectWeapon_Arrow));
		PRINT_CONSOLE("  * ObjectWeapon_Meteo\t%08d \n", sizeof(ObjectWeapon_Meteo));
		PRINT_CONSOLE("  * ObjectWeapon_MeteoRefrect\t%08d \n", sizeof(ObjectWeapon_MeteoRefrect));
		PRINT_CONSOLE("  * ObjectWeapon_Missile\t%08d \n", sizeof(ObjectWeapon_Missile));
		PRINT_CONSOLE("  * Path\t%08d \n", sizeof(Path));
		PRINT_CONSOLE("  * ResourceLayout\t%08d \n", sizeof(ResourceLayout));
		PRINT_CONSOLE("  * ResourceManager\t%08d \n", sizeof(ResourceManager));
		PRINT_CONSOLE("  * ResourceModel\t%08d \n", sizeof(ResourceModel));
		PRINT_CONSOLE("  * SceneBase \t%08d \n", sizeof(SceneBase ));
//		PRINT_CONSOLE("  * SceneEvent\t%08d \n", sizeof(SceneEvent));
		PRINT_CONSOLE("  * SceneManager\t%08d \n", sizeof(SceneManager));
		PRINT_CONSOLE("  * SceneMainGame\t%08d \n", sizeof(SceneMainGame));
//		PRINT_CONSOLE("  * SceneSelectFile\t%08d \n", sizeof(SceneSelectFile));
//		PRINT_CONSOLE("  * SceneTitle\t%08d \n", sizeof(SceneTitle));
		PRINT_CONSOLE("  * Shadow \t%08d \n", sizeof(Shadow ));
		PRINT_CONSOLE("  * ShapeBase\t%08d \n", sizeof(ShapeBase));
		PRINT_CONSOLE("  * ShapeBoard\t%08d \n", sizeof(ShapeBoard));
		PRINT_CONSOLE("  * ShapeBox\t%08d \n", sizeof(ShapeBox));
		PRINT_CONSOLE("  * ShapeCapsule\t%08d \n", sizeof(ShapeCapsule));
		PRINT_CONSOLE("  * ShapeLine\t%08d \n", sizeof(ShapeLine));
		PRINT_CONSOLE("  * SoundComponent\t%08d \n", sizeof(SoundComponent));
		PRINT_CONSOLE("  * SoundManager\t%08d \n", sizeof(SoundManager));
		PRINT_CONSOLE("  * TotalInfo \t%08d \n", sizeof(TotalInfo ));
		PRINT_CONSOLE("  * Utility\t%08d \n", sizeof(Utility));
//		PRINT_CONSOLE("  * VectorNode\t%08d \n", sizeof(VectorNode));

		sClassDumpFlag = TRUE;

	}

#endif // _DEBUG
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
SceneMainGame::~SceneMainGame()
{
	TotalInfo::GetInstance()->SetEnableWinMenu(FALSE);

	Destroy();

	EventManager::GetInstance()->EndEvent();

	sPlayer.Destroy();


	// ���y���Ƃ߂�
	SoundManager::GetInstance()->StopBGM();
}

//---------------------------------------------------
/**
   �쐬
 */
//---------------------------------------------------
void
SceneMainGame::Create(int debugArg)
{
	SceneBase::Create(debugArg);

	spScenBase			= this;	// �������g�̃|�C���^���o�������Ă���
	sStageIndex			= mDebugArg;
	sStagePlaceIndex	= 0;

	// �f�[�^�����[�h����
	TotalInfo::tLoadWork load_work;
	TotalInfo::GetInstance()->GetLoadWork(&load_work);
	if (load_work.mPlaceIndex != 0) {	// ���[�h�����ł����
		sStageIndex							= load_work.mPlaceIndex;
		sPlayer.SetLoadParameter(load_work.mMaxHP, load_work.mNowHP, load_work.mNowMP);
		sPlayer.ItemWork().mItemSubWeapon	= (eItemKind)load_work.mNowWeapon;
		sPlayer.CosplayIndex()				= load_work.mNowCosplayIndex;
		sStagePlaceIndex					= load_work.mSubPlaceIndex;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::Update(void)
{
	int phase = mPhase;

	(this->*mBasePhaseFunc[mPhase])();

	if (mPhase > ePHASE_SETUP) {
		// HP�̉������n�[�g�̕`��
		int hitpoint = sPlayer.GetHitpoint();
		if (hitpoint < 0) hitpoint = 0;
		int max_hitpoint = sPlayer.GetMaxHitpoint();

		// �n�[�g�̃��C�A�E�g�̍X�V
		TotalInfo::GetInstance()->UpdateHeartLayout(max_hitpoint, hitpoint);
	}

	mPhaseCounter += kADD_PHASE_COUNTER_VALUE;
	// �t�F�[�Y���؂�ւ�����̂Ȃ�
	if (phase != mPhase) {

		// ���ʏ����ȊO�̓i�r�H�g�[�N�̐ݒ���N���A����
		if (!(( phase == ePHASE_SELECT_WEAPON_END || phase == ePHASE_PLAY_OCARINA) &&
			(mPhase == ePHASE_GAME))) {
			mPhaseCounter = 0.0f;
			// �i�r�H�̃g�[�N�C���f�b�N�X��ύX
			sStrNaviTalkPtr = NULL;
			// DrawFontFunc���N���A
			GraphicManager::GetInstance()->EntryDrawFontFunc(NULL);
		}
	}

	/* �h���b�O���h���b�v���̏������L�� */
	SetDragFileValidFlag(TRUE);
	int drag_num = GetDragFileNum();
	if (drag_num > 0) {
		char buf[256] = {0};
		GetDragFilePath(buf);

		// �g���q���擾����
		char extention[8] = {0};
		for (int iI = 0; iI < (signed)strlen(buf); iI ++) {
			if (buf[iI] == '.') {
				strcpy_s(extention, sizeof(extention), &buf[iI+1]);
			}
		}

		// �g���q����t�@�C����ʂ𔻕ʂ���
		if ((strcmp(extention, "ogg") == 0) || (strcmp(extention, "mp3") == 0) || (strcmp(extention, "wav") == 0)) {
			// �����t�@�C���ǂݍ��݂̏���
			PlayMusic(buf, DX_PLAYTYPE_LOOP);
			PRINT_CONSOLE("drag file name : %s \n", buf);
		} else if ((strcmp(extention, "mv1") == 0)) {
			// ���f���t�@�C���ǂݍ��݂̏���
			ResourceModel::GetInstance()->LoadDragMv1Resource(buf);
			sPlayer.DeleteModel();
			sPlayer.LinkModel(buf);
			sPlayer.Setup(false, sStageIndex);
			sPlayer.StartMotion(0, true, 20.0f);
		} else if ( (strcmp(extention, "jpg") == 0) ){
			// �X�J�C�h�[���e�N�X�`���t�@�C���ǂݍ��݂̏���
			int graph_handle = -1;
			char file_name[64] = {0};
			char file_path[32] = {0};
			Path::GetOnlyFileName(file_name, sizeof(file_name), buf);
			sprintf_s(file_path, sizeof(file_path), "Skydome/%s", file_name);
			ResourceLayout::GetInstance()->LoadLayoutResource(file_path);
			graph_handle = ResourceLayout::GetInstance()->GetHandle(file_path);
			MV1SetTextureGraphHandle(sModelXfile[eMD_SKYDOME].GetModelHandle(),
									 0,											// TexIndex ��0�Œ�
									 graph_handle,
									 FALSE);
#ifdef _DEBUG
		}
		else if ( (strcmp(extention, "efk") == 0) ) {

			EffekseerManager::GetInstance()->DeleteEffect(eEFK_DRAG_DROP_SAMPLE);

			char file_name[64] = {0};
			Path::GetOnlyFileName(file_name, sizeof(file_name), buf);
			PRINT_CONSOLE("drag file name : %s \n", file_name);

			EffekseerManager::GetInstance()->LoadResource(eEFK_DRAG_DROP_SAMPLE, file_name);
			
			VECTOR eff_pos = sPlayer.Position();
			eff_pos.y += sPlayer.BodyHeight();
			float eff_scale = 60.0f;

			EffekseerManager::GetInstance()->Play(eEFK_DRAG_DROP_SAMPLE,
												  &eff_pos,
												  eff_scale);
		}
		else if ( (strcmp(extention, "fx") == 0) ) {

			static int sEffHandle = -1;

			// �G�t�F�N�g�t�@�C���̓ǂݍ���
			char file_name[64] = {0};
			char dir_name[32]  = {0};
			char file_path[32] = {0};

			if (sEffHandle != -1) {
				EffectManager::GetInstance()->Stop(sEffHandle);
				sEffHandle =-1;
				return;
			}
			EffectManager::GetInstance()->UnloadResource(eEF_DRAG_DROP);

			Path::GetOnlyFileName(file_name, sizeof(file_name), buf);
			memcpy(dir_name, file_name, (strlen(file_name) - 3) );
			sprintf_s(file_path, sizeof(file_path), "%s/%s", dir_name, file_name);
			PRINT_CONSOLE("drag file name : %s \n", file_path);
			EffectManager::GetInstance()->LoadResource(eEF_DRAG_DROP, file_path, NULL);

			//			eff_handle = EffectManager::GetInstance()->Play(eEF_DRAG_DROP, &sPlayer.Position(), 50.0f, 0.0f);
			sEffHandle = EffectManager::GetInstance()->Play(eEF_DRAG_DROP, 
															&sPlayer.Position(), 
															10.0f);
			
#endif // _DEBUG
		}
	}

#ifdef _DEBUG
	if (CHECK_TRIG_KEY(DBG_KEY_DUMP_STATUS)) {

		PRINT_CONSOLE(" * ========================================================= * \n");
		PRINT_CONSOLE(" * Dump Start ============================================== * \n");
		PRINT_CONSOLE(" * ========================================================= * \n");
		// �C�x���g�t���O��\��������
		int place_index = ConvertPlaceIndex_TO_EventDefine(sStageIndex);
		if (place_index != eEVENT_ERROR) {
			ULONG flag = TotalInfo::GetInstance()->GetEventFlagValue(place_index);
			PRINT_CONSOLE(" * event flag[%d] = 0x%08x \n", place_index, flag);
		}

		/* CameraManager ��Dump */
		CameraManager::GetInstance()->DumpStatus();

		PRINT_CONSOLE(" * ========================================================= * \n");
		PRINT_CONSOLE(" * Dump End ================================================ * \n");
		PRINT_CONSOLE(" * ========================================================= * \n");
	}
#endif // _DEBUG

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
SceneMainGame::GetLoadingPercent(void)
{
	int load_num = GetASyncLoadNum();
	float rate = (float)load_num / (float)eLOAD_MAX;
	return (int)(rate * 100.0f);

	//	return (int)(((float)(eLOAD_MAX - GetASyncLoadNum()) / (float)eLOAD_MAX) * 100.0f);
	//	return (int)( ( (float)GetASyncLoadNum() / (float)eLOAD_MAX) * 100.0f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::ReviseAllModel(void)
{
#ifdef _DEBUG
	SoundManager::GetSndComponent()->PlaySound(eSE_DECIDE_KEY);
#endif // _DEBUG

	VECTOR move_vec = sPlayer.Position();
	ModelBase* p_model = NULL;

	// ReviseAllModel �̍��W���L��������
	sReviseDiffPos = VAdd(sReviseDiffPos, move_vec);
	PRINT_CONSOLE("revise all model, sReviseDiffPos = {%.2f, %.2f, %.2f}, dist = %.2f \n", 
				  sReviseDiffPos.x, sReviseDiffPos.y, sReviseDiffPos.z, VSize(sReviseDiffPos) );

	sPlayer.ReviseModel(&move_vec);
  
	for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
		p_model = sppEnemy[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}

	for (int iI = 0; iI < sNpcNum; iI ++) {
		p_model = &spNpc[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
		spNpc[iI].SetShadowVector(move_vec);
	}

	for (int iI = 0; iI < sObjectBaseNum; iI ++) {
		p_model = sppObjectBase[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}


	p_model = &sModelMap;
	p_model->Position() = VSub(p_model->Position(), move_vec);
	p_model->ForciblySetPositionByCentering(true);
	
	for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
		p_model = &spSubModelMap[ iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}

	for (int iI = 0; iI < eMD_MAX; iI ++) {
		p_model = &sModelXfile[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}

	EffectManager* const p_effmgr = EffectManager::GetInstance();
	for (int iI = 0; iI < EffectManager::kEFFECT_NODE_NUM; iI ++) {
		p_effmgr->Position(iI) = VSub(p_effmgr->Position(iI), move_vec);
	}

	// �M�~�b�N�N���X�� mOnPosition �̒l���X�V����
	GimmickBase::ReviseAllGimmickOnPosition(move_vec);

	CollisionManager::GetInstance()->Refresh();

	// �|�C���g���C�g�̍X�V
	{
		VECTOR pos;
		GraphicManager::GetInstance()->GetPLightPosition(&pos);
		pos = VSub(pos, move_vec);
		GraphicManager::GetInstance()->SetPLightPosition(&pos);
	}

	CameraManager::GetInstance()->ReviseCamera(move_vec);

	spStageFunc->ReviseAllModel(move_vec);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::PauseReviseAllModel(bool isPause)
{
#ifdef _DEBUG
	//	SoundManager::GetSndComponent()->PlaySound(eSE_DECIDE_KEY);
#endif // _DEBUG

	//	VECTOR move_vec = sPlayer.Position();
	VECTOR move_vec = VScale(sReviseDiffPos, (isPause == true) ? -1.0f : 1.0f);
	ModelBase* p_model = NULL;

	// ReviseAllModel �̍��W���L��������
	sPlayer.ReviseModel(&move_vec);
  
	for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
		p_model = sppEnemy[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}

	for (int iI = 0; iI < sNpcNum; iI ++) {
		p_model = &spNpc[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
		spNpc[iI].SetShadowVector(move_vec);
	}

	for (int iI = 0; iI < sObjectBaseNum; iI ++) {
		p_model = sppObjectBase[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}


	p_model = &sModelMap;
	p_model->Position() = VSub(p_model->Position(), move_vec);
	p_model->ForciblySetPositionByCentering(true);
	
	for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
		p_model = &spSubModelMap[ iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}

	for (int iI = 0; iI < eMD_MAX; iI ++) {
		p_model = &sModelXfile[iI];
		p_model->Position() = VSub(p_model->Position(), move_vec);
		p_model->ForciblySetPositionByCentering(true);
	}

	EffectManager* const p_effmgr = EffectManager::GetInstance();
	for (int iI = 0; iI < EffectManager::kEFFECT_NODE_NUM; iI ++) {
		p_effmgr->Position(iI) = VSub(p_effmgr->Position(iI), move_vec);
	}

	// �M�~�b�N�N���X�� mOnPosition �̒l���X�V����
	GimmickBase::ReviseAllGimmickOnPosition(move_vec);

	CollisionManager::GetInstance()->Refresh();

	// �|�C���g���C�g�̍X�V
	{
		VECTOR pos;
		GraphicManager::GetInstance()->GetPLightPosition(&pos);
		pos = VSub(pos, move_vec);
		GraphicManager::GetInstance()->SetPLightPosition(&pos);
	}

	CameraManager::GetInstance()->ReviseCamera(move_vec);

	spStageFunc->ReviseAllModel(move_vec);

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
//StageFuncBase*  spStageFunc				= NULL;
/*static*/StageFuncBase*
SceneMainGame::GetStageFunc(void)
{
	return spStageFunc;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/USHORT
SceneMainGame::GetMyWeaponNum(void)
{
	return (USHORT)sMyWeaponList.size();
}

//---------------------------------------------------
/**
   �w��̃L�����Ƀv���C���[��ύX����
 */
//---------------------------------------------------
/*static*/void 
SceneMainGame::ChangePlayerModel(const char* strModelName, BOOL isEffect/*=TRUE*/)
{
	sPlayer.DisableLockonChara();

	// ���f���t�@�C���ǂݍ��݂̏���
	ResourceModel::GetInstance()->LoadCharaResource( (char *)strModelName);
	sPlayer.DeleteModel();
	sPlayer.LinkModel(strModelName);
	sPlayer.Setup(false, sStageIndex);
	sPlayer.StartMotion(0, true, 0.5f);
	sPlayer.SetPositionOnCollision();
	
	if (strcmp( (char *)strModelName, Name_Lino) == 0) {
		sPlayer.ResumeAttach(CharaPlayer::eATTACH_MODEL_SWORD);
		sPlayer.SetActionLimit(spBGInfo->mActionLimit,
							   sPlayer.ItemWork().mItemSubWeapon);
	}
	else {
		sPlayer.PauseAttach(CharaPlayer::eATTACH_MODEL_SWORD);
		sPlayer.SetActionLimit(eACTION_NG, sPlayer.ItemWork().mItemSubWeapon);
	}
	
	if (isEffect) {
		EffekseerManager::GetInstance()->Play(eEFK_GETKEYITEM, &sPlayer.Position(), 10.0f);
		// �w�������I�x
		SoundManager::GetVcComponent(Name_Lino)->PlaySound(eVC_ATTACK_2);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/const char*
SceneMainGame::GetStageName(int index)
{
	//	return sBGInfoTable[index % kBG_INFO_NUM].mFileName;
	return sResourceStageNameTable[sBGInfoTable[index % kBG_INFO_NUM].mFileIndex];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/int
SceneMainGame::GetStageIndex(void)
{
	return sStageIndex;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/int
SceneMainGame::GetStageActionLimit(void)
{
	return spBGInfo->mActionLimit;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/bool
SceneMainGame::GetIsBattle(void)
{
	return sIsBattle;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/bool
SceneMainGame::RequestChangeState(void* pParam)
{
	ActionTaskManager::tTaskInfo* p_param;
	p_param = (ActionTaskManager::tTaskInfo *)pParam;

	// ePHASE_GAME �ȊO�͒e��
	if (!(
		  (mPhase == SceneMainGame::ePHASE_GAME) || 
		  (mPhase == SceneMainGame::ePHASE_FLYING_MOVE) || 
		  (mPhase == SceneMainGame::ePHASE_PLAY_OCARINA) || 
		  ((mPhase == SceneMainGame::ePHASE_GET_ITEM) && (p_param->mTaskType == ActionTaskManager::eTASK_MOVE)) ||
		  ((mPhase == SceneMainGame::ePHASE_EVENT) && (p_param->mTaskType == ActionTaskManager::eTASK_MOVE))
	)) {

#ifdef _DEBUG
		PRINT_CONSOLE("Cannot Execute RequestChangeState() because, not permitted phase \n");
#endif // _DEBUG
		return false;
	}

	sTaskInfo.mTaskType		= p_param->mTaskType;
	sTaskInfo.mTaskValueA	= p_param->mTaskValueA;
	sTaskInfo.mTaskValueB	= p_param->mTaskValueB;

	switch (sTaskInfo.mTaskType) {
	case ActionTaskManager::eTASK_MOVE:

		PRINT_CONSOLE("Accept Task : [MOVE] , %d , %d\n", sTaskInfo.mTaskValueA, sTaskInfo.mTaskValueB);

		sPlayer.PermitAcceptInput(false);

		// �I�u�W�F�N�g�g�p��Ԃ���I�u�W�F�N�g�t���O���X�V
		setObjectFlagFromObjectUsed();

		if (sStageIndex == sTaskInfo.mTaskValueA) {

			// �o�g���C���f�b�N�X�t���O�͌��ɖ߂�
			sBattleIndex = -1;
			// �C�x���g�o�g���t���O�����ɖ߂�
			sEventBattleFlag = -1;
			
			// �C�x���g�t���O�̃L�����Z���\��ݒ�
			if (sEventRecoveryFlag != -1) {
				TotalInfo::GetInstance()->DisableEventFlag(BATTLEFLAG_TO_TASKVALUE_A(sEventRecoveryFlag),
														   BATTLEFLAG_TO_TASKVALUE_B(sEventRecoveryFlag));
			}

			// ���b�N�I������
			sPlayer.DisableLockonChara();

			destroyAllEnemy(); // �G��S�ł�����
			mPhase = SceneMainGame::ePHASE_MOVE_PLACE;
		}
		else {
			//			GimmickBase::DeleteAllGimmick();

			// �M�~�b�N�̒����猻�݂̎w��̃C�x���g���_�@�ɓ��삷��M�~�b�N����������
			GimmickBase* p_gimmick = NULL;
			for (int iI = 0; iI < GimmickBase::GetGimmickBaseNum(); iI ++) {
				p_gimmick = (GimmickBase *)GimmickBase::GetGimmickBase(iI);
				p_gimmick->SetState(0);
			}

			mPhase = SceneMainGame::ePHASE_CHANGE_STAGE;
		}
		sStageIndex			= sTaskInfo.mTaskValueA;
		sStagePlaceIndex	= sTaskInfo.mTaskValueB;

		// �t�F�[�h�A�E�g
		FADE_BOARD()->StartFade(255, 0.5f);

		break;

	case ActionTaskManager::eTASK_FALLDOWN:

		// �ߖ�
		SoundManager::GetVcComponent(Name_Lino)->PlaySound(eVC_FALL_DOWN);

		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);

		GimmickBase::SetGimmickPauseAttach(TRUE);

		mPhase = SceneMainGame::ePHASE_FALL_DOWN;
		FADE_OUT(0.75f);

		break;

	case ActionTaskManager::eTASK_EVENT:
		unsigned long event_kind;
		unsigned long event_flag;
		event_kind = sTaskInfo.mTaskValueA;
		APP_ASSERT_PRINT(event_kind < eEVENT_MAX,
						 "event_kind : %d is Invalid Value\n", event_kind);
		event_flag = sTaskInfo.mTaskValueB;
		APP_ASSERT_PRINT(event_flag < kEVENT_FLAG_NUM,
						 "event_flag : 0x%d is Invalid Value\n", event_flag);
		if (TotalInfo::GetInstance()->GetEventFlagEnable(event_kind, event_flag) == false) {
			mPhase = SceneMainGame::ePHASE_EVENT_BEFORE;
		} 
		else {
			// �C�x���g���{��̏ꍇ�֑͋�����
			//			sPlayer.PermitAcceptInput(true);
			return false;
		}

		break;

	case ActionTaskManager::eTASK_ENEMY:

		// �A�N�V��������OK�̏ꍇ�̂݁A�G���o��������
		if (spBGInfo->mActionLimit == eACTION_OK) {
			if (sBattleIndex != sTaskInfo.mTaskValueA) {
				// ���b�N�I������
				sPlayer.DisableLockonChara();

				/* �C�x���g�J�n���͓G��S�ł����� */
				destroyAllEnemy();

				sBattleIndex = sTaskInfo.mTaskValueA;

				mPhase = SceneMainGame::ePHASE_START_BATTLE;
			}
		}
		break;

	case ActionTaskManager::eTASK_NAVI:
		if (sStrNaviTalkPtr == NULL) {
			int event_def =	ConvertPlaceIndex_TO_EventDefine(sStageIndex);
			APP_ASSERT_PRINT((event_def != eEVENT_ERROR),
							"Select event_def %d is invalid value \n", event_def);
			//			const char* talk_msg = sStrNaviTalkMessage[event_def][sTaskInfo.mTaskValueB];
			const char* talk_msg = GetStrNaviTalkMessage(event_def, sTaskInfo.mTaskValueB);

			if (TotalInfo::GetInstance()->GetNaviTalkFlagEnable(event_def, sTaskInfo.mTaskValueB) == false) {
				SceneMainGame::StartNaviTalk(talk_msg);
				TotalInfo::GetInstance()->SetNaviTalkFlag(event_def, sTaskInfo.mTaskValueB);
			}
		}
		break;

	case ActionTaskManager::eTASK_SHOP:
		sPlayer.PermitAcceptInput(false);
		mPhase = SceneMainGame::ePHASE_SHOP;
		break;

	case ActionTaskManager::eTASK_GETITEM:
		sPlayer.PermitAcceptInput(false);
		mPhase = SceneMainGame::ePHASE_GET_ITEM_BEFORE;
		break;

	case ActionTaskManager::eTASK_ENEMY_ITEM:
		/* �����ŃA�C�e���𑝂₷�� mDrawModelVector ��j�󂵂Ă��܂��̂ŁA
		   �ʂ̃^�C�~���O�ŏ��� */
		spDisappearEnemy = (CharaEnemy *)sTaskInfo.mTaskValueA;
		break;

	case ActionTaskManager::eTASK_SAVE:
		sPlayer.PermitAcceptInput(false);
		mPhase = SceneMainGame::ePHASE_SAVE;

		MSG_TYPE_TELOP();
		MSG_FADE_IN(0.2f);
		break;

	case ActionTaskManager::eTASK_NAVI_EX:
		if (sStrNaviTalkPtr == NULL) {
			const char* talk_msg = (const char *)sTaskInfo.mTaskValueA;
			SceneMainGame::StartNaviTalk(talk_msg);
		}
		break;

	case ActionTaskManager::eTASK_CHECK_POINT:

		SoundManager::GetSndComponent()->PlaySound(eSE_PRESS_KEY);
		SceneMainGame::StartNaviTalk(strNaviTalk_CheckPoint);
		sPlayer.SetCheckPointPtr((ObjectBase *)sTaskInfo.mTaskValueA);
		break;

	case ActionTaskManager::eTASK_FLYING_MOVE:
		sPlayer.PermitAcceptInput(false);
		mPhase = SceneMainGame::ePHASE_FLYING_MOVE;
		//		mPhaseCounter = 0.0f;

		// ����� eTASK_MOVE �����s����邪�A���̃^�C�~���O�ł̓I�u�W�F�N�g�t���O���X�V����Ȃ��̂ł��̃^�C�~���O�ŌĂԕK�v����
		// �I�u�W�F�N�g�g�p��Ԃ���I�u�W�F�N�g�t���O���X�V
		setObjectFlagFromObjectUsed();

		spSubSceneInst = new SubSceneFlyingMove();
		APP_ASSERT((spSubSceneInst != NULL),
				   "Failed new SubSceneFlyingMove class \n");
		spSubSceneInst->Create();

		break;


	case ActionTaskManager::eTASK_PLAY_OCARINA:

		sPlayer.PermitAcceptInput(false);	// ���͋֎~
		mPhase = SceneMainGame::ePHASE_PLAY_OCARINA;

		spSubSceneInst = new SubScenePlayOcarina();
		APP_ASSERT((spSubSceneInst != NULL),
					"Failed new SubScenePlayOcarina class \n");
		spSubSceneInst->Create();
		((SubScenePlayOcarina *)spSubSceneInst)->SetOcarinaSongSelect(sOcarinaSongSelect);
		break;

	default:
		break;

	};

	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::destroyEnemyWork(int enemyIndex)
{
	sppEnemy[enemyIndex]->ProcessDamage(kSYSTEM_DAMAGE_VALUE, true, (VECTOR *)&ZERO_VEC, (VECTOR *)&ZERO_VEC, false, eATTACK_TYPE_NORMAL); // �[���I�ɓG��|��
}
		
//---------------------------------------------------
/**
   BG���f������v���C���[�̍��W�E�p�x���擾����
 */
//---------------------------------------------------
int
SceneMainGame::getItemNumFromMapData(void)
{
	int			bRet			= DXLIB_ERROR;
	int			map_mdl_hnd		= 0;
	USHORT		stage_item_num	= 0;
	USHORT		mat_idx			= 0;
	char*		mat_name		= NULL;
	USHORT		item_cnt		= 0;
	MV1_REF_POLYGONLIST ref_polylist;

	// �}�b�v�̃��f���n���h���擾
	map_mdl_hnd = sModelMap.GetModelHandle();

	// �Q�Ɨp���b�V���̃Z�b�g�A�b�v
	bRet = MV1SetupReferenceMesh(map_mdl_hnd, eBG_TYPE_ITEM, TRUE);
	APP_ASSERT((bRet == DXLIB_OK),
			   "Failed MV1SetupReferenceMesh() \n");

	// �Q�Ɨp���b�V�����擾����
	ref_polylist = MV1GetReferenceMesh(map_mdl_hnd, eBG_TYPE_ITEM, TRUE);

	// eBG_TYPE_ITEM �̃}�e���A�������擾����
	// eBG_TYPE_ITEM ���͑S�Đ����`�̃|���S����z�肵�Ă���̂�
	// ���|���S�������Q�Ŋ��������m���}�e���A�����Ƃ���
	stage_item_num = (ref_polylist.PolygonNum / 2);
	PRINT_CONSOLE("stage item num = %d \n", stage_item_num);

	for (int iI = 0; iI < stage_item_num; iI ++) {
		// �}�e���A���ԍ����擾���A
		// ����Ƀ}�e���A���������߂���
		mat_idx = ref_polylist.Polygons[(iI * 2)].MaterialIndex;
		mat_name = (char *)MV1GetMaterialName(map_mdl_hnd, mat_idx);

		// �}�e���A�������A�C�e���p�̃}�e���A�������`�F�b�N����
		if (strlen(mat_name) < BG_TYPE_ITEM_MATERIAL_NAME_LEN) {
			continue;
		}

		// �}�e���A��������K�v�ȃ��x���̕���������o��
		// �����K���Ƃ��āA �wTresuOOO_III_RRR�x �Ƃ��� 
		// OOO �̓I�u�W�F�N�g���(�󔠁A��...
		// III �̓A�C�e�����(�}�X�^�[�\�[�h�A�n�[�g...)
		// RRR �͊p�x (0 - 360)
		CHAR type_label	 [LABEL_TYPE_NAME_LEN + 1];

		memcpy(type_label, &mat_name[0], LABEL_TYPE_NAME_LEN);
		type_label[LABEL_TYPE_NAME_LEN] = '\0';

		// �w��̃L�[���[�h�����m�F����
		if ((strcmp(type_label, BG_TYPE_ITEM_KEYWORD) == 0) ||
			//			(strcmp(type_label, "TRE") == 0)) {
			(memcmp(mat_name, "TRE", strlen("TRE")) == 0)) {
			
			// �����p�̃J�E���^���C���N�������g
			item_cnt ++;
		}
	}
	return item_cnt ++;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::setupItemParameterFromMapData(void)
{
	int			bRet			= DXLIB_ERROR;
	int			map_mdl_hnd		= 0;
	USHORT		stage_item_num	= 0;
	USHORT		mat_idx			= 0;
	char*		mat_name		= NULL;
	USHORT		item_cnt		= 0;
	MV1_REF_POLYGONLIST ref_polylist;

	// �}�b�v�̃��f���n���h���擾
	map_mdl_hnd = sModelMap.GetModelHandle();

	// �Q�Ɨp���b�V���̃Z�b�g�A�b�v
	bRet = MV1SetupReferenceMesh(map_mdl_hnd, eBG_TYPE_ITEM, TRUE);
	APP_ASSERT((bRet == DXLIB_OK),
			   "Failed MV1SetupReferenceMesh() \n");

	// �Q�Ɨp���b�V�����擾����
	ref_polylist = MV1GetReferenceMesh(map_mdl_hnd, eBG_TYPE_ITEM, TRUE);

	// eBG_TYPE_ITEM �̃}�e���A�������擾����
	// eBG_TYPE_ITEM ���͑S�Đ����`�̃|���S����z�肵�Ă���̂�
	// ���|���S�������Q�Ŋ��������m���}�e���A�����Ƃ���
	stage_item_num = (ref_polylist.PolygonNum / 2);
	PRINT_CONSOLE("stage item num = %d \n", stage_item_num);

	for (int iI = 0; iI < stage_item_num; iI ++) {
		// �}�e���A���ԍ����擾���A
		// ����Ƀ}�e���A���������߂���
		mat_idx = ref_polylist.Polygons[(iI * 2)].MaterialIndex;
		mat_name = (char *)MV1GetMaterialName(map_mdl_hnd, mat_idx);

		// �}�e���A�������A�C�e���p�̃}�e���A�������`�F�b�N����
		if (strlen(mat_name) < BG_TYPE_ITEM_MATERIAL_NAME_LEN) {
			continue;
		}

		// �e���x��������A�C�e�������擾����
		int object_idx = 0;
		int hideevent_idx = 0;
		int item_idx = 0;
		int rot_value = 0;

		// ���o�[�W�����̎w��
		if (memcmp(mat_name, BG_TYPE_ITEM_KEYWORD, strlen(BG_TYPE_ITEM_KEYWORD)) == 0) {
			int object_index= 0;
			sscanf(mat_name, "Tresu%d_%d_%d", 
				   &object_index, &item_idx, &rot_value);
			object_idx		= (object_index % 100);
			hideevent_idx	= (object_index / 100);
		}
		// �V�o�[�W�����̎w��
		else {
			sscanf(mat_name, "TRE%d_%d_%d_%d", 
				   &hideevent_idx, &object_idx, &item_idx, &rot_value);
		}

		// �I�u�W�F�N�g��ݒu����ꏊ�����߂���
		VECTOR center_pos = ZERO_VEC;
		int vtx_idx = 0;
		for (int iPolyCnt = 0; iPolyCnt < 2; iPolyCnt ++) {
			for (int iVtxCnt = 0; iVtxCnt < 3; iVtxCnt ++) {
				vtx_idx = ref_polylist.Polygons[(iI * 2) + iPolyCnt].VIndex[iVtxCnt];
				center_pos = VAdd(center_pos, ref_polylist.Vertexs[vtx_idx].Position);
//				PRINT_CONSOLE("poly[%d].vtx[%d] = %d : {%.2f, %.2f, %.2f} \n", iPolyCnt, iVtxCnt, vtx_idx, center_pos.x, center_pos.y, center_pos.z);
			}
		}
		center_pos = VScale(center_pos, (1.0f / 6.0f));
		center_pos = VScale(center_pos, spBGInfo->mScale);

		// �e�I�u�W�F�N�g�ɉ������C���X�^���X�𐶐� 
		sppObjectBase[item_cnt] = ObjectBase::CreateInstance(object_idx, sStageIndex);

		APP_ASSERT(sppObjectBase[item_cnt] != NULL,
				   "Failed new ObjectBase \n");

		sppObjectBase[item_cnt]->Setup(object_idx, (eItemKind)item_idx, &center_pos, rot_value);
		
		//<! �C�x���g��ɔ���������󔠂̃G���g���[
		switch (object_idx) {
		case eOBJECT_KIND_TRESURE:
		case eOBJECT_KIND_SMALL_TRESURE:
		case eOBJECT_KIND_CRYSTAL:
		case eOBJECT_KIND_SEESAW:
		case eOBJECT_KIND_CARD_JK:
		case eOBJECT_KIND_STONE:
			if (hideevent_idx != 0) {
				int event_index = ConvertPlaceIndex_TO_EventDefine(sStageIndex);
				if (event_index != eEVENT_ERROR) {
					if (TotalInfo::GetInstance()->GetEventFlagEnable(event_index, hideevent_idx) == false) {
						((ObjectTresure *)sppObjectBase[item_cnt])->HideTreasure(hideevent_idx);
					}
				}
			}
			break;
		default:
			APP_ASSERT_PRINT((hideevent_idx == 0),
							 "Not Support HideEvent object index : %d \n", object_idx);
			break;
		};

		// �����p�̃J�E���^���C���N�������g
		item_cnt ++;
	}

	//�@�Q�Ɨp���b�V���̃Z�b�g�A�b�v
	bRet = MV1TerminateReferenceMesh(map_mdl_hnd, eBG_TYPE_ITEM, TRUE);
	APP_ASSERT((bRet == DXLIB_OK),
			   "Failed MV1TerminateReferenceMesh() \n");
	return;
}

//---------------------------------------------------
/**
 * NPC �Ƃ̃C�x���g�����E���������A�C�e���Ɠ���
 */
//---------------------------------------------------
void
SceneMainGame::updateNpcEvent(void)
{
	CharaNpc* p_npc = NULL;

	VECTOR vecA, vecB;
	float len;
	vecA = sPlayer.Position();
	//	vecA.y = 0.0f;

	bool is_talk_enable = false;
	float player_body_size = sPlayer.BodySize();
	if ((sPlayer.GetState() != CharaBase::eCHARA_STATE_JUMP_UP) &&
		(sPlayer.GetState() != CharaBase::eCHARA_STATE_JUMP_DOWN)) {
		// ��b�̋����̏���
		if (CHECK_TRIG_ANY_JOY()) {
			is_talk_enable = true;
		}
	}

	if (is_talk_enable) {
		for (int iI = 0; iI < sNpcNum; iI ++) {
			p_npc = &spNpc[iI];

			// ���b�Z�[�W�̏���
			vecB = p_npc->Position();
			//			vecB.y = 0.0f;
			len = VSize(VSub(vecA, vecB) );
			APP_ASSERT_PRINT(len > 0.000f,
							 "Failed updateNpcEvent, player & npc pos is SamePosition\n");
			if (len < ((p_npc->BodySize() + player_body_size) * 1.2f) ) {
				sPlayer.PermitAcceptInput(false);
				CameraManager::GetInstance()->SaveTemporaryCamWork();

				// �S�Ă̓G�̓������~�߂�
				CharaEnemy::SetStopAllEnemyFlag(TRUE);

				mPhase = SceneMainGame::ePHASE_MESSAGE_BEFORE;
				sIsMessageRange = iI;
				break;
			}
		}
	}

	if (sNpcNum > 0) {

		p_npc = &spNpc[sCheckNpcIndex];
		// ���b�Z�[�W�̏���
		vecB = p_npc->Position();
		//		vecB.y = 0.0f;
		len = VSize(VSub(vecA, vecB) );

		// �֌W�Ȃ��\���͔�\���ɂ�����
		if (len > sDissappearLength) {
			GraphicManager::GetInstance()->RemoveDrawModel(p_npc);
			p_npc->DisableShadow();

		} else {
			GraphicManager::GetInstance()->EntryDrawModel(p_npc);
			p_npc->EnableShadow();
		}
		sCheckNpcIndex = ( (sCheckNpcIndex + 1) % sNpcNum);

	}	
}

//---------------------------------------------------
/**
 * �I�u�W�F�N�g�Ɠ��͎��̏���
 */
//---------------------------------------------------
void
SceneMainGame::updateObjectInputCheck(void)
{
	BOOL is_check_key = FALSE;
	BOOL has_key = TotalInfo::GetInstance()->GetHasFieldKey(sStageIndex);

	// �L�[���͂̃`�F�b�N
	if ((CHECK_TRIG_JOY(JOY_INPUT_AWAY)) ||
		(CHECK_TRIG_JOY(JOY_INPUT_ATTACK)) ) {
		int player_state = sPlayer.GetState();
		if ( (player_state != CharaBase::eCHARA_STATE_JUMP_UP) &&
			 (player_state != CharaBase::eCHARA_STATE_JUMP_DOWN) &&
			 (player_state != CharaPlayer::eCHARA_STATE_ATTACK_JUMP) &&
			 (sPlayer.CosplayIndex() != eWINMENU_COSPLAY_TITANIS) ) {
			is_check_key = true;
		}
	}

	VECTOR ch_pos = sPlayer.Position();
	VECTOR vecA = VGet(ch_pos.x, 0.0f, ch_pos.z);
	VECTOR vecB;
	float dist;

	if (is_check_key == TRUE) {

		float ch_size = sPlayer.BodySize();
		float ch_height = sPlayer.BodyHeight();

		std::vector<ObjectBase *>::iterator ite;
		for (ite = ObjectBase::GetActiveObjectListRef().begin(); ite != ObjectBase::GetActiveObjectListRef().end(); ite++) {
			vecB = (*ite)->Position();
			vecB.y = 0.0f;

			dist = VSize(VSub(vecA, vecB));
			APP_ASSERT_PRINT( (dist > 0.0f),
							  "Failed updateObjectBaseEvent, player & item pos is SamePosition\n");
			
			float total_size = ((ch_size + (*ite)->BodySize()) * 1.6f);
			float object_center = (*ite)->Position().y + ((*ite)->BodySize() * 0.5f);

			if ((dist < total_size) &&
				((ch_pos.y - ch_height) < object_center) && ((ch_pos.y + ch_height) > object_center) ) {

				if ( ((*ite)->GetObjectKind() == eOBJECT_KIND_TRESURE) && 
					 (has_key == FALSE) && (sStageIndex != 22) ) {
					// �����Ȃ��R�����g
					StartNaviTalk(strNaviTalkNoKey);
				}
				else {
					// �A�C�e���擾�C�x���g����
					(*ite)->StartCheckEvent();
				}
				break; // �P�̕�̂��΂ɑ��̕�͎�����Ȃ��̂�
			}
		}
	}
	


	// �����̊֌W�Ȃ��\�����\���ɂ�����
	int obj_num = ObjectBase::GetActiveObjectListRef().size();
	if (obj_num > 0) {
		ObjectBase* p_object = ObjectBase::GetActiveObjectListRef().at(sCheckObjectIndex % obj_num);
		ObjectBase::eObjectType obj_type = p_object->GetObjectType();
		vecB = p_object->Position();
		vecB.y = 0.0f;
	
		dist = VSize(VSub(vecA, vecB));
	
		if (dist > sDissappearLength) {
			if (obj_type == ObjectHand::eOBJECT_TYPE_HAND) {
				if ( ( (ObjectHand *)p_object)->State() == ObjectHand::eHAND_STATE_ON_EARTH) {
					GraphicManager::GetInstance()->RemoveDrawModel(p_object);
				} 
				else {
					// eOBJECT_TYPE_HAND �ł��� eHAND_STATE_ON_EARTH �ȊO�̏ꍇ�́A�����̃��f�����\���ɂ��Ȃ�
					// nop.
				}
			}
			else {
				GraphicManager::GetInstance()->RemoveDrawModel(p_object);
			}
		}
		else {
			p_object->InsertDrawModelSelf();
		}
		sCheckObjectIndex = ( (sCheckObjectIndex + 1) % obj_num);
	}

}

/*=====================================*

 *=====================================*/
//---------------------------------------------------
/**
 * �I�u�W�F�N�g�t���O����I�u�W�F�N�g�g�p��Ԃ��X�V
 */
//---------------------------------------------------
/*static*/void
SceneMainGame::updateObjectUsedFromObjectFlag(void)
{
	// �t�B�[���h������Ɏg�p�ς݃A�C�e���̏������s��
	unsigned long event_def = ConvertPlaceIndex_TO_EventDefine(sStageIndex);
	if (event_def != eEVENT_ERROR) {

		// ��A�X�C�b�`�A�A�C�e���̃I�u�W�F�N�g�^�C�v�݂̂̃��X�g���쐬����
		std::vector<ObjectBase*> save_obj_list;
		ObjectBase::eObjectType obj_type;
		for (int iI = 0; iI < sObjectBaseNum; iI ++) {
			obj_type = sppObjectBase[iI]->GetObjectType();
			if ((obj_type == ObjectBase::eOBJECT_TYPE_TREASURE)		||
				(obj_type == ObjectBase::eOBJECT_TYPE_SWITCH)		||
				(obj_type == ObjectBase::eOBJECT_TYPE_ITEM)) {
					
				save_obj_list.push_back(sppObjectBase[iI]);
			}
		}
		APP_ASSERT_PRINT((save_obj_list.size() < (sizeof(DWORD) * 8)),
						 "save_obj_list is over %d / 32 \n", save_obj_list.size());

		// �쐬�������X�g�ƃt���O����A�g�p�σI�u�W�F�N�g�̐ݒ������
		unsigned long getflag = TotalInfo::GetInstance()->GetObjectFlagBit(event_def);
		for (unsigned int iI = 0; iI < save_obj_list.size(); iI ++) {
			if (getflag & (1 << iI)) {
				save_obj_list.at(iI)->SetUsed();	// �g�p�ς݂ɐݒ肷��
			}
		}
			
		PRINT_CONSOLE("=>update object used From bit (obj_num : %d, getflag : 0x%08x) \n",
					  save_obj_list.size(), getflag);
	}
}

//---------------------------------------------------
/**
 * �I�u�W�F�N�g�g�p��Ԃ���I�u�W�F�N�g�t���O���X�V
 */
//---------------------------------------------------
/*static*/void
SceneMainGame::setObjectFlagFromObjectUsed(void)
{
	DWORD event_index = ConvertPlaceIndex_TO_EventDefine(sStageIndex);
	if (event_index != eEVENT_ERROR) {

		// ��A�X�C�b�`�A�A�C�e���̃I�u�W�F�N�g�^�C�v�݂̂̃��X�g���쐬����
		std::vector<ObjectBase*> save_obj_list;
		ObjectBase::eObjectType obj_type;
		for (int iI = 0; iI < sObjectBaseNum; iI ++) {
			obj_type = sppObjectBase[iI]->GetObjectType();
			if ((obj_type == ObjectBase::eOBJECT_TYPE_TREASURE)		||
				(obj_type == ObjectBase::eOBJECT_TYPE_SWITCH)		||
				(obj_type == ObjectBase::eOBJECT_TYPE_ITEM)) {
					
				save_obj_list.push_back(sppObjectBase[iI]);
			}
		}
		APP_ASSERT_PRINT((save_obj_list.size() < (sizeof(DWORD) * 8)),
						 "save_obj_list is over %d / 32 \n", save_obj_list.size());

		// �쐬�������X�g�̊֐�����t���O�̒l�����߂�
		unsigned long setflag = 0;
		for (unsigned int iI = 0; iI < save_obj_list.size(); iI ++) {
			if (save_obj_list.at(iI)->IsUsed()) {
				setflag |= (1 << iI);
			}
		}
		TotalInfo::GetInstance()->SetObjectFlagBit(event_index, setflag);

		PRINT_CONSOLE("=>update bit From object used (obj_num : %d, setflag : 0x%08x) \n",
					  save_obj_list.size(), setflag);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::StartNaviTalk(const char* strTalk)
{
	SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_IN);
	MSG_TYPE_MESSAGE();
	MSG_FADE_IN(0.2f);
				
	sStrNaviTalkPtr = (char *)strTalk;
}

//---------------------------------------------------
/**
 * �V���h�E�}�b�v�`��O�ɃR�[�������n���h��
 */
//---------------------------------------------------
/*static*/void
SceneMainGame::BeforeCreateShadowFunc(void* args)
{
	// �v���C���[�̃X�t�B�A�}�b�v������
	sPlayer.ChangeSphereTexAll(-1);
	sGimmick.ChangeSphereTexAll(-1);

	EventManager* const p_evtmgr =  EventManager::GetInstance();
	for (int iI = 0; iI < p_evtmgr->GetModelNum(); iI ++) {
		p_evtmgr->GetModelPmd(iI)->ChangeSphereTexAll(-1);
	}
	for (int iI = 0; iI < sNpcNum; iI ++) {
		spNpc[iI].ChangeSphereTexAll(-1);
	}	
}

//---------------------------------------------------
/**
 * �V���h�E�}�b�v�`���ɃR�[�������n���h��
 */
//---------------------------------------------------
/*static*/void
SceneMainGame::AfterCreateShadowFunc(void* args)
{
	// �v���C���[�̃X�t�B�A�}�b�v�����ɖ߂�
	sPlayer.RevertSphereTexAll();
	sGimmick.RevertSphereTexAll();

	EventManager* const p_evtmgr =  EventManager::GetInstance();
	for (int iI = 0; iI < p_evtmgr->GetModelNum(); iI ++) {
		p_evtmgr->GetModelPmd(iI)->RevertSphereTexAll();
	}
	for (int iI = 0; iI < sNpcNum; iI ++) {
		spNpc[iI].RevertSphereTexAll();
	}	
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::attachSkydomeTexture(const char* texName)
{
	return;

	int lyt_handle = -1;

	ResourceLayout::GetInstance()->LoadLayoutResource((char *)texName);
	lyt_handle = ResourceLayout::GetInstance()->GetHandle(texName);
	MV1SetTextureGraphHandle(sModelXfile[eMD_SKYDOME].GetModelHandle(), 0, lyt_handle, FALSE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::Destroy(void)
{
	PRINT_CONSOLE("start SceneMainGame::destroy \n");

#ifdef _DEBUG
	/* �g�p�������[���_���v */
	Utility::DumpUsingMemory();
#endif // _DEBUG

	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);

	if (spSubSceneInst != NULL) {
		spSubSceneInst->Destroy();
		delete spSubSceneInst;
		spSubSceneInst = NULL;
	}

	// �R���W�����̔j��
	PRINT_CONSOLE(" CollisionManager::Destroy() \n");
	CollisionManager::GetInstance()->Destroy();

	// ���f����`�悩��O��
	PRINT_CONSOLE(" GraphicManager::RemoveDrawModelAll() \n");
	GraphicManager::GetInstance()->RemoveDrawModelAll();
	
	// ���f��(�}�b�v�`��O)��`�悩��O��
	PRINT_CONSOLE(" GraphicManager::RemoveDrawPreUpdateModelAll() \n");
	GraphicManager::GetInstance()->RemoveDrawPreUpdateModelAll();

	PRINT_CONSOLE(" GraphicManager::RemoveDrawMapModelAll() \n");
	GraphicManager::GetInstance()->RemoveDrawMapModelAll();

	// �i�r�H�J�[�\���𖳌��ɂ���
	sNavi.DisableNaviCursor();

	/*
	// ���C�A�E�g��`�悩��O��
	PRINT_CONSOLE(" GraphicManager::RemoveDrawLayout() \n");
	for (int iI = 0; iI < eLY_MAX; iI ++) {
		GraphicManager::GetInstance()->RemoveDrawLayout(&sLayout[iI]);
	}
	*/

	// ���f���𓖂���Ώۂ���O��
	PRINT_CONSOLE(" CollisionManager::RemoveBodyCollisionAll() \n");
	CollisionManager::GetInstance()->RemoveBodyCollisionAll();

	PRINT_CONSOLE(" CollisionManager::RemoveAttackCollisionAll() \n");
	CollisionManager::GetInstance()->RemoveAttackCollisionAll();

	// �V���h�E��`�悩��O��
	PRINT_CONSOLE(" GraphicManager::RemoveDrawShadowAll() \n");
	GraphicManager::GetInstance()->RemoveDrawShadowAll();

	// �G�t�F�N�g�̔j��
	//	PRINT_CONSOLE(" EffectManager::Destroy() \n");
	//	EffectManager::GetInstance()->Destroy();

	// �G�t�F�N�g�̑S��~
	EffectManager::GetInstance()->StopAllSystem();


	PRINT_CONSOLE(" EffekseerManager::StopAll() \n");
	EffekseerManager::GetInstance()->StopAll();

	PRINT_CONSOLE(" CharaPlayer::DeleteModel() \n");
	sPlayer.DeleteModel();

	PRINT_CONSOLE(" CharaEnemy::DeleteModel() \n");
	if (sppEnemy != NULL) {
		for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
			if (sppEnemy[iI] != NULL) {
				sppEnemy[iI]->DeleteModel();
				delete sppEnemy[iI];
			}
		}
		delete[] sppEnemy;
		sppEnemy = NULL;
	}
	
	PRINT_CONSOLE(" CharaNpc::DeleteModel() \n");
	if (spNpc != NULL) {
		for (int iI = 0; iI < sNpcNum; iI ++) {
			spNpc[iI].DeleteModel();
		}
		delete[] spNpc;
		spNpc = NULL;
	}
	//<! �����I�ɃR�����g�A�E�g
	// mNpcNum �� �e�[�u���̒l�𒼐ڎQ�Ƃ��Ă���ׁA
	// �O�ɂ���ƃe�[�u�����̂��㏑�����Ă��܂��̂łO�ɂ͐ݒ肵�Ȃ�
	//	sNpcNum = 0;

	PRINT_CONSOLE(" ObjectBase::DeleteModel() \n");
	if (sppObjectBase != NULL) {
		for (int iI = 0; iI < sObjectBaseNum; iI ++) {
			if (sppObjectBase[iI] != NULL) {
				sppObjectBase[iI]->DeleteModel();
				delete sppObjectBase[iI];
			}
		}
		delete[] sppObjectBase;
		sppObjectBase = NULL;
	}
	sObjectBaseNum = 0;

	PRINT_CONSOLE(" enemy item DeleteModel() \n");
	for (int iI = 0; iI < kENEMY_ITEM_NUM; iI ++) {
		sDropItem[iI].DeleteModel();
	}

	if (spShopItemLayout != NULL) {
		delete[] spShopItemLayout;
		spShopItemLayout = NULL;
	}

	if (spStageClearLayoutList != NULL) {
		delete[] spStageClearLayoutList;
		spStageClearLayoutList = NULL;
	}

	sEventCsv.DeleteFile();
	//	sItemCsv.DeleteFile();

	// ���f�����A�����[�h����
	for (int iI = 0; iI < eMD_MAX; iI ++) {
		sModelXfile[iI].DeleteModel();
	}


	// �X�e�[�W���Ƃ̏��������Ă���ꍇ�͔j������
	if (sTaskInfo.mTaskType != ActionTaskManager::eTASK_MOVE) {
		if (spStageFunc != NULL) {
			spStageFunc->DestroyBuf();
		}
	}

	// �X�e�[�W���̏���j��
	if (spStageFunc != NULL) {
		delete spStageFunc;
		spStageFunc = NULL;
	}

	// �X�e�[�W���̔j��
	sModelMap.DeleteModel();
	//<!	ResourceModel::GetInstance()->UnloadResource((char *)sResourceStageNameTable[spBGInfo->mFileIndex]);

	if (spSubModelMap != NULL) {
		for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
			spSubModelMap[iI].DeleteModel();
			//<!			ResourceModel::GetInstance()->UnloadResource((char *)sResourceStageNameTable[sSubStageInfoList.at(iI).mFileIndex]);
		}
		delete[] spSubModelMap;
		spSubModelMap = NULL;
	}
	sSubStageInfoList.clear();

	sGimmick.DeleteModel();

	sNavi.DeleteModel();

	// ���������e�ʁA�������͊Y���̃X�e�[�W�Ɉړ�����O�Ƀ��\�[�X�̃��������������
	DWORD using_mem = Utility::GetUsingMemory();
	PRINT_CONSOLE("using_mem : %d MB \n", using_mem);
	if ((sStageIndex == 16) ||								// �X�e�[�W���X�̏ꍇ
		(using_mem > kCHECK_USING_MEMORY_ATTENTION_SIZE)) {	// �g�p�������x���T�C�Y
		PRINT_CONSOLE("!!!! relase resource memory before change stage \n");
		ResourceModel::GetInstance()->Destroy();
	}

	for (int iI = 0; iI < sMapIconMove.mLayoutNum; iI ++) {
		GraphicManager::GetInstance()->RemoveDrawLayout(&sMapIconMove.mpLayout[iI]);
	}


	// �M�~�b�N����j������
	GimmickBase::DeleteAllGimmick();

	// �t�F�[�h��L���ɂ���
	FADE_BOARD()->SetAlpha(255);

	GraphicManager::GetInstance()->EntryDrawFontFunc(NULL);

	// �V���h�E�}�b�v�`��ݒ�n���h������
	ShadowMap::SetBeforeCreateCallback(NULL);
	ShadowMap::SetAfterCreateCallback(NULL);

	PRINT_CONSOLE("end SceneMainGame::destroy \n");

	sStrNaviTalkPtr = NULL;

#ifdef _DEBUG
	/* �g�p�������[���_���v */
	Utility::DumpUsingMemory();
#endif // _DEBUG
}

/*=====================================*
 * phase
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseLoadBefore(void)
{
	// �񓯊��ǂݍ��݂��J�n����
	//	SetUseASyncLoadFlag(TRUE);

	if (sStageIndex == -1) {
		sStageIndex = mDebugArg;
		sStagePlaceIndex = 0; // �Ƃ肠�����O��ݒ�
	}
	spBGInfo = (tBGInfo *)&sBGInfoTable[sStageIndex % kBG_INFO_NUM];

	// �X�e�[�W�����X�g
	sSubStageInfoList.clear();

	// �t�H�O�𖳌��ɂ���
	//	GraphicManager::GetInstance()->DisableFogPattern();

	// �i�r�H���C�g�𖳌��ɂ���
	sNavi.EnableNaviLight(FALSE);

	// NPC�̐ݒ�̓f�t�H���g�̂O�ɖ߂�
	CharaNpc::SetCsvSubNo(0);

	/* �X�e�[�W���̏����������Ȃ� */
	if (spStageFunc != NULL) {
		delete spStageFunc;
		spStageFunc = NULL;
	}
	// Stagefunc �̃C���X�^���X�𐶐�
	spStageFunc = StageFuncBase::CreateInstance(sStageIndex);
	APP_ASSERT_PRINT((spStageFunc != NULL),
					 "spStageFunc is NULL, %d \n", sStageIndex);

	// �e�C�x���g���̐ݒ���s��
	spStageFunc->SetupBeforeLoad();

	sLoadStatus = 0;


	/**** �����𐮂��� ****/
	sMyWeaponList.clear();
	addMyWeapon(eITEM_KIND_NOWEAPON);
	for (int iI = 0; iI < eITEM_KIND_MAX; iI ++) {
		if (TotalInfo::GetInstance()->IsWeaponFlag((eItemKind)iI)) {
			addMyWeapon((eItemKind)iI);
		}
	}

	// ���̉S�͍ŏ�����o���Ă���B
	TotalInfo::GetInstance()->AddSongType(eSONG_TYPE_WING);

#ifdef _DEBUG
	/* ----------------------------------------------------------- */
	//<! �f�o�b�O�p
	addMyWeapon(eITEM_KIND_LONGHOOK);
	addMyWeapon(eITEM_KIND_BOOMERANG);
	addMyWeapon(eITEM_KIND_PORTION);
	addMyWeapon(eITEM_KIND_BOMB);
	addMyWeapon(eITEM_KIND_ARCHERY);

	addMyWeapon(eITEM_KIND_MAGIC_THUNDER);
	addMyWeapon(eITEM_KIND_MAGIC_FLAME);
	addMyWeapon(eITEM_KIND_MAGIC_TORNADE);
	//	addMyWeapon(eITEM_KIND_COPY_ROD);
	//	addMyWeapon(eITEM_KIND_PIKOHUM);
	addMyWeapon(eITEM_KIND_OCARINA);

	TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_MASTERSWORD);
	TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_LIB_SHIELD);
	TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_ANGEL_WING);
	TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_WALLET_A);
	TotalInfo::GetInstance()->AddKeyItem(eKEYITEM_BOOK);

	TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_FRES);
	TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_MASK);
	TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_LAPAN);
	TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_DABAH);

	TotalInfo::GetInstance()->AddSongType(eSONG_TYPE_SUN);
	TotalInfo::GetInstance()->AddSongType(eSONG_TYPE_WILDNESS);
	TotalInfo::GetInstance()->AddSongType(eSONG_TYPE_WING);
	TotalInfo::GetInstance()->AddSongType(eSONG_TYPE_HANDLING);

#endif // _DEBUG


#ifdef _DEBUG
	PRINT_CONSOLE("**** phaseLoadBefore : **** \n");

	PRINT_CONSOLE("loading log:\tstage name : %s \n", GetStageName(sStageIndex) );

	DWORD using_mem = Utility::GetUsingMemory();
	PRINT_CONSOLE("loading log:\t%d\t%s\t%.1f\t%d\t\t[load sec] [using mem] \n", 
				  -1, "start", 0.0f, using_mem );
	Counter::StartCounter(Counter::eCOUNT_TYPE_LOAD_ALL);

#endif // _DEBUG

	mPhase = SceneMainGame::ePHASE_LOADING;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseLoading(void)
{
	ResourceManager*	p_res_mgr	= ResourceManager::GetInstance();
	ResourceModel*		p_res_mdl	= ResourceModel::GetInstance();
	ResourceLayout*		p_res_lyt	= ResourceLayout::GetInstance();

	/* ���[�h���̃t�@�C��������΃��^�[�� */
	if (GetASyncLoadNum() != 0) {
		return;
	}

#ifdef _DEBUG
	//	PRINT_CONSOLE("**** phaseLoading[%d] : start %s **** \n", sLoadStatus, strLoadStateName[sLoadStatus]);
	Counter::StartCounter(Counter::eCOUNT_TYPE_LOAD_EACH);
#endif 

	switch (sLoadStatus) {
	case eLOAD_PLAYER:
		p_res_mgr->LoadCharaResource(Name_Lino);
		SoundManager::GetVcComponent(Name_Lino)->SetVolumeScale(TotalInfo::GetInstance()->GetConfigVoiceVolumeScale());
		break;
		
	case eLOAD_ENEMY:
		for (int iI = 0; iI < GET_ENEMY_PATTERN_NUM(); iI ++) {
			for (int iJ = 0; iJ < kENEMY_ONCE_MAX; iJ ++) {
				eEnemyKind kind = GET_ENEMY_KIND(iI, iJ);
				if (kind == (eEnemyKind)-1) continue;
				p_res_mgr->LoadCharaResource((char *)GetEnemyWorkTable(kind)->mResourceName);

				LoadEnemyAttachmentModel(kind);
			}
		}
		break;

	case eLOAD_NPC:
		CharaNpc::SetupNpcResourceInfo(sStageIndex);
		sNpcNum = CharaNpc::GetNpcResourceNum();
		for (int iI = 0; iI < sNpcNum; iI ++) {
			p_res_mgr->LoadCharaResource(CharaNpc::GetNpcResourceName(iI));
		}
		break;

	case eLOAD_SKYDOME:
		// �X�J�C�h�[���Ƃ��Ċ��蓖�Ă�e�N�X�`��
		p_res_mdl->LoadModelResource("BG/Skydome/sky_day.mv1");
		sModelXfile[eMD_SKYDOME	].LinkModel("BG/Skydome/sky_day.mv1");
		{
			// �}�b�v���[�f�B���O�`�F�b�N
			char* map_name = p_res_mdl->GetResourceName("BG", 0);
			if (map_name != NULL) {
				PRINT_CONSOLE("Find BG Data = %s \n", map_name);
			}
		}
		break;

	case eLOAD_MAP:
		// �X�e�[�W�̓ǂݍ���
		ResourceModel::GetInstance()->LoadStageResource((char *)sResourceStageNameTable[spBGInfo->mFileIndex],
														spBGInfo->mIsExtData);
		break;

	case eLOAD_SUB_MAP:
		spSubModelMap = NULL;
		if (sSubStageInfoList.size() != 0) {
			spSubModelMap = new ModelMap[sSubStageInfoList.size()];
			APP_ASSERT_PRINT((spSubModelMap != NULL),
							 "Failed new ModelMap[%d] \n", sSubStageInfoList.size()) ;
			for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
				ResourceModel::GetInstance()->LoadStageResource((char *)sResourceStageNameTable[sSubStageInfoList.at(iI).mFileIndex], 
																false);
			}
		}
		break;

	case eLOAD_SHOW_A_MAP:
		break;

	case eLOAD_OBJECT:
		// �풓���f���̃��[�h
		for (int iI = 0; iI < eRES_MDL_MAX; iI ++) {
			p_res_mdl->LoadItemResource(sResourceModelNameTable[iI]);
		}

		// ��풓���f���̃��[�h
		for (int iI = 0; iI < eRES_MDL_NON_RESI; iI ++) {
			// �Y���̃X�e�[�W�ԍ����A�ΏۃA�C�e�������Ɏ����Ă���ꍇ�A���f�������[�h����
			if ((sStageIndex == sResourceModelTableNonResi[iI].mStageIndex) ||
				((sResourceModelTableNonResi[iI].mItemKind != -1) &&	(TotalInfo::GetInstance()->IsWeaponFlag(sResourceModelTableNonResi[iI].mItemKind))) ){
				p_res_mdl->LoadItemResource(sResourceModelTableNonResi[iI].mModelName);
			}
		}
		p_res_mgr->LoadCharaResource(NAME_Navi);
		p_res_mgr->LoadCharaResource(NAME_Mogu);

		sModelXfile[eMD_SWORD].LinkModel(RES_MDL_MASTERSWORD);
		sModelXfile[eMD_LOCKON].LinkModel(RES_MDL_LOCKON);
		sModelXfile[eMD_SHIELD].LinkModel(RES_MDL_LIB_SHIELD);

		// �S�ẴA�C�e�������炩���߃��[�h
		ObjectBase::LoadAllObjectBase();
		ObjectBase::SetGetItemHandler((GET_ITEM_HANDLER)updateGetItem);

		//�M�~�b�N�֘A�����[�h����
		GimmickBase::LoadAllGimmick(sStageIndex);
		break;

	case eLOAD_TEXTURE:
		// �e�N�X�`���̓ǂݍ���
		for (int iI = 0; iI < eRES_LYT_MAX; iI ++) {
			p_res_lyt->LoadLayoutResource(sResourceLayoutNameTable[iI]);
		}
		break;

	case eLOAD_SOUND:
		// �T�E���h��ǂݍ���
		break;

	case eLOAD_EFFECT:

		{
			// �G�t�F�N�g�ǂݍ���
			BOOL is_load_effect = EffectManager::GetInstance()->Create(eEF_MAX);
			if (is_load_effect == FALSE) {
				char model_name_buf[32] = {0};
				char* p_name;
				// �S�ẴG�t�F�N�g�����[�h
				for (int iI = 0; iI < eEF_MAX; iI ++) {
					if (iI == eEF_DRAG_DROP) continue;
					if (sEffectResourceTable[iI].mModelName == NULL) {
						p_name = NULL;
					}
					else {
						sprintf_s(model_name_buf, sizeof(model_name_buf), "ModelCommon/%s", sEffectResourceTable[iI].mModelName);
						p_name = model_name_buf;
					}
					EffectManager::GetInstance()->LoadResource(iI, sEffectResourceTable[iI].mShaderName, p_name);
				}
			}
		}

		{
			/* Effekseer�̃G�t�F�N�g�ꗗ�����[�h */
			BOOL is_load_effkseer = EffekseerManager::GetInstance()->SetupResourceBufferNum(eEFK_MAX);
			if (is_load_effkseer == FALSE) {
				for (int iI = 0; iI < eEFK_MAX; iI ++) {
#ifdef _DEBUG
					if (iI == eEFK_DRAG_DROP_SAMPLE) continue;
#endif // _DEBUG
					EffekseerManager::GetInstance()->LoadResource(iI, sEffekseerResourceName[iI]);
				}
			}
		}
		break;

	default:
		APP_ASSERT_PRINT((NULL), "Invalid Load Status :%d \n", sLoadStatus);
		break;
		
	};
	
#ifdef _DEBUG
	Counter::StopCounter(Counter::eCOUNT_TYPE_LOAD_EACH);
	//	PRINT_CONSOLE(" * load time : %.1f sec\n", Counter::GetCountSec(Counter::eCOUNT_TYPE_LOAD_EACH));
	//	Utility::DumpUsingMemory();
	PRINT_CONSOLE("loading log:\t%d\t%s\t%.1f\t%u\t\t[load sec] [using mem] \n", 
				  sLoadStatus, 
				  strLoadStateName[sLoadStatus],
				  Counter::GetCountSec(Counter::eCOUNT_TYPE_LOAD_EACH), 
				  Utility::GetUsingMemory() );
#endif // _DEBUG

	sLoadStatus ++;

	if (sLoadStatus == eLOAD_MAX) {
		SetUseASyncLoadFlag(FALSE);

		/**** ���[�h�I�� ****/
#ifdef _DEBUG
		Counter::StopCounter(Counter::eCOUNT_TYPE_LOAD_ALL);
		//		PRINT_CONSOLE("**** Finish Load All : %lld nsecd\n", Counter::GetCountSec(Counter::eCOUNT_TYPE_LOAD_ALL));
		PRINT_CONSOLE("loading log:\t**** Finish Load All : %f sec\n", Counter::GetCountSec(Counter::eCOUNT_TYPE_LOAD_ALL));
#endif 
		mPhase = SceneMainGame::ePHASE_SETUP;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseSetup(void)
{
	int before_time, after_time;
	LayoutBase* p_layout;

#ifdef _DEBUG
	GraphicManager::GetInstance()->DbgSetRenderString(false);
#endif // _DEBUG

	ActionTaskManager::GetInstance()->Setup(SceneMainGame::RequestChangeState);

	EventManager::GetInstance()->Setup();

	// �O���[�o���ϐ�������
	sIsBattle = false;
	sBattleIndex = -1;
	sCheckObjectIndex = 0;
	sCheckNpcIndex = 0;

	//	spCheckPointPtr = NULL;
	sPlayer.SetCheckPointPtr(NULL);		// �`�F�b�N�|�C���g�����N���A

	sNextEventNo = -1;


	TotalInfo::GetInstance()->SetEnableWinMenu(TRUE);

	// for debug.
	// �R�X�v���V�X�e��
	TotalInfo::GetInstance()->AddCosplay(eWINMENU_COSPLAY_NONE);
	/* ----------------------------------------------------------- */

	sModelXfile[eMD_SKYDOME].Setup();
	sModelXfile[eMD_SKYDOME].Position()= ZERO_VEC;
	//	sModelXfile[eMD_SKYDOME].Scale()	= VGet(600.0f, 600.0f, 600.0f); // �ȑO�܂ł̃X�J�C�h�[���l
	sModelXfile[eMD_SKYDOME].Scale()	= VGet(kSKYDOME_DEF_SCALE, kSKYDOME_DEF_SCALE, kSKYDOME_DEF_SCALE);

	// ���f���}�b�v�̃Z�b�g�A�b�v
	sModelMap.LinkModel( sResourceStageNameTable[spBGInfo->mFileIndex] );
	sModelMap.Setup(spBGInfo->mScale);
	sModelMap.Position()= ZERO_VEC;
	sModelMap.Scale()	= VGet(spBGInfo->mScale, spBGInfo->mScale, spBGInfo->mScale);

	for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
		spSubModelMap[iI].LinkModel( sResourceStageNameTable[sSubStageInfoList.at(iI).mFileIndex] );
		spSubModelMap[iI].Setup(spBGInfo->mScale);
		spSubModelMap[iI].Position()= ZERO_VEC;
		spSubModelMap[iI].Scale()	= VGet(spBGInfo->mScale, spBGInfo->mScale, spBGInfo->mScale);
	}

	sModelXfile[eMD_SWORD].Setup();
	//	MV1SetFrameVisible(sModelXfile[eMD_SWORD].GetModelHandle(), 2, FALSE); // ����\���ɂ���
	sModelXfile[eMD_SWORD].SetVisible(false, 2); // ����\���ɂ���
	sModelXfile[eMD_GETTED_ITEM].Setup();
	sModelXfile[eMD_LOCKON].Setup();
	sModelXfile[eMD_SHIELD].Setup();
	// ���̃��b�V�����\���ɂ���
	MV1SetMeshVisible(sModelXfile[eMD_SHIELD].GetModelHandle(), LIBSHIELD_LIGHT_MESH_IDX, FALSE);

	/**** �R���W�����̐ݒ� ****/
	CollisionManager::GetInstance()->Setup(&sModelMap, &spSubModelMap[0]);

	CollisionManager::GetInstance()->EntryEarthCollision(sModelMap.GetModelHandle(), eBG_TYPE_EARTH);
	CollisionManager::GetInstance()->EntryWallCollision(sModelMap.GetModelHandle(), eBG_TYPE_WALL);

	CollisionManager::GetInstance()->EntryEarthCollision(sModelMap.GetModelHandle(), eBG_TYPE_ALPHA_EARTH);
	CollisionManager::GetInstance()->EntryWallCollision(sModelMap.GetModelHandle(), eBG_TYPE_ALPHA_WALL);

	for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
		if (sSubStageInfoList.at(iI).mIsCollision == true) {
			CollisionManager::GetInstance()->EntryEarthCollision(spSubModelMap[iI].GetModelHandle(), eBG_TYPE_EARTH);
			CollisionManager::GetInstance()->EntryWallCollision(spSubModelMap[iI].GetModelHandle(), eBG_TYPE_WALL);

			CollisionManager::GetInstance()->EntryEarthCollision(spSubModelMap[iI].GetModelHandle(), eBG_TYPE_ALPHA_EARTH);
			CollisionManager::GetInstance()->EntryWallCollision(spSubModelMap[iI].GetModelHandle(), eBG_TYPE_ALPHA_WALL);
		}
	}

	sNavi.LinkModel(NAME_Navi);
	sNavi.Setup();
	sNavi.Scale()		= VGet(7.0f, 7.0f, 7.0f);
	sNavi.StartMotion(0, true, 20.0f);
	sNavi.DisableShadow();

	VECTOR ch_pos;
	float ch_rot;
	bool ret;

	// �ǂݍ��񂾃��f���������N������
	before_time = GetNowCount();
	{
		sPlayer.LinkCharaResource(Name_Lino);
		sPlayer.Scale()		= CHARA_DEF_SCALE;

		// ���f���f�[�^����L�����̍��W�����߂���
		if ((sStagePlaceIndex == 99) ||
			(sStagePlaceIndex == 999)) {
			// �ړ������Ȃ��I (RevisePos �͏���)
			sPlayer.Position() = VAdd(sPlayer.Position(), sReviseDiffPos);
		} 
		else {
			ret = getPlayerPosRotFromBGModel(&ch_pos, &ch_rot, sStagePlaceIndex);
			if (ret == false) {
				// �K���ɔz�u
				sPlayer.Position() = VGet(500.0f, 20200.0f, 200.0f);
				sPlayer.Rotation().y = 0.0f;

			} 
			else {
				sPlayer.Position() = VGet(ch_pos.x + 10.0f, ch_pos.y, ch_pos.z + 10.0f);
				sPlayer.Rotation().y = ch_rot;

			}
		}
		sPlayer.Setup(false, sStageIndex);

		sPlayer.SetDrawFunc((ModelBase::DRAW_FUNC)DrawLinoFunc);		/* ���m��p�̕`��֐���ݒ� */
		//		sPlayer.SetDrawFunc((ModelBase::DRAW_FUNC)DrawUsamiFunc);		/* ���m��p�̕`��֐���ݒ� */
		GraphicManager::GetInstance()->EntryDrawModel(&sPlayer);	// �`��G���g���[
		CollisionManager::GetInstance()->EntryBodyCollision(&sPlayer);		// �����蔻��G���g���[
		CollisionManager::GetInstance()->EntryAttackCollision(&sPlayer);	// �����蔻��G���g���[

		// ��x�S�ăA�^�b�`���͂���
		sPlayer.DeattachModelAll();

		/* �}�X�^�[�\�[�h���A�^�b�` */
		sPlayer.AttachModel(&sModelXfile[eMD_SWORD], 
							ModelPmd::eATTACHED_MODEL_NORMAL,
							sPlayer.GetFrameIndexFromName("�����w�P"));

		/* �T�u�E�F�|�����A�^�b�` */
		sPlayer.AttachModel(&sGimmick, 
							ModelPmd::eATTACHED_MODEL_MOTION, 
							sPlayer.GetFrameIndexFromName("�E���w�P"));

		// �V�g�̗����A�^�b�`������
		sPlayer.SetupAttachAngelWing();

		/* �����A�^�b�` */
		sPlayer.AttachModel(&sModelXfile[eMD_SHIELD], 
							ModelPmd::eATTACHED_MODEL_NORMAL,
							sPlayer.GetFrameIndexFromName("�E���w�P"));

		// ���ݑ������Ă���T�u�E�F�|����ݒ肳����
		changeSubWeapon(sPlayer.ItemWork().mItemSubWeapon);

		/*===================================== *
		 * NPC�̐���							*
		 *===================================== */
		if (spNpc == NULL) {
			spNpc = CharaNpc::CreateNpcInstance(spBGInfo->mScale);
		}

		/*=====================================*
		 * �A�C�e���̐���
		 *=====================================*/
		sObjectBaseNum = getItemNumFromMapData(); /* �A�C�e�����擾 */

		if (sObjectBaseNum != 0) {
			sppObjectBase = new ObjectBase*[sObjectBaseNum];
			APP_ASSERT(sppObjectBase != NULL, "Failed new ObjectBase\n");
			for (int iI = 0; iI < sObjectBaseNum; iI ++) {
				sppObjectBase[iI] = NULL;
			}
		}
		// �ꊇ�Ńp�����[�^�ݒ�
		setupItemParameterFromMapData();

		/*=====================================*
		 * �I�u�W�F�N�g�t���O����I�u�W�F�N�g�g�p��Ԃ��X�V
		 *=====================================*/
		updateObjectUsedFromObjectFlag();

		// �`�F�b�N�\�ȃI�u�W�F�N�g�̐ݒ���s��
		// �I�u�W�F�N�g���X�g�Ɉȉ��̏����̃|�C���^�̂ݒǉ�����
		// �ECHECK_ENABLE �̍���
		// �E���g�p�ς݂̃I�u�W�F�N�g
		ObjectBase::GetActiveObjectListRef().clear();
		
		int obj_kind = 0;
		for (int iI = 0; iI < sObjectBaseNum; iI ++) {
			obj_kind = sppObjectBase[iI]->GetObjectKind();
			if ((ObjectBase::GetObjectBaseParam(obj_kind)->mObjectCheck == ObjectBase::eOBJECT_CHECK_ENABLE) &&
				(sppObjectBase[iI]->IsUsed() == false) ) {
				ObjectBase::GetActiveObjectListRef().push_back(sppObjectBase[iI]);
			}
		}


		/*=====================================*
		 * �M�~�b�N�̐ݒ�
		 *=====================================*/
		GimmickBase::SetupAllGimmick();

		/*=====================================*
		 * �G�L�����̐���						
		 *=====================================*/
		sppEnemy = new CharaEnemy*[kENEMY_ONCE_MAX];
		APP_ASSERT(sppEnemy != NULL, "Failed new CharaEnemy[]\n");
		for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
			// �ŏ��̓_�~�[�łƂ肠��������Ƃ�
			sppEnemy[iI] = new CharaEnemy;
			APP_ASSERT( (sppEnemy[iI] != NULL), 
						"Failed new CharaEnemy() \n");
		}

		// �S�Ă̓G�̓�����L���ɂ���
		CharaEnemy::SetStopAllEnemyFlag(FALSE);
	}

	after_time = GetNowCount();
	PRINT_CONSOLE("PMD Link : %d mm second\n", after_time - before_time);

	/**** �`��G���g���[ ****/
	for (int iI = 0; iI < eMD_MAX; iI ++) {
		if (iI == eMD_GETTED_ITEM) continue;
		GraphicManager::GetInstance()->EntryDrawModel(&sModelXfile[iI]);
	}

	for (unsigned int iI = 0; iI < sSubStageInfoList.size(); iI ++) {
		GraphicManager::GetInstance()->EntryDrawMapModel(&spSubModelMap[iI]);
	}
	GraphicManager::GetInstance()->EntryDrawMapModel(&sModelMap);
	
	/**** �}�b�v�̃Z�b�g�A�b�v ****/
	int mesh_idx = sModelMap.SearchMaterialName(eBG_TYPE_TASK, "FAL_000_000");
	if (mesh_idx != -1) {
		// FAL_000_000 �̃}�e���A����Min/Max�����������ꍇ
		VECTOR max_pos, min_pos;
		float view_angle = Utility::DegToRad(60.0f);
		VECTOR center_pos;
		float length;
		sModelMap.GetMeshMinPosition(&min_pos, mesh_idx);
		sModelMap.GetMeshMaxPosition(&max_pos, mesh_idx);
		max_pos.y = min_pos.y;

		center_pos = VScale(VAdd(max_pos, min_pos), 0.5f);
		length = abs(VSize(VSub(max_pos, min_pos)) * 0.5f);
		sShowMapWork.mIsZoom			= TRUE;
		sShowMapWork.mCamDist			= tan(view_angle) * length;

		float far_dist = (CameraManager::GetInstance()->GetFarClip());
		if (sShowMapWork.mCamDist > far_dist) {
			// �ł�������}�b�v�͏������ʂ�
			//			sShowMapWork.mCamDist = far_dist;
			sShowMapWork.mCamDist *= 0.3f;
		}	

		sShowMapWork.mCamCenterPos		= center_pos;
	}
	else {
		sShowMapWork.mIsZoom			= TRUE;
		sShowMapWork.mCamDist			= 75000.0f;
		sShowMapWork.mCamCenterPos		= sPlayer.Position();
	}

	// �ړ��p�̃��C�A�E�g�̏���
	sMapIconMove.mLayoutNum = MV1GetFrameMeshNum(sModelMap.GetModelHandle(), eBG_TYPE_ROOF);

	APP_ASSERT_PRINT(sMapIconMove.mLayoutNum < (sizeof(sMapIconMove.mpLayout) / sizeof(sMapIconMove.mpLayout[0])),
					 "Get Layout Num is Over Capacity \n" );
	for (int iI = 0; iI < sMapIconMove.mLayoutNum; iI ++) {
		p_layout = &sMapIconMove.mpLayout[iI];
		p_layout->LinkLayout(RES_LYT_ICON_MAP_MOVE);
		p_layout->SetPositionByCentering(40, 440);
		p_layout->SetSize(16, 16);
		p_layout->SetAlpha(0);
	}
	for (int iI = 0; iI < sMapIconMove.mLayoutNum; iI ++) {
		p_layout = &sMapIconMove.mpLayout[iI];
		GraphicManager::GetInstance()->EntryDrawLayout(p_layout);
	}

#if 0
	/*=====================================*
	 * ���C�A�E�g�̐ݒ�						*
	 *=====================================*/
	p_layout = &sLayout[eLY_LINO_FACE];
	p_layout->LinkLayout(RES_LYT_LINO_FACE_A);
	p_layout->SetPositionByCentering(80, 60);
	p_layout->SetSize(80, 80);
	p_layout->SetAlpha(255);

	/* �T�u�E�F�|���̃A�C�R���ݒ� */
	p_layout = &sLayout[eLY_SUB_WEAPON];
	/* LinkLayout�̏����� changeSubWeapon() ���Ŋ��Ɏ��s�ς� */
	p_layout->SetPositionByCentering(568, 64);
	p_layout->SetSize(80, 80);
	p_layout->SetAlpha(255);

	// ���b�Z�[�W��
	float aspect_rate = GraphicManager::GetInstance()->GetAspectRate();
	p_layout = &sLayout[eLY_BALLOON];
	p_layout->LinkLayout(RES_LYT_BALLOON_MSG);
	p_layout->SetPositionByCentering(320, 384);
	p_layout->SetSize( (int)(630.0f / aspect_rate), 192);
	p_layout->SetAlpha(0);

	// Info
	p_layout = &sLayout[TotalInfo::eLY_INFO];
	p_layout->SetPositionByCentering(320, 240);
	p_layout->SetSize(512, 128);
	p_layout->SetAlpha(0);

	/* �g�o�Q�[�W�̘g */
	p_layout = &sLayout[eLY_GAUGE_FRAME];
	p_layout->LinkLayout(RES_LYT_ICON_GAUGE_FRAME);
	p_layout->SetPositionByCentering(212, 440);
	p_layout->SetSize(256, 16);
	p_layout->SetAlpha(0);

	/* �g�o�Q�[�W�̐� */
	p_layout = &sLayout[eLY_GAUGE_LINE];
	p_layout->LinkLayout(RES_LYT_ICON_GAUGE_LINE);
	p_layout->SetPositionByCentering(212, 440);
	p_layout->SetSize(256, 16);
	p_layout->SetAlpha(0);

	/* �g�o�Q�[�W�̐� */
	p_layout = &sLayout[eLY_ENEMY_FACE];
	p_layout->LinkLayout(RES_LYT_FACE);
	p_layout->SetPositionByCentering(40, 440);
	p_layout->SetSize(64, 64);
	p_layout->SetAlpha(0);

	// �}�b�v�������Ă��邩
	p_layout = &sLayout[eLY_HAS_MAP];
	p_layout->LinkLayout(RES_LYT_ITEM_RING_MAP);
	p_layout->SetPositionByCentering(590, 280);
	p_layout->SetSize(48, 48);
	p_layout->SetAlpha(220);

	// ���������Ă��邩
	p_layout = &sLayout[eLY_HAS_KEY];
	p_layout->LinkLayout(RES_LYT_ITEM_RING_BOSS_KEY);
	p_layout->SetPositionByCentering(590, 390);
	p_layout->SetSize(48, 48);
	p_layout->SetAlpha(220);

	/* �}�b�v�p�̖�� */
	p_layout = &sLayout[eLY_MAP_ARROW];
	p_layout->LinkLayout(RES_LYT_MAPARROW);
	p_layout->SetPositionByCentering(40, 440);
	p_layout->SetSize(48, 64);
	p_layout->SetAlpha(0);

	/* �}�b�v�̕\���A�C�R�� */
	p_layout = &sLayout[eLY_MAP_POSITION];
	p_layout->LinkLayout(RES_LYT_ICON_MAP_LINO_FACE);
	p_layout->SetPositionByCentering(40, 440);
	p_layout->SetSize(32, 32);
	p_layout->SetAlpha(0);

	//	for (int iI = 0; iI < eLY_MAX; iI ++) {
	for (int iI = 0; iI < eLY_DEFAULT_DRAW_MAX; iI ++) {
		if (iI == TotalInfo::eLY_INFO) continue;
		GraphicManager::GetInstance()->EntryDrawLayout(&sLayout[iI]);
	}

	if (GraphicManager::GetInstance()->CheckEntrySystemLayout(&sLayout[TotalInfo::eLY_INFO]) == false) {
		GraphicManager::GetInstance()->EntrySystemLayout(&sLayout[TotalInfo::eLY_INFO]);
	}
#endif // 0

	TotalInfo::GetInstance()->SetActiveLockonEnemyName(TRUE);

	CameraManager::GetInstance()->SetPatternGameBegine();

	// sReviseDiffPos���N���A
	sReviseDiffPos = ZERO_VEC;

	// ���͋֎~
	sPlayer.PermitAcceptInput(false);

	// ���݂���t�B�[���h�̃X�^�[�g�t���O�𗧂Ă�
	TotalInfo::GetInstance()->SetEnableFieldInfoFlag(sStageIndex, kFIELD_INFO_START_BIT);

	// �V���h�E�}�b�v�`��ݒ�n���h���o�^
	ShadowMap::SetBeforeCreateCallback(BeforeCreateShadowFunc);
	ShadowMap::SetAfterCreateCallback(AfterCreateShadowFunc);

	mPhase = SceneMainGame::ePHASE_GAME_BEFORE;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseGameBefore(void)
{
	// �u���[���̓V�X�e���Ƃ��Ďg�p��
	RenderBloom::SetEnableSystemBloom(TRUE);

	// �F�ʕ␳�Ƃ��ɗL���Ɂi�������A���j���[�ݒ肪�D��j
	TotalInfo::GetInstance()->SetEnableColorRevision(TRUE);

	sPlayer.SetPositionOnCollision();
	for (int iI = 0; iI < sNpcNum; iI ++) {
		spNpc[iI].SetPositionOnCollision();
		spNpc[iI].SetupOnceDrawShadow();
	}
	for (int iI = 0; iI < sObjectBaseNum; iI ++) {
		sppObjectBase[iI]->SetPositionOnCollision();
	}

	// �Q�[���J�n�O�� Far/Near�N���b�v��߂�
	CameraManager::GetInstance()->SetDefaultNearFarClip();

	// ��p��߂�
	CameraManager::GetInstance()->SetPerspectiveCamera(kDEFAULT_PERSPECTIVE_FOV);

	// �R�X�v�����Ȃ�ύX������
	if ((sPlayer.CosplayIndex() == eWINMENU_COSPLAY_TITANIS) ||
	    (sPlayer.CosplayIndex() == eWINMENU_COSPLAY_GUARDIAN)) {
		sPlayer.RevertCosplay();
	}
	if (sPlayer.CosplayIndex() != eWINMENU_COSPLAY_NONE) {
		TotalInfo::tWinMenuCosplayInfo* p_cosplay_info = TotalInfo::GetInstance()->GetCosplayInfo(sPlayer.CosplayIndex());
		SceneMainGame::ChangePlayerModel(p_cosplay_info->mModelName, FALSE);
	}

	// ���[�h��̃}�b�v���̃Z�b�g�A�b�v
	spStageFunc->SetupAfterLoad((int&)mPhase);
	StageFuncBase::UpdateLightInfo();

	// �t�H�O�̃o�b�N�A�b�v�����
	GraphicManager::GetInstance()->BackupFogParam();

	if (mPhase == SceneMainGame::ePHASE_EVENT_BEFORE) {
		return;
	}

	// �i�r�H�J�[�\����L���ɂ���
	sNavi.EnableNaviCursor();

	/****  �Đ���BGM ****/
	SoundManager::GetInstance()->PlayBGM(spBGInfo->mMusicName);

	VISIBLE_BATTLE_ICON();	// �o�g���p�A�C�R���\��

	// �t�F�[�h�C��
	FADE_BOARD()->StartFade(0, 0.5f);

	sPlayer.PermitAcceptInput(true);
	mPhase = SceneMainGame::ePHASE_GAME;

	sModelXfile[eMD_SKYDOME].Rotation().y += 0.00025f;
	sModelXfile[eMD_SKYDOME].Position().x = sPlayer.Position().x;
	sModelXfile[eMD_SKYDOME].Position().z = sPlayer.Position().z;

	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseGame(void)
{
#ifdef _DEBUG
	static int sEffIndex = -1;
	// �C�x���g�t���O�̑S�N���A
	if (CHECK_TRIG_KEY(DBG_KEY_EVENT_CLEAR)) {
		TotalInfo::GetInstance()->ResetAllEventFlag();

		char message[32] = {0};
		VECTOR player_pos;
		player_pos = sPlayer.Position();
		sprintf_s(message, sizeof(message), "%.1f,%.1f,%.1f", player_pos.x, player_pos.y, player_pos.z);
		PRINT_CONSOLE("PlayerPos = %s\n", message);
	}

	if (CHECK_TRIG_KEY(eKEY_NUMPAD0)) {
		PRINT_CONSOLE("ReloadInformation() \n");
		GimmickBase::ReloadInformationAll(sStageIndex);
	}

	if (CHECK_TRIG_KEY(DBG_KEY_START_EVENT)) {

		double x, y;
		GetWindowSizeExtendRate(&x, &y);
		PRINT_CONSOLE("GetWindowSizeExtendRate(%lf, %lf \n", x, y);
		SetWindowSizeExtendRate(1.1, 1.1);
		GetWindowSizeExtendRate(&x, &y);
		PRINT_CONSOLE("GetWindowSizeExtendRate(%lf, %lf \n", x, y);
	}
#endif // _DEBUG

	/*=====================================*
	 * �L�����␳�̏���
	 *=====================================*/
	float dist = VSize(sPlayer.Position());
#ifdef _DEBUG
	PRINT_SCREEN(GetColor(255, 255, 0), "dist : %.2f", dist);
#endif _DEBUG
	if (dist > ALL_MODEL_REVISE_DIST) {
		this->ReviseAllModel();
	}

	/*=====================================*S
	 * �X�J�C�h�[���̏���
	 *=====================================*/
	sModelXfile[eMD_SKYDOME].Rotation().y += 0.00025f;
	
	sModelXfile[eMD_SKYDOME].Position().x = sPlayer.Position().x;
	sModelXfile[eMD_SKYDOME].Position().z = sPlayer.Position().z;

	/*=====================================*
	 * �J�������[�N
	 *=====================================*/
	ModelCollision* p_lockon = sPlayer.GetLockonChara();

	/*=====================================*
	 * ���b�N�I������
	 *=====================================*/
	if (p_lockon != NULL) {
		float size = 1.5f;
		float hp_rate = (float)p_lockon->GetHitpoint() / (float)p_lockon->GetMaxHitpoint();

		int gauge_size = (int)((float)(256.0f * hp_rate));

		sModelXfile[eMD_LOCKON].Scale() = VGet(size, size, size);

		VECTOR lockonpos;
		p_lockon->GetLockonPosition(&lockonpos);
		sModelXfile[eMD_LOCKON].Position() = lockonpos;

		TotalInfo::GetInstance()->FadeinLockonEnemyLayout(gauge_size);
	}
	else {
		float size = 0.0f;
		sModelXfile[eMD_LOCKON].Scale() = VGet(size, size, size);

		TotalInfo::GetInstance()->FadeoutLockonEnemyLayout();
	}

	/*=====================================*
	 * �\���؂�ւ�
	 *=====================================*/
	bool is_encount = false;
	for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
		if (sppEnemy[iI]->GetIsActive() == true) {
			is_encount = true;
			break;
		}
	}
	TotalInfo::GetInstance()->ChangePlayerFaceLayout(is_encount);

	/*=====================================*
	 * �X�e�[�W���̏���
	 *=====================================*/
	spStageFunc->Update((int)mPhaseCounter);

	/*=====================================*
	 * NPC�Ƃ̏���
	 *=====================================*/

	updateNpcEvent();

	/*=====================================*
	 * �I�u�W�F�N�g�Ɠ��͎��̏���
	 *=====================================*/
	updateObjectInputCheck();

	/*=====================================*
	 * �A�C�e������
	 *=====================================*/
	if (spDisappearEnemy != NULL) {
		int rand_value = GetRand(100-1);
		int object_index;
		int percent;
		spDisappearEnemy->GetItemInfo(&object_index, &percent);
		if (rand_value < percent) {
			for (int iI = 0; iI < kENEMY_ITEM_NUM; iI ++) {
				if (sDropItem[iI].GetIsActive() == false) {
					if (spDisappearEnemy->GetIsActive() == true) {
						ObjectItem* p_item = &sDropItem[iI];
						VECTOR setpos = spDisappearEnemy->Position();
						p_item->Setup(object_index, (eItemKind)0, &spDisappearEnemy->Position(), 0);
						p_item->SetPositionOnCollision();
						p_item->SetRotationToTarget(&sPlayer.Position());
						break;
					}
				}
			}
		}
		spDisappearEnemy = NULL;
	}

	/*=====================================*
	 * �C�x���g�o�g���̏���
	 *=====================================*/
	if (sIsBattle == true) {
		int active_enmy_num = 0;
		for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
			if (sppEnemy[iI]->GetIsActive()) {
				active_enmy_num ++;
			}
		}
		if (active_enmy_num == 0) {
			// �o�g���p�̕ǁA��\��
			CollisionManager::GetInstance()->RemoveWallCollision(sModelMap.GetModelHandle(),
																 eBG_TYPE_BATTLE_WALL);
			sModelMap.UnvisibleBattleBox();
			sPlayer.UnvisibleWeaponArrow();
			SoundManager::GetSndComponent()->PlaySound(eSE_CLEAR_BATTLEBOX);

			if (sEventBattleFlag != -1) {
				ActionTaskManager::tTaskInfo param;
				param.mTaskType = ActionTaskManager::eTASK_EVENT;
				param.mTaskValueA = BATTLEFLAG_TO_TASKVALUE_A(sEventBattleFlag);
				param.mTaskValueB = BATTLEFLAG_TO_TASKVALUE_B(sEventBattleFlag);
				RequestChangeState(&param);
				sEventBattleFlag = -1;

				sEventRecoveryFlag = -1;
			}
			sIsBattle = false;
		}
	}

	/*=====================================*
	 * �}�b�v�\���ؑցE�T�u�E�F�|���ؑ�
	 *=====================================*/
	int player_state = sPlayer.GetState();
	if ((player_state == CharaBase::eCHARA_STATE_IDLE) ||
		(player_state == CharaPlayer::eCHARA_STATE_SLEEP)) {
		if (CHECK_TRIG_JOY(JOY_INPUT_SHOW_MAP)) {

			if (TotalInfo::GetInstance()->GetHasFieldMap(sStageIndex)) {
				mPhase = SceneMainGame::ePHASE_SHOW_MAP;
			} else {
				StartNaviTalk(strNaviTalkNoMap);
			}
		}
		else if (CHECK_TRIG_JOY(JOY_INPUT_SELECT_SUB)) {
			if ((sPlayer.CosplayIndex() != eWINMENU_COSPLAY_TITANIS) &&
				(sPlayer.CosplayIndex() != eWINMENU_COSPLAY_GUARDIAN)) {
				if (sPlayer.IsSubWeaponUsing() == false) {

					spSubSceneInst = new SubSceneItemMenu();
					APP_ASSERT((spSubSceneInst != NULL),
							   "Failed new spSubSceneInst\n");
				
					((SubSceneItemMenu *)spSubSceneInst)->Create(sItemMenuDefault, sMyWeaponList.size());

					TotalInfo::GetInstance()->SetActiveLockonEnemyName(FALSE);

					mPhase = SceneMainGame::ePHASE_SELECT_WEAPON;
					CHECK_TRIG_JOY(JOY_INPUT_SELECT_SUB);
				}
			}
			else {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
			}

		}
	}

	/*=====================================*
	 * �i�r�H�̃g�[�N����
	 *=====================================*/
	if (sStrNaviTalkPtr != NULL) {
		phaseNaviTalk();
	}

	/*=====================================*
	 * F4�ł̃��J�o���[����
	 *=====================================*/
	if (CHECK_TRIG_KEY(eKEY_F4)) {
		if (false) { //<! �̌��łł͂O�ł��ł��邱�Ƃ�
			// ����������Ȃ��ꍇ
			SoundManager::GetSndComponent()->PlaySound(eSE_NG);
		} else {

			if ((sPlayer.CosplayIndex() != eWINMENU_COSPLAY_TITANIS) &&
				(sPlayer.CosplayIndex() != eWINMENU_COSPLAY_GUARDIAN)) {

				sPlayer.PermitAcceptInput(false);
				// ���f����Update���Ƃ߂�
				GraphicManager::GetInstance()->SetStopModeUpdate(true);

				SetVolumeMusic(128);
				SoundManager::GetSndComponent()->PlaySound(eSE_WARP);

				FADE_COL_WHITE();
				FADE_OUT(1.0f);
				mPhase = SceneMainGame::ePHASE_RECOVERY;
			} 
			else {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
			}

		}
	}

	/*=====================================*
	 * �Q�[���I�[�o�[�̔���
	 *=====================================*/
	if (sPlayer.GetState() == CharaBase::eCHARA_STATE_RETIRE_END) {
		FADE_BOARD()->StartFade(200, 3.0f);
		SoundManager::GetInstance()->StopBGM();

		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);
		mPhase = SceneMainGame::ePHASE_GAMEOVER_BEFORE;
	}
#ifdef _DEBUG
	debugPramInput();
#endif // _DEBUG

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseShowMap(void)
{
	/* phaseShowMap �̏����� */
	if (EQUAL_PHASE_SECOND(0.0f)) {

		// �X�e�[�W���̏������K�v�ȏꍇ
		spStageFunc->ProcessShowMapBefore();

		sPlayer.PermitAcceptInput(false);

		/* ���C�A�E�g�E���f���̕\��/��\�� */
		HIDE_BATTLE_ICON();
		TotalInfo::GetInstance()->SetCommapssForShowMap();
		sModelXfile[eMD_SKYDOME	].SetVisible(false);

		for (int iI = 0; iI < sMapIconMove.mLayoutNum; iI ++) {
			sMapIconMove.mpLayout[iI].SetAlpha(255);
		}

		GraphicManager::GetInstance()->SetStopModeUpdate(true);

		/* �J�����̒l���Z�[�u */
		CameraManager::GetInstance()->SaveTemporaryCamWork();

		// �J�����ʒu�̓��m�̐^���
		CameraManager::GetInstance()->Target() = sPlayer.Position();

		/* �t�H�O�̒l���ꎞ�I�ɖ����ɂ��� */
		GraphicManager::GetInstance()->PauseFog();

		/* �}�b�v�̐����̎����̏ꍇ�͈ꎞ�I�ɃV���h�E�}�b�v���������� */
		ShadowMap::PauseShadowMap();

		/* �V�X�e���Ƃ��ău���[���𖳌��ɂ��� (�����̂��\��������Ă��܂��o�O�����������) */
		RenderBloom::SetEnableSystemBloom(FALSE);

		sShowMapWork.mIsZoom = TRUE;

		TotalInfo::GetInstance()->SetActiveLockonEnemyName(FALSE);

		/* �ォ�猩�グ��J������ݒ肷�� */
		return; // �Ӑ}�I�Ƀ��^�[��

	} // if (EQUAL_PHASE_SECOND(0.0f)) {


	/* �e�\���L�[�ɉ����ăJ�������ړ������� */
	if (sShowMapWork.mIsZoom == TRUE) {
		static const float sMapCursorSpeed = 160.0f;
		if (CHECK_PRESS_JOY(JOY_INPUT_UP)) {
			CameraManager::GetInstance()->Target().z += sMapCursorSpeed;
		}
		else if (CHECK_PRESS_JOY(JOY_INPUT_DOWN)) {
			CameraManager::GetInstance()->Target().z -= sMapCursorSpeed;
		}
		if (CHECK_PRESS_JOY(JOY_INPUT_RIGHT)) {
			CameraManager::GetInstance()->Target().x += sMapCursorSpeed;
		}
		else if (CHECK_PRESS_JOY(JOY_INPUT_LEFT)) {
			CameraManager::GetInstance()->Target().x -= sMapCursorSpeed;
		}
	}

	// �t�@�[�N���b�v��ݒ�
	CameraManager::GetInstance()->SetFarClip(sShowMapWork.mCamDist*1.2f);

	if (sShowMapWork.mIsZoom) {
		CameraManager::GetInstance()->SetPatternDist(sShowMapWork.mCamDist*0.3f);
		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_SHOW_MAP);

	} else {
		CameraManager::GetInstance()->SetPatternDist(
			sShowMapWork.mCamDist);
		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_SHOW_MAP);

	}
	// ���ˉe�J�����ɐݒ�
	CameraManager::GetInstance()->SetOrthoCamera( CameraManager::GetInstance()->GetCameraDist() );

	VECTOR screen_pos, world_pos;
	float aspect_rate = GraphicManager::GetInstance()->GetAspectRate();
	/* �L�����̏ꏊ���QD�ŕ\������ */
	world_pos = sPlayer.Position();
	screen_pos = ConvWorldPosToScreenPos(world_pos);
	screen_pos.x /= GraphicManager::GetInstance()->GetLayoutScaleRateX();
	screen_pos.y /= GraphicManager::GetInstance()->GetLayoutScaleRateY();

	LayoutBase* p_lyt_pos	= TotalInfo::GetInstance()->GetSystemLayout(TotalInfo::eLY_MAP_POSITION);
	LayoutBase* p_lyt_arrow = TotalInfo::GetInstance()->GetSystemLayout(TotalInfo::eLY_MAP_ARROW);

	p_lyt_pos->SetPositionByCentering((int)(screen_pos.x * aspect_rate), (int)screen_pos.y);
	p_lyt_arrow->SetPositionByCentering((int)(screen_pos.x * aspect_rate), (int)screen_pos.y);

	// �X�̏ꍇ�̂݁A�R���p�X��������������
	if (sStageIndex == 16) {
		p_lyt_arrow->Rotation() += 0.2f;
		if (p_lyt_arrow->Rotation() > (PHI_F * 2.0f)) {
			p_lyt_arrow->Rotation() -= (PHI_F * 2.0f);
		}
	}
	else {
		p_lyt_arrow->Rotation() = (sPlayer.Rotation().y + PHI_F);
	}

	// ���C�A�E�g��_�ł�����
	if (((int)mPhaseCounter % 80) > 40) {
		p_lyt_pos->SetAlpha(0);
		p_lyt_arrow->SetAlpha(0);
	} else {
		p_lyt_pos->SetAlpha(255);
		p_lyt_arrow->SetAlpha(255);
	}

	/* �C�x���g�̏ꏊ�ɃA�C�R����\�� */
	const char* mat_name;
	int mesh_idx, mat_idx, iI;
	int model_handle = sModelMap.GetModelHandle();
	int mesh_num = MV1GetFrameMeshNum(model_handle, eBG_TYPE_ROOF);
	VECTOR max_pos, min_pos;
	LayoutBase* p_layout = NULL;
	int cnt_event = 0;
	int cnt_move = 0;

	for (iI = 0; iI < mesh_num; iI ++) {
		mesh_idx = MV1GetFrameMesh(model_handle, eBG_TYPE_ROOF, iI);
		mat_idx = MV1GetMeshMaterial(model_handle, mesh_idx) ;
		mat_name = MV1GetMaterialName(model_handle, mat_idx);
		p_layout = NULL; /* need */
		p_layout = &sMapIconMove.mpLayout[cnt_move];
		cnt_move ++;

		if (p_layout != NULL) {
			min_pos = MV1GetMeshMinPosition(model_handle, mesh_idx);
			min_pos = VScale(min_pos, sModelMap.Scale().x);
			max_pos = MV1GetMeshMaxPosition(model_handle, mesh_idx);
			max_pos = VScale(max_pos, sModelMap.Scale().x);
			world_pos = VScale(VAdd(max_pos, min_pos), 0.5f);
			world_pos = VAdd(world_pos, MV1GetFramePosition(model_handle, eBG_TYPE_ROOF));

			screen_pos = ConvWorldPosToScreenPos(world_pos);
			screen_pos.x /= GraphicManager::GetInstance()->GetLayoutScaleRateX();
			screen_pos.y /= GraphicManager::GetInstance()->GetLayoutScaleRateY();
			p_layout->SetPositionByCentering((int)(screen_pos.x * aspect_rate), (int)screen_pos.y);
			p_layout->SetAlpha(255);
		}
	}

	/* �}�b�v�\���I������ */
	if (CHECK_TRIG_JOY(JOY_INPUT_ATTACK) ||
		CHECK_TRIG_JOY(JOY_INPUT_AWAY) ||
		CHECK_TRIG_JOY(JOY_INPUT_JUMP) ||
		CHECK_TRIG_JOY(JOY_INPUT_SUB_WEAPON) ||
		CHECK_TRIG_JOY(JOY_INPUT_START) ||
		CHECK_TRIG_JOY(JOY_INPUT_SHOW_MAP)) {
		
		if (sShowMapWork.mIsZoom == TRUE) {
			// �J�����ʒu�̓��m�̐^���
			CameraManager::GetInstance()->Target() = sPlayer.Position();
			sShowMapWork.mIsZoom = FALSE;
		}
		else {

			// ���ˉe�J�����ɐݒ�
			CameraManager::GetInstance()->SetPerspectiveCamera(kDEFAULT_PERSPECTIVE_FOV);
			CameraManager::GetInstance()->LoadTemporaryCamWork();
			CameraManager::GetInstance()->Up() = VGet(0.0f, 1.0f, 0.0f);
			CameraManager::GetInstance()->SetDefaultNearFarClip();

			// �X�e�[�W���̏������K�v�ȏꍇ
			spStageFunc->ProcessShowMapAfter();

			sPlayer.PermitAcceptInput(true);

			VISIBLE_BATTLE_ICON();

			p_lyt_pos->SetAlpha(0);
			p_lyt_arrow->SetAlpha(0);

			for (int iI = 0; iI < sMapIconMove.mLayoutNum; iI ++) {
				sMapIconMove.mpLayout[iI].SetAlpha(0);
			}
			sModelXfile[eMD_SKYDOME	].SetVisible(true);

			GraphicManager::GetInstance()->SetStopModeUpdate(false);

			// �J������߂�
			if (sPlayer.GetLockonChara() == NULL) {
				CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
			}
			else {
				CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_LOCKON);
			}
			/* �t�H�O�̒l���ꎞ�I�ɗL���ɂ��� */
			GraphicManager::GetInstance()->ResumeFog();

			/* �ꎞ�I�ɃV���h�E�}�b�v�����������̃X�e�[�^�X�ɖ߂� */
			ShadowMap::ResumeShadowMap();

			/* �V�X�e���Ƃ��ău���[����L���ɖ߂� */
			RenderBloom::SetEnableSystemBloom(TRUE);

			TotalInfo::GetInstance()->SetActiveLockonEnemyName(TRUE);

			mPhase = SceneMainGame::ePHASE_GAME;
		}
	}

#ifdef _DEBUG
	debugPramInput();
#endif // _DEBUG
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseChangeStage(void)
{
	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);

	if (FADE_BOARD()->IsFade() == false) {

		Destroy();
		sPlayer.PermitAcceptInput(true);
		mPhase = SceneMainGame::ePHASE_LOAD_BEFORE;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseFallDown(void)
{
	sPlayer.PermitAcceptInput(false);

	if (FADE_BOARD()->IsFade() == false) {
	
		VECTOR water_pos = ZERO_VEC;
		sModelMap.GetFramePosFromIndex(&water_pos, eBG_TYPE_WATER);
		float water_height;
		water_height = water_pos.y + sModelMap.WaterOffsetPos();

		// �M�~�b�N��S�ă��[�h���Ȃ���
		GimmickBase::ReloadInformationAllFallDown(sStageIndex);

		//  ���m���f���̃M�~�b�N��OnNode�������Z�b�g����B
		INT8 col_index = sPlayer.GetOnCollisionIndex();
		GimmickBase* p_gimmick = NULL;
		p_gimmick = (GimmickBase *)GimmickBase::GetGimmickBaseFromEarthCollIndex(col_index);
		if (p_gimmick != NULL) {
			p_gimmick->ResetOnNodeInfo(&sPlayer);
		}

		if (sStageIndex == 39) {
			spStageFunc->SetPatternLight(0);
		}

		sPlayer.ProcessFallDown(water_height);

		// �t�F�[�h�C��
		FADE_BOARD()->StartFade(0, 0.75f);
		sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_IDLE);
		sPlayer.PermitAcceptInput(true);
		sPlayer.SetActionLimit(spBGInfo->mActionLimit,
							   sPlayer.ItemWork().mItemSubWeapon);

		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);

		GimmickBase::SetGimmickPauseAttach(FALSE);

		// �t�F�[�Y�J�E���^�[�����Z�b�g����
		mPhaseCounter = 0;

		mPhase = SceneMainGame::ePHASE_GAME;
	} 
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseRecovery(void)
{
	if (FADE_BOARD()->IsFade() == false) {
	
		// �o�g���{�b�N�X����������
		CollisionManager::GetInstance()->RemoveWallCollision(sModelMap.GetModelHandle(), eBG_TYPE_BATTLE_WALL);
		sModelMap.UnvisibleBattleBox();
		sPlayer.UnvisibleWeaponArrow();
		/* �G��S�ł����� */
		for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
			if (sppEnemy[iI]->GetIsActive() == true) {
				sppEnemy[iI]->ProcessDamage(kSYSTEM_DAMAGE_VALUE, true, &VGet(0.0f, 0.0f, 0.0f), (VECTOR *)&ZERO_VEC, false, eATTACK_TYPE_NORMAL); // �[���I�ɓG��|��
			}
		}

		// �o�g���C���f�b�N�X�t���O�͌��ɖ߂�
		sBattleIndex = -1;
		// �C�x���g�o�g���t���O�����ɖ߂�
		sEventBattleFlag = -1;
	
		// �G�L������~�t���O
		CharaEnemy::SetStopAllEnemyFlag(FALSE);

		// �C�x���g�t���O�̃L�����Z���\��ݒ�
		if (sEventRecoveryFlag != -1) {
			TotalInfo::GetInstance()->DisableEventFlag(BATTLEFLAG_TO_TASKVALUE_A(sEventRecoveryFlag),
													   BATTLEFLAG_TO_TASKVALUE_B(sEventRecoveryFlag));
		}

		// �I�u�W�F�N�g�g�p��Ԃ���I�u�W�F�N�g�t���O���X�V
		setObjectFlagFromObjectUsed();

		//		// �ړ����̃t�F�[�h�A�E�g���ɌĂ΂��n���h��
		//		spStageFunc->NotifyMovePlaceWhileFadeout();

		GimmickBase::ReloadInformationAll(sStageIndex);

		//  ���m���f���̃M�~�b�N��OnNode�������Z�b�g����B
		INT8 col_index = sPlayer.GetOnCollisionIndex();
		GimmickBase* p_gimmick = NULL;
		p_gimmick = (GimmickBase *)GimmickBase::GetGimmickBaseFromEarthCollIndex(col_index);
		if (p_gimmick != NULL) {
			p_gimmick->ResetOnNodeInfo(&sPlayer);
		}

		// �`�F�b�N�|�C���g��ʉ߂��Ă���΂��̒n�_�ɖ߂�
		ObjectBase* p_check_point = sPlayer.GetCheckPointPtr();
		if (p_check_point != NULL) {
			sPlayer.Position() = p_check_point->Position();
			sPlayer.Position().x += 1.0f;		// ����
			sPlayer.Rotation() = p_check_point->Rotation();
		}
		else {
			getPlayerPosRotFromBGModel(&sPlayer.Position(), &sPlayer.Rotation().y, sStagePlaceIndex);
		} 
		sPlayer.SetPositionOnCollision();

		sPlayer.ProcessRecovery();

		//<! ���J�o���[���ɂ����̊֐����ĂԂ̂Œ��ӁI
		spStageFunc->SetupAfterLoad((int&)mPhase);
		StageFuncBase::UpdateLightInfo();

		SoundManager::GetInstance()->PlayBGM(spBGInfo->mMusicName);

		SetVolumeMusic(255);

		// �t�F�[�h�C��
		FADE_COL_BLACK();
		FADE_IN(0.75f);
		//		FADE_BOARD()->StartFade(0, 0.75f);

		// ���f����Update���ĊJ����
		GraphicManager::GetInstance()->SetStopModeUpdate(false);

		sPlayer.PermitAcceptInput(true);
		sPlayer.SetActionLimit(spBGInfo->mActionLimit,
							   sPlayer.ItemWork().mItemSubWeapon);

		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
		CameraManager::GetInstance()->SetPatternGameBegine();

		mPhase = SceneMainGame::ePHASE_GAME;
	} 
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseMessageBefore(void)
{
	APP_ASSERT(sIsMessageRange != -1, "sIsMessageRange is -1\n");

	CharaNpc* p_npc = &spNpc[sIsMessageRange];

	CameraManager::GetInstance()->SetPatternCharaBase((ModelCollision *)p_npc);
	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_MESSAGE);

	sPlayer.SetRotationToTarget(&p_npc->Position());

	bool is_next = false;

	if (p_npc->GetOptionValue() == CharaNpc::eNPC_OPTION_NOT_TURN) {
		is_next = true;
	}
	else {
		VECTOR vec, tmp;
		vec = VSub(sPlayer.Position(), p_npc->Position());
		vec.y = 0.0f;
		float len = VSize(vec);
		tmp.x = sin(p_npc->Rotation().y) * -len;
		tmp.y = 0.0f;
		tmp.z = cos(p_npc->Rotation().y) * -len;
		float dot;
		dot = VDot(VNorm(vec), VNorm(tmp));
		tmp = VAdd(VScale(tmp, 6.0f), sPlayer.Position());
		p_npc->SetRotationToTarget(&tmp);

		if (dot > 0.999f) {
			is_next = true;
		}
	}

	if (is_next) {
		SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_IN);
		MSG_TYPE_MESSAGE();
		MSG_FADE_IN(0.3f);
		spNpc[sIsMessageRange].StartTalkMotion();
		mPhase = SceneMainGame::ePHASE_MESSAGE;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseMessage(void)
{
	APP_ASSERT(sIsMessageRange != -1, 
			   "sIsMessageRange is -1\n");
	CameraManager::GetInstance()->SetPatternCharaBase((ModelCollision *)&spNpc[sIsMessageRange]);
	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_MESSAGE);

	if (MSG_IS_FADE() == true) {
		return;
	}

	spNpc[sIsMessageRange].RenderNpcMessage();

	if (CHECK_TRIG_ANY_JOY()) {
		SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_OUT);
		MSG_FADE_OUT(0.4f);

		spNpc[sIsMessageRange].StartIdleMotion();

		CameraManager::GetInstance()->LoadTemporaryCamWork();
		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
		mPhase = SceneMainGame::ePHASE_GAME;

		if (spNpc[sIsMessageRange].StartAfterTalkEvent() == false) {
			// ��b��C�x���g���Ȃ��ꍇ�̓L�����֑̋�����������
			sPlayer.PermitAcceptInput(true);

			// �S�Ă̓G�̓�����L���ɂ���
			CharaEnemy::SetStopAllEnemyFlag(FALSE);
		}
	}
}

/*=====================================*
 * EventFunc
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseEventBefore(void)
{
	/* �C�x���g�����̔��菈�� */
	bool is_ignore_event = false;
	bool is_change_camera = true;

	unsigned long event_kind = sTaskInfo.mTaskValueA;
	unsigned long event_flag = sTaskInfo.mTaskValueB;
	switch (event_kind) {

	case eEVENT_14:
		if (event_flag == eEVENT_14_ABOUT_SULT_MINE) {
			if (IS_END_EVT(eEVENT_35, eEVENT_35_AFTER_GET_GREEN_CRYSTAL) == false) {
				is_ignore_event = true;
			}
		}
		break;

	case eEVENT_29:
		if (event_flag == eEVENT_29_ABOUT_MARUIFORCE) {
			if (IS_END_EVT(eEVENT_29, eEVENT_29_COME_FRES) == false) {
				is_ignore_event = true;
			}
		}
		break;

	case eEVENT_36:
		if (event_flag == eEVENT_36_FALL_ONLY_BOSS) {
			if ((IS_END_EVT(eEVENT_36, eEVENT_36_BOSS_START) == true) &&
				(IS_END_EVT(eEVENT_36, eEVENT_36_BOSS_END) == false)) {
				//				is_ignore_event = true;
				CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);
				mPhase = SceneMainGame::ePHASE_FALL_DOWN;
				FADE_OUT(0.75f);
				SoundManager::GetVcComponent(Name_Lino)->PlaySound(eVC_FALL_DOWN);
				return;
			}
			else {
				is_ignore_event = true;
			}
		}
		else if (event_flag == eEVENT_36_WARP_TREASUREROOM) {

			is_ignore_event = true;

			if ((sPlayer.CosplayIndex() != eWINMENU_COSPLAY_MASK) &&
				(sPlayer.IsDamage() == false)) {
				VECTOR damage_vec;
				damage_vec = VScale(VNorm(sPlayer.MoveVec()), -1.0f);
				sPlayer.ProcessDamage(2, false, &sPlayer.Position(), &damage_vec, true, eATTACK_TYPE_ENEMY_NORMAL);
			}

		}
		break;

	case eEVENT_38:
		if (event_flag == eEVENT_38_WARP_OUT_WORLD) {
			// �y�̃N���X�^���̏ꍇ
			if (IS_END_EVT(eEVENT_38, eEVENT_38_CRYSTAL_EARTH) == true) {
				if ((IS_END_EVT(eEVENT_38, eEVENT_38_CRYSTAL_EARTH) == false) ||
					(IS_END_EVT(eEVENT_38, eEVENT_38_TALK_YODEL) == false)) {
					is_ignore_event = true;
				}
			}
			// �X�̃N���X�^���̏ꍇ
			else {
				if ((IS_END_EVT(eEVENT_38, eEVENT_38_CRYSTAL_FOREST) == false) ||
					(IS_END_EVT(eEVENT_38, eEVENT_38_TALK_SENTO) == false)) {
					is_ignore_event = true;
				}
			}
		}
		break;


	case eEVENT_39:
		if ((event_flag == eEVENT_39_GO_TO_OUTSIDE) ||
			(event_flag == eEVENT_39_GO_TO_INSIDE)) {			
			is_change_camera = false;
		}
		break;

	case eEVENT_40:
		if (event_flag == eEVENT_40_BATTLE_USIDERMAN) {
			if (IS_END_EVT(eEVENT_40, eEVENT_40_TORNADE_EVENT) == false) {
				is_ignore_event = true;
			}
		}
		else if (event_flag == eEVENT_40_MOVE_BABEL) {
			if (IS_END_EVT(eEVENT_40, eEVENT_40_WAKEUP_BABEL) == false) {
				is_ignore_event = true;
			}
		}
		else if (event_flag == eEVENT_40_BLOCK_TORNADE) {
			if ((IS_END_EVT(eEVENT_40, eEVENT_40_TORNADE_EVENT) == false) ||
				(IS_END_EVT(eEVENT_40, eEVENT_40_DISAPPER_TORNADE) == true)) {
				is_ignore_event = true;
			}
		}
		else if (event_flag == eEVENT_40_FALL_ONLY_BOSS) {
			if ((IS_END_EVT(eEVENT_40, eEVENT_40_BATTLE_USIDERMAN) == true) &&
				(IS_END_EVT(eEVENT_40, eEVENT_40_DISAPPER_TORNADE) == false)) {
				CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);
				mPhase = SceneMainGame::ePHASE_FALL_DOWN;
				FADE_OUT(0.75f);
				SoundManager::GetVcComponent(Name_Lino)->PlaySound(eVC_FALL_DOWN);
				return;
			}
			else {
				is_ignore_event = true;
			}
		}

		break;

	default:
		break;
	};

	if (is_ignore_event) {
		PRINT_CONSOLE("%s : ignore event \n", __FUNCTION__);
		mPhase = SceneMainGame::ePHASE_GAME;
		return;
	}

	sPlayer.PermitAcceptInput(false);

	// ��x���W�����ɖ߂�
	PauseReviseAllModel(true);
	CameraManager::GetInstance()->Update();
	
	if (is_change_camera) {
		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);
	}

	sEventPhase = 0;
	sEventCount = 0;

	sPlayer.DisableLockonChara();
	sPlayer.processCancelArchery();
	TotalInfo::GetInstance()->DisableLockonLayout();

	// �i�r�H�J�[�\���𖳌��ɂ���
	sNavi.DisableNaviCursor();
	
	if (IsLongEventFlag(event_kind, event_flag) == TRUE) {
		// �C�x���g�J�n���͓G��S�ł�����
		for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
			if (sppEnemy[iI]->GetIsActive() == true) {
				sppEnemy[iI]->ProcessDamage(kSYSTEM_DAMAGE_VALUE, true, (VECTOR *)&ZERO_VEC, (VECTOR *)&ZERO_VEC, false, eATTACK_TYPE_NORMAL); // �[���I�ɓG��|��
			}
		}
	}
	else {
		// �S�Ă̓G�̓������~�߂�
		CharaEnemy::SetStopAllEnemyFlag(TRUE);
	}

	//	sStrNaviTalkPtr = NULL;

	mPhase = SceneMainGame::ePHASE_EVENT;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseEvent(void)
{
	unsigned long event_kind;
	unsigned long event_flag;
	event_kind = sTaskInfo.mTaskValueA;
	event_flag = sTaskInfo.mTaskValueB;

	int phase = sEventPhase;
	bool is_end_evnt = false;

	ULONG event_flag_value = TotalInfo::GetInstance()->GetEventFlagValue(event_kind);

	//	is_end_evnt = (this->*mEventFunc[event_kind][event_flag])();
	is_end_evnt = ExecuteEventFunc(event_kind, event_flag);

	EventManager::GetInstance()->Update();

	sEventCount ++;
	if (phase != sEventPhase) {
		sEventCount = 0;
	}

	if (is_end_evnt) {

		// ���̃C�x���g�����݂���ꍇ�͑����đJ�ڂ���
		if (sNextEventNo != -1) {
			
			sTaskInfo.mTaskValueB = sNextEventNo;
			sNextEventNo = -1;

			mPhase = SceneMainGame::ePHASE_EVENT_BEFORE;
			return;
		}

		FADE_COL_BLACK();
		FADE_IN(0.3f);
		MSG_FADE_OUT(0.3f);

		// ��x���W�����ɖ߂�
		PauseReviseAllModel(false);

		// ��p��߂�
		CameraManager::GetInstance()->SetPerspectiveCamera(kDEFAULT_PERSPECTIVE_FOV);

		// �C�x���g�I�����ɕ\�������󔠂�����΁A�\�������
		for (int iI = 0; iI < sObjectBaseNum; iI ++) {
			if ((sppObjectBase[iI]->GetObjectType() == ObjectBase::eOBJECT_TYPE_TREASURE) ||
				(sppObjectBase[iI]->GetObjectType() == ObjectBase::eOBJECT_TYPE_SWITCH)) {
				ObjectTresure* p_treasure = (ObjectTresure *)sppObjectBase[iI];
				if (p_treasure->GetHideEventIndex() == sTaskInfo.mTaskValueB) {
					p_treasure->DeHideTreasure();
					p_treasure->SetPositionOnCollision();
				}
			}
		}

		/* NPC�͑S�ĕ\���ɂ����� */
		for (int iI = 0; iI < sNpcNum; iI ++) {
			spNpc[iI].SetVisible(true);
		}

		/* �X�e�[�W�N���A�ȊO�̃t�F�[�Y�̏ꍇ */
		if ((mPhase != SceneMainGame::ePHASE_STAGECLEAR) &&
			(mPhase != SceneMainGame::ePHASE_GAMEOVER_BEFORE)) {
			
			if (sStageIndex != 22) {	//<! Kitchen �ȊO�̓i�r�H�J�[�\����L���ɂ���
				// �i�r�H�J�[�\����L���ɂ���
				sNavi.EnableNaviCursor();
			}

			CameraManager::GetInstance()->SetPatternGameBegine();

			// ��l���̓��͋֎~������
			sPlayer.PermitAcceptInput(true);

			// �S�Ă̓G�̓�����L���ɂ���
			CharaEnemy::SetStopAllEnemyFlag(FALSE);

			if (event_flag_value != TotalInfo::GetInstance()->GetEventFlagValue(event_kind) ) {
				CameraManager::GetInstance()->SetPatternGameBegine();
			}

			GimmickBase* p_gimmick = NULL;
			p_gimmick = (GimmickBase *)GimmickBase::GetGimmickBaseFromEarthCollIndex(sPlayer.GetOnCollisionIndex());
			if ( (p_gimmick != NULL) && (p_gimmick->GetKind() == eGIMMICK_KIND_TROLLEY_EX) ) {
				// �g���b�R�ɏ���Ă���ꍇ�̂݁A��O�����ŃJ�����̋�����߂��Ȃ��B
				// ���g���b�R�ړ����̃C�x���g�p
				/* nothing to do. */
			}
			else {
				CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
			}

			mPhase = SceneMainGame::ePHASE_GAME;

		}
		// �C�x���g�t���O�𗧂Ă� �� ���ꂼ��̊֐��ŗ��Ă�悤�ɕύX * @data   2012/08/11 15:20:36
		//		TotalInfo::GetInstance()->SetEventFlag(event_kind, event_flag);
	}

	/*=====================================*
	 * �L�����␳�̏���
	 *=====================================*/
	/*
	float dist = VSize(sPlayer.Position());
#ifdef _DEBUG
	PRINT_SCREEN(GetColor(255, 255, 0), "dist : %.2f", dist);
#endif _DEBUG
	if (dist > ALL_MODEL_REVISE_DIST) {
		this->ReviseAllModel();
	}
	*/

	/*=====================================*
	 * �X�J�C�h�[���̏���
	 *=====================================*/
	sModelXfile[eMD_SKYDOME].Rotation().y += 0.00025f;
	sModelXfile[eMD_SKYDOME].Position().x = CameraManager::GetInstance()->Position().x;
	sModelXfile[eMD_SKYDOME].Position().z = CameraManager::GetInstance()->Position().z;

	/*=====================================*
	 * �X�e�[�W���̏���
	 *=====================================*/
	spStageFunc->Update((int)mPhaseCounter);


#ifdef _DEBUG
	debugPramInput();
#endif // _DEBUG

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseGetItemBefore(void)
{
	CameraManager::GetInstance()->SaveTemporaryCamWork();
	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);

	//	HIDE_BATTLE_ICON();
	//	TotalInfo::GetInstance()->HideBattleIcon();

	// �S�Ă̓G�̓������~�߂�
	CharaEnemy::SetStopAllEnemyFlag(TRUE);

	// �A�C�e���擾�̃T�u�V�[���N���X�̍쐬
	spSubSceneInst = new SubSceneGetItem();
	APP_ASSERT((spSubSceneInst != NULL),
			   "Failed new SubSceneGetItem class \n");
	spSubSceneInst->Create();

	mPhase = SceneMainGame::ePHASE_GET_ITEM;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseGetItem(void)
{
	if (spSubSceneInst->Update() == true) {
		spSubSceneInst->Destroy();
		delete spSubSceneInst;
		spSubSceneInst = NULL;

		mPhase = SceneMainGame::ePHASE_GAME;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseGameOverBefore(void)
{
	//	LayoutBase* p_layout = SceneManager::GetInstance()->GetInfoLayout();
	LayoutBase* p_layout = SceneManager::GetInstance()->GetInfoLayout();

	if (FADE_BOARD()->IsFade() == false) {

		//		GraphicManager::GetInstance()->EntryDrawLayout(&sLayout[eLY_GAMEOVER]);
		p_layout->LinkLayout("Info_GameOver.png");
		p_layout->SetPositionByCentering(320, 240);
		p_layout->SetSize(512, 128);
		p_layout->SetAlpha(0);
		p_layout->StartFade(255, 1.0f);
		
		mPhase = SceneMainGame::ePHASE_GAMEOVER;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseGameOver(void)
{
	//	LayoutBase* p_layout = &sLayout[TotalInfo::eLY_INFO];
	LayoutBase* p_layout = SceneManager::GetInstance()->GetInfoLayout();

	if (p_layout->IsFade() == false) {

		//		if (CHECK_TRIG_ANY_JOY()) {

		// �R���e�B�j���[���̎���
		GraphicManager::GetInstance()->RenderString(GraphicManager::eSTRING_TYPE_MESSAGE_1, 
													"Continure�H", 
													GetColor(255, 255, 255));
		Utility::eQuestion qs;
		qs = Utility::ExecuteQuestion();
		if (qs != Utility::eQUESTION_NONE) {

			p_layout->SetAlpha(0);
			// �o�g���C���f�b�N�X�t���O�͌��ɖ߂�
			sBattleIndex = -1;
			// �C�x���g�o�g���t���O�����ɖ߂�
			sEventBattleFlag = -1;
			// �C�x���g�t���O�̃L�����Z���\��ݒ�
			if (sEventRecoveryFlag != -1) {
				TotalInfo::GetInstance()->DisableEventFlag(BATTLEFLAG_TO_TASKVALUE_A(sEventRecoveryFlag),
														   BATTLEFLAG_TO_TASKVALUE_B(sEventRecoveryFlag));
			}

			// �I�u�W�F�N�g�g�p��Ԃ���I�u�W�F�N�g�t���O���X�V
			setObjectFlagFromObjectUsed();
		}
		if (qs == Utility::eQUESTION_YES) {
			// ���g���C

			sPlayer.SetHitpoint(sPlayer.GetMaxHitpoint());
			sPlayer.PermitAcceptInput(true);

			Destroy();
			mPhase = SceneMainGame::ePHASE_LOAD_BEFORE;

		} else if (qs == Utility::eQUESTION_NO) {

			TotalInfo::GetInstance()->HideBattleIcon();

			Destroy();

			SceneManager::GetInstance()->RequestChangeTask(eMENU_TITLE);
			this->EndScene();
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseStageclear(void)
{
	static const BYTE kLYT_STAGE_CLEAR_NUM = 5;

	static tLayoutSetting sStageClearLayoutTable[kLYT_STAGE_CLEAR_NUM] = {
		{RES_LYT_INFO_STAGE,		85, 360-320,	144, 72,		255,},
		{RES_LYT_INFO_1,			0xFF,0xFF,		72,	 72,		255,},
		{RES_LYT_INFO_MAP_1,		0xFF,0xFF,		288, 72,		255,},
		{RES_LYT_INFO_MAP_SUB_1,	510, 96,		384, 48,		255,},		// �\�����e�ɂ����X���������ŕύX���Ă���
		//		{RES_LYT_INFO_FRAME,		320, 232,		640, 144,		255,},
		{RES_LYT_INFO_FRAME,		320, 232,		640, 1,		255,},
	};

	PRINT_SCREEN(GetColor(255, 255, 0), "mPhaseCounter = %.2f (%.2f) \n", mPhaseCounter, (mPhaseCounter/60.0f));

	LayoutBase* p_layout = NULL;

	if (EQUAL_PHASE_SECOND( 1.0f )) {

		// ��ɃA�C�R����\���ݒ�
		HIDE_BATTLE_ICON();

		// ���C�A�E�g���쐬
		if (spStageClearLayoutList == NULL) {
			spStageClearLayoutList = new LayoutBase[kLYT_STAGE_CLEAR_NUM];
		}
		APP_ASSERT((spStageClearLayoutList != NULL),
				   "Failed new spStageClearLayoutList \n");

		SoundManager::GetInstance()->StopBGM();

		sPlayer.PermitAcceptInput(true);		// ��l���̓��͋֎~������
		sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_PUT_SWORD);

	}
	else if (EQUAL_PHASE_SECOND( 1.7f )) {

		VECTOR end_pos;
		end_pos = sPlayer.Position();
		end_pos.x += sin(sPlayer.Rotation().y) * -180.0f;
		end_pos.y += 100.0f;
		end_pos.z += cos(sPlayer.Rotation().y) * -180.0f;

		CameraManager::GetInstance()->StartMoveCamera(0.4f, 
													  &end_pos,
													  &CameraManager::GetInstance()->Target());


	}
	else if (EQUAL_PHASE_SECOND( 3.2f ) ) {

		VECTOR world, vec, cam_pos, cam_tar;
		const float mRate = 0.9f;

		cam_pos = CameraManager::GetInstance()->Position();
		cam_tar = CameraManager::GetInstance()->Target();
		vec = VSub(cam_tar, cam_pos);
		world = VAdd(cam_pos, VScale(vec, mRate) );
	
		EFK_HANDLE handle;
		handle = EffekseerManager::GetInstance()->Play(eEFK_STAGE_CLEAR,
													   &world,
													   50.0f);
		EffekseerManager::GetInstance()->SetRotationAxisY(handle, (atan2(vec.x, vec.z) ));
	}
	else if (EQUAL_PHASE_SECOND( 2.7f )) {

		//		FADE_BOARD()->StartFade(128, 0.6f);
		FADE_BOARD()->StartFade(64, 0.6f);
		SoundManager::GetSndComponent()->PlaySound(eSE_STAGE_CLEAR);

		//		p_layout = &sLayout[TotalInfo::eLY_INFO];
		p_layout = SceneManager::GetInstance()->GetInfoLayout();
		p_layout->LinkLayout(RES_LYT_INFO_STAGE_CLEAR);
		p_layout->SetPositionByCentering(320, 240);
		p_layout->SetSize(512, 1);
		p_layout->SetAlpha(0);
		p_layout->StartFade(255, 0.4f);

		float aspect_rate = GraphicManager::GetInstance()->GetAspectRate();
		const tLayoutSetting* p_setting = NULL;
		for (int iI = 0; iI < kLYT_STAGE_CLEAR_NUM; iI ++) {
			p_layout	= &spStageClearLayoutList[iI];
			p_setting	= &sStageClearLayoutTable[iI];

			p_layout->LinkLayout(p_setting->mStrLayoutName);
			p_layout->SetPositionByCentering(p_setting->mPosX, p_setting->mPosY);
			p_layout->SetSize((int)((float)p_setting->mSizeX / aspect_rate),
							  (int)((float)p_setting->mSizeY / aspect_rate) );
			p_layout->SetAlpha(p_setting->mAlpha);
			//			p_layout->StartFade(255, 0.4f);
			GraphicManager::GetInstance()->EntrySystemLayout(p_layout);
		}
		// �_���W�����ɉ����ă��C�A�E�g����ύX����
		spStageClearLayoutList[1].LinkLayout(sResourceLayoutNameTable[eRES_LYT_INFO_1			+ sDungeonIndex]);
		spStageClearLayoutList[2].LinkLayout(sResourceLayoutNameTable[eRES_LYT_INFO_MAP_1		+ sDungeonIndex]);
		spStageClearLayoutList[3].LinkLayout(sResourceLayoutNameTable[eRES_LYT_INFO_MAP_SUB_1	+ sDungeonIndex]);
		
		// �����񂪒������͈ʒu�����炷
		if ((sDungeonIndex == 1) ||	(sDungeonIndex == 3)) {
			spStageClearLayoutList[3].SetPositionByCentering(430, 96);
		}

		// �c��Q�͊e���C�A�E�g�̃T�C�Y�ɍ��킹��X�����v�Z���Ĕz�u����
		int pos_x1, pos_y1, size_x1;
		int pos_x2, pos_y2, size_x2;
		int size_y_tmp;
		spStageClearLayoutList[0].GetPositionByCentering(&pos_x1, &pos_y1);
		spStageClearLayoutList[0].GetSize(&size_x1, &size_y_tmp);
		size_x1 = (int)((float)size_x1 * aspect_rate);
		spStageClearLayoutList[1].GetSize(&size_x2, &size_y_tmp);
		size_x2 = (int)((float)size_x2 * aspect_rate);
		
		pos_x2 = pos_x1 + (size_x1 / 2) + 8;
		pos_y2 = pos_y1;
		spStageClearLayoutList[1].SetPositionByCentering(pos_x2, pos_y2);

		spStageClearLayoutList[2].GetSize(&size_x1, &size_y_tmp);
		size_x1 = (int)((float)size_x1 * aspect_rate);
		
		pos_x1 = pos_x2 + (size_x1 / 2) + 8;
		pos_y1 = pos_y2;
		spStageClearLayoutList[2].SetPositionByCentering(pos_x1, pos_y1);

	}
	else if (EQUAL_PHASE_SECOND(9.5f)) {
		FADE_IN(0.5f);
		//		p_layout = &sLayout[TotalInfo::eLY_INFO];
		p_layout = SceneManager::GetInstance()->GetInfoLayout();
		p_layout->StartFade(0, 0.5f);

		for (int iI = 0; iI < kLYT_STAGE_CLEAR_NUM; iI ++) {
			p_layout	= &spStageClearLayoutList[iI];
			p_layout->StartFade(0, 0.5f);
		}
	}
	else if (EQUAL_PHASE_SECOND(10.0f)) {
		
		for (int iI = 0; iI < kLYT_STAGE_CLEAR_NUM; iI ++) {
			p_layout	= &spStageClearLayoutList[iI];
			GraphicManager::GetInstance()->RemoveSystemLayout(p_layout);
		}

		if (spStageClearLayoutList != NULL) {
			delete[] spStageClearLayoutList;
			spStageClearLayoutList = NULL;
		}

		sPlayer.PermitAcceptInput(true);		// ��l���̓��͋֎~������
		sPlayer.ChangeState(CharaBase::eCHARA_STATE_IDLE);

		CameraManager::GetInstance()->SetPatternGameBegine();
		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
		mPhase = SceneMainGame::ePHASE_GAME;

		VISIBLE_BATTLE_ICON();

		if (sEventBattleFlag != -1) {
			ActionTaskManager::tTaskInfo param;
			param.mTaskType = ActionTaskManager::eTASK_EVENT;
			param.mTaskValueA = BATTLEFLAG_TO_TASKVALUE_A(sEventBattleFlag);
			param.mTaskValueB = BATTLEFLAG_TO_TASKVALUE_B(sEventBattleFlag);
			RequestChangeState(&param);
			sEventBattleFlag = -1;

		}
		else {
			mPhase = SceneMainGame::ePHASE_GAME;

			//<! for debug
			//			SceneBase::EndScene(); // �V�[�����I��������
		}
	}

	//	if (BETWEEN_PHASE_SECOND(2.7f, 9.5f)) {
	if (BETWEEN_PHASE_SECOND(3.7f, 9.5f)) {
		spStageClearLayoutList[4].AddSize(0, 4, 640, 144);
		
		p_layout = SceneManager::GetInstance()->GetInfoLayout();
		p_layout->AddSize(0, 4, 512, 128);
	}

#ifdef _DEBUG
	debugPramInput();
#endif // _DEBUG

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseShop(void)
{
	static Vec2 center = Vec2(320, 180);
	static const int kROT_COUNT = 30;

	LayoutBase* p_layout = NULL;
	float theta;
	char msg1[48] = {0};
	char msg2[48] = {0};

	static int sNowItemIndex = 0;
	static int sNextItemIndex = 0;
	static int sRotCnt = 0;
	static bool sIsRight = true;
	static float sIconDist = 0;


	int shop_index = sTaskInfo.mTaskValueA;

	const tShopInfo* p_info = NULL;
	p_info = GetShopInfoList(shop_index);
	APP_ASSERT((p_info != NULL),
			   "Failed GetShopInfoList()\n");

	const eItemKind*	p_shop		= NULL;
	int					layout_num	= 0;
	p_shop		= p_info->mpItemList;
	layout_num	= p_info->mItemNum;
	
	const tItemDetailInfo * p_item_detail = GetItemDetailInfo(p_shop[sNowItemIndex]);
	int item_money = p_item_detail->mPrice;

	TotalInfo* const p_total_info = TotalInfo::GetInstance();

	/**** ���������� ****/
	if (EQUAL_PHASE_SECOND(0.0f)) {

		if (spShopItemLayout == NULL) {
			spShopItemLayout = new LayoutBase[layout_num];
		}
		for (int iI = 0; iI < layout_num; iI ++) {
			const tItemDetailInfo * p_item_detail = GetItemDetailInfo(p_shop[iI]);
			p_layout = &spShopItemLayout[iI];

			ResourceLayout::GetInstance()->LoadLayoutResource(p_item_detail->mLayoutName);
			p_layout->LinkLayout(p_item_detail->mLayoutName);
			p_layout->SetSize(80, 80);
			p_layout->SetAlpha(255);
			GraphicManager::GetInstance()->EntryDrawLayout(p_layout);
		}

		MSG_TYPE_TELOP();
		MSG_FADE_IN(0.3f);
		sNextItemIndex = 0;
		sNowItemIndex = layout_num / 2;
		sRotCnt = 0;
		sIconDist = 300.0f;
		mPhaseCounter = SEC2FRAME(21.0f);
		SoundManager::GetSndComponent()->PlaySound(eSE_ITEM_APPEAR);

		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);

		center.x = (int)(320.0f / GraphicManager::GetInstance()->GetAspectRate());
		center.y = 180;

		return;
	}

#define SHOP_STATE_BUY_CHECK		(1.0f)
#define SHOP_STATE_BUY_FINISH		(3.0f)
#define SHOP_STATE_NO_MONEY			(7.0f)
#define SHOP_STATE_EXIST_ITEM		(10.0f)
#define SHOP_STATE_END				(13.0f)
#define SHOP_STATE_NO_EMPTY_BIN		(16.0f)
#define SHOP_STATE_SELECT_ITEM		(21.0f)

	/*=====================================*
	 * �w���m�F���
	 *=====================================*/
	if (BETWEEN_PHASE_SECOND(SHOP_STATE_BUY_CHECK, (SHOP_STATE_BUY_CHECK + 1))) {
		// �w���m�F���
		mPhaseCounter = SEC2FRAME(SHOP_STATE_BUY_CHECK);

		sprintf_s(msg1, sizeof(msg1), "%s�𔃂��܂����H", p_item_detail->mDispName);
		sprintf_s(msg2, sizeof(msg2), "�� %d�m���s�[", item_money);
		RENDER_MESSAGE_COL("", msg1, msg2, "", "", GetColor(0, 0, 0));

		eItemKind select_item = p_shop[sNowItemIndex];

		Utility::eQuestion qs;
		qs = Utility::ExecuteQuestion(false);
		if (qs == Utility::eQUESTION_YES) {
			if (( p_total_info->IsWeaponFlag( select_item ) ) &&
				(!(select_item == eITEM_KIND_BIN_MAGENTA)) &&
				(!(select_item == eITEM_KIND_BIN_CYAN)) &&
				(!(select_item == eITEM_KIND_BIN_YELLOW)) &&
				(!(select_item == eITEM_KIND_BIN_FAIRY))) {
				
				//<! ���ɍw���A�C�e���������Ă���ꍇ
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
				mPhaseCounter = SEC2FRAME(SHOP_STATE_EXIST_ITEM);
			}
			else if ( 
					 ((select_item == eITEM_KIND_BIN_MAGENTA)	||
					  (select_item == eITEM_KIND_BIN_CYAN)		||
					  (select_item == eITEM_KIND_BIN_YELLOW)	||
					  (select_item == eITEM_KIND_BIN_FAIRY) ) &&
					 (isEmptyBinContents() == FALSE)) {
				//<! ����w���������A�󂫕r�����݂��Ȃ��ꍇ
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
				mPhaseCounter = SEC2FRAME(SHOP_STATE_NO_EMPTY_BIN);
			}
			else {
				if (p_total_info->GetNowMoney() >= item_money) {
					p_total_info->AddMoney(-item_money);
					//<! �܂��ǂ̃X�e�[�^�X��
					updateGetItem( select_item );
					SoundManager::GetSndComponent()->PlaySound(eSE_DECIDE_KEY);
					mPhaseCounter = SEC2FRAME(SHOP_STATE_BUY_FINISH);
				} else {
					//<! ����������Ȃ��X�e�[�^�X��
					SoundManager::GetSndComponent()->PlaySound(eSE_NG);
					mPhaseCounter = SEC2FRAME(SHOP_STATE_NO_MONEY);
				}
			}

		}
		else if (qs == Utility::eQUESTION_NO) {
			//<! �L�����Z���I��
			SoundManager::GetSndComponent()->PlaySound(eSE_CANCEL);
			mPhaseCounter = SEC2FRAME(SHOP_STATE_SELECT_ITEM);
		}
		return;
	}

	/*=====================================*
	 * �܂��ǂ̃X�e�[�^�X
	 *=====================================*/
	if (BETWEEN_PHASE_SECOND(SHOP_STATE_BUY_FINISH, (SHOP_STATE_BUY_FINISH + 2.0f))) {
		/**** �����������̉� ****/
		sprintf_s(msg1, sizeof(msg1), "%s���Ăɂ��ꂽ�I", p_item_detail->mDispName);
		RENDER_MESSAGE_COL("", msg1, "", "", "", GetColor(0, 0, 0));
		return;
	}
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_BUY_FINISH + 2.0f), (SHOP_STATE_BUY_FINISH + 3.0f))) {
		// ��������A�b�v�f�[�g�ɃV�[�P���X��߂�
		mPhaseCounter = SEC2FRAME(SHOP_STATE_SELECT_ITEM);
		return;
	}

	/*=====================================*
	 * ����������Ȃ��ꍇ�̃V�[�P���X
	 *=====================================*/
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_NO_MONEY), (SHOP_STATE_NO_MONEY + 1.0f))) {
		/**** ���������s�̉� ****/
		RENDER_MESSAGE_COL("", "����������Ȃ��E�E�E���ƁE�E�E�I�H", "", "", "", GetColor(0, 0, 0));
		return;
	}
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_NO_MONEY + 1.0f), (SHOP_STATE_NO_MONEY + 2.0f))) {
		// ��������A�b�v�f�[�g�ɃV�[�P���X��߂�
		mPhaseCounter = SEC2FRAME(SHOP_STATE_SELECT_ITEM);
	}

	/*=====================================*
	 * ���Ɏ�����������΂���
	 *=====================================*/
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_EXIST_ITEM), (SHOP_STATE_EXIST_ITEM + 1.0f))) {
		/**** ���������s�̉� ****/
		RENDER_MESSAGE_COL("", "���̃A�C�e���͂������ĂȂ���", "", "", "", GetColor(0, 0, 0));
		return;
	}
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_EXIST_ITEM + 1.0f), (SHOP_STATE_EXIST_ITEM + 2.0f))) {
		// ��������A�b�v�f�[�g�ɃV�[�P���X��߂�
		mPhaseCounter = SEC2FRAME(SHOP_STATE_SELECT_ITEM);
	}

	/*=====================================*
	 * �󂫃r���������ꍇ
	 *=====================================*/
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_NO_EMPTY_BIN), (SHOP_STATE_NO_EMPTY_BIN + 1.0f))) {
		/**** ���������s�̉� ****/
		RENDER_MESSAGE_COL("", "�󂫃r�����Ȃ��Ƃ���͎��ĂȂ���", "", "", "", GetColor(0, 0, 0));
		return;
	}
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_NO_EMPTY_BIN + 1.0f), (SHOP_STATE_NO_EMPTY_BIN + 2.0f))) {
		// ��������A�b�v�f�[�g�ɃV�[�P���X��߂�
		mPhaseCounter = SEC2FRAME(SHOP_STATE_SELECT_ITEM);
	}
	
	/*=====================================*
	 * �A�C�e�����I���̃V�[�P���X
	 *=====================================*/
	if (BETWEEN_PHASE_SECOND((SHOP_STATE_END), (SHOP_STATE_END + 1.0f))) {
		for (int iI = 0; iI < layout_num; iI ++) {
			GraphicManager::GetInstance()->RemoveDrawLayout(&spShopItemLayout[iI]);
		}
		if (spShopItemLayout != NULL) {
			delete[] spShopItemLayout;
			spShopItemLayout = NULL;
		}
		MSG_FADE_OUT(0.3f);

		// ��l���̓��͋֎~������
		sPlayer.PermitAcceptInput(true);

		// �S�Ă̓G�̓�����L���ɂ���
		CharaEnemy::SetStopAllEnemyFlag(FALSE);

		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
		mPhase = SceneMainGame::ePHASE_GAME;
		return;
	}


	/**** �A�b�v�f�[�g���� ****/
	/**** ��������͑I�����Ԃ̏��� ****/
	sIconDist -= 20.0f;
	if (sIconDist < 100.0f) {
		sIconDist = 100.0f;
	}

	if (sRotCnt == 0) {
		if (CHECK_TRIG_JOY(JOY_INPUT_LEFT)) {
			sIsRight = false;
			sNextItemIndex = SELECT_ROTATION_ADD(sNextItemIndex, layout_num);
			SoundManager::GetSndComponent()->PlaySound(eSE_SELECT_KEY);
		}
		if (CHECK_TRIG_JOY(JOY_INPUT_RIGHT)) {
			sIsRight = true;
			sNextItemIndex = SELECT_ROTATION_SUB(sNextItemIndex, layout_num);
			SoundManager::GetSndComponent()->PlaySound(eSE_SELECT_KEY);
		}
	}

	float div_rad = Utility::DegToRad(360.0f / (float)layout_num);
	if (sNowItemIndex == sNextItemIndex) {
		for (int iI = 0; iI < layout_num; iI ++) {
			p_layout = &spShopItemLayout[iI];
			theta = (iI - sNowItemIndex) * div_rad;
			p_layout->SetPosition(center.x - (int)(sin(theta) * sIconDist * 1.5f),
								  center.y - (int)(cos(theta) * sIconDist));
		}
		sRotCnt = 0;
	} else {
		for (int iI = 0; iI < layout_num; iI ++) {
			p_layout = &spShopItemLayout[iI];

			float now_theta, next_theta;
			now_theta = (iI - sNowItemIndex) * div_rad;
			next_theta = (iI - sNextItemIndex) * div_rad;
			float offset_theta = next_theta - now_theta;
			if ((sIsRight == false) && (offset_theta > 0)) {
				offset_theta += -Utility::DegToRad(360.0f);
			}
			if ((sIsRight == true) && (offset_theta < 0)) {
				offset_theta += Utility::DegToRad(360.0f);
			}
			theta = now_theta + (offset_theta / kROT_COUNT) * sRotCnt;
			p_layout->SetPosition(center.x - (int)(sin(theta) * sIconDist * 1.5f),
								  center.y - (int)(cos(theta) * sIconDist));
			sRotCnt ++;
			if (sRotCnt >= kROT_COUNT) {
				sNowItemIndex = sNextItemIndex;
			}
		}
	}

	sprintf_s(msg1, sizeof(msg1), "%s �F %d�m���s�[", p_item_detail->mDispName, item_money);
	RENDER_MESSAGE_COL("", msg1, 
					   p_item_detail->mExplain0,
					   p_item_detail->mExplain1,
					   p_item_detail->mExplain2,
					   GetColor(0, 0, 0));

	if ((CHECK_TRIG_JOY(JOY_INPUT_ATTACK)) ||
		(CHECK_TRIG_JOY(JOY_INPUT_AWAY))) {
			
		// �w���m�F��ʂ�
		SoundManager::GetSndComponent()->PlaySound(eSE_PRESS_KEY);
		mPhaseCounter = SEC2FRAME(SHOP_STATE_BUY_CHECK);
	}

	/**** �I������ ****/
	if (CHECK_TRIG_JOY(JOY_INPUT_JUMP)) {
		mPhaseCounter = SEC2FRAME(SHOP_STATE_END);
		SoundManager::GetSndComponent()->PlaySound(eSE_CANCEL);
		
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseSelectWeapon(void)
{
	if (spSubSceneInst->Update() == true) {
		((SubSceneItemMenu *)spSubSceneInst)->SetDefMenuKind(sItemMenuDefault);
		spSubSceneInst->Destroy();
		// <! �^�C�~���O�̓s����A�����ł� delete ���Ȃ��I

		mPhase = SceneMainGame::ePHASE_SELECT_WEAPON_END;
	}

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseSelectWeaponEnd(void)
{
	if (MSG_IS_FADE() == false) {
		sPlayer.PermitAcceptInput(true);		// ��l���̓��͋֎~������

		TotalInfo::GetInstance()->SetActiveLockonEnemyName(TRUE);

		eItemKind selected_weapon = ((SubSceneItemMenu *)spSubSceneInst)->GetSelectWeapon();
		switch (selected_weapon) {
		case eITEM_KIND_PORTION:
			if (sPlayer.CosplayIndex() == 0) {
				sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_GUTS_POSE);
				sPlayer.SetUsingRecoverItemKind(selected_weapon);
				deleteMyWeapon(selected_weapon);
			}else {	SoundManager::GetSndComponent()->PlaySound(eSE_NG);	}
			break;
			
		case eITEM_KIND_BIN_MAGENTA:
			if (sPlayer.CosplayIndex() == 0) {
				sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_GUTS_POSE);
				sPlayer.SetUsingRecoverItemKind(selected_weapon);
				useBinContents(eITEM_KIND_BIN_MAGENTA);
			}else {	SoundManager::GetSndComponent()->PlaySound(eSE_NG);	}
			break;

		case eITEM_KIND_BIN_CYAN:
			if (sPlayer.CosplayIndex() == 0) {
				sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_GUTS_POSE);
				sPlayer.SetUsingRecoverItemKind(selected_weapon);
				useBinContents(eITEM_KIND_BIN_CYAN);
			}else {	SoundManager::GetSndComponent()->PlaySound(eSE_NG);	}
			break;

		case eITEM_KIND_BIN_YELLOW:
			if (sPlayer.CosplayIndex() == 0) {
				sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_GUTS_POSE);
				sPlayer.SetUsingRecoverItemKind(selected_weapon);
				useBinContents(eITEM_KIND_BIN_YELLOW);
			}else {	SoundManager::GetSndComponent()->PlaySound(eSE_NG);	}
			break;


		case eITEM_KIND_MAGIC_THUNDER:
			if (sPlayer.CosplayIndex() != 0) {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
			}
			else if (sPlayer.CheckEnableMagic(eITEM_KIND_MAGIC_THUNDER) == FALSE) {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
				SceneMainGame::StartNaviTalk(strNaviTalk_NoMagicPoint);
			}
			else {
				sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_MAGIC_THUNDER);
			}
			break;

		case eITEM_KIND_MAGIC_FLAME:
			if (sPlayer.CosplayIndex() != 0) {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
			}
			else if (sPlayer.CheckEnableMagic(eITEM_KIND_MAGIC_FLAME) == FALSE) {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
				SceneMainGame::StartNaviTalk(strNaviTalk_NoMagicPoint);
			}
			else {
				sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_MAGIC_FLAME);
			}
			break;

		case eITEM_KIND_MAGIC_TORNADE:
			if (sPlayer.CosplayIndex() != 0) {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
			}
			else if (sPlayer.CheckEnableMagic(eITEM_KIND_MAGIC_TORNADE) == FALSE) {
				SoundManager::GetSndComponent()->PlaySound(eSE_NG);
				SceneMainGame::StartNaviTalk(strNaviTalk_NoMagicPoint);
			}
			else {
				sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_MAGIC_TORNADE);
			}
			break;
			
		default:
			break;
		};

		delete spSubSceneInst;
		spSubSceneInst = NULL;

		MSG_FADE_OUT(0.2f);

		// �J������߂�
		if (sPlayer.GetLockonChara() == NULL) {
			CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
		}
		else {
			CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_LOCKON);
		}

		mPhase = SceneMainGame::ePHASE_GAME;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseStartBattle(void)
{
	int model_handle = sModelMap.GetModelHandle();
	int mesh_num = MV1GetFrameMeshNum(model_handle, eBG_TYPE_TASK);

	int material_idx;
	int mesh_index = 0;
	for (int iI = 0; iI < mesh_num; iI ++) {
		mesh_index = MV1GetFrameMesh(model_handle, eBG_TYPE_TASK, iI);
		material_idx = MV1GetMeshMaterial(model_handle, mesh_index);

		// �}�e���A�������擾����
		const char* mat_name = MV1GetMaterialName(model_handle, material_idx);
		// �wENM_XXX_YYY�x
		// �����K���Ƃ��āA �wPlaceXXX_YYY�x �Ƃ��� (XXX �͏ꏊ�C���f�b�N�X YYY �͊p�x)
		// ���O�̒����� 12 �����͂͂���
		if (strlen(mat_name) < strlen("ENM_XXX_YYY")) {
			continue;
		}
		char cmd_name[3+1];
		memcpy(&cmd_name[0], &mat_name[0], sizeof(cmd_name));
		cmd_name[3] = '\0';
		if (strcmp(cmd_name, "ENM") != 0) {
			continue;
		}
			
		char place_label[3+1];
		int place_index;
		memcpy(&place_label[0], &mat_name[4], sizeof(place_label));
		place_label[3] = '\0';
		place_index = atoi(place_label);

		if (place_index == sTaskInfo.mTaskValueA) {

			VECTOR min_pos, max_pos;
			VECTOR vec;
			VECTOR world = MV1GetFramePosition(model_handle, eBG_TYPE_TASK);
			min_pos = MV1GetMeshMinPosition(model_handle, mesh_index);
			max_pos = MV1GetMeshMaxPosition(model_handle, mesh_index);
			min_pos = VScale(min_pos, spBGInfo->mScale);
			max_pos = VScale(max_pos, spBGInfo->mScale);

			vec = VSub(max_pos, min_pos);
			vec = VScale(vec, 0.1f);

			eEnemyKind enemy_kind;

			for (int iI = 0; iI < kENEMY_ONCE_MAX; iI ++) {
				const tEnemyTableInfo* p_work;
				p_work = GetEnemyTableInfo(spBGInfo->mEnemyTblIdx);
				enemy_kind = *(p_work->mpEnemyKind + (place_index * kENEMY_ONCE_MAX) + iI);

				if (enemy_kind == (eEnemyKind)-1) {
					continue;
				}

				static const float sAppearSignLength = 12000.0f;
	
				for (int retryCnt = 0; retryCnt < 5000; retryCnt ++) {
				//				for (;;) {
					int rndx, rndz;
					rndx = GetRand(10-2) + 1;
					rndz = GetRand(10-2) + 1;

					VECTOR pos;
					pos.x = min_pos.x + (vec.x * rndx);
					//					pos.y = (min_pos.y + max_pos.y) / 2.0f;
					pos.y = max_pos.y;
					pos.z = min_pos.z + (vec.z * rndz);

					pos = VAdd(world, pos);
					pos = VAdd(pos, VGet((float)iI, (float)iI, (float)iI));
					float dist = VSize(VSub(sPlayer.Position(), pos));
#ifdef _DEBUG
					//					PRINT_CONSOLE("dist : %.2f \n", dist);
#endif // _DEBUG
					if (dist < sAppearSignLength) {
						createEnemyWork2(iI, enemy_kind, &pos);
						break;
					}

				}

			}

			break;
		}
	}

	// �o�g���p�̕ǂ���
	//	CollisionManager::GetInstance()->EnableBattleBox();
	int is_battle_box = (sTaskInfo.mTaskValueB % 10);
	if (is_battle_box == 1) { // 1 = �o�g���{�b�N�X�ǂ���
		CollisionManager::GetInstance()->EntryWallCollision(sModelMap.GetModelHandle(), eBG_TYPE_BATTLE_WALL);
		sModelMap.VisibleBattleBox();
	}

	int after_event_index = (sTaskInfo.mTaskValueB / 10);
	if (after_event_index != 0) {
		int event_index = ConvertPlaceIndex_TO_EventDefine(sStageIndex);
		APP_ASSERT_PRINT((event_index != eEVENT_ERROR),
						"Select event_index %d is invalid value\n", event_index);
		sEventBattleFlag = TASKVALUE_TO_BATTLEFLAG(event_index, after_event_index);
	}

	MSG_FADE_OUT(0.2f);

	//	sPlayer.PermitAcceptInput(true);		// ��l���̓��͋֎~������
	mPhase = SceneMainGame::ePHASE_GAME;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseNaviTalk(void)
{
	

	static int sTalkCount = 0;
	if (MSG_IS_FADE() == false) {
		RENDER_MESSAGE("�i�r�H", sStrNaviTalkPtr, "", "", "");
		sTalkCount ++;
		if (sTalkCount > 180) {
			SoundManager::GetSndComponent()->PlaySound(eSE_MESSAGE_OUT);

			TotalInfo::GetInstance()->SetActiveLockonEnemyName(TRUE);

			MSG_FADE_OUT(0.2f);
			// �L�����N�^�[�̃A�b�v�f�[�g���ĊJ����
			sStrNaviTalkPtr = NULL;
		}
	}
	else {
		TotalInfo::GetInstance()->SetActiveLockonEnemyName(FALSE);

		sTalkCount = 0;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseNaviTalkEnd(void)
{
	if (MSG_IS_FADE() == false) {
		sPlayer.PermitAcceptInput(true);		// ��l���̓��͋֎~������
		mPhase = SceneMainGame::ePHASE_GAME;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseSave(void)
{
	if (EQUAL_PHASE_SECOND(0.0f)) {
		MSG_FADE_IN(0.2f);
		mPhaseCounter = SEC2FRAME(10.0f);
	}

	if (BETWEEN_PHASE_SECOND(5.0f, 7.0f)) {
		RENDER_MESSAGE_COL("", "�Z�[�u���������܂���", "", "", "", GetColor(0, 0, 0));
	}
	else if (BETWEEN_PHASE_SECOND(7.0f, 8.0f)) {

		// ��l���̓��͋֎~������
		sPlayer.PermitAcceptInput(true);

		// �S�Ă̓G�̓�����L���ɂ���
		CharaEnemy::SetStopAllEnemyFlag(FALSE);

		CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
		mPhase = SceneMainGame::ePHASE_GAME;
		
		MSG_FADE_OUT(0.2f);
		
	}
	else {

		mPhaseCounter = SEC2FRAME(10.0f);	// ��Ԃ�J�ڂ����Ȃ��悤��
		GraphicManager::GetInstance()->RenderString(GraphicManager::eSTRING_TYPE_MESSAGE_0, 
													"�Z�[�u���܂����H", 
													GetColor(0, 0, 0));
		Utility::eQuestion qs;
		qs = Utility::ExecuteQuestion();

		if (qs == Utility::eQUESTION_YES) {
			// �I�u�W�F�N�g�g�p��Ԃ���I�u�W�F�N�g�t���O���X�V
			setObjectFlagFromObjectUsed();

			BYTE sub_place_index = 0;
			// ��ԋ߂����W���擾����
			{
				float nearest = 99999.0f;
				VECTOR pos; float rot;
				VECTOR ch_pos = sPlayer.Position();
				float dist = 0.0f;
				int cnt = 0;
				for (;;) {
					if (getPlayerPosRotFromBGModel(&pos, &rot, cnt)) {
						dist = VSize(VSub(ch_pos, pos));
						if (nearest > dist) {
							nearest = dist;
							sub_place_index = cnt;
						}
						cnt ++;
					}
					else {
						break;
					}
				}
			}

			// �Z�[�u
			TotalInfo::GetInstance()->SaveData(-1,
											   sStageIndex, 
											   sPlayer.GetMaxHitpoint(), 
											   sPlayer.GetHitpoint(),
											   sPlayer.ItemWork().mItemSubWeapon,
											   sPlayer.CosplayIndex(),
											   sub_place_index,
											   //											   nearest_npc_index,
											   sPlayer.NowMP() );
			
			mPhaseCounter = SEC2FRAME(5.0f);

		} else if (qs == Utility::eQUESTION_NO) {
			sPlayer.PermitAcceptInput(true);		// ��l���̓��͋֎~������

			// �S�Ă̓G�̓�����L���ɂ���
			CharaEnemy::SetStopAllEnemyFlag(FALSE);

			CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
			mPhase = SceneMainGame::ePHASE_GAME;
			MSG_FADE_OUT(0.2f);
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseMovePlace(void)
{
	if (IS_FADE() == false) {

		// �ړ����̃t�F�[�h�A�E�g���ɌĂ΂��n���h��
		spStageFunc->NotifyMovePlaceWhileFadeout();

		VECTOR pos;
		float rot;
		getPlayerPosRotFromBGModel(&pos, &rot, sStagePlaceIndex);
		sPlayer.Position() = pos;
		sPlayer.Rotation().y = rot;
		FADE_IN(0.5f);

		CameraManager::GetInstance()->SetPatternGameBegine();

		sPlayer.PermitAcceptInput(true);

		mPhase = SceneMainGame::ePHASE_GAME;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseFlyingMove(void)
{
	if (spSubSceneInst->Update() == true) {
		spSubSceneInst->Destroy();
		delete spSubSceneInst;
		spSubSceneInst = NULL;

		mPhase = SceneMainGame::ePHASE_GAME;
	}
	
#ifdef _DEBUG
	debugPramInput();
#endif // _DEBUG
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phasePlayOcarina(void)
{
	if (spSubSceneInst->Update() == true) {

		UCHAR next_state = ((SubScenePlayOcarina *)spSubSceneInst)->GetNextState();
		sOcarinaSongSelect = ((SubScenePlayOcarina *)spSubSceneInst)->GetOcarinaSongSelect();

		spSubSceneInst->Destroy();
		delete spSubSceneInst;
		spSubSceneInst = NULL;

		switch (next_state) {

		case SubScenePlayOcarina::eNEXT_STATE_GAME:
			mPhase = SceneMainGame::ePHASE_GAME;
			break;

		case SubScenePlayOcarina::eNEXT_STATE_WAKEUP_EVENT:
			{
				BOOL is_event = FALSE;

				// �����̐X�ɂ�
				if (sStageIndex == 34) {
					ActionTaskManager::tTaskInfo info = {
						ActionTaskManager::eTASK_EVENT, eEVENT_34, eEVENT_34_PLAYED_SONG_OF_SUN};
					ActionTaskManager::GetInstance()->NotifyAction(&info);
					is_event = TRUE;
				}
				// �`�X�ɂ�
				else if ((sStageIndex == 11) &&
						 (IS_END_EVT(eEVENT_11, eEVENT_11_WAKEUP_TAKORUKA) == FALSE)) {
					CharaNpc* p_tako = NULL;
					for (int iI = 0; iI < sNpcNum; iI ++) {
						if (strcmp(CharaNpc::GetNpcResourceName(iI), NAME_Takoruka) == 0) {
							p_tako = &spNpc[iI];
							break;
						}
					}
					APP_ASSERT((p_tako != NULL),
							   "Not Found PoppoStone Npc Model \n");
					
					// �^�R�邩������X�e�[�W�ł��A�������߂��ꍇ
					if ((p_tako != NULL) &&
						(VSize(VSub(sPlayer.Position(), p_tako->Position())) < 10000.0f)) {
						ActionTaskManager::tTaskInfo info = {
							ActionTaskManager::eTASK_EVENT, eEVENT_11, eEVENT_11_WAKEUP_TAKORUKA};
						ActionTaskManager::GetInstance()->NotifyAction(&info);
						is_event = TRUE;
					}
				}

				if (is_event == FALSE) {
					// ����������Ȃ��P�[�X
					StartNaviTalk(strNaviTalk_NothingToDoSongSun);
					SoundManager::GetInstance()->SetVolumeBGM(kSOUND_VOLUME_DEFAULT);
					mPhase = SceneMainGame::ePHASE_GAME;
				}
			}
			break;

		case SubScenePlayOcarina::eNEXT_STATE_APPEAR_TITANIS:
			{
				sPlayer.StartMotionEx(&sMotionParam[eMT_IDLE_NOWEAPON]);
				sPlayer.ChangeAttachPattern(CharaPlayer::eATTACH_PATTERN_NORMAL);
				ActionTaskManager::tTaskInfo info = {
					ActionTaskManager::eTASK_EVENT, eEVENT_10, eEVENT_10_RIDE_VEHICLE_LONG};
				ActionTaskManager::GetInstance()->NotifyAction(&info);
			}
			break;

		case SubScenePlayOcarina::eNEXT_STATE_FLYING_MOVE:
			{
				// �ۂ��ە����C�x���g���I���Ă���ꍇ
				if (IS_END_EVT(eEVENT_29, eEVENT_29_REVIVAL_POPPO) == TRUE) {
					// �ۂ��۔��݂����҂ł���ꍇ
					if (SubSceneFlyingMove::CheckFlyingMoveEnable(sStageIndex)) {
						ActionTaskManager::tTaskInfo info = {
							ActionTaskManager::eTASK_FLYING_MOVE, 0, 0};
						ActionTaskManager::GetInstance()->NotifyAction(&info);
					}
					// �ۂ��۔��݂����҂ł��Ȃ��ꍇ
					else {
						StartNaviTalk(strNaviTalk_CannotFlyingMove);
						SoundManager::GetInstance()->SetVolumeBGM(kSOUND_VOLUME_DEFAULT);
						mPhase = SceneMainGame::ePHASE_GAME;
					}
				}
				// �ۂ��ە����C�x���g���I���Ă��Ȃ��ꍇ
				else {
					CharaNpc* p_poppo = NULL;

					// ���x���j�A��ɂ�
					if (sStageIndex == 29) {
						for (int iI = 0; iI < sNpcNum; iI ++) {
							if (strcmp(CharaNpc::GetNpcResourceName(iI), NAME_PoppoStone) == 0) {
								p_poppo = &spNpc[iI];
								break;
							}
						}
						APP_ASSERT((p_poppo != NULL),
								   "Not Found PoppoStone Npc Model \n");
					}

					// �ۂ��۔��݂�����X�e�[�W�ł��A�������߂��ꍇ
					if ((p_poppo != NULL) &&
						(VSize(VSub(sPlayer.Position(), p_poppo->Position())) < 5000.0f)) {
						ActionTaskManager::tTaskInfo info = {
							ActionTaskManager::eTASK_EVENT, eEVENT_29, eEVENT_29_REVIVAL_POPPO};
						ActionTaskManager::GetInstance()->NotifyAction(&info);
					}
					else {
						// ����������Ȃ��P�[�X
						StartNaviTalk(strNaviTalk_NothingToDoSongSun);
						SoundManager::GetInstance()->SetVolumeBGM(kSOUND_VOLUME_DEFAULT);
						mPhase = SceneMainGame::ePHASE_GAME;
					}
				}


			}
			break;

		case SubScenePlayOcarina::eNEXT_STATE_HANDLING_MAGICARMER:
			{
				CharaGuardian* p_guardin = spStageFunc->GetGuardianPtr();
				// �}�W�b�N�A�[�}�[������ꍇ
				if (p_guardin != NULL) {
					ActionTaskManager::tTaskInfo info = {
						ActionTaskManager::eTASK_EVENT, eEVENT_52, eEVENT_52_BOOTUP_MAGIC_ARMER };
					ActionTaskManager::GetInstance()->NotifyAction(&info);
				}
				// �}�W�b�N�A�[�}�[�����Ȃ��ꍇ
				else {
					StartNaviTalk(strNaviTalk_NotFoundMagicArmer);
					SoundManager::GetInstance()->SetVolumeBGM(kSOUND_VOLUME_DEFAULT);
					mPhase = SceneMainGame::ePHASE_GAME;
				}
			}
			break;

		default:
			APP_ASSERT_PRINT((NULL),
							 "Invalid next_state \n");
			break;
		};

		if (mPhase == SceneMainGame::ePHASE_GAME) {
			
			sPlayer.PermitAcceptInput(true);
			CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_NORMAL);
			sPlayer.ChangeAttachPattern(CharaPlayer::eATTACH_PATTERN_NORMAL);
			sPlayer.ChangeState(CharaPlayer::eCHARA_STATE_AFTER_EVENT);

		}

	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneMainGame::phaseTestConnect(void)
{
#ifdef _DEBUG
	// GameTestTool �Ƃ̘A�g���s���֐�
	updateGameTestTool();
#endif // _DEBUG
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
#ifdef _DEBUG
void
SceneMainGame::debugPramInput(void)
{
	int set_phase = -1;

	static unsigned char sWaitCnt = 0;

	/* 60�b�ԂɂP��CSV�t�@�C�����X�V����Ă��邩���m�F���A
	   �X�V����Ă����CSV���������{���� */
	if ((sWaitCnt % 6) == 0) {
	//	if (true) {

		// GameTestTool �Ƃ̘A�g���s���֐�
		updateGameTestTool();

	}
	sWaitCnt ++;

}
#endif // _DEBUG


/**** end of file ****/


