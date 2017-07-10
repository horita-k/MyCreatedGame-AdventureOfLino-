/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * messagedef.cpp
 * @author SPATZ.
 * @data   2015/06/26 00:10:32
 */
//---------------------------------------------------
#include "messagedef.h"

/*=====================================*
 * �V���b�v�Ɋւ�����
 *=====================================*/
// ���x���j�A
const eItemKind sShop21_00[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_BIN_MAGENTA,
	eITEM_KIND_BIN_CYAN,
	eITEM_KIND_WALLET_C,
};
// �M���X
const eItemKind sShop13_00[] = {
	eITEM_KIND_FLAME_RING,
	eITEM_KIND_CHICK_PIERCE,
	eITEM_KIND_FAIRY_BRACELET,
};
// �`�X
const eItemKind sShop11[] = {
	eITEM_KIND_BIN_MAGENTA,
	eITEM_KIND_BIN_CYAN,
	eITEM_KIND_BIN_YELLOW,
	eITEM_KIND_WALLET_B,
};
// �W���g��
const eItemKind sShop35[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_BIN_MAGENTA,
	eITEM_KIND_BIN_CYAN,
	eITEM_KIND_BIN_YELLOW,
};
// �W�p���O
const eItemKind sShopXX[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_BIN_MAGENTA,
	eITEM_KIND_BIN_CYAN,
	eITEM_KIND_BIN_YELLOW,
};
// ���p���E�n�����H
const eItemKind sShopLapin024[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_HEART,
};
const eItemKind sShopLapin025[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_HEART,
};
const eItemKind sShopLapin016[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_HEART,
	eITEM_KIND_PLATE_BOMB,
};
const eItemKind sShopLapin036[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_HEART,
	eITEM_KIND_PLATE_BOMB,
	eITEM_KIND_PLATE_ARROW,
	eITEM_KIND_BIN_MAGENTA,
};
const eItemKind sShopLapin040[] = {
	eITEM_KIND_PORTION,
	eITEM_KIND_HEART,
	eITEM_KIND_PLATE_BOMB,
	eITEM_KIND_PLATE_ARROW,
	eITEM_KIND_BIN_MAGENTA,
	eITEM_KIND_BIN_YELLOW,
};
const eItemKind sShopCosplay21[] = {
	eITEM_KIND_COSPLAY_BAT,
	eITEM_KIND_COSPLAY_SUNOPHONE,
	eITEM_KIND_COSPLAY_PRONAMA,
	eITEM_KIND_COSPLAY_CHILNO,
	eITEM_KIND_COSPLAY_YUKARI,
	eITEM_KIND_COSPLAY_NAKANO,
};

// �W���g��
const eItemKind sShop14[] = {
	eITEM_KIND_COPY_ROD,
};

/* �V���b�v���e�[�u�� */
static const tShopInfo sShopInfoList[eSHOP_MAX] = {
	{sShop21_00,		(sizeof(sShop21_00)		/ sizeof(eItemKind)),},		// eSHOP_21_GIMURI,
	{sShop13_00,		(sizeof(sShop13_00)		/ sizeof(eItemKind)),},		// eSHOP_13_LAPAN,
	{sShop11,			(sizeof(sShop11)		/ sizeof(eItemKind)),},		//	eSHOP_011,					// 29	// �`���E�X
	{sShop35,			(sizeof(sShop35)		/ sizeof(eItemKind)),},		//	eSHOP_035,					// 30	// �W���g���E�X
	{sShopXX,			(sizeof(sShopXX)		/ sizeof(eItemKind)),},		//	eSHOP_0XX,					// 31	// �W�p���O�E�X
	{sShopLapin024,		(sizeof(sShopLapin024)	/ sizeof(eItemKind)),},		//	eSHOP_LAPIN_024,			// 32	// ���p���n�����H�E�X
	{sShopLapin025,		(sizeof(sShopLapin025)	/ sizeof(eItemKind)),},		//	eSHOP_LAPIN_025,			// 33	// ���p���n���S�E�X
	{sShopLapin016,		(sizeof(sShopLapin016)	/ sizeof(eItemKind)),},		//	eSHOP_LAPIN_016,			// 34	// ���p���X�E�X
	{sShopLapin036,		(sizeof(sShopLapin036)	/ sizeof(eItemKind)),},		//	eSHOP_LAPIN_036,			// 35	// ���p���z�R�E�X
	{sShopLapin040,		(sizeof(sShopLapin040)	/ sizeof(eItemKind)),},		//	eSHOP_LAPIN_040,			// 36	// ���p���n���b�p�E�X
	{sShopCosplay21,	(sizeof(sShopCosplay21) / sizeof(eItemKind)),},		//	eSHOP_COSPLAY_021,			// 37	// �R�X�v���E�X
	{sShop14,			(sizeof(sShop14)		/ sizeof(eItemKind)),},		//	eSHOP_014,					// 41	// ���p��w�E�X
};

/*=====================================*
 * ���b�Z�[�W�Ɋւ�����
 *=====================================*/
const ActionTaskManager::tTaskInfo sNpcTaskMove005_000			= {ActionTaskManager::eTASK_MOVE, 	5, 					0,};
const ActionTaskManager::tTaskInfo sNpcTaskMove000_B			= {ActionTaskManager::eTASK_MOVE, 	8, 					0,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent010_000 		= {ActionTaskManager::eTASK_EVENT,	eEVENT_10, 			0,};
const ActionTaskManager::tTaskInfo sNpcTaskMove021_000			= {ActionTaskManager::eTASK_MOVE, 	21, 				1,};
const ActionTaskManager::tTaskInfo sNpcTaskShop021_000			= {ActionTaskManager::eTASK_SHOP, 	eSHOP_21_GIMURI,	0,};
const ActionTaskManager::tTaskInfo sNpcTaskMove024_000			= {ActionTaskManager::eTASK_EVENT,	eEVENT_24, 			eEVENT_24_WARP_ENTRANCE,};
const ActionTaskManager::tTaskInfo sNpcTaskSave					= {ActionTaskManager::eTASK_SAVE, 	0, 					0,};
const ActionTaskManager::tTaskInfo sNpcTaskBattleQuest1 		= {ActionTaskManager::eTASK_EVENT,	eEVENT_21, 			eEVENT_21_BATTLE_QUEST_1,};
const ActionTaskManager::tTaskInfo sNpcTaskShop013_000			= {ActionTaskManager::eTASK_SHOP, 	eSHOP_13_ACCSE, 	0,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent029_012			= {ActionTaskManager::eTASK_EVENT,	eEVENT_29, 			eEVENT_29_MINO_GUARDIAN_OR_MOVE,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent021_BinShop		= {ActionTaskManager::eTASK_EVENT,	eEVENT_21, 			eEVENT_21_BIN_SHOP,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent010_002			= {ActionTaskManager::eTASK_EVENT,	eEVENT_10, 			eEVENT_10_RIDE_VEHICLE,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent016_014			= {ActionTaskManager::eTASK_EVENT,	eEVENT_16, 			eEVENT_16_SWORD_TREASURE,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent016_017			= {ActionTaskManager::eTASK_EVENT,	eEVENT_16, 			eEVENT_16_REVERT_USACHU,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent036_016			= {ActionTaskManager::eTASK_EVENT,	eEVENT_36, 			eEVENT_36_ADVICE_TROLLEY,};
const ActionTaskManager::tTaskInfo sNpcTaskEvent036_017			= {ActionTaskManager::eTASK_EVENT,	eEVENT_36, 			eEVENT_36_ADVICE_MINE,};
const ActionTaskManager::tTaskInfo sNpcTaskBattleQuest2 		= {ActionTaskManager::eTASK_EVENT,	eEVENT_08, 			eEVENT_08_BATTLE_QUEST_2,};
const ActionTaskManager::tTaskInfo sNpcTaskBattleQuest3 		= {ActionTaskManager::eTASK_EVENT,	eEVENT_29, 			eEVENT_29_BATTLE_QUEST_3,};
const ActionTaskManager::tTaskInfo sNpcTaskGetMagicFlame		= {ActionTaskManager::eTASK_EVENT,	eEVENT_29, 			eEVENT_29_GET_MAGIC_FLAME,};
const ActionTaskManager::tTaskInfo sNpcTaskGetMagicTornade		= {ActionTaskManager::eTASK_EVENT,	eEVENT_10, 			eEVENT_10_GET_MAGIC_TORNADE,};
const ActionTaskManager::tTaskInfo sNpcTaskGetMagicThunder		= {ActionTaskManager::eTASK_EVENT,	eEVENT_40, 			eEVENT_40_GET_MAGIC_THUNDER,};
const ActionTaskManager::tTaskInfo sNpcTaskHintStone			= {ActionTaskManager::eTASK_EVENT,	eEVENT_11, 			eEVENT_11_HINT_STONE,};
const ActionTaskManager::tTaskInfo sNpcTaskGetSongWildness		= {ActionTaskManager::eTASK_EVENT,	eEVENT_35, 			eEVENT_35_GET_SONG_WILDNESS,};
const ActionTaskManager::tTaskInfo sNpcTaskGetSongWing			= {ActionTaskManager::eTASK_EVENT,	eEVENT_35, 			eEVENT_35_GET_SONG_WING,};
const ActionTaskManager::tTaskInfo sNpcTaskGetSongHandling		= {ActionTaskManager::eTASK_EVENT,	eEVENT_48, 			eEVENT_48_GET_SONG_HANDLING,};
const ActionTaskManager::tTaskInfo sNpcTaskMoveUpperCastle		= {ActionTaskManager::eTASK_EVENT,	eEVENT_29, 			eEVENT_29_MOVE_UPPER_CASTLE,};
const ActionTaskManager::tTaskInfo sNpcTaskMoveLowerCastle		= {ActionTaskManager::eTASK_EVENT,	eEVENT_29, 			eEVENT_29_MOVE_LOWER_CASTLE,};
const ActionTaskManager::tTaskInfo sNpcTaskGetSongSun			= {ActionTaskManager::eTASK_EVENT,	eEVENT_35, 			eEVENT_35_GET_SONG_SUN,};
const ActionTaskManager::tTaskInfo sNpcTaskGetFaily				= {ActionTaskManager::eTASK_EVENT,	eEVENT_10, 			eEVENT_10_GET_FAILY,};
const ActionTaskManager::tTaskInfo sNpcTaskShop011				= {ActionTaskManager::eTASK_SHOP, 	eSHOP_011,			0,};								// 29	// �`���E�X
const ActionTaskManager::tTaskInfo sNpcTaskShop035				= {ActionTaskManager::eTASK_SHOP, 	eSHOP_035,			0,};								// 30	// �W���g���E�X
const ActionTaskManager::tTaskInfo sNpcTaskShop0XX				= {ActionTaskManager::eTASK_SHOP, 	eSHOP_0XX,			0,};								// 31	// �W�p���O�E�X
const ActionTaskManager::tTaskInfo sNpcTaskShopLAPIN_024		= {ActionTaskManager::eTASK_SHOP, 	eSHOP_LAPIN_024,	0,};								// 32	// ���p���n�����H�E�X
const ActionTaskManager::tTaskInfo sNpcTaskShopLAPIN_025		= {ActionTaskManager::eTASK_SHOP, 	eSHOP_LAPIN_025,	0,};								// 33	// ���p���n���S�E�X
const ActionTaskManager::tTaskInfo sNpcTaskShopLAPIN_016		= {ActionTaskManager::eTASK_SHOP, 	eSHOP_LAPIN_016,	0,};								// 34	// ���p���X�E�X
const ActionTaskManager::tTaskInfo sNpcTaskShopLAPIN_036		= {ActionTaskManager::eTASK_SHOP, 	eSHOP_LAPIN_036,	0,};								// 35	// ���p���z�R�E�X
const ActionTaskManager::tTaskInfo sNpcTaskShopLAPIN_040		= {ActionTaskManager::eTASK_SHOP, 	eSHOP_LAPIN_040,	0,};								// 36	// ���p���n���b�p�E�X
const ActionTaskManager::tTaskInfo sNpcTaskShopCOSPLAY_021		= {ActionTaskManager::eTASK_SHOP, 	eSHOP_COSPLAY_021,	0,};								// 37	// �R�X�v���E�X

const ActionTaskManager::tTaskInfo sNpcTaskMovePrison			= {ActionTaskManager::eTASK_EVENT,	eEVENT_13, 			eEVENT_13_GOTO_PRISON,};            // 38	// �S���C�x���g
const ActionTaskManager::tTaskInfo sNpcTaskMoveFressRoom		= {ActionTaskManager::eTASK_EVENT,	eEVENT_29, 			eEVENT_29_GO_TO_FRESS_ROOM,};		// 39	// �t���X�̕���
const ActionTaskManager::tTaskInfo sNpcTaskMovePrisonOut		= {ActionTaskManager::eTASK_MOVE,	25,					3,};			                    // 40	// �S���C�x���g�E�S������o��
const ActionTaskManager::tTaskInfo sNpcTaskShop014				= {ActionTaskManager::eTASK_SHOP, 	eSHOP_014,			0,};								// 41	// ���p��w�E�X
const ActionTaskManager::tTaskInfo sNpcTaskWiseMan				= {ActionTaskManager::eTASK_EVENT, 	eEVENT_14,			eEVENT_14_WISEMAN,};				// 42	// ���m��΂������
const ActionTaskManager::tTaskInfo sNpcTaskGoToOverSea			= {ActionTaskManager::eTASK_EVENT, 	eEVENT_11,			eEVENT_11_GOTO_OVERSEA,};			// 43	// �`�����̍�
const ActionTaskManager::tTaskInfo sNpcTaskBackToOverSea		= {ActionTaskManager::eTASK_EVENT, 	eEVENT_11,			eEVENT_11_BACKTO_OVERSEA,};			// 44	// ���̍����`


const ActionTaskManager::tTaskInfo* spNpcTaskTable[eMSG_TASK_MAX] = {
	&sNpcTaskMove005_000,				// 0
	&sNpcTaskMove000_B,
	&sNpcTaskEvent010_000,
	&sNpcTaskMove021_000,
	&sNpcTaskShop021_000,
	&sNpcTaskMove024_000,				// 5
	&sNpcTaskSave,
	&sNpcTaskBattleQuest1,				// 7	// �������E�n��\�I
	&sNpcTaskShop013_000,
	&sNpcTaskEvent029_012,				// 9	//�~�m�^�E���X�ԕ� or �ړ��C�x���g
	&sNpcTaskEvent021_BinShop,			// 10	// ���΂������̃r������
	&sNpcTaskEvent010_002,				// 11
	&sNpcTaskEvent016_014,				// 12	// ���X�C�b�`�C�x���g
	&sNpcTaskEvent016_017,				// 13	// �������イ�߂��C�x���g
	&sNpcTaskEvent036_016,				// 14	// �g���b�R��������C�x���g
	&sNpcTaskEvent036_017,				// 15	// �z�R�����C�x���g
	&sNpcTaskBattleQuest2,				// 16	// �������E�{��
	&sNpcTaskBattleQuest3,				// 17	// �������E�S�����
	&sNpcTaskGetMagicFlame,				// 18	// ���@�擾�E��
	&sNpcTaskGetMagicTornade,			// 19	// ���@�擾�E����
	&sNpcTaskGetMagicThunder,			// 20	// ���@�擾�E��
	&sNpcTaskHintStone,					// 21	// �Δ�̖��@�q���g
	&sNpcTaskGetSongWildness,			// 22	// �S�擾�E�����̉S 
	&sNpcTaskGetSongWing,				// 23	// �S�擾�E���̉S
	&sNpcTaskGetSongHandling,			// 24	// �S�擾�E����̉S
	&sNpcTaskMoveUpperCastle,			// 25	// ��㕔��
	&sNpcTaskMoveLowerCastle,			// 26	// �鉺����
	&sNpcTaskGetSongSun,				// 27	// �S�擾�E���z�̉S
	&sNpcTaskGetFaily,					// 28	// �d���擾�C�x���g
	&sNpcTaskShop011,		    		// 29	// �`���E�X
	&sNpcTaskShop035,		    		// 30	// �W���g���E�X
	&sNpcTaskShop0XX,		    		// 31	// �W�p���O�E�X
	&sNpcTaskShopLAPIN_024,     		// 32	// ���p���n�����H�E�X
	&sNpcTaskShopLAPIN_025,     		// 33	// ���p���n���S�E�X
	&sNpcTaskShopLAPIN_016,     		// 34	// ���p���X�E�X
	&sNpcTaskShopLAPIN_036,     		// 35	// ���p���z�R�E�X
	&sNpcTaskShopLAPIN_040,     		// 36	// ���p���n���b�p�E�X
	&sNpcTaskShopCOSPLAY_021,   		// 37	// �R�X�v���E�X

	&sNpcTaskMovePrison,		        // 38	// �S���C�x���g
	&sNpcTaskMoveFressRoom,				// 39	// �t���X�̕���
	&sNpcTaskMovePrisonOut,		        // 40	// �S���C�x���g�E�S������o��
	&sNpcTaskShop014,					// 41	// ���p��w�E�X
	&sNpcTaskWiseMan,					// 42	// ���m��΂������

	&sNpcTaskGoToOverSea,				// 43	// �`�����̍�
	&sNpcTaskBackToOverSea,				// 44	// ���̍����`

};

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
const ActionTaskManager::tTaskInfo* GetNpcTaskTable(int task_id)
{
	APP_ASSERT_PRINT( (task_id < eMSG_TASK_MAX),
					  "Select task_id %d is Invalid value \n", task_id);
	return spNpcTaskTable[task_id];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
const tShopInfo* GetShopInfoList(int shopIndex)
{
	APP_ASSERT_PRINT( (shopIndex < eSHOP_MAX),
					  "Select shopIndex %d is Invalid value", shopIndex);
	return &sShopInfoList[shopIndex];
}


/**** end of file ****/
