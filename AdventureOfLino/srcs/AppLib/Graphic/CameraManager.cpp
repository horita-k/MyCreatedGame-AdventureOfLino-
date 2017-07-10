/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �J���������}�l�[�W���N���X
 * @author SPATZ.
 * @data   2011/11/21 00:06:02
 */
//---------------------------------------------------
#include "AppLib/Basic/Basic.h"
#include "AppLib/Input/InputManager.h"
#include "AppLib/Collision/CollisionManager.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "Charactor/CharaBase.h"
#include "Charactor/CharaPlayer.h"
#include "Gimmick/GimmickBase.h"
#include "CameraManager.h"
#include "Utility/Utility.h"
#include "Scene/SceneManager.h"
#include "Scene/TotalInfo.h"

#include <math.h>

/*=====================================*
 * static
 *=====================================*/
//static float sTargetHeightCenter = 100.0f;
static const float	sTargetHeightCenter = 115.0f;		// �J�����ʒu�������グ��

/*static*/CameraManager::CameraFunc
CameraManager::mCamStateFunc[eCAM_STATE_MAX] = {
	&CameraManager::setPatternDummy,
	&CameraManager::setPatternNormal,
	&CameraManager::setPatternLockon,
	&CameraManager::setPatternHookShot,
	&CameraManager::setPatternBoomerang,
	&CameraManager::setPatternMessage,
	&CameraManager::setPatternShowMap,
	&CameraManager::setPatternReset,
	&CameraManager::setPatternArchery,
	&CameraManager::setPatternLookAround,
	&CameraManager::setPatternTrolley,
};

/*static*/CameraManager::CameraFunc
CameraManager::mCameraFunc[eCAMERA_MAX] = {
	&CameraManager::cameraFuncNormal,
	&CameraManager::cameraFuncDelay,
	&CameraManager::cameraFuncDelayToNormal,
	&CameraManager::cameraFuncMove,
	&CameraManager::cameraFuncShake,
};

/*=====================================*
 * define
 *=====================================*/

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/CameraManager * const
CameraManager::GetInstance(void)
{
	static CameraManager sInstance;
	return &sInstance;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
CameraManager::CameraManager()
{
	memset(&mShakeWork, 0x00, sizeof(mShakeWork));
	memset(&mMoveWork, 0x00, sizeof(mMoveWork));
	memset(&mDelayWork, 0x00, sizeof(mDelayWork));

	memset(&mQuakeWork, 0x00, sizeof(mQuakeWork));
	
	mCamraType	= eCAMERA_NORMAL;

	mCamVecZ	= VGet(0.0f, 0.0f, -1.0f);
	mPosition	= ZERO_VEC;
	mTarget		= VGet(0.0f, 0.0f, 1.0f);
	mUp			= VGet(0.0f, 1.0f, 0.0f);
	mLength		= 300.0f;
	mArcheryCamType	= eARCHERY_CAM_TYPE_NORMAL;

	mNearClip	= 0.0f;
	mFarClip	= 0.0f;

	mTmpPos		= ZERO_VEC;
	mTmpTar		= VGet(0.0f, 0.0f, 1.0f);

	mFov = 0.0f;

	mCamState	= eCAM_STATE_DUMMY;

	mpSetPatternCharaBase	= NULL;
	mSetPatternDist			= 0.0f;

	memset(&mPatternTrolleyParam, 0, sizeof(mPatternTrolleyParam));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
CameraManager::~CameraManager()
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::Create(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::Setup(void)
{
	/* ���ߖ@�J�����̐ݒ� */
	SetPerspectiveCamera(kDEFAULT_PERSPECTIVE_FOV);

	/* Near Far �N���b�v�ݒ� */
	SetDefaultNearFarClip();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::Update(void)
{
	(this->*mCamStateFunc[mCamState])();

	(this->*mCameraFunc[mCamraType])();

	/* �J�����U������ (���Ǌ֐����������̎n��... */
	VECTOR set_pos;
	set_pos = mPosition; /* mPosition �Ƀv���X�A���t�@�����l��ݒ� */
	float shake_second	= mQuakeWork.mSetSecond;
	float max_shake_size= mQuakeWork.mSetSize;
	float shake_speed	= mQuakeWork.mSetSpeed;
	if (mQuakeWork.mIsActive) {
		set_pos.y += (max_shake_size * sin(mQuakeWork.mCount * shake_speed));
		mQuakeWork.mCount += 1.0f;
		if (mQuakeWork.mCount > (shake_second * 60.0f) ) {
			StopVibeCamera();
		}
	}

	SetCameraPositionAndTargetAndUpVec(set_pos, mTarget, mUp);

	// �P�t���O�̒l��ۑ�
	mDelayWork.mPrevPosition	= mPosition;
	mDelayWork.mPrevTarget		= mTarget;

	PRINT_SCREEN(GetColor(255, 255, 0), "Camera {State:%d Far:%.1f Near:%.2f, \n", mCamState, mFarClip, mNearClip);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::Destroy(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternDummy(void)
{
	// �������Ȃ�
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternNormal(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	VECTOR crossX, crossY;

	int value_x, value_y;
	value_x = value_y = 0;
	InputManager::GetInstance()->GetJoyAnalogRight(&value_x, &value_y);

	/* �}�E�X�ł̃J�����ؑ� */
	static int sMouseX = 0;
	static int sMouseY = 0;
	static float sScale = 8.0f;
	static float sAmp = 0.85f;
	if (CHECK_PRESS_MOUSE(MOUSE_INPUT_RIGHT)) {
		int mouse_x, mouse_y;
		InputManager::GetInstance()->GetMouseMoveValue(&mouse_x, &mouse_y);
		if ((abs(mouse_x) + abs(mouse_y)) == 0) {
			// �������������ꍇ�͂Ƃ߂�
			sMouseX = (int)((float)sMouseX * 0.2f);
			sMouseY = (int)((float)sMouseY * 0.2f);
		} else {
			// ����ȊO�͒ʏ�ړ�
			sMouseX += (int)((float)mouse_x * sScale);
			sMouseY += (int)((float)mouse_y * sScale);
		}
		value_x -= sMouseX;
		value_y -= sMouseY;
	} else {
		sMouseX = (int)((float)sMouseX * sAmp);
		sMouseY = (int)((float)sMouseY * sAmp);
		value_x -= sMouseX;
		value_y -= sMouseY;
	}

	// �J�����̋������X�V
	updateCamLength();

	VECTOR vecScalarZ = VScale(mCamVecZ, mLength);
	
	static const float move_theta = tan(Utility::DegToRad(0.16f));
	float move_value = move_theta * mLength;

	crossX = VCross(VGet(0.0f, 1.0f, 0.0f), vecScalarZ);
	crossY = VCross(crossX, vecScalarZ);
	crossX = VNorm(crossX);
	crossX = VScale(crossX, ((float)value_x * -0.02f * move_value));

	// ���K��
	crossY = VNorm(crossY);
	crossY = VScale(crossY, ((float)value_y * 0.02f * move_value));
	
	if ((mCamVecZ.y > 0.9f) && (crossY.y > 0.0f)) {
		crossY = ZERO_VEC;
	}
	if ((mCamVecZ.y < -0.9f) && (crossY.y < 0.0f)) {
		crossY = ZERO_VEC;
	}
	
	vecScalarZ = VAdd(VAdd(vecScalarZ, crossX), crossY);
	mCamVecZ = VNorm(vecScalarZ);		// �p�x�����肳����

	// �ǔ�����s���O�̃J�����̉����W�����߂�
	VECTOR tmp_cam_pos;
	mTarget = p_mine->Position();
	mTarget.y += (sTargetHeightCenter);
	tmp_cam_pos = VAdd(mTarget, vecScalarZ);
 
	VECTOR hitpos, normal;
	int coll_index = 0;
	if (CollisionManager::GetInstance()->CheckWallCollision(mTarget, tmp_cam_pos, &hitpos, &coll_index)) {
		// �M�~�b�N�R���W�����łȂ��ꍇ�̂݃J�����̕␳���s��
		if (GimmickBase::GetGimmickBaseFromWallCollIndex(coll_index) == NULL) {
			// �R���W�����ɓ��������J�����̍��W���擾����.
			getCollisionCameraPos(&tmp_cam_pos, hitpos);
			StartDeleyCamera(eDELAY_SPEED_HIGH, true);
		}
	}
	else {
		coll_index = CollisionManager::GetInstance()->CheckEarthCollision(mTarget, tmp_cam_pos, &hitpos, &normal);
		if (coll_index != NOT_ON_COLLISION) {
			// �M�~�b�N�R���W�����łȂ��ꍇ�̂݃J�����̕␳���s��
			if (GimmickBase::GetGimmickBaseFromEarthCollIndex(coll_index) == NULL) {
				// �R���W�����ɓ��������J�����̍��W���擾����.
				getCollisionCameraPos(&tmp_cam_pos, hitpos);
				StartDeleyCamera(eDELAY_SPEED_HIGH, true);
			}
		}
	}

	mPosition = tmp_cam_pos;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternLockon(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	CharaBase* p_your = (CharaBase *)p_mine->GetLockonChara();

	VECTOR vec, tmp;
	VECTOR update_target, tmp_cam_pos;
	VECTOR posA, posB;

	posA.x = p_mine->Position().x;
	posA.y = 0.0f;
	posA.z = p_mine->Position().z;

	p_your->GetLockonPosition(&posB);
	posB.y = 0.0f;
	
	vec = VSub(posA, posB);

	// �J�����̋������X�VD
	updateCamLength();

	float dist = VSize(vec);
	float total_size = p_mine->BodySize() + p_your->BodySize();

	if ((dist) >= (total_size * 0.2f)) {

		tmp = p_mine->Position();
		tmp.y += sTargetHeightCenter;

		mCamVecZ = VNorm(vec);

		tmp_cam_pos = VAdd(tmp, VScale(mCamVecZ, mLength));
		tmp_cam_pos.y += sTargetHeightCenter;

		VECTOR tmp_mine = p_mine->Position();
		VECTOR tmp_your;
		p_your->GetLockonPosition(&tmp_your);
		update_target = VScale(tmp_mine, 1.0f);
		update_target = VAdd(update_target, VScale(tmp_your, 3.0f));
		update_target = VScale(update_target, 0.25f);
		update_target.y += sTargetHeightCenter;
		mTarget = update_target;

		VECTOR hitpos, normal;
		if (CollisionManager::GetInstance()->CheckWallCollision(mTarget, tmp_cam_pos, &hitpos, &normal)) {

			float dist_coll, dist_player;
			dist_coll	= VSize(VSub(mTarget, hitpos));
			dist_player = VSize(VSub(mTarget, tmp_mine));

			if (dist_coll > dist_player) {
				// �R���W�����ɓ��������J�����̍��W���擾����.
				getCollisionCameraPos(&tmp_cam_pos, hitpos);
				StartDeleyCamera(eDELAY_SPEED_HIGH, true);
			}
		}

		mPosition = tmp_cam_pos;
	}
}

//---------------------------------------------------
/**
 * �t�b�N�V���b�g���̃J����
 */
//---------------------------------------------------
void
CameraManager::setPatternHookShot(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	
	bool result = false;

	result = p_mine->GetFramePosFromName(&mPosition, "����");
	APP_ASSERT(result, "Failed GetFramePosFromNamn\n");

	result = p_mine->GetFramePosFromName(&mTarget, "���ڐ�");
	APP_ASSERT(result, "Failed GetFramePosFromNamn\n");

	mCamVecZ = VNorm(VSub(mPosition, mTarget));
}

//---------------------------------------------------
/**
 * �u�[���������̃J����
 */
//---------------------------------------------------
void
CameraManager::setPatternBoomerang(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	p_mine->GetCameraGeneralVec(&mTarget);

	VECTOR vecZ, chara_pos;
	chara_pos = p_mine->Position();
	chara_pos.y += (p_mine->BodyHeight() * 1.0f);

	mCamVecZ = VNorm(VSub(chara_pos, mTarget));
	vecZ = VScale(mCamVecZ, mLength);

	// �ǔ�����s���O�̃J�����̉����W�����߂�
	VECTOR tmp_cam_pos;
	tmp_cam_pos = VAdd(chara_pos, vecZ);
	tmp_cam_pos.y = chara_pos.y;

	VECTOR tmp_cam_tar;
	tmp_cam_tar = VAdd(chara_pos, VScale(mCamVecZ, -5000.0f));


	VECTOR hitpos;
	int coll_index = 0;

	if (CollisionManager::GetInstance()->CheckWallCollisionNearest(tmp_cam_tar,
																   tmp_cam_pos, 
																   &hitpos, 
																   &coll_index)) {
		// �M�~�b�N�R���W�����łȂ��ꍇ�̂݃J�����̕␳���s��
		if (GimmickBase::GetGimmickBaseFromWallCollIndex(coll_index) == NULL) {
			// �R���W�����ɓ��������ʒu���L������艜����O���H
			VECTOR vecA, vecB;
			vecA = VNorm(VSub(tmp_cam_tar, chara_pos) );
			vecB = VNorm(VSub(hitpos, chara_pos) );

			float vec_dot;
			vec_dot = VDot(vecA, vecB);
			PRINT_SCREEN(COLOR_RED, "vec_dot : %.2f \n", vec_dot);

			// ��O�ɓ��������ꍇ
			if (vec_dot < 0.0f) {
				// �R���W�����ɓ��������J�����̍��W���擾����.
				getCollisionCameraPos(&tmp_cam_pos, hitpos);
				StartDeleyCamera(eDELAY_SPEED_HIGH, true);
			}

		}
	}
	else {
		coll_index = CollisionManager::GetInstance()->CheckEarthCollisionNearest(tmp_cam_pos,
																				 tmp_cam_tar, 
																				 &hitpos);
		if (coll_index != NOT_ON_COLLISION) {
			// �M�~�b�N�R���W�����łȂ��ꍇ�̂݃J�����̕␳���s��
			if (GimmickBase::GetGimmickBaseFromEarthCollIndex(coll_index) == NULL) {

				// �R���W�����ɓ��������ʒu���L������艜����O���H
				VECTOR vecA, vecB;
				vecA = VNorm(VSub(tmp_cam_tar, chara_pos) );
				vecB = VNorm(VSub(hitpos, chara_pos) );

				float vec_dot;
				vec_dot = VDot(vecA, vecB);
				PRINT_SCREEN(COLOR_RED, "vec_dot : %.2f \n", vec_dot);

				// ��O�ɓ��������ꍇ
				if (vec_dot < 0.0f) {
					// �R���W�����ɓ��������J�����̍��W���擾����.
					getCollisionCameraPos(&tmp_cam_pos, hitpos);
					StartDeleyCamera(eDELAY_SPEED_HIGH, true);
				}

			}
		}
	}

	mPosition = tmp_cam_pos;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternMessage(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	ModelCollision* p_your = (ModelCollision *)mpSetPatternCharaBase;

	VECTOR vec, cross;
	float length;
	vec = VSub(p_mine->Position(), p_your->Position());
	length = VSize(vec);
	if (length < 100.0f) {
		length = 100.0f;
	}

	vec = VNorm(vec);
	cross = VCross(vec, VGet(0.0f, 1.0f, 0.0f));
	cross = VScale(cross, 1.2f);

	VECTOR tmpA, tmpB;
	tmpA = p_mine->Position();
	tmpA.y += (p_mine->BodyHeight() * 0.5f);
	tmpB = p_your->Position();
	tmpB.y += (p_your->BodyHeight() * 0.75f);
	mTarget = VScale(VAdd(tmpA, tmpB), 0.5f);

	static const float sVecRate = 1.1f;
	static const float sHeightRate = 0.1f;

	tmpA = vec;
	tmpA = VAdd(tmpA, cross);
	tmpA = VAdd(tmpA, VGet(0.0f, 1.0f, 0.0f));
	tmpA = VScale(tmpA, (length * sVecRate));
	mPosition = VAdd(p_mine->Position(), tmpA);
	mPosition.y += p_mine->BodyHeight() * sHeightRate;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternShowMap(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	mPosition = mTarget;
	mPosition.y += mSetPatternDist;
	mUp = VGet(0.0f, 0.0f, 1.0f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternArchery(void)
{
	VECTOR now_tar, now_pos;

	CharaPlayer* p_player = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	p_player->GetCameraGeneralVec(&now_tar);

	updateCamLengthArchery();

	VECTOR vecZ, chara_pos, crossX;
	float default_dist;

	chara_pos = p_player->Position();
	chara_pos.y += (p_player->BodyHeight() * 1.2f);

	vecZ = VSub(chara_pos, now_tar);
	default_dist = VSize(vecZ);

	float sArcheryCamDistTable[eARCHERY_CAM_TYPE_MAX] = {
		-(default_dist * 0.6f),			// eARCHERY_CAM_TYPE_FAR
		200.0f,							// eARCHERY_CAM_TYPE_NORMAL
		400.0f,							// eARCHERY_CAM_TYPE_NEAR
	};

	crossX = VCross(VNorm(vecZ), VGet(0.0f, 1.0f, 0.0f)); // normalize ������ԂŊO�ς̒l���擾
	crossX = VScale(crossX, -140.0f);
	vecZ = VNorm(VAdd(vecZ, crossX));
	mCamVecZ = vecZ;

	vecZ = VScale(vecZ, (sArcheryCamDistTable[mArcheryCamType] + default_dist));

	now_pos = VAdd(now_tar, vecZ);
	now_pos.y -= 50.0f;

	if (vecZ.y < 0.0f) {
		if (now_pos.y < (chara_pos.y - 180.0f)) {
			now_pos.y = (chara_pos.y - 180.0f);
		}
	}

	VECTOR prev_pos, prev_tar, moved_vec;
	p_player->GetMovedVec(&moved_vec);
	prev_pos = VAdd(mPosition,	moved_vec);
	prev_tar = VAdd(mTarget,	moved_vec);

	VECTOR vec;
	vec = VSub(now_pos, prev_pos);
	mPosition	= VAdd(VScale(vec, 0.05f), prev_pos);
	vec = VSub(now_tar, prev_tar);
	mTarget		= VAdd(VScale(vec, 0.05f), prev_tar);

	if (p_player->GetMatrixType() == ModelBase::eMATRIX_TYPE_MATRIX) {
		mPosition = now_pos;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternLookAround(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	
	VECTOR posA, posB;
	bool result = false;

	result = p_mine->GetFramePosFromName(&posA, "����");
	APP_ASSERT(result, "Failed GetFramePosFromNamn\n");

	result = p_mine->GetFramePosFromName(&posB, "���ڐ�");
	APP_ASSERT(result, "Failed GetFramePosFromNamn\n");

	static const float sDiffPosY = 30.0f;
	mTarget		= VGet(posB.x, posB.y - sDiffPosY, posB.z);
	mPosition	= VGet(posA.x, posA.y - sDiffPosY, posA.z);

	mCamVecZ = VNorm(VSub(mPosition, mTarget));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternTrolley(void)
{
	CharaPlayer* p_player = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR ch_pos = p_player->Position();
	VECTOR trolley_vec, tmp_pos;
	static const float rate = 0.01f;

	mLength = mPatternTrolleyParam.mCamLength;

	mUp		= UP_VEC;

	// Target�̊m��
	mTarget = VGet(ch_pos.x, (ch_pos.y + sTargetHeightCenter), ch_pos.z);
	
	// Target�̈ʒu����g���b�R�����Ƌt�̃x�N�g���̐�ɉ����W��ݒ�
	trolley_vec = mPatternTrolleyParam.mVector;
	trolley_vec = VScale(VAdd(mPatternTrolleyParam.mVector, 
							  VScale(mPatternTrolleyParam.mPrevVector, 9.0f)), 1.0f/10.0f);

	tmp_pos = VAdd(mTarget, VScale(trolley_vec, -mLength));

	// ---- ���ʂ���̃J�����ł͌��ɂ����ׁA���Əc�ɏ������炷 ----
	VECTOR crossX = VCross(trolley_vec, UP_VEC);
	//	tmp_pos = VAdd(tmp_pos, VScale(crossX, -460.0f));
	tmp_pos = VAdd(tmp_pos, VScale(crossX, mPatternTrolleyParam.mCamWidth));

	//	tmp_pos.y += 300.0f;
	tmp_pos.y += mPatternTrolleyParam.mCamHeight;

	/* �P�t���O�̏����X�V */
	mPatternTrolleyParam.mPrevVector = trolley_vec;

	mPosition = tmp_pos;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::setPatternReset(void)
{
	enum eResetCamState {
		eRESET_CAM_NONE = 0,
		eRESET_CAM_RIGHT,
		eRESET_CAM_LEFT,
		eRESET_CAM_FINISH,
	};
	static eResetCamState sResetCamState = eRESET_CAM_NONE;
	static float sNowRot = 0.0f;
	static float sTarRot = 0.0f;

	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	int value_x = 0;
	float next_rot;
	VECTOR vec;

	// ����̂ݎ��s
	if (sResetCamState == eRESET_CAM_NONE) {

		sTarRot = p_mine->Rotation().y;
		sTarRot += PHI_F;
		if (sTarRot > (PHI_F * 2.0f)) {
			sTarRot -= (PHI_F * 2.0f);
		}

		vec = VSub(mTarget, mPosition);
		sNowRot = atan2(vec.x, vec.z);
		Utility::CorrectAngleFrom000To360(&sNowRot);

		// �E�������A���������𒲂ׂ�
		if (abs(sTarRot - sNowRot) > PHI_F) {
			// �Ⴂ���𑫂�
			if (sTarRot > sNowRot) {
				sNowRot += (PHI_F * 2.0f);
			} else {
				sTarRot += (PHI_F * 2.0f);
			}
		}

		if (abs(sTarRot - sNowRot) < Utility::DegToRad(5.0f)) {
			// �ړ��͈͏��Ȃ��ꍇ�͏I��������
			sNowRot = sTarRot;
			sResetCamState = eRESET_CAM_FINISH;
		} else if (sTarRot > sNowRot) {
			sResetCamState = eRESET_CAM_LEFT;
			PRINT_CONSOLE("Move Left \n");
		} else {
			sResetCamState = eRESET_CAM_RIGHT;
			PRINT_CONSOLE("Move Right \n");
		}
	}

	// �J�����̏�Ԃ𔻕ʂ��Ĉړ��ʂ����肳����
	if (sResetCamState == eRESET_CAM_RIGHT) {
		value_x = (int)mLength * 10;
	} else if (sResetCamState == eRESET_CAM_LEFT) {
		value_x = (int)mLength * -10;
	}

	
	VECTOR crossX, crossY;
	VECTOR vecScalarZ = VScale(mCamVecZ, mLength);

	crossX = VCross(VGet(0.0f, 1.0f, 0.0f), vecScalarZ);
	crossY = VCross(crossX, vecScalarZ);
	crossX = VNorm(crossX);
	crossX = VScale(crossX, ((float)value_x * -0.02f));

	vecScalarZ = VAdd(vecScalarZ, crossX);

	// �����_�̓L�����N�^�[
	mTarget = p_mine->Position();
	mTarget.y += sTargetHeightCenter;

	mCamVecZ = VNorm(vecScalarZ);

	// �ǔ�����s���O�̃J�����̉����W�����߂�
	VECTOR tmp_cam_pos;
	tmp_cam_pos = VAdd(mTarget, vecScalarZ);
	
	vec = VSub(mTarget, tmp_cam_pos);
	next_rot = atan2(vec.x, vec.z);
	Utility::CorrectAngleFrom000To360(&next_rot);

	if (abs(next_rot - sNowRot) > PHI_F) {
		// �Ⴂ���𑫂�
		if (next_rot > sNowRot) {
			sNowRot += (PHI_F * 2.0f);
		} else {
			next_rot += (PHI_F * 2.0f);
		}
	}

	// �E�������A���������𒲂ׂ�
	if (abs(sTarRot - sNowRot) > PHI_F) {
		// �Ⴂ���𑫂�
		if (sTarRot > sNowRot) {
			sNowRot += (PHI_F * 2.0f);
		} else {
			sTarRot += (PHI_F * 2.0f);
		}
	}

	if (((sTarRot >= sNowRot) && (sTarRot < next_rot)) ||
		((sTarRot > next_rot) && (sTarRot <= sNowRot)) ||
		(sResetCamState == eRESET_CAM_FINISH)) {
		
		VECTOR add_pos;
		float tmp_pos_y;
		tmp_pos_y = tmp_cam_pos.y;
		add_pos.x = (sin(sTarRot+PHI_F) * mLength);
		add_pos.y = 0.0f;
		add_pos.z = (cos(sTarRot+PHI_F) * mLength);
		tmp_cam_pos = VAdd(mTarget, add_pos);
		tmp_cam_pos.y = tmp_pos_y;

		sResetCamState = eRESET_CAM_NONE;
		ChangeCameraState(eCAM_STATE_NORMAL);
	}
	sNowRot = next_rot;

	VECTOR hitpos, normal;
	if (CollisionManager::GetInstance()->CheckWallCollision(mTarget, tmp_cam_pos, &hitpos, &normal)) {
			
		// �R���W�����ɓ��������J�����̍��W���擾����.
		getCollisionCameraPos(&tmp_cam_pos, hitpos);
		StartDeleyCamera(eDELAY_SPEED_HIGH, true);
	}

	mPosition = tmp_cam_pos;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::StartMoveCamera(float setSec, VECTOR* pEndPos, VECTOR* pEndTar)
{
	if (setSec == 0.0f) return;

	float max_frame = (60.0f * setSec);
	float move_rate = 1.0f / max_frame;

	mCamraType	= eCAMERA_MOVE;
	mMoveWork.mPositionMoveVec	= VScale(VSub(*pEndPos, mPosition), move_rate);
	mMoveWork.mTargetMoveVec	= VScale(VSub(*pEndTar, mTarget), move_rate);
	mMoveWork.mNowSec			= 0.0f;
	mMoveWork.mMaxSec			= setSec;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::StartShakeCamera(float setSec, float ampWidth, float ampSpeed)
{
	if (setSec == 0.0f) return;

	mCamraType = eCAMERA_SHAKE;
	mShakeWork.mSec		= 0.0f;
	mShakeWork.mMaxSec	= setSec;
	mShakeWork.mAmpWidth= ampWidth;
	mShakeWork.mAmpSpeed= ampSpeed;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::StartDeleyCamera(eDelayType type, bool isReturnsNormalModeCaseValueSmall)
{
	switch (type) {
	case eDELAY_SPEED_HIGH:
		mDelayWork.mDelaySpeedRate = 0.2f;
		break;
	case eDELAY_SPEED_MIDDLE:
		mDelayWork.mDelaySpeedRate = 0.1f;
		break;
	case eDELAY_SPEED_LOW:
		mDelayWork.mDelaySpeedRate = 0.05f;
		break;
	case eDELAY_SPEED_VERY_LOW:
		mDelayWork.mDelaySpeedRate = 0.000005f;
		break;
	default:
		break;
	}

	if (isReturnsNormalModeCaseValueSmall) {
		mCamraType = eCAMERA_DELAY_TO_NORMAL;
	} else {
		mCamraType = eCAMERA_DELAY;
	}

	mDelayWork.mPrevPosition	= mPosition;
	mDelayWork.mPrevTarget		= mTarget;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::StartVibeCamera(float setSec, float setSize, float setSpeed)
{
	mQuakeWork.mIsActive = true;
	mQuakeWork.mCount = 0.0f;

	mQuakeWork.mSetSecond	= setSec;
	mQuakeWork.mSetSize		= setSize;
	mQuakeWork.mSetSpeed	= setSpeed;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
CameraManager::IsVibeCamera(void)
{
	return mQuakeWork.mIsActive;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::StopVibeCamera(void)
{
	mQuakeWork.mIsActive = false;
	mUp = VGet(0.0f, 1.0f, 0.0f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::RevertCamera(void)
{
	SetCameraPositionAndTargetAndUpVec(mPosition, mTarget, mUp);
}

//---------------------------------------------------
/**
 * �J�����̈ʒu�ƃ^�[�Q�b�g�̋��������߂�
 */
//---------------------------------------------------
float
CameraManager::GetCameraDist(void)
{
	return VSize(VSub(mPosition, mTarget));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetPerspectiveCamera(float fov)
{
	SetupCamera_Perspective(fov);	
	mFov = fov;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetOrthoCamera(float size)
{
	SetupCamera_Ortho(size);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetDefaultNearFarClip(void)
{
	mNearClip	= DEFAULT_NEAR_CLIP;
	mFarClip	= DEFAULT_FAR_CLIP;

	SetCameraNearFar( mNearClip, mFarClip );
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetNearClip(float value)
{
	mNearClip = value;
	SetCameraNearFar(mNearClip, mFarClip);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetFarClip(float value)
{
	mFarClip = value;
	SetCameraNearFar(mNearClip, mFarClip);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::RevertNearFarClip(void)
{
	SetCameraNearFar( mNearClip, mFarClip );

	SetupCamera_Perspective(mFov);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SaveTemporaryCamWork(void)
{
	mTmpPos		= mPosition;
	mTmpTar		= mTarget;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::LoadTemporaryCamWork(void)
{
	mPosition	= mTmpPos;
	mTarget		= mTmpTar;
}

//---------------------------------------------------
/**
 * �J�����X�e�[�^�X�̕ύX
 */
//---------------------------------------------------
void
CameraManager::ChangeCameraState(eCamState setState)
{
	switch (setState) {
	case eCAM_STATE_ARCHERY:
		if (mCamState != eCAM_STATE_ARCHERY) {

			mArcheryCamType = eARCHERY_CAM_TYPE_NORMAL;
		}
		break;

	case eCAM_STATE_TROLLEY:
		if (mCamState != eCAM_STATE_TROLLEY) {
			mPatternTrolleyParam.mPrevVector = mPatternTrolleyParam.mVector;
		}
		break;

	default:
		break;
	};
	
	mCamState = setState;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
eCamState
CameraManager::GetCameraState(void)
{
	return mCamState;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetPatternGameBegine(void)
{
	CharaPlayer* p_mine = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	bool is_target = false;

	VECTOR add_pos;

	mTarget = p_mine->Position();
	mTarget.y += sTargetHeightCenter;

	add_pos.x = (sin(p_mine->Rotation().y ) * mLength);
	add_pos.y = 0.0f;
	add_pos.z = (cos(p_mine->Rotation().y ) * mLength);

	mPosition = VAdd(mTarget, add_pos);

	mCamVecZ = VNorm(VSub(mPosition, mTarget) );

	mUp = VGet(0.0f, 1.0f, 0.0f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetPatternCameraByVector(VECTOR& rVector, VECTOR& rUp)
{
	CharaPlayer* p_player = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	bool is_target = false;

	VECTOR add_pos;

	mTarget = p_player->Position();
	mTarget.y += sTargetHeightCenter;

	mLength = 800.0f;

	add_pos = VScale(VNorm(VAdd(VScale(rVector, -1.0f), VScale(rUp, 0.4f))), mLength);

	mPosition = VAdd(mTarget, add_pos);

	mCamVecZ = VNorm(VSub(mPosition, mTarget) );

	mUp = rUp;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetTrolleyParam(CameraManager::tPatternTrolleyParam* pParam)
{
	mPatternTrolleyParam.mVector	= VNorm(pParam->mVector);
	mPatternTrolleyParam.mUp		= pParam->mUp;
	mPatternTrolleyParam.mSpeedRate	= pParam->mSpeedRate;

	mPatternTrolleyParam.mCamLength	= pParam->mCamLength;
	mPatternTrolleyParam.mCamWidth	= pParam->mCamWidth;
	mPatternTrolleyParam.mCamHeight	= pParam->mCamHeight;
}

//---------------------------------------------------
/**
 * �L�����̎��ӂɐݒ肷��J����
 */
//---------------------------------------------------
void
CameraManager::SetPatternArroundPlayer(float camDegY, float camLen, float camPosY, float camTarY)
{
	CharaPlayer* p_player = (CharaPlayer *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	CameraManager::GetInstance()->ChangeCameraState(eCAM_STATE_DUMMY);

	VECTOR cam_pos, cam_tar;
	cam_tar = p_player->Position();
	cam_tar.y += camTarY;
	cam_pos = p_player->Position();
	cam_pos.x += sin(p_player->Rotation().y + Utility::DegToRad(camDegY)) * camLen;
	cam_pos.y += camPosY;
	cam_pos.z += cos(p_player->Rotation().y + Utility::DegToRad(camDegY)) * camLen;
	CameraManager::GetInstance()->Position() = cam_pos;
	CameraManager::GetInstance()->Target() = cam_tar;
}

//---------------------------------------------------
/**
 * �J�����̌X�������Z�b�g����
 */
//---------------------------------------------------
void
CameraManager::ResetCameraPosY(void)
{
	mCamVecZ.y = 0.0f;
	mCamVecZ = VNorm(mCamVecZ);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::SetCamVecZ(VECTOR& rCamVec)
{
	mCamVecZ = rCamVec;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::ReviseCamera(VECTOR& rMoveVec)
{
	mPosition	= VSub(mPosition,	rMoveVec);
	mTarget		= VSub(mTarget,		rMoveVec);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
#ifdef _DEBUG
void
CameraManager::DumpStatus(void)
{
	PRINT_CONSOLE(" * CameraManager::DumpStatus() : Start * \n");
	
	PRINT_CONSOLE_INT(mCamraType);
	PRINT_CONSOLE_INT(mCamState);
	PRINT_CONSOLE_VEC(mPosition);
	PRINT_CONSOLE_VEC(mTarget);
	PRINT_CONSOLE_VEC(mUp);

	PRINT_CONSOLE_VEC(mCamVecZ);
	
	PRINT_CONSOLE_FLT(mLength);
	PRINT_CONSOLE_FLT(mNearClip);
	PRINT_CONSOLE_FLT(mFarClip);
	PRINT_CONSOLE_FLT(mFov);

	PRINT_CONSOLE(" * CameraManager::DumpStatus() : End * \n");
}
#endif // _DEBUG

//---------------------------------------------------
/**
   �p�~����...
 */
//---------------------------------------------------
void
CameraManager::funcVibe(void)
{
	float shake_second	= mQuakeWork.mSetSecond;
	float max_shake_size= mQuakeWork.mSetSize;
	float shake_speed	= mQuakeWork.mSetSpeed;

	if (mQuakeWork.mIsActive) {

		VECTOR vec, cross;
		vec = VSub(mTarget, mPosition);
		vec = VNorm(vec);
		cross = VCross(VGet(0.0f, 1.0f, 0.0f), vec);
		//		cross = vec;

		cross = VScale(cross, (max_shake_size * sin(mQuakeWork.mCount * shake_speed)));
		mUp = VAdd(VGet(0.0f, 1.0f, 0.0f), cross);
		mUp = VScale(mUp, 0.5f);
		mUp = VNorm(mUp);

		mQuakeWork.mCount += 1.0f;

		if (mQuakeWork.mCount > (shake_second * 60.0f) ) {
			
			StopVibeCamera();
		}
		
	}
}

//---------------------------------------------------
/**
   �J�����̋������X�V
 */
//---------------------------------------------------
void
CameraManager::updateCamLength(void)
{
	static const float kCAM_DIST_TABLE[] = {
		240.0f, 500.0f, 840.0f,
	};
	static const BYTE	kCAM_DIST_TABLE_NUM = (sizeof(kCAM_DIST_TABLE) / sizeof(const float));

	static const float	kMIN_DIST			= 150.0f;
	static const float	kMAX_DIST			= 1000.0f;
	static const int	kLENGTH_SCALE		= -30;

	static float		sTargetCamLength	= mLength;
	static int			sCamZoomIndex		= 0;
	// �J�����̉�]���[�h�ƃY�[�����[�h�ؑ�
	if (CHECK_TRIG_JOY(JOY_INPUT_CAM_SWITCH)) {
		sCamZoomIndex = (sCamZoomIndex + 1) % kCAM_DIST_TABLE_NUM;
		sTargetCamLength = kCAM_DIST_TABLE[sCamZoomIndex];
	}
	// �J�����̋����̒l���X�V
	sTargetCamLength += (GetMouseWheelRotVol() * kLENGTH_SCALE);

	if (sTargetCamLength < kMIN_DIST) {
		sTargetCamLength = kMIN_DIST;
	} else if (sTargetCamLength > kMAX_DIST) {
		sTargetCamLength = kMAX_DIST;
	}
	mLength = ((sTargetCamLength - mLength) * 0.1f) + mLength;
}

//---------------------------------------------------
/**
   �J�����̋������X�V
 */
//---------------------------------------------------
void
CameraManager::updateCamLengthArchery(void)
{
	static bool sIsRolVal = false;

	// �J�����̉�]���[�h�ƃY�[�����[�h�ؑ�
	if (CHECK_TRIG_JOY(JOY_INPUT_CAM_SWITCH)) {
		mArcheryCamType = (mArcheryCamType + 1) % eARCHERY_CAM_TYPE_MAX;
	}

	// �J�����̋����̒l���X�V
	int wheel_rot = GetMouseWheelRotVol();

	if (sIsRolVal == false) {
		if (wheel_rot > 0.9f) {
			if ((mArcheryCamType - 1) >= 0) {
				mArcheryCamType --;
				sIsRolVal = true;
			}
		}
		else if (wheel_rot < -0.9f) {
			if ((mArcheryCamType + 1) < eARCHERY_CAM_TYPE_MAX) {
				mArcheryCamType ++;
				sIsRolVal = true;
			}
		}
	} 
	else {
		if (wheel_rot == 0) {
			sIsRolVal = false;
		}
	}
}

//---------------------------------------------------
/**
 * �R���W�����ɓ��������J�����̍��W���擾����.
 */
//---------------------------------------------------
void
CameraManager::getCollisionCameraPos(VECTOR* pOutCamPos, VECTOR& rHitPos)
{
	// �����_�ƕǂƂ̓��������ʒu�Ƃ̋��������߂�
	float dist_tarpos_to_hitpos;
	dist_tarpos_to_hitpos = VSize(VSub(rHitPos, mTarget) );

	// ��t�̌��������������Ȃ��l�ɏ�����O�ɃJ�������W��ݒ肷��
	dist_tarpos_to_hitpos -= 100.0f;
		
	// �t�ɒ����_�ƃJ�����ʒu���߂��Ȃ肷���Ȃ��l�ɕ␳����
	if (dist_tarpos_to_hitpos < 30.0f) {
		dist_tarpos_to_hitpos = 30.0f;
	}
	*pOutCamPos = VAdd(mTarget, VScale(mCamVecZ, dist_tarpos_to_hitpos) );
}

//---------------------------------------------------
/**

 */
//---------------------------------------------------
void
CameraManager::cameraFuncNormal(void)
{
	// �������Ȃ�
	return;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::cameraFuncDelay(void)
{
	VECTOR vec;
	vec = VSub(mPosition, mDelayWork.mPrevPosition);

	mPosition = VAdd(VScale(vec, mDelayWork.mDelaySpeedRate), mDelayWork.mPrevPosition);
	mTarget = VAdd(VScale(VSub(mTarget, mDelayWork.mPrevTarget), mDelayWork.mDelaySpeedRate), mDelayWork.mPrevTarget);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::cameraFuncDelayToNormal(void)
{
	VECTOR vec;
	vec = VSub(mPosition, mDelayWork.mPrevPosition);

	mPosition = VAdd(VScale(vec, mDelayWork.mDelaySpeedRate), mDelayWork.mPrevPosition);
	mTarget = VAdd(VScale(VSub(mTarget, mDelayWork.mPrevTarget), mDelayWork.mDelaySpeedRate), mDelayWork.mPrevTarget);

	// �������������Ȃ�΃J������߂�
	if (VSize(vec) < 1.0f) {
		mCamraType = eCAMERA_NORMAL;
		return;
	}
	mDelayWork.mDelaySpeedRate += 0.1f;
	if (mDelayWork.mDelaySpeedRate > 1.0f) {
		mDelayWork.mDelaySpeedRate = 1.0f;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::cameraFuncMove(void)
{
	mPosition	= VAdd(mPosition, mMoveWork.mPositionMoveVec);
	mTarget		= VAdd(mTarget, mMoveWork.mTargetMoveVec);
	mMoveWork.mNowSec += (1.0f / 60.0f);
	if (mMoveWork.mMaxSec <= mMoveWork.mNowSec) {
		mCamraType = eCAMERA_NORMAL;
	}		
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CameraManager::cameraFuncShake(void)
{
	mTarget.y += sin(mShakeWork.mSec * mShakeWork.mAmpSpeed) * mShakeWork.mAmpWidth;
	mShakeWork.mSec += (1.0f / 60.0f);
	if (mShakeWork.mMaxSec <= mShakeWork.mSec) {
		mCamraType = eCAMERA_NORMAL;
	}
}

 /**** end of file ****/
	
