/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ActionTaskManager.cpp
 * �^�X�N(�C�x���g�Ǘ�)�N���X
 * @author SPATZ.
 * @data   2012/04/04 14:48:17
 */
//---------------------------------------------------
#include "ActionTaskManager.h"
#include "AppLib/Collision/CollisionManager.h"
#include "Scene/SceneMainGame.h"
#ifdef _DEBUG
#include "AppLib/Input/InputManager.h"
#endif // _DEBUG

/*static*/
NOTIFY_FUNC	ActionTaskManager::mNotifyFunc = NULL;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/ActionTaskManager* const 
ActionTaskManager::GetInstance(void)
{
	static ActionTaskManager sInstance;
	return &sInstance;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ActionTaskManager::Setup(NOTIFY_FUNC func)
{
	mNotifyFunc = func;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ActionTaskManager::Update(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ActionTaskManager::UpdateActionTask(VECTOR* pVecStart, VECTOR* pVecEnd)
{
	const char* strCMD_NAME[eTASK_COLLISION_MAX] = {
		NULL,		// eTASK_NONE,		// 0
		"MOV",		// eTASK_MOVE,		// 1
		"FAL",		// eTASK_FALLDOWN,	// 2
		"EVT",		// eTASK_EVENT,		// 3
		"ENM",		// eTASK_ENEMY,		// 4
		"NAV",		// eTASK_NAVI,		// 5
	};

	tTaskInfo info;
	memset(&info, 0x00, sizeof(info));
	char* material_name;
	VECTOR normal;

	if (CollisionManager::GetInstance()->CheckTaskCollision(*pVecStart, *pVecEnd, (void *)&material_name, &normal)) {

		VECTOR vec = VNorm(VSub(*pVecEnd, *pVecStart));
		float vec_dot = VDot(vec, normal);
		
		// �����K���Ƃ��ā@�w XXX_YYY_ZZZ �x�Ƃ���
		// XXX �̓R�}���h���AYYY�̓p�����[�^�P�AZZZ�̓p�����[�^�Q�Ƃ���
		if (strlen(material_name) < 11/* =strlen("XXX_YYY_ZZZ") */) {
			// �s���R�}���h
			return;
		}

		char cmd[3+1];
		char v_1[3+1];
		char v_2[3+1];
		memcpy(cmd, &material_name[0], sizeof(cmd));
		cmd[3] = '\0';
		for (int iI = 1; iI < eTASK_COLLISION_MAX; iI ++) {
			if (strcmp(cmd, strCMD_NAME[iI]) == 0) {
				info.mTaskType = (eTaskType)iI;
				break;
			}
		}
		if (info.mTaskType == eTASK_NONE) {
			// �Y���R�}���h�����炸
			return;
		}

		memcpy(v_1, &material_name[4], sizeof(v_1));
		v_1[3] = '\0';
		memcpy(v_2, &material_name[8], sizeof(v_2));
		v_2[3] = '\0';

		info.mTaskValueA = atoi(v_1);
		info.mTaskValueB = atoi(v_2);

		/* �C�x���g�̏ꍇ�́Aplace index ���� enum index �ɕϊ����K�v */
		if (info.mTaskType == eTASK_EVENT) {
			info.mTaskValueA = ConvertPlaceIndex_TO_EventDefine(info.mTaskValueA);
		}

		// ��ʂɒʒm����
		this->NotifyAction(&info);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
ActionTaskManager::NotifyAction(tTaskInfo* pTaskInfo)
{
	return mNotifyFunc((void *)pTaskInfo);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ActionTaskManager::ActionTaskManager()
{
	
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ActionTaskManager::~ActionTaskManager()
{

}

 /**** end of file ****/
