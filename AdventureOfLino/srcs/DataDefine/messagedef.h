/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * MESSAGE�t�@�C���̒�`	
 * @author SPATZ.
 * @data   2012/04/04 16:15:43
 */
//---------------------------------------------------
#ifndef __MESSAGE_DEF_H__
#define __MESSAGE_DEF_H__

#include "dxlib.h"
#include "AppLib/Basic/Basic.h"
#include "Scene/EventFunctions.h"
#include "Scene/ActionTaskManager.h"
#include "CharaDef.h"
#include "ItemDef.h"


/*=====================================*
 * �V���b�v���
 *=====================================*/
struct tShopInfo {
	const eItemKind*	mpItemList;
	BYTE				mItemNum;
};

enum {
	eSHOP_21_GIMURI = 0,
	eSHOP_13_ACCSE,
	eSHOP_011,					// 29	// �`���E�X
	eSHOP_035,					// 30	// �W���g���E�X
	eSHOP_0XX,					// 31	// �W�p���O�E�X
	eSHOP_LAPIN_024,			// 32	// ���p���n�����H�E�X
	eSHOP_LAPIN_025,			// 33	// ���p���n���S�E�X
	eSHOP_LAPIN_016,			// 34	// ���p���X�E�X
	eSHOP_LAPIN_036,			// 35	// ���p���z�R�E�X
	eSHOP_LAPIN_040,			// 36	// ���p���n���b�p�E�X
	eSHOP_COSPLAY_021,			// 37	// �R�X�v���E�X
	eSHOP_014,					// 41	// ���p��w�E�X

	eSHOP_MAX,
};

/*=====================================*
 * ���b�Z�[�W��̊J�n�^�X�N
 *=====================================*/
// ���\�[�X�̕��ɂ��ǉ����鎖�I
enum {
	eMSG_TASK_MOVE_005_000 = 0,			// 0
	eMSG_TASK_MOVE_000_B,				// 1
	eMSG_TASK_EVENT_010_000,			// 2
	eMSG_TASK_MOVE_021_000,				// 3
	eMSG_TASK_SHOP_021,					// 4	// ���x���j�A�E�X
	eMSG_TASK_MOVE_024_000,				// 5
	eMSG_TASK_SAVE,						// 6
	eMSG_TASK_BATTLE_QUEST_1,			// 7	// �������E�n��\�I
	eMSG_TASK_SHOP_ACS_013,				// 8	// �M���X�E�X
	eMSG_TASK_EVENT_029_012,			// 9	// �~�m�^�E���X�ԕ� or �ړ��C�x���g
	eMSG_TASK_EVENT_021_BIN_SHOP,		// 10	// ���΂������̃r������
	eMSG_TASK_RIDE_VEHICLE,				// 11
	eMSG_TASK_EVENT_016_014,			// 12	// ���X�C�b�`�C�x���g
	eMSG_TASK_EVENT_016_017,			// 13	// �������イ�߂��C�x���g
	eMSG_TASK_EVENT_036_016,			// 14	// �g���b�R��������C�x���g
	eMSG_TASK_EVENT_036_017,			// 15	// �z�R�����C�x���g
	eMSG_TASK_BATTLE_QUEST_2,			// 16	// �������E�{��
	eMSG_TASK_BATTLE_QUEST_3,			// 17	// �������E�S�����
	eMSG_TASK_GET_MAGIC_FLAME,			// 18	// ���@�擾�E��
	eMSG_TASK_GET_MAGIC_TORNADE,		// 19	// ���@�擾�E����
	eMSG_TASK_GET_MAGIC_THUNDER,		// 20	// ���@�擾�E��
	eMSG_TASK_HINT_STONE,				// 21	// �Δ�̖��@�q���g
	eMSG_TASK_GET_SONG_WILDNESS,		// 22	// �S�擾�E�����̉S
	eMSG_TASK_GET_SONG_WING,			// 23	// �S�擾�E���̉S
	eMSG_TASK_GET_SONG_HANDLING,		// 24	// �S�擾�E����̉S
	eMSG_TASK_MOVE_UPPER_CASTLE,		// 25	// ��㕔��
	eMSG_TASK_MOVE_LOWER_CASTLE,		// 26	// �鉺����
	eMSG_TASK_GET_SONG_SUN,				// 27	// �S�擾�E���z�̉S
	eMSG_TASK_GET_FAILY,				// 28	// �d���擾�C�x���g
	eMSG_TASK_SHOP_011,					// 29	// �`���E�X
	eMSG_TASK_SHOP_035,					// 30	// �W���g���E�X
	eMSG_TASK_SHOP_0XX,					// 31	// �W�p���O�E�X
	eMSG_TASK_SHOP_LAPIN_024,			// 32	// ���p���n�����H�E�X
	eMSG_TASK_SHOP_LAPIN_025,			// 33	// ���p���n���S�E�X
	eMSG_TASK_SHOP_LAPIN_016,			// 34	// ���p���X�E�X
	eMSG_TASK_SHOP_LAPIN_036,			// 35	// ���p���z�R�E�X
	eMSG_TASK_SHOP_LAPIN_040,			// 36	// ���p���n���b�p�E�X
	eMSG_TASK_SHOP_COSPLAY_021,			// 37	// �R�X�v���E�X
	
	eMSG_TASK_MOVE_PRISON,				// 38	// �S���C�x���g
	eMSG_TASK_MOVE_FRESS_ROOM,			// 39	// �t���X�̕���
	eMSG_TASK_MOVE_PRISON_OUT,			// 40	// �S���C�x���g�E�S������o��
	
	eMSG_TASK_SHOP_014,					// 41	// ���p��w�E�X
	eMSG_TASK_WISE_MAN,					// 42	// ���m��΂������

	eMSG_TASK_GOTO_OVERSEA,				// 43	// �`���a�̍�
	eMSG_TASK_BACKTO_OVERSEA,			// 44	// �a�̍����`


	eMSG_TASK_MAX,
};

enum eUseToon {
	eUSE_TOON_DISABLE,	// �g�D�[�����g�p
	eUSE_TOON_ENABLE,	// �g�D�[���g�p

	eUSE_TOON_MAX,
};

// NPC���擾�֐�
const ActionTaskManager::tTaskInfo* GetNpcTaskTable(int task_id);
const tShopInfo* GetShopInfoList(int shopIndex);


#endif // __MESSAGE_DEF_H__

 /**** end of file ****/
