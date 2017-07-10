/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ItemDef.h
 * �A�C�e���Ǘ��w�b�_�[
 * @author SPATZ.
 * @data   2012/04/04 14:48:17
 */
//---------------------------------------------------
#ifndef __ITEM_DEF_H__
#define __ITEM_DEF_H__

/*=====================================*
 * item kind
 *=====================================*/
enum eItemKind {
	/* �ȉ��̒�`��CSV�ƘA�����Ă��邽�߁A���Ԃ͕ς��Ȃ��ق�������*/
	eITEM_KIND_NONE = -1,

	eITEM_KIND_MASTERSWORD = 0,
	eITEM_KIND_LONGHOOK,
	eITEM_KIND_MAP,
	eITEM_KIND_MONEY_5,
	eITEM_KIND_HEART,
	// 5
	eITEM_KIND_PIKOHUM,
	eITEM_KIND_NOWEAPON,		// �����Ȃ�
	eITEM_KIND_BOOMERANG,
	eITEM_KIND_HEART_CASE,		// �n�[�g�̊�
	eITEM_KIND_BOSSKEY,			// �{�X�̌�
	// 10
	eITEM_KIND_PORTION,			// �|�[�V����
	eITEM_KIND_MONEY_10,
	eITEM_KIND_MONEY_50,
	eITEM_KIND_MONEY_100,
	eITEM_KIND_MONEY_200,
	// 15
	eITEM_KIND_MONEY_400,
	eITEM_KIND_SMILE_0,
	eITEM_KIND_SMILE_1,
	eITEM_KIND_SMILE_2,
	eITEM_KIND_SMILE_3,
	// 20
	eITEM_KIND_SMILE_4,
	eITEM_KIND_SMILE_5,
	eITEM_KIND_DANCE,
	eITEM_KIND_LIB_SHIELD,
	eITEM_KIND_CARD_K,
	// 25
	eITEM_KIND_CARD_Q,
	eITEM_KIND_CARD_J,
	eITEM_KIND_CRYSTAL_Y,	//eITEM_KIND_GIZERL,			// �U�U�[���̖��
	eITEM_KIND_BOMB,			// ���e
	eITEM_KIND_CRYSTAL_G,	
	// 30
	eITEM_KIND_ARCHERY,			// �|��
	eITEM_KIND_ANGEL_WING,		// �V�g�̗�
	// 32 bit�ȏ㑝�₷�ꍇ�͒��ӂ��邱��
	eITEM_KIND_COSPLAY_PRONAMA,	// �R�X�v���P�D
	eITEM_KIND_MONEY_1,
	eITEM_KIND_MONEY_20,

	// 35
	eITEM_KIND_PLATE_ARROW,
	eITEM_KIND_PLATE_BOMB,
	eITEM_KIND_BIN_EMPTY,
	eITEM_KIND_BIN_MAGENTA,
	eITEM_KIND_BIN_CYAN,
	// 40
	eITEM_KIND_BIN_YELLOW,
	eITEM_KIND_BIN_FAIRY,
	eITEM_KIND_COSPLAY_DABAH,	
	eITEM_KIND_WALLET_A,
	eITEM_KIND_WALLET_B,
	// 45
	eITEM_KIND_WALLET_C,
	eITEM_KIND_SEAL_KEY,
	eITEM_KIND_COSPLAY_TDA_MIKU,
	eITEM_KIND_COSPLAY_LAPAN,	
	eITEM_KIND_COSPLAY_FRES,
	// 50
	eITEM_KIND_COSPLAY_MASK,
	eITEM_KIND_MAGIC_THUNDER,
	eITEM_KIND_MAGIC_FLAME,
	eITEM_KIND_MAGIC_TORNADE,
	eITEM_KIND_OCARINA,
	// 55
	eITEM_KIND_COPY_ROD,
	eITEM_KIND_COSPLAY_SUNOPHONE,
	eITEM_KIND_COSPLAY_BAT,
	eITEM_KIND_COSPLAY_CHILNO,
	eITEM_KIND_COSPLAY_HONGKONG,
	// 60
	eITEM_KIND_COSPLAY_LOANA,
	eITEM_KIND_COSPLAY_YUKARI,
	eITEM_KIND_COSPLAY_NAKANO,
	eITEM_KIND_HEART_PIECE,		// �n�[�g�̂�����
	eITEM_KIND_TROPHY,
	// 65
	eITEM_KIND_NAVI_LIGHT,
	eITEM_KIND_BOOK,
	eITEM_KIND_FLAME_RING,		// ���̎w��
	eITEM_KIND_CHICK_PIERCE,	// �q���R�̃s�A�X
	eITEM_KIND_FAIRY_BRACELET,	// �d���̘r��
	// 70

	eITEM_KIND_MAX,
};

/*=====================================*
 * �I�u�W�F�N�g��ނ̒�`
 *=====================================*/
enum {
	eOBJECT_KIND_TRESURE = 0,		// 0
	eOBJECT_KIND_MOGU_SWITCH,		// 1
	eOBJECT_KIND_FACE_SWITCH,		// 2
	eOBJECT_KIND_HEART,				// 3
	eOBJECT_KIND_MONEY_1,			// 4
	eOBJECT_KIND_SMALL_TRESURE,		// 5
	eOBJECT_KIND_HEART_CASE,		// 6
	eOBJECT_KIND_JAR_TYPE_A,		// 7
	eOBJECT_KIND_BOMB,				// 8 
	eOBJECT_KIND_LOCK,				// 9 
	eOBJECT_KIND_STONE,				// 10
	eOBJECT_KIND_SUNOHARA,			// 11
	eOBJECT_KIND_SEESAW,			// 12
	eOBJECT_KIND_USACHU,			// 13
	eOBJECT_KIND_CRYSTAL,			// 14
	eOBJECT_KIND_MONEY_5,			// 15
	eOBJECT_KIND_MONEY_10,			// 16
	eOBJECT_KIND_MONEY_20,			// 17
	eOBJECT_KIND_MONEY_50,			// 18
	eOBJECT_KIND_ARROW,				// 19
	eOBJECT_KIND_MONEY_100,			// 20
	eOBJECT_KIND_MONEY_200,			// 21
	eOBJECT_KIND_METEO,				// 22
	eOBJECT_KIND_METEO_REFRECT,		// 23
	eOBJECT_KIND_MONEY_400,			// 24
	//	eOBJECT_KIND_DUMMY_25,	// eOBJECT_KIND_TROLLEY,			// 25	(���g�p)
	eOBJECT_KIND_DUMMY_OBJ,		// 25
	eOBJECT_KIND_DUMMY_26,	// eOBJECT_KIND_ELEVATOR,			// 26	(���g�p)
	eOBJECT_KIND_DUMMY_27,	// eOBJECT_KIND_DANCER,				// 27	(���g�p)
	eOBJECT_KIND_FACE_SWITCH_BIG,	// 28
	eOBJECT_KIND_CRYSTAL_SWITCH,	// 29
	eOBJECT_KIND_MISSILE,			// 30
	eOBJECT_KIND_GETDOWN,			// 31
	// 32 bit �ȏ㑝�₷�ꍇ�͒��ӂ��邱�ƁI

	eOBJECT_KIND_CHECK_POINT,		// 32
	eOBJECT_KIND_PLATE_ARROW,		// 33
	eOBJECT_KIND_PLATE_BOMB,		// 34
	eOBJECT_KIND_CARD_JK,			// 35

	eOBJECT_KIND_CARD_K,			// 36
	eOBJECT_KIND_CARD_J,			// 37
	eOBJECT_KIND_CARD_Q,			// 38
	eOBJECT_KIND_SMALL_LOCK,		// 39
	eOBJECT_KIND_JAR_TYPE_B,		// 40
	eOBJECT_KIND_JAR_TYPE_C,		// 41
	eOBJECT_KIND_HEART_PIECE,		// 42
	eOBJECT_KIND_BOOK,				// 43
	eOBJECT_KIND_CRYSTAL_SWITCH_BIG,// 44

	eOBJECT_KIND_MAX,
};

/*=====================================*
 * �L�[�A�C�e���֘A
 *=====================================*/
enum {
	eKEYITEM_MASTERSWORD = 0,
	eKEYITEM_SEAL_KEY,
	eKEYITEM_WALLET_A,
	eKEYITEM_WALLET_B,
	eKEYITEM_WALLET_C,
	eKEYITEM_LIB_SHIELD,
	eKEYITEM_ANGEL_WING,
	eKEYITEM_TROPHY,
	eKEYITEM_BOOK,
	eKEYITEM_FLAME_RING,
	eKEYITEM_CHICK_PIERCE,
	eKEYITEM_FAIRY_BRACELET,

	eKEYITEM_NAVI_LIGHT,
	eKEYITEM_CARD_K,
	eKEYITEM_CARD_Q,
	eKEYITEM_CARD_J,

	eKEYITEM_SMILE_0,
	eKEYITEM_SMILE_1,
	eKEYITEM_SMILE_2,
	eKEYITEM_SMILE_3,
	eKEYITEM_SMILE_4,
	eKEYITEM_SMILE_5,
	eKEYITEM_DANCE,

	eKEYITEM_MAX,
};

/*=====================================*
 * �I�J���i�S���
 *=====================================*/
// �S�̎��
enum {
	eSONG_TYPE_SUN,			// ���z�̉S
	eSONG_TYPE_WILDNESS,	// �����̉S
	eSONG_TYPE_WING,		// ���̉S
	eSONG_TYPE_HANDLING,	// ����̉S	

	eSONG_TYPE_MAX,
};

/*=====================================*
 * ����
 *=====================================*/
enum {
	eTREASURE_KIND_BIG,
	eTREASURE_KIND_SMALL,
	eTREASURE_KIND_CRYSTAL,
	eTREASURE_KIND_FALLEN,		// ���Ƃ���

	eTREASURE_KIND_MAX,
};

#define NAME_MDL(index)			sResourceModelNameTable[index]

/*=====================================*
 * �X�e�[�W��`
 *=====================================*/
enum {
	eRES_STG_NONE = 0,
	eRES_STG_DUMMY,
	eRES_STG_BATOKIN,
	eRES_STG_FORTRESS_CITY,
	eRES_STG_FORTRESS_FIELD,
	eRES_STG_REVELNEAR2ND_HORBOR,
	eRES_STG_REVELNEAR2ND_EARTH,
	eRES_STG_REVELNEAR2ND_RICHCITY,
	eRES_STG_VUAL_LIBRARY,
	eRES_STG_VUAL_SUB,
	eRES_STG_MARISA_ROOM,
	eRES_STG_FOREST,
	eRES_STG_FOREST_SUB_1,
	eRES_STG_FOREST_SUB_2,
	eRES_STG_FOREST_SUB_3,
	eRES_STG_OLD_PALACE,
	eRES_STG_DEPTH_SEA,
	eRES_STG_FOREST_CAVE,
	eRES_STG_GRAVEYARD,
	eRES_STG_REVELNEAR2ND_MERGE,
	eRES_STG_KITCHEN,
	eRES_STG_LIBRARY,
	eRES_STG_WATERWAY,
	eRES_STG_PRISON,
	eRES_STG_UNDERGROUND_LAKE,
	eRES_STG_TREASURE_ROOM,
	eRES_STG_CRYSTAL_CAVE,
	eRES_STG_CASTLE_KINGDOM,
	eRES_STG_CASTLE_EARTH,
	eRES_STG_SAILLING_SHIP,
	eRES_STG_WATER_CAVE,
	eRES_STG_BIG_SHIP,
	eRES_STG_DATCHMAN,
	eRES_STG_VILLAGE,
	eRES_STG_SULT_MANE,
	eRES_STG_SULT_MANE_TO_HARAPPA,
	eRES_STG_SULT_MANE_TROLLEY,
	eRES_STG_CRYSTAL_ROOM,
	eRES_STG_HARAPPA_SUB,
	eRES_STG_HARAPPA_WHITE_PLACE,
	eRES_STG_HARAPPA_MAIN,
	eRES_STG_PARANOIA,
	eRES_STG_PARANOIA_CORE,
	eRES_STG_PARANOIA_SPIRAL,
	eRES_STG_BABEL_ELEVATOR,
	eRES_STG_BABEL_GATEWAY,
	eRES_STG_BABEL_PRISON,
	eRES_STG_AIRSHIP,
	eRES_STG_HARAPPA_CAVE,
	eRES_STG_PARANOIA_2F,
	eRES_STG_SKYLAND,
	eRES_STG_AIR_RUINS,
	eRES_STG_ROVININ_VILLAGE,
	eRES_STG_KARUN,
	eRES_STG_KARUN_THRONE,
	eRES_STG_KARUN_BARADGULARAN,
	eRES_STG_KARUN_CASTLE,
	eRES_STG_KARUN_GATE,
	eRES_STG_KARUN_TOWN,
	eRES_STG_KARUN_FORGE,
	eRES_STG_ELEVATOR,

	eRES_STG_LOST_FOREST,
	eRES_STG_LINO_HOUSE,
	eRES_STG_FRESS_ROOM,
	eRES_STG_JAPAN_1,
	eRES_STG_JAPAN_2,
#ifdef _DEBUG
	eRES_STG_PITAGORA,
	eRES_STG_BAKUGEKI_CASTLE,
	eRES_STG_SHINGEKI_NO_KABE,
	eRES_STG_NEO_CITY,
	eRES_STG_BLUE_DEVIL,
	eRES_STG_VALLEY,
	eRES_STG_ASH_STAGE,
	eRES_STG_DEVIL_CASTLE,
	eRES_STG_KINGDOM_GARDEN,
	eRES_STG_HAKUGYOKURO,

	eRES_STG_MY_FOREST,
	eRES_STG_BLACK_BOX,

	eRES_STG_SIN_REI_CHO,
	eRES_STG_SHIRA_TAMA_ROW,

	eRES_STG_MOB_HARBOR,

	eRES_STG_INN,
	eRES_STG_CHIREIDEN_HONDEN,
	eRES_STG_CHIREIDEN_ROBY,
	eRES_STG_ARENA,
	eRES_STG_BIG_WOOD,
	eRES_STG_MIKAGETSU,
	eRES_STG_OUTSET_ISLAND,
	eRES_STG_NIGHTS_BAR,

	eRES_STG_KAIPELINA,
	eRES_STG_NEO_VENICE,
	eRES_STG_HARRAPPA_TEST,
	eRES_STG_COOL_COOL_MOUNTAIN,
	eRES_STG_KAGITABA_FIELD_01,
	eRES_STG_KAGITABA_FIELD_02,
	eRES_STG_JOSAITOSHI,
	eRES_STG_TOY_BOX_STAGE,
	eRES_STG_STAR_CHART_STAGE,
	eRES_STG_SKY_COLOR_CITY,

	eRES_STG_CAVE,
	eRES_STG_CAVE_LOAD,
	eRES_STG_PARANOIA_PIPE,

	eRES_STG_UTOPIA,
	eRES_STG_SAN_MARUKO_PLAZA,
	eRES_STG_US_CONSTITUTION,
	eRES_STG_VILLAGE_077,
	eRES_STG_HARBOR,
	eRES_STG_TANI,
	eRES_STG_MEIRO,
	eRES_STG_DRMITORY,
	eRES_STG_SAIDAN_MQO,
	eRES_STG_SAIDAN_X,
	eRES_STG_ACTION,
	eRES_STG_BALL_COASTER,
	eRES_STG_PLANETS_CORE,
	eRES_STG_CAMOME_CITY,
	eRES_STG_BOMB_BATTLE_FIELD,
	eRES_STG_REVELNEAR,
	eRES_STG_TMP_STAGE,
	eRES_STG_AREA_C,
	eRES_STG_BOSS_STAGE,
	eRES_STG_WARM_ISLAND,
	eRES_STG_HALLOWEEN_TOWN,
	eRES_STG_GAREKI_CITY,
	eRES_STG_GARAGE,
	eRES_STG_STEEMPANK_FACTORY,

	eRES_STG_TEMPLE_OF_FOREST,
	eRES_STG_FIRST_PLACE,

#endif // _DEBUG
	eRES_STG_MAX,
};

static const char* sResourceStageNameTable[eRES_STG_MAX] = {
	"",
	"dummy.mv1",
	"batokin/000_batokin.mqo",
	"Belca/006_FortressCity.mqo", 
	"Belca/007_FortressFiled.mqo", 
	"Revelnear2nd/009_Stage.mv1", 
	"Revelnear2nd/010_Stage.mv1",  
	"Revelnear2nd/013_Stage.mv1", 
	"VualLibrary/Stage.mv1",
	"VualLibrary/Sub.mv1",
	"�������̕���/Stage.mv1",
	"Forest/StageMain.mv1", 
	"Forest/StageSub1.mv1",
	"Forest/StageSub2.mv1",
	"Forest/StageSub3.mv1",
	"�Â��{�a/Stage.mv1",
	"�[�C���͋��_/Stage.mv1",
	"ForestCave/Stage.mv1",
	"Graveyard/Stage.mqo", 
	"Revelnear2nd/021_Comm_Vill.mv1", 
	"Kitchen/022_Stage.mv1", 
	"Library/023_Stage.mv1", 
	"Waterway/024_Stage.mv1", 
	"Prison/Stage.mv1", 
	"UndergroundLake/Stage.mv1", 
	"TreasureRoom/Stage.mv1", 
	"CrystalCave/Stage.mv1", 
	"Revelnear2nd/Kingdom.mv1", 
	"Revelnear2nd/Earth.mv1",
	"SaillingShip/Stage.mv1", 
	"UnityStage/���̓��A�����.mqo", 
	"�唿�D/Stage.mqo",
	"�_�b�`�}��/Stage.mqo",
	"ToealVillage/Stage.mv1",
	"SultMine/SultMine.mv1",
	"SultMine/SultMine2�r��.mv1",
	"SultMine/SultMineTrolley.mv1",
	"CrtstalRoom/Stage.mv1",
	"Harappa/SubStage.mv1",
	"Harappa/WhitePlace.mv1",
	"Harappa/MainStage.mv1",
	"Paranoia/Paranoia.mqo", 
	"Paranoia/Core.mqo", 
	"Paranoia/Spiral.mqo", 
	"BabelElevator/Mdl.mv1",
	"BabelRoom/Gateway.mv1",
	"Paranoia/Prison.mv1",
	"AirShip/Mdl.mv1",
	"BabelElevator/Cave.mv1",
	"Paranoia/Paranoia_2F.mv1",
	"Skyland/Stage.mv1", 
	"AirRuins/Stage.mv1", 
	"�����B�j����/Stage.mqo",
	"Karun/014_uru.mv1", 
	"Karun/015_cd_throne.mqo", 
	"Karun/016_baradgularan.mqo", 
	"Karun/017_cd_castle.mqo", 
	"Karun/018_cd_gate.mqo", 
	"Karun/019_cd_town.mqo", 
	"Karun/020_forge.mqo", 
	"Elevator/Model.mv1",

	"LostForest/Mdl.mv1",
	"LinoHouse/Stage.mv1",
	"Revelnear2nd/FresRoom.mv1",
	"Japan/Stage1.mv1",
	"Japan/Stage1.mv1",
#ifdef _DEBUG
	"�s�^�S��/Stage.mqo", 
	"������/Stage.mqo", 
	"�i���̕�/Stage.mv1", 
	"NeoCity/Stage.mqo", 
	"BlueDevil/Stage.mqo", 
	"�k�J2/�k�J2.mv1", 
	"�p�ЃX�e�[�W/�p��.mv1", 
	"DevilCastle/Stage.mqo", 
	"Kingdom/Garden.mv1", 
	"Hakugyokuro/Stage.mqo", 

	"����X�X�e�[�W/Stage.mqo",
	"BlackBox/Stage.mqo", 

	"�_��_/�_��_.mqo",
	"���ʘO/Stage.mqo",

	"���u�`�X�e�[�W/Stage.mqo",

	"�h���̈ꎺ/Stage.mqo",
	"Chireiden/Honden.mqo", 
	"Chireiden/Roby.mqo", 
	"Arena/Arena.mqo", 
	"BigWood/Stage.mqo", 
	"Mikagetsu/Stage.mqo", 
	"OutsetIsland/OutsetIsland.mqo", 
	"Night'sBar/Stage.mqo", 

	"�J�C�y���[�j��/�J�C�y���[�j��A.mqo",
	"�l�I���F�l�c�B�A/�l�I���F�l�c�B�A.mv1",
	"HarappaTest/MainStage.mqo",
	"CoolCoolMountain/CoolCoolMountain.mqo",
	"�������t�B�[���h01/�t�B�[���h01.mv1"  ,
	"�؁X�̂���ד�/Model.mqo",
	"Josaitoshi/Stage.mqo", 
	"�ߋst/box.mv1",
	"�����ՃX�e�[�W/�����ՃX�e�[�Wver1.0.mv1",
	"SkyColorCity/sorairo1.4.mv1", 

	"���A�̂悤�Ȃ���/crystal1.mv1",
	"���A�̂悤�Ȃ���/������.mv1",
	"Paranoia/�p�C�v�Q.mqo",

	"22_utopia/utopia.mqo", 
	"�T���E�}���R�L��/�T���E�}���R�L��.mqo",
	"59_US Constitution/Constitution.mqo", 
	"�l��ver0.77/�l��.mv1", 
	"�R�`/�R�`.mqo", 
	"Tani/Stage.mqo", 
	"Meiro/Stage.mqo", 
	"Dormitory/Stage.mqo", 
	"Saidan/Stage.mqo", 
	"Saidan/Stage.x", 
	"Action/Stage.mqo", 
	"BallCoaster/Stage.mqo", 
	"PlanetsCore/Stage.mqo", 
	"Camome/Stage.mqo", 
	"BobombBattlefeild/Stage.mqo", 
	"Revelnear/Stage.mqo", 
	"TmpStage/Stage.mqo", 
	"AreaC/Stage.mqo", 
	"BossStage/Stage.mqo", 
	"WarmIsland/Island.mqo", 
	"HalloweenTown/HalloweenTown.mqo", 
	"GarekiCity/GarekiCity.mqo", 
	"Garage/���u�K���[�W�X�e�[�W.mv1",
	"�X�`�[���p���N�H��/Mdl.mv1",

	"�X�̐_�a/Stage.mv1",
	"�n�܂�̏ꏊ/Stage.mqo",
	
#endif // _DEBUG
};

/*=====================================*
 * struct
 *=====================================*/
struct tNonResidentInfo {
	const char*	mModelName;
	BYTE		mStageIndex;
	eItemKind	mItemKind;
};

/*=====================================*
 * ���f����`
 * �풓���f��
 *=====================================*/
enum {
	eRES_MDL_MASTERSWORD,             // �}�X�^�[�\�[�h
	eRES_MDL_LOCKON,                  // ���b�N�I���Ə����f��
	eRES_MDL_TREASURE,                // ��
	eRES_MDL_MAP,                     // �n�}
	eRES_MDL_SMILE,                   // ���m�̏Ί�
	eRES_MDL_MONEY_1,                 // �����P
	eRES_MDL_MONEY_5,                 // �����T
	eRES_MDL_MONEY_10,                // �����P�O
	eRES_MDL_MONEY_20,                // �����Q�O
	eRES_MDL_MONEY_50,                // �����T�O
	eRES_MDL_MONEY_100,               // �����P�O�O
	eRES_MDL_HEART,                   // �n�[�g
	eRES_MDL_HEART_CASE,              // �n�[�g�̊�
	eRES_MDL_BOSSKEY,                 // �{�X�̌�
	eRES_MDL_LANCE,                   // �t���X�̑�
	eRES_MDL_GIZERL,                  // �U�U�[���̖��
	eRES_MDL_BOMB,                    // ���e
	eRES_MDL_JAR,                     // �c�{
	eRES_MDL_LOCK,                    // ��
	eRES_MDL_STONE,                   // ��
	eRES_MDL_CRYSTAL,                 // �N���X�^��
	eRES_MDL_LOCK_CENTER,             // ��(������p)
	eRES_MDL_MOGUREVER,               // ���O���o�[
	eRES_MDL_FACESWITCH,              // �V���{�[���X�C�b�`
	eRES_MDL_CRYSTAL_SWITCH,          // �N���X�^���X�C�b�`
	eRES_MDL_ANGEL_WING,              // �y�K�T�X�̊�
	eRES_MDL_PLATE_COMMON,            // ���ʃv���[�g
	eRES_MDL_BIN,                     // �r��
	eRES_MDL_LIB_SHIELD,              // ���x���j�A�̏�
	eRES_MDL_ARROW,                   // ��
	eRES_MDL_LONGHOOK,                // �����O�t�b�N
	eRES_MDL_HEART_PIECE,             // �n�[�g�̂�����
	eRES_MDL_DUMMY_OBJ,               // �_�~�[�I�u�W�F�N�g

	eRES_MDL_MAX,
};

static const char* sResourceModelNameTable[eRES_MDL_MAX] = {
	"MasterSword/MasterSword.mv1",       // �}�X�^�[�\�[�h"
	"Lockon/Lockon.mv1",                 // ���b�N�I���Ə����f��"
	"Treasure/Treasure.mv1",             // ��"
	"Map/Map.mv1",                       // �n�}"
	"Smile/Smile.mv1",                   // ���m�̏Ί�"
	"Money/1rupee.mv1",                  // �����P"
	"Money/5rupee.mv1",                  // �����T"
	"Money/10rupee.mv1",                 // �����P�O"
	"Money/20rupee.mv1",                 // �����Q�O"
	"Money/50rupee.mv1",                 // �����T�O"
	"Money/100rupee.mv1",                // �����P�O�O"
	"HeartCase/Heart.mv1",               // �n�[�g"
	"HeartCase/HeartCase.mv1",           // �n�[�g�̊�"
	"BossKey/BossKey.mv1",               // �{�X�̌�"
	"Lance/snow_white.mv1",              // �t���X�̑�"
	"Gizerl/Gizerl.mv1",                 // �U�U�[���̖��"
	"Bomb/Bomb.mv1",                     // ���e"
	"Jar/Jar.mv1",                       // �c�{"
	"Lock/Lock.mv1",                     // ��"
	"Stone/Stone.mv1",                   // ��"
	"CrysG/CrysG.mv1",                   // �N���X�^��"
	"Lock/LockCenter.mv1",               // ��(������p)"
	"MoguRever/MoguRever.mv1",           // ���O���o�["
	"FaceSwitch/FaceSwitch.mv1",         // �V���{�[���X�C�b�`"
	"CrystalSwitch/CrystalSwitch.mv1",   // �N���X�^���X�C�b�`"
	"Wing/Wing.mv1",                     // �y�K�T�X�̊�"
	"PlateCmn/Mdl.mv1",                  // ���ʃv���[�g"
	"Bin/Mdl.mv1",                       // �r��"
	"LibShield/LibShield.mv1",           // ���x���j�A�̏�"
	"Arrow/Arrow.mv1",                   // ��"
	"LongHook/LongHook.mv1",             // �����O�t�b�N"
	"HeartCase/HeartPiece.mv1",          // �n�[�g�̂�����"
	"dummy.mv1",                         // �_�~�[�I�u�W�F�N�g"
};

#define RES_MDL_MASTERSWORD         sResourceModelNameTable[eRES_MDL_MASTERSWORD]
#define RES_MDL_LOCKON              sResourceModelNameTable[eRES_MDL_LOCKON]
#define RES_MDL_TREASURE            sResourceModelNameTable[eRES_MDL_TREASURE]
#define RES_MDL_MAP                 sResourceModelNameTable[eRES_MDL_MAP]
#define RES_MDL_SMILE               sResourceModelNameTable[eRES_MDL_SMILE]
#define RES_MDL_MONEY_1             sResourceModelNameTable[eRES_MDL_MONEY_1]
#define RES_MDL_MONEY_5             sResourceModelNameTable[eRES_MDL_MONEY_5]
#define RES_MDL_MONEY_10            sResourceModelNameTable[eRES_MDL_MONEY_10]
#define RES_MDL_MONEY_20            sResourceModelNameTable[eRES_MDL_MONEY_20]
#define RES_MDL_MONEY_50            sResourceModelNameTable[eRES_MDL_MONEY_50]
#define RES_MDL_MONEY_100           sResourceModelNameTable[eRES_MDL_MONEY_100]
#define RES_MDL_HEART               sResourceModelNameTable[eRES_MDL_HEART]
#define RES_MDL_HEART_CASE          sResourceModelNameTable[eRES_MDL_HEART_CASE]
#define RES_MDL_BOSSKEY             sResourceModelNameTable[eRES_MDL_BOSSKEY]
#define RES_MDL_LANCE               sResourceModelNameTable[eRES_MDL_LANCE]
#define RES_MDL_GIZERL              sResourceModelNameTable[eRES_MDL_GIZERL]
#define RES_MDL_BOMB                sResourceModelNameTable[eRES_MDL_BOMB]
#define RES_MDL_JAR                 sResourceModelNameTable[eRES_MDL_JAR]
#define RES_MDL_LOCK                sResourceModelNameTable[eRES_MDL_LOCK]
#define RES_MDL_STONE               sResourceModelNameTable[eRES_MDL_STONE]
#define RES_MDL_CRYSTAL             sResourceModelNameTable[eRES_MDL_CRYSTAL]
#define RES_MDL_LOCK_CENTER         sResourceModelNameTable[eRES_MDL_LOCK_CENTER]
#define RES_MDL_MOGUREVER           sResourceModelNameTable[eRES_MDL_MOGUREVER]
#define RES_MDL_FACESWITCH          sResourceModelNameTable[eRES_MDL_FACESWITCH]
#define RES_MDL_CRYSTAL_SWITCH      sResourceModelNameTable[eRES_MDL_CRYSTAL_SWITCH]
#define RES_MDL_ANGEL_WING          sResourceModelNameTable[eRES_MDL_ANGEL_WING]
#define RES_MDL_PLATE_COMMON        sResourceModelNameTable[eRES_MDL_PLATE_COMMON]
#define RES_MDL_BIN                 sResourceModelNameTable[eRES_MDL_BIN]
#define RES_MDL_LIB_SHIELD          sResourceModelNameTable[eRES_MDL_LIB_SHIELD]
#define RES_MDL_ARROW               sResourceModelNameTable[eRES_MDL_ARROW]
#define RES_MDL_LONGHOOK            sResourceModelNameTable[eRES_MDL_LONGHOOK]
#define RES_MDL_HEART_PIECE         sResourceModelNameTable[eRES_MDL_HEART_PIECE]
#define RES_MDL_DUMMY_OBJ           sResourceModelNameTable[eRES_MDL_DUMMY_OBJ]

/*=====================================*
 * ��풓���f��(�T�u�E�F�|��)
 *=====================================*/
enum {
	eRES_MDL_PIKOHUM,                   // �s�R�n��
	eRES_MDL_FRES_SHIELD,               // �t���X�̏�
	eRES_MDL_CARD_JK,                   // �W���b�N�̃J�[�h
	eRES_MDL_CARD_COMMON,               // �J�[�h����
	eRES_MDL_SUNOHARA,                  // �t������
	eRES_MDL_SEESAW,                    // �V�[�\�[
	eRES_MDL_USACHU,                    // �������イ
	eRES_MDL_ARCHERY,                   // �|
	eRES_MDL_BOOMERANG,                 // �u�[������
	eRES_MDL_OCARINA,                   // �I�J���i
	eRES_MDL_COPY_ROD,                  // �R�s�[���b�h

	eRES_MDL_NON_RESI,
};

static const tNonResidentInfo sResourceModelTableNonResi[eRES_MDL_NON_RESI] = {
	{"PikoHum/Weapon.mv1", 41, eITEM_KIND_PIKOHUM },      // �s�R�n��
	{"FresShield/FresShield.mv1", 29, eITEM_KIND_NONE },  // �t���X�̏�
	{"Trump/Joker.mv1", 29, eITEM_KIND_NONE },            // �W���b�N�̃J�[�h
	{"Trump/Common.mv1", 29, eITEM_KIND_NONE },           // �J�[�h����
	{"Sunohara/Sunohara.mv1", 16, eITEM_KIND_NONE },      // �t������
	{"Seesaw/Seesaw.mv1", 16, eITEM_KIND_NONE },          // �V�[�\�[
	{"Usachu/Usachu.mv1", 16, eITEM_KIND_NONE },          // �������イ
	{"Archery/Archery.mv1", 39, 	eITEM_KIND_ARCHERY }, // �|"
	{"Boomerang/Boomerang.mv1", 27, eITEM_KIND_BOOMERANG },// �u�[������
	{"Ocarina/Mdl.mv1", 16, eITEM_KIND_OCARINA },          // �I�J���i
	{"CopyRod/Mdl.mv1", 41, 	eITEM_KIND_COPY_ROD },     // �R�s�[���b�h"
};

#define RES_MDL_PIKOHUM        sResourceModelTableNonResi[eRES_MDL_PIKOHUM].mModelName
#define RES_MDL_FRES_SHIELD        sResourceModelTableNonResi[eRES_MDL_FRES_SHIELD].mModelName
#define RES_MDL_CARD_JK        sResourceModelTableNonResi[eRES_MDL_CARD_JK].mModelName
#define RES_MDL_CARD_COMMON        sResourceModelTableNonResi[eRES_MDL_CARD_COMMON].mModelName
#define RES_MDL_SUNOHARA        sResourceModelTableNonResi[eRES_MDL_SUNOHARA].mModelName
#define RES_MDL_SEESAW        sResourceModelTableNonResi[eRES_MDL_SEESAW].mModelName
#define RES_MDL_USACHU        sResourceModelTableNonResi[eRES_MDL_USACHU].mModelName
#define RES_MDL_ARCHERY        sResourceModelTableNonResi[eRES_MDL_ARCHERY].mModelName
#define RES_MDL_BOOMERANG        sResourceModelTableNonResi[eRES_MDL_BOOMERANG].mModelName
#define RES_MDL_OCARINA        sResourceModelTableNonResi[eRES_MDL_OCARINA].mModelName
#define RES_MDL_COPY_ROD        sResourceModelTableNonResi[eRES_MDL_COPY_ROD].mModelName

/*=====================================*
 * ��풓���f��(���̑�)
 *=====================================*/
#define RES_MDL_SWORD        "Sword/sword023.mv1"
#define RES_MDL_AX        "Ax/Ax.mv1"
#define RES_MDL_STAFF        "Staff/Staff.mv1"
#define RES_MDL_SHIELD        "Shield/Shield.mv1"
#define RES_MDL_BOOK_OPEN        "Book/Mdl.mv1"
#define RES_MDL_BOOK_CLOSE        "Book/Sub.mv1"

/*=====================================*
 * �M�~�b�N�p
 *=====================================*/
#define RES_MDL_LIFT			"Lift/Lift.mv1"
#define RES_MDL_SHIP			"Ship/Ship.mv1"
#define RES_MDL_ROLLMOGU		"RollMogu/Mogu.mv1"
#define RES_MDL_BIGKEY			"BigKey/BigKey.mv1"
#define RES_MDL_TROLLEY			"Trolley/Trolley.mv1"
#define RES_MDL_ELEVATOR		"Elevator/Model.mv1"
#define RES_MDL_DANCER			"Mask/Model.mv1"
#define RES_MDL_TOWER			"Tower/Tower.mv1"
#define RES_MDL_ELEVATOR_B		"ElevatorB/ElevatorB.mv1"
#define RES_MDL_BABEL_SWITCH	"Tower/Switch.mv1"
#define RES_MDL_HOLE			"Hole/Model.mv1"
#define RES_MDL_WHELL			"Wheel/Mdl.mv1"
#define RES_MDL_EYE				"Eye/Mdl.mv1"
#define RES_MDL_CANNON			"Cannon/Mdl.mv1"
#define RES_MDL_AIRSHIP			"Airship/Mdl.mv1"
#define RES_MDL_PIECES			"Pieces/Mdl.mv1"
#define RES_MDL_CLOCK			"Clock/Mdl.mv1"
#define RES_MDL_CLOCK_VISIBLE	"Clock/Visible.mv1"
#define RES_MDL_TETO			"Teto/Mdl.mv1"
#define RES_MDL_BARRIER			"Barrier/Mdl.mv1"
#define RES_MDL_MAGIC_CUBE		"MagicCube/Mdl.mv1"
#define RES_MDL_LEAN_PASSAGE	"LeanPassage/Mdl.mv1"
#define RES_MDL_ROLLING_PIPE	"RollingPipe/Mdl.mv1"
#define RES_MDL_SCREW			"Screw/Mdl.mv1"
#define RES_MDL_LASER_BEAM		"LaserBeam/Mdl.mv1"

#define RES_MDL_DRAGON_SWORD	"DragonSword/Mdl.mv1"

/*=====================================*
 * ���C�A�E�g�̃��\�[�X���
 *=====================================*/
enum {
	eRES_LYT_SHADOW,
	eRES_LYT_RIPPLE,
	eRES_LYT_ITEM_RING_NONE,
	eRES_LYT_ITEM_RING_MASTERSWORD,
	eRES_LYT_ITEM_RING_LONGHOOK,
	eRES_LYT_ITEM_RING_DUMMY,
	eRES_LYT_ITEM_RING_PIKO_HUM,
	eRES_LYT_ITEM_RING_BOOMERANG,
	eRES_LYT_ITEM_RING_PORTION,
	eRES_LYT_ITEM_RING_SHIELD,
	eRES_LYT_ITEM_RING_BOMB,
	eRES_LYT_ITEM_RING_ARCHERY,
	eRES_LYT_ICON_OBJECT,
	eRES_LYT_INFO_STAGE_CLEAR,
	eRES_LYT_INFO_GAME_OVER,
	eRES_LYT_EDGE,
	eRES_LYT_ICON_MAP_EVENT,
	eRES_LYT_ICON_MAP_MOVE,
	eRES_LYT_LIBERUNIA,
	eRES_LYT_ITEM_RING_WING,
	eRES_LYT_CRYSTAL_GREEN,
	eRES_LYT_CRYSTAL_YELLOW,
	eRES_LYT_ITEM_RING_BIN_EMPTY,
	eRES_LYT_ITEM_RING_BIN_MAGENTA,
	eRES_LYT_ITEM_RING_BIN_CYAN,
	eRES_LYT_ITEM_RING_BIN_YELLOW,
	eRES_LYT_ITEM_RING_BIN_FIARY,
	eRES_LYT_INFO_STAGE,
	eRES_LYT_INFO_1,
	eRES_LYT_INFO_2,
	eRES_LYT_INFO_3,
	eRES_LYT_INFO_4,
	eRES_LYT_INFO_5,
	eRES_LYT_INFO_MAP_1,
	eRES_LYT_INFO_MAP_2,
	eRES_LYT_INFO_MAP_3,
	eRES_LYT_INFO_MAP_4,
	eRES_LYT_INFO_MAP_5,
	eRES_LYT_INFO_MAP_5_2,
	eRES_LYT_INFO_MAP_SUB_1,
	eRES_LYT_INFO_MAP_SUB_2,
	eRES_LYT_INFO_MAP_SUB_3,
	eRES_LYT_INFO_MAP_SUB_4,
	eRES_LYT_INFO_MAP_SUB_5,
	eRES_LYT_INFO_BOSS_1,
	eRES_LYT_INFO_BOSS_3,
	eRES_LYT_INFO_BOSS_4,
	eRES_LYT_INFO_BOSS_5,
	eRES_LYT_INFO_FRAME,
	eRES_LYT_INFO_ITEM,
	eRES_LYT_INFO_COSPLAY,
	eRES_LYT_INFO_KEYITEM,
	eRES_LYT_PLATE_COMMON,
	eRES_LYT_INFO_SQUARE,
	eRES_LYT_RING_WALLET_A,
	eRES_LYT_RING_WALLET_B,
	eRES_LYT_RING_WALLET_C,
	eRES_LYT_RING_SEAL_KEY,
	eRES_LYT_CARD_K,
	eRES_LYT_CARD_Q,
	eRES_LYT_CARD_J,
	eRES_LYT_RING_MAGIC_THUNDER,
	eRES_LYT_RING_MAGIC_FLAME,
	eRES_LYT_RING_MAGIC_TORNADE,
	eRES_LYT_RING_OCARINA,
	eRES_LYT_RING_COPY_ROD,
	eRES_LYT_HEART_PIECE,
	eRES_LYT_RING_TROPY,
	eRES_LYT_RING_NAVI_LIGHT,
	eRES_LYT_RING_BOOK,

	eRES_LYT_RING_FLAME_RING,
	eRES_LYT_RING_CHICK_PIERCE,
	eRES_LYT_RING_FAIRY_BRACELET,

	eRES_LYT_MAX,
};

static const char* sResourceLayoutNameTable[eRES_LYT_MAX] = {
	"Shadow.tga",
	"Ripple.tga",
	"RingNone.png",
	"RingMasterSword.jpg",
	"RingLonghook.png",
	"RingDummy.png",
	"RingPikoHum.png",
	"RingBoomerang.png",
	"RingPortion.png",
	"RingShield.jpg",
	"RingBomb.png",
	"RingArchery.png",
	"IconObject.png",
	"Info_StageClear.png",
	"Info_GameOver.png",
	"Edge.png",
	"IconMapEvent.png",
	"IconMapMove.png",
	"Liberunia.png",
	"RingWing.jpg",
	"CrysG.bmp",
	"CrysY.bmp",
	"RingBinEmpty.png",
	"RingBinM.png",
	"RingBinC.png",
	"RingBinY.png",
	"RingBinT.png",
	"Info_Stage.png",
	"Info_1.png",
	"Info_2.png",
	"Info_3.png",
	"Info_4.png",
	"Info_5.png",
	"Info_Map1.png",
	"Info_Map2.png",
	"Info_Map3.png",
	"Info_Map4.png",
	"Info_Map5.png",
	"Info_Map5_2.png",
	"Info_MapSub1.png",
	"Info_MapSub2.png",
	"Info_MapSub3.png",
	"Info_MapSub4.png",
	"Info_MapSub5.png",
	"Info_Boss1.png",
	"Info_Boss3.png",
	"Info_Boss4.png",
	"Info_Boss5.png",
	"Info_Frame.png",
	"Info_Item.png",
	"Info_Cosplay.png",
	"Info_KeyItem.png",
	"Plate.png",
	"Info_Square.png",
	"RingWalletA.png",
	"RingWalletB.png",
	"RingWalletC.png",
	"RingSealKey.jpg",
	"PlateCardK.jpg",
	"PlateCardQ.jpg",
	"PlateCardJ.jpg",
	"Magic_Lightning.png",
	"Magic_Flame.png",
	"Magic_Tornade.png",
	"RingOcarina.png",
	"RingCopyRod.png",
	"heart_piece.png",
	"RingTrophy.jpg",
	"RingNaviLight.jpg",
	"RingBook.jpg",

	"RingFlameRing.png",
	"RingChickPierce.png",
	"RingFairyBracelet.png",

};

#define RES_LYT_SHADOW                          sResourceLayoutNameTable[eRES_LYT_SHADOW]
#define RES_LYT_RIPPLE                          sResourceLayoutNameTable[eRES_LYT_RIPPLE]
#define RES_LYT_ITEM_RING_NONE                  sResourceLayoutNameTable[eRES_LYT_ITEM_RING_NONE]
#define RES_LYT_ITEM_RING_MASTERSWORD           sResourceLayoutNameTable[eRES_LYT_ITEM_RING_MASTERSWORD]
#define RES_LYT_ITEM_RING_LONGHOOK              sResourceLayoutNameTable[eRES_LYT_ITEM_RING_LONGHOOK]
#define RES_LYT_ITEM_RING_DUMMY                 sResourceLayoutNameTable[eRES_LYT_ITEM_RING_DUMMY]
#define RES_LYT_ITEM_RING_PIKO_HUM              sResourceLayoutNameTable[eRES_LYT_ITEM_RING_PIKO_HUM]
#define RES_LYT_ITEM_RING_BOOMERANG             sResourceLayoutNameTable[eRES_LYT_ITEM_RING_BOOMERANG]
#define RES_LYT_ITEM_RING_PORTION               sResourceLayoutNameTable[eRES_LYT_ITEM_RING_PORTION]
#define RES_LYT_ITEM_RING_SHIELD                sResourceLayoutNameTable[eRES_LYT_ITEM_RING_SHIELD]
#define RES_LYT_ITEM_RING_BOMB                  sResourceLayoutNameTable[eRES_LYT_ITEM_RING_BOMB]
#define RES_LYT_ITEM_RING_ARCHERY               sResourceLayoutNameTable[eRES_LYT_ITEM_RING_ARCHERY]
#define RES_LYT_ICON_OBJECT                     sResourceLayoutNameTable[eRES_LYT_ICON_OBJECT]
#define RES_LYT_INFO_STAGE_CLEAR                sResourceLayoutNameTable[eRES_LYT_INFO_STAGE_CLEAR]
#define RES_LYT_INFO_GAME_OVER                  sResourceLayoutNameTable[eRES_LYT_INFO_GAME_OVER]
#define RES_LYT_EDGE                            sResourceLayoutNameTable[eRES_LYT_EDGE]
#define RES_LYT_ICON_MAP_EVENT                  sResourceLayoutNameTable[eRES_LYT_ICON_MAP_EVENT]
#define RES_LYT_ICON_MAP_MOVE                   sResourceLayoutNameTable[eRES_LYT_ICON_MAP_MOVE]
#define RES_LYT_LIBERUNIA						sResourceLayoutNameTable[eRES_LYT_LIBERUNIA]
#define RES_LYT_ITEM_RING_WING					sResourceLayoutNameTable[eRES_LYT_ITEM_RING_WING]
#define RES_LYT_CRYSTAL_GREEN					sResourceLayoutNameTable[eRES_LYT_CRYSTAL_GREEN]
#define RES_LYT_CRYSTAL_YELLOW					sResourceLayoutNameTable[eRES_LYT_CRYSTAL_YELLOW]
#define RES_LYT_ITEM_RING_BIN_EMPTY				sResourceLayoutNameTable[eRES_LYT_ITEM_RING_BIN_EMPTY]
#define RES_LYT_ITEM_RING_BIN_MAGENTA			sResourceLayoutNameTable[eRES_LYT_ITEM_RING_BIN_MAGENTA]
#define RES_LYT_ITEM_RING_BIN_CYAN				sResourceLayoutNameTable[eRES_LYT_ITEM_RING_BIN_CYAN]
#define RES_LYT_ITEM_RING_BIN_YELLOW			sResourceLayoutNameTable[eRES_LYT_ITEM_RING_BIN_YELLOW]
#define RES_LYT_ITEM_RING_BIN_FIARY				sResourceLayoutNameTable[eRES_LYT_ITEM_RING_BIN_FIARY]
#define RES_LYT_INFO_STAGE						sResourceLayoutNameTable[eRES_LYT_INFO_STAGE]
#define RES_LYT_INFO_1							sResourceLayoutNameTable[eRES_LYT_INFO_1]
#define RES_LYT_INFO_2							sResourceLayoutNameTable[eRES_LYT_INFO_2]
#define RES_LYT_INFO_3							sResourceLayoutNameTable[eRES_LYT_INFO_3]
#define RES_LYT_INFO_4							sResourceLayoutNameTable[eRES_LYT_INFO_4]
#define RES_LYT_INFO_5							sResourceLayoutNameTable[eRES_LYT_INFO_5]
#define RES_LYT_INFO_MAP_1						sResourceLayoutNameTable[eRES_LYT_INFO_MAP_1]
#define RES_LYT_INFO_MAP_2						sResourceLayoutNameTable[eRES_LYT_INFO_MAP_2]
#define RES_LYT_INFO_MAP_3						sResourceLayoutNameTable[eRES_LYT_INFO_MAP_3]
#define RES_LYT_INFO_MAP_4						sResourceLayoutNameTable[eRES_LYT_INFO_MAP_4]
#define RES_LYT_INFO_MAP_5						sResourceLayoutNameTable[eRES_LYT_INFO_MAP_5]
#define RES_LYT_INFO_MAP_5_2					sResourceLayoutNameTable[eRES_LYT_INFO_MAP_5_2]
#define RES_LYT_INFO_MAP_SUB_1					sResourceLayoutNameTable[eRES_LYT_INFO_MAP_SUB_1]
#define RES_LYT_INFO_MAP_SUB_2					sResourceLayoutNameTable[eRES_LYT_INFO_MAP_SUB_2]
#define RES_LYT_INFO_MAP_SUB_3					sResourceLayoutNameTable[eRES_LYT_INFO_MAP_SUB_3]
#define RES_LYT_INFO_MAP_SUB_4					sResourceLayoutNameTable[eRES_LYT_INFO_MAP_SUB_4]
#define RES_LYT_INFO_MAP_SUB_5					sResourceLayoutNameTable[eRES_LYT_INFO_MAP_SUB_5]
#define RES_LYT_INFO_BOSS_1						sResourceLayoutNameTable[eRES_LYT_INFO_BOSS_1]
#define RES_LYT_INFO_BOSS_3						sResourceLayoutNameTable[eRES_LYT_INFO_BOSS_3]
#define RES_LYT_INFO_BOSS_4						sResourceLayoutNameTable[eRES_LYT_INFO_BOSS_4]
#define RES_LYT_INFO_BOSS_5						sResourceLayoutNameTable[eRES_LYT_INFO_BOSS_5]
#define RES_LYT_INFO_FRAME						sResourceLayoutNameTable[eRES_LYT_INFO_FRAME]
#define RES_LYT_INFO_ITEM						sResourceLayoutNameTable[eRES_LYT_INFO_ITEM]
#define RES_LYT_INFO_COSPLAY					sResourceLayoutNameTable[eRES_LYT_INFO_COSPLAY]
#define RES_LYT_INFO_KEYITEM					sResourceLayoutNameTable[eRES_LYT_INFO_KEYITEM]
#define RES_LYT_PLATE_COMMON					sResourceLayoutNameTable[eRES_LYT_PLATE_COMMON]
#define RES_LYT_INFO_SQUARE						sResourceLayoutNameTable[eRES_LYT_INFO_SQUARE]
#define RES_LYT_RING_WALLET_A					sResourceLayoutNameTable[eRES_LYT_RING_WALLET_A]
#define RES_LYT_RING_WALLET_B					sResourceLayoutNameTable[eRES_LYT_RING_WALLET_B]
#define RES_LYT_RING_WALLET_C					sResourceLayoutNameTable[eRES_LYT_RING_WALLET_C]
#define RES_LYT_RING_SEAL_KEY					sResourceLayoutNameTable[eRES_LYT_RING_SEAL_KEY]
#define RES_LYT_CARD_K							sResourceLayoutNameTable[eRES_LYT_CARD_K]
#define RES_LYT_CARD_Q							sResourceLayoutNameTable[eRES_LYT_CARD_Q]
#define RES_LYT_CARD_J							sResourceLayoutNameTable[eRES_LYT_CARD_J]
#define	RES_LYT_RING_MAGIC_THUNDER				sResourceLayoutNameTable[eRES_LYT_RING_MAGIC_THUNDER]
#define	RES_LYT_RING_MAGIC_FLAME				sResourceLayoutNameTable[eRES_LYT_RING_MAGIC_FLAME]
#define	RES_LYT_RING_MAGIC_TORNADE				sResourceLayoutNameTable[eRES_LYT_RING_MAGIC_TORNADE]
#define	RES_LYT_RING_OCARINA					sResourceLayoutNameTable[eRES_LYT_RING_OCARINA]
#define	RES_LYT_RING_COPY_ROD					sResourceLayoutNameTable[eRES_LYT_RING_COPY_ROD]
#define	RES_LYT_HEART_PIECE						sResourceLayoutNameTable[eRES_LYT_HEART_PIECE]
#define	RES_LYT_RING_TROPHY						sResourceLayoutNameTable[eRES_LYT_RING_TROPY]
#define	RES_LYT_RING_NAVI_LIGHT					sResourceLayoutNameTable[eRES_LYT_RING_NAVI_LIGHT]
#define	RES_LYT_RING_BOOK						sResourceLayoutNameTable[eRES_LYT_RING_BOOK]

#define	RES_LYT_RING_FLAME_RING					sResourceLayoutNameTable[eRES_LYT_RING_FLAME_RING]
#define	RES_LYT_RING_CHICK_PIERCE				sResourceLayoutNameTable[eRES_LYT_RING_CHICK_PIERCE]
#define	RES_LYT_RING_FAIRY_BRACELET				sResourceLayoutNameTable[eRES_LYT_RING_FAIRY_BRACELET]


#define RES_LYT_COMPASS			("IconCompass.png")
#define RES_LYT_MONEY			("IconCrystal.png")
#define RES_LYT_NUMBER_0		("Number/IconNum0.png")
#define RES_LYT_NUMBER_1		("Number/IconNum1.png")
#define RES_LYT_NUMBER_2		("Number/IconNum2.png")
#define RES_LYT_NUMBER_3		("Number/IconNum3.png")
#define RES_LYT_NUMBER_4		("Number/IconNum4.png")
#define RES_LYT_NUMBER_5		("Number/IconNum5.png")
#define RES_LYT_NUMBER_6		("Number/IconNum6.png")
#define RES_LYT_NUMBER_7		("Number/IconNum7.png")
#define RES_LYT_NUMBER_8		("Number/IconNum8.png")
#define RES_LYT_NUMBER_9		("Number/IconNum9.png")
#define RES_LYT_LOCKON			("IconLockon.png")
#define RES_LYT_HEART_FULL		("heart_full.png")
#define RES_LYT_HEART_3DIV4		("heart_3div4.png")
#define RES_LYT_HEART_2DIV4		("heart_2div4.png")
#define RES_LYT_HEART_1DIV4		("heart_1div4.png")
#define RES_LYT_HEART_EMPTY		("heart_empty.png")
#define RES_LYT_SKYDOME_BLUE_A	("Skydome/BlueSky.jpg")
#define RES_LYT_SKYDOME_HARAPPA	("Skydome/Harappa.jpg")
#define RES_LYT_ICON_GAUGE_FRAME ("IconGaugeFrame.png")
#define RES_LYT_ICON_GAUGE_LINE	 ("IconGaugeLine.png")
#define RES_LYT_PIYOPIYO		("piyopiyo.png")
#define RES_LYT_MAGIC_GAUGE		("MagicGaugeIcon.png")

#define RES_LYT_MAPARROW						"IconMapArrow.png"
#define RES_LYT_LINO_FACE_A						"LinoFaceA.png"
#define RES_LYT_LINO_FACE_B                     "LinoFaceB.png"
#define RES_LYT_LINO_FACE_C                     "LinoFaceC.png"
#define RES_LYT_LINO_FACE_D                     "LinoFaceD.png"
#define RES_LYT_BALLOON                         "Balloon.png"
#define RES_LYT_BALLOON_MSG                     "BalloonMsg.png"
#define RES_LYT_ICON_MAP_LINO_FACE              "IconMapLinoFace.png"
#define RES_LYT_ITEM_RING_MAP                   "RingMap.png"
#define RES_LYT_ITEM_RING_BOSS_KEY              "RingBossKey.png"
#define RES_LYT_RING_FRAME						"RingFrame.png"

// �v���[�g
#define RES_LYT_PLATE_ARROW		("PlateArrow.png")
#define RES_LYT_PLATE_BOMB		("PlateBomb.png")

// �R�X�v��
#define RES_LYT_COSPLAY_LINO	("Cosplay_Lino.jpg")
#define RES_LYT_COSPLAY_PRONAMA	("Cosplay_Pronama.jpg")
#define RES_LYT_COSPLAY_DABAH	("Cosplay_Dabah.jpg")
#define RES_LYT_COSPLAY_LAPAN	("Cosplay_Lapan.jpg")
#define RES_LYT_COSPLAY_FRES	("Cosplay_Fres.jpg")
#define RES_LYT_COSPLAY_TDA_MIKU ("Cosplay_TdaMiku.jpg")
#define RES_LYT_COSPLAY_HATOPHONE ("Cosplay_Hatophon.jpg")
#define RES_LYT_COSPLAY_MASK	("Cosplay_Mask.jpg")
#define RES_LYT_COSPLAY_BAT		("Cosplay_Bat.jpg")
#define RES_LYT_COSPLAY_CHILNO	("Cosplay_Chilno.jpg")
#define RES_LYT_COSPLAY_HONG_KONG ("Cosplay_HongKong.jpg")
#define RES_LYT_COSPLAY_LOANA	("Cosplay_Loana.jpg")
#define RES_LYT_COSPLAY_YUKARI	("Cosplay_Yukari.jpg")
#define RES_LYT_COSPLAY_NAKANO	("Cosplay_Nakano.jpg")

// ����
#define RES_LYT_SONG_SUN		("SongSun.png")
#define RES_LYT_SONG_WILDNESS	("SongWildness.png")
#define RES_LYT_SONG_WING		("SongWing.png")
#define RES_LYT_SONG_HANDLING	("SongHandling.png")

// �^�C�g���֘A
#define RES_LYT_TITLE_BACK		("TitleBack.png")
#define RES_LYT_TITLE_SUB		("TitleSub.png")
#define RES_LYT_TITLE_MAIN		("TitleMain.png")
#define RES_LYT_TITLE_SUB2		("TitleSub2.png")
#define RES_LYT_TITLE_PRESSANYKEY ("TitlePressAnyKey.png")
#define RES_LYT_TITLE_SPATZ		("TitleSpatz.png")


struct tItemDetailInfo {
	const char*		mModelName;
	const char*		mLayoutName;
	char*			mDispName;
	WORD			mPrice;
	char*			mExplain0;
	char*			mExplain1;
	char*			mExplain2;
};

const tItemDetailInfo * GetItemDetailInfo(BYTE itemKind);
eItemKind				GetKeyitemItemKind(BYTE keyitemKind);

char* GetItemPlateName(BYTE itemKind);
char* GetItemCardName(BYTE itemKind);
char* GetSongName(BYTE songType);


#endif //__ITEM_DEF_H__

 /**** end of file ****/
