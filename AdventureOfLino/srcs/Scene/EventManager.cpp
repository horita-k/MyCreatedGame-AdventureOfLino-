/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * EventManager.cpp
 * �^�X�N(�C�x���g�Ǘ�)�N���X
 * @author SPATZ.
 * @data   2012/04/04 14:48:17
 */
//---------------------------------------------------
#include "AppLib/Basic/Basic.h"
#include "AppLib/Collision/CollisionManager.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "Scene/SceneMainGame.h"
#include "Scene/CommandFunctions.h"

#include "EventManager.h"
#include "Utility/Utility.h"

#ifdef _DEBUG
#include "AppLib/Input/InputManager.h"
#endif // _DEBUG

static const int kCAMERA_FRAME_SIZE			= 61;
static const float kADD_FRAME_COUNTER_VALUE	= 0.5f;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/EventManager* const 
EventManager::GetInstance(void)
{
	static EventManager sInstance;
	return &sInstance;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EventManager::Setup(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EventManager::LoadCameraFile(const char* fileName, float scaleCamera)
{
	mCameraKeyNum		= 0;

	mFileHandle = FileRead_open(fileName, FALSE);
	APP_ASSERT_PRINT(mFileHandle != -1, 
					 "Failed open %s \n", fileName);
	int size;
	size = (int)FileRead_size(fileName);
	APP_ASSERT_PRINT(size != -1,
					 "Faild FileRead_size() : %s \n", fileName);

	Header hed;
	DWORD boneCount; // �{�[���L�[�t���[����
	DWORD morphCount; // �\��L�[�t���[����

	FileRead_read((void *)&hed, sizeof(Header), mFileHandle);
	FileRead_read((void *)&boneCount, sizeof(boneCount), mFileHandle);
	if (boneCount != 0) {
		/* �ȗ� */
	}
	FileRead_read((void *)&morphCount, sizeof(morphCount), mFileHandle);
	if (morphCount != 0) {
		/* �ȗ� */
	}
	FileRead_read((void *)&mCameraKeyNum, sizeof(mCameraKeyNum), mFileHandle);

	if (mpCameraFrame == NULL) {
		if (mCameraKeyNum != 0) {
			mpCameraFrame = new char[(kCAMERA_FRAME_SIZE * mCameraKeyNum)];
			APP_ASSERT_PRINT(mpCameraFrame != NULL,
							 "Failed new CameraFrame[%d] \n", mCameraKeyNum);
			FileRead_read((void *)mpCameraFrame, (kCAMERA_FRAME_SIZE * mCameraKeyNum), mFileHandle);
		}


		int* p_tmp = NULL;
		p_tmp = new int[mCameraKeyNum];
		mpIndex = new int[mCameraKeyNum];
		for (DWORD iI = 0; iI < mCameraKeyNum; iI ++) {
			CameraFrame* p_frame;
			p_frame = (CameraFrame *)&mpCameraFrame[iI * kCAMERA_FRAME_SIZE];
			PRINT_CONSOLE("p_frame[%d] : %d \n", iI, p_frame->frame);
			mpIndex[iI] = iI;
			p_tmp[iI] = p_frame->frame;
		}

		Basic::QuickSortEx(p_tmp, mpIndex, 0, mCameraKeyNum-1);

		for (DWORD iI = 0; iI < mCameraKeyNum; iI ++) {
			PRINT_CONSOLE("p_tmp[%d] = %d \n", mpIndex[iI], p_tmp[iI]);
		}

		delete[] p_tmp;
		p_tmp = NULL;
	}

	if (mFileHandle != -1) {
		FileRead_close(mFileHandle);
		mFileHandle = -1;
	}

	mCameraScale = scaleCamera;

	mIsOffsetCamera = false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EventManager::StartEvent(CsvAccesser*	pCsvAccesser/*=NULL*/, 
						 const			VECTOR* pShiftPos/*=NULL*/,		// �V�t�g���W�ݒ�(NULL�̏ꍇ�̓I�t�Z�b�g�Ȃ�)
						 bool 			isVisibleNpc/*=false*/,			// NPC�\���t���O
						 bool			isVisibleObject/*=false*/)		// �A�C�e���\���t���O
{
	/* Reset parameter */
	mFrameCounter		= 0.0f;
	mNextKey			= 0;
	mShifCamPos			= ZERO_VEC;
	// �V�t�g�ݒ�̎w�肪����ꍇ�̓J�������V�t�g������
	if (pShiftPos != NULL) {
		mShifCamPos = *pShiftPos;
	}

	// �����ϐ��֕ۑ�
	mpCsvAccesser = pCsvAccesser;

	/* NPC�͑S�Ĕ�\���ɂ����� */
	visibleNpcAll(isVisibleNpc);

	/* �A�C�e����S�Ĕ�\���ɂ����� */
	visibleObjectAll(isVisibleObject);

	mIsStartEvent = true;
	mIsPauseEvent = FALSE;

	if (mpAssignLayout == NULL) {
		mpAssignLayout = new LayoutBase[kEVTMGR_LAYOUT_NUM];
		APP_ASSERT((mpAssignLayout != NULL),
				   "Failed new LayoutBase[] \n");

		for (int iI = 0; iI < kEVTMGR_LAYOUT_NUM; iI ++) {
			mpAssignLayout[iI].Setup();
			GraphicManager::GetInstance()->InsertDrawLayout(&mpAssignLayout[iI],
															TotalInfo::GetInstance()->GetSystemLayout(TotalInfo::eLY_BALLOON));
		}

	}

	if (mpAssignSystemLayout == NULL) {
		mpAssignSystemLayout = new LayoutBase[kEVTMGR_LAYOUT_NUM];
		APP_ASSERT((mpAssignSystemLayout != NULL),
				   "Failed new LayoutBase \n");

		for (int iI = 0; iI < kEVTMGR_LAYOUT_NUM; iI ++) {
			mpAssignSystemLayout[iI].Setup();
			GraphicManager::GetInstance()->EntrySystemLayout(&mpAssignSystemLayout[iI]);
		}
	}


 }

 //---------------------------------------------------
 /**
  * �C�x���g���w�莞�Ԃ܂ŃV�[�N������
  */
 //---------------------------------------------------
 void
 EventManager::SeekEvent(float seekFrame)
 {
	 int start_frame;
	 start_frame = (int)mFrameCounter;	// ��xint�ɃL���X�g���āA�����ɂ���(0.5���ƃR�}���h���s�Ɏ��s���邽��)

	 // �R�}���h�����݂̃t���[������w��t���[���܂őS�Ď��s������
	 for (mFrameCounter = (float)start_frame; mFrameCounter < seekFrame; mFrameCounter ++) {
		 updateCsvAction(mpCsvAccesser);
	 }

	 // �J�������Ōォ�珇�ԂɃt���[�����Ō����������A��ԍŐV�̍��W��K�p����
	 CameraFrame* p_cam_frame = NULL;
	 int access_key = 0;
	 VECTOR pos, tar;
	 for (DWORD iI = (mCameraKeyNum - 1); iI >= 0; iI --) {
		 // �w��̃L�[�̎擾
		 access_key = mpIndex[iI];
		 // �J�����L�[�t���[���v�f�f�[�^�̎擾
		 p_cam_frame = (CameraFrame *)&mpCameraFrame[(access_key * kCAMERA_FRAME_SIZE)];

		 // ���̃t���[�����Ⴂ�t���[�����������ꍇ
		 if (p_cam_frame->frame <= mFrameCounter) {
			 // �w��̃t���[���̃J�����̍��W���𔽉f�����ău���[�N�Ŕ�����
			 getFrameCamPosTar(p_cam_frame, &pos, &tar);
			 CameraManager::GetInstance()->Position()= pos;
			 CameraManager::GetInstance()->Target()	= tar;
			 CameraManager::GetInstance()->SetPerspectiveCamera(Utility::DegToRad((float)p_cam_frame->viewAngle));
			 break;
		 }
	 }

 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 void
 EventManager::Update(void)
 {
	 if ((mIsStartEvent == false) || (mIsPauseEvent == TRUE)) {
		 return;
	 }

 #if 0
	 // �f�o�b�O���[�h�ł̓C�x���g�̑��삪�\
	 {
		 static bool sIsPlay = true;
		 float shift_speed = 0.5f;
		 CameraFrame* p_cam_frame = NULL;
		 int access_key = 0;

		 // �X�y�[�X�L�[�Ŏ����Đ��ƃt���[�������؂�ւ���
		 if (CHECK_TRIG_KEY(eKEY_SPACE)) {
			 sIsPlay ^= true;
			 PRINT_CONSOLE("=>EventManager : Stream %s \n", (sIsPlay == true) ? "Play" : "Stop");
		 }

		 //<! Ctrl-���E�Ŏ��̃J�����t���[���܂ňړ�
		 if ((CHECK_PRESS_KEY(eKEY_LCONTROL)) &&
			 (CHECK_TRIG_KEY(eKEY_RIGHT))) {
			 // �J������擪���珇�ԂɃL�[�t���[���Ō����������A���̃L�[�t���[���փV�[�N����
			 for (DWORD iI = 0; iI < mCameraKeyNum; iI ++) {
				 // �w��̃L�[�t���[�����擾
				 access_key = mpIndex[iI];
				 // �J�����L�[�t���[���v�f�f�[�^�̎擾
				 p_cam_frame = (CameraFrame *)&mpCameraFrame[(access_key * kCAMERA_FRAME_SIZE)];
				 // ���̃t���[�����Ⴂ�t���[�����������ꍇ
				 if (p_cam_frame->frame > mFrameCounter) {
					 // ���̃J�����t���[���փV�[�N����
					 this->SeekEvent((float)p_cam_frame->frame);
					 sIsPlay = true;
					 PRINT_CONSOLE("=>EventManager : SeekFrame = %.2f \n", (float)p_cam_frame->frame);
					 break;
				 }					
			 }
		 }

		 if (sIsPlay) {
			 // �ʏ�ʂ�t���[����i�s������
			 mFrameCounter += kADD_FRAME_COUNTER_VALUE;		// (kADD_PHASE_COUNTER_VALUE / 2.0f);
		 } else {
			 // ���E�L�[�Ńt���[�����ړ��A�V�t�g�Ŕ{���x
			 if (CHECK_PRESS_KEY(eKEY_LSHIFT)) {
				 shift_speed *= 2.0f;
			 }
			 if (CHECK_PRESS_KEY(eKEY_RIGHT)) {
				 mFrameCounter += shift_speed;
			 }
			 if (CHECK_PRESS_KEY(eKEY_LEFT)) {
				 mFrameCounter -= shift_speed;
			 }
		 }
	 }
 #else //_DEBUG
	 mFrameCounter += kADD_FRAME_COUNTER_VALUE;		// (kADD_PHASE_COUNTER_VALUE / 2.0f);
 #endif // _DEBUG

	 mIsEndCamera = false;

	 CameraFrame* p_frame;
	 int access_key;

	 for (DWORD iI = mNextKey; iI < mCameraKeyNum; iI ++) {
		 access_key = mpIndex[iI];
		 p_frame = (CameraFrame *)&mpCameraFrame[access_key * kCAMERA_FRAME_SIZE];
		 if (p_frame->frame == (DWORD)mFrameCounter) {
			 mNextKey ++;
			 break;
		 }
	 }

	 int prev_key = mNextKey - 1;
	 PRINT_SCREEN(GetColor(255, 255, 0), "prev_key = %d \n", prev_key);
	 PRINT_SCREEN(GetColor(255, 255, 0), "mFrameCounter = %.2f \n", mFrameCounter);

	 if (mNextKey >= mCameraKeyNum) {
		 mIsEndCamera = true;
		 return;
	 }
	 if (prev_key < 0) return;

	 float  prev_cnt, next_cnt, diff_cnt;
	 VECTOR prev_pos, next_pos, diff_pos;
	 VECTOR prev_tar, next_tar, diff_tar;

	 access_key = mpIndex[prev_key];
	 p_frame = (CameraFrame *)&mpCameraFrame[access_key * kCAMERA_FRAME_SIZE];
	 prev_cnt = (float)p_frame->frame;
	 getFrameCamPosTar(p_frame, &prev_pos, &prev_tar);

	 access_key = mpIndex[mNextKey];
	 p_frame = (CameraFrame *)&mpCameraFrame[access_key * kCAMERA_FRAME_SIZE];
	 next_cnt = (float)p_frame->frame;
	 getFrameCamPosTar(p_frame, &next_pos, &next_tar);

	 diff_cnt = next_cnt - prev_cnt;
	 diff_pos = VSub(next_pos, prev_pos);
	 diff_tar = VSub(next_tar, prev_tar);

	 VECTOR pos, tar;
	 float cnt;
	 cnt = mFrameCounter - prev_cnt;

	 if ((diff_cnt != 1.0f) || (cnt != 0.5f)) {
		 pos.x = ((diff_pos.x / diff_cnt) * cnt) + prev_pos.x;
		 pos.y = ((diff_pos.y / diff_cnt) * cnt) + prev_pos.y;
		 pos.z = ((diff_pos.z / diff_cnt) * cnt) + prev_pos.z;

		 tar.x = ((diff_tar.x / diff_cnt) * cnt) + prev_tar.x;
		 tar.y = ((diff_tar.y / diff_cnt) * cnt) + prev_tar.y;
		 tar.z = ((diff_tar.z / diff_cnt) * cnt) + prev_tar.z;

		 /* �I�t�Z�b�g�������s�� */
		 if (mIsOffsetCamera) {
			 pos.x += (mSetCamPosX - tar.x);
			 pos.z += (mSetCamPosZ - tar.z);
			 tar.x = mSetCamPosX;
			 tar.z = mSetCamPosZ;

			 pos.y += mOffsetCamPosY;
			 tar.y += mOffsetCamPosY;

			 VECTOR vecA, vecB;
			 vecA = VSub(pos, tar);
			 vecA.y = 0.0f;
			 vecB.x = vecA.x * cos(-mOffsetCamRot) - vecA.z * sin(-mOffsetCamRot);
			 vecB.y = 0.0f;
			 vecB.z = vecA.x * sin(-mOffsetCamRot) + vecA.z * cos(-mOffsetCamRot);
			 pos.x = tar.x + vecB.x;
			 pos.z = tar.z + vecB.z;
		 }

		 CameraManager::GetInstance()->Position()= pos;
		 CameraManager::GetInstance()->Target()	= tar;
		 CameraManager::GetInstance()->SetPerspectiveCamera(Utility::DegToRad((float)p_frame->viewAngle));
	 }

	 PRINT_SCREEN(GetColor(255, 255, 0), "cam = {%.2f, %.2f, %.2f } \n", pos.x, pos.y, pos.z);
	 PRINT_SCREEN(GetColor(255, 255, 0), "tar = {%.2f, %.2f, %.2f } \n", tar.x, tar.y, tar.z);
	 PRINT_SCREEN(GetColor(255, 255, 0), "rot = {%.2f, %.2f, %.2f } \n", p_frame->rx, p_frame->ry, p_frame->rz);
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 void
 EventManager::EndEvent(void)
 {
	if (mpAssignLayout != NULL) {
		for (int iI = 0; iI < kEVTMGR_LAYOUT_NUM; iI ++) {
			GraphicManager::GetInstance()->RemoveDrawLayout(&mpAssignLayout[iI]);
		}
		delete[] mpAssignLayout;
		mpAssignLayout = NULL;
	}

	if (mpAssignSystemLayout != NULL) {
		for (int iI = 0; iI < kEVTMGR_LAYOUT_NUM; iI ++) {
			GraphicManager::GetInstance()->RemoveSystemLayout(&mpAssignSystemLayout[iI]);
		}
		delete[] mpAssignSystemLayout;
		mpAssignSystemLayout = NULL;
	}

	 // �J�n���Ă��Ȃ��ꍇ�͋����I�ɏI��������
	 if (mIsStartEvent == false) {
		 return;
	 }

	 if (mFileHandle != -1) {
		 FileRead_close(mFileHandle);
		 mFileHandle = -1;
	 }

	 if (mpCameraFrame != NULL) {
		 delete[] mpCameraFrame;
		 mpCameraFrame = NULL;
	 }

	 if (mpCharaBase != NULL) {
		 CharaNpc* p_pmd;
		 for (int iI = 0; iI < mModelNum; iI ++) {
			 p_pmd = &mpCharaBase[iI];
			 GraphicManager::GetInstance()->RemoveDrawModel(p_pmd);
			 p_pmd->DeleteModel();
		 }
		 delete[] mpCharaBase;
		 mpCharaBase = NULL;
	 }
	 mModelNum = 0;

	 if (mpModelAnimWork != NULL) {
		 delete[] mpModelAnimWork;
		 mpModelAnimWork = NULL;
	 }

	 if (mpLayoutAnimWork != NULL) {
		 delete[] mpLayoutAnimWork;
		 mpLayoutAnimWork = NULL;
	 }

	 // �G�t�F�N�g�`���ɕ`�悳��郂�f�� �̔j��
	 GraphicManager::GetInstance()->EntryAfterDrawModel(NULL);

	 if (mpIndex != NULL) {
		 delete[] mpIndex;
		 mpIndex = NULL;
	 }

	 /* NPC�͑S�ĕ\���ɂ����� */
	 visibleNpcAll(true);

	 /* �I�u�W�F�N�g��S�ĕ\�������� */
	 visibleObjectAll(true);

	 mIsStartEvent = false;
	 mIsPauseEvent = FALSE;
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 void
 EventManager::EntryModelNum(int num)
 {
	 mModelNum = num;

	 mpCharaBase = new CharaNpc[mModelNum];
	 APP_ASSERT_PRINT(mpCharaBase != NULL,
					  "Failed new CharaBase[%d] \n", mModelNum);

	 mpModelAnimWork = new tModelAnimInfo[mModelNum];
	 APP_ASSERT_PRINT(mpModelAnimWork != NULL,
					  "Failed new tModelAnimInfo[%d] \n", mModelNum);
	 memset(mpModelAnimWork, 0, (sizeof(tModelAnimInfo) * mModelNum));

	 mpLayoutAnimWork = new tLayoutAnimInfo[kEVTMGR_LAYOUT_NUM];
	 APP_ASSERT_PRINT(mpLayoutAnimWork != NULL,
					  "Failed new tModelAnimInfo[%d] \n", kEVTMGR_LAYOUT_NUM);
	 memset(mpLayoutAnimWork, 0, (sizeof(tLayoutAnimInfo) * kEVTMGR_LAYOUT_NUM));
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 void
 EventManager::SetModelParameter(int modelIdx, const char* strName, int motionIdx, ModelBase* pInsertBeforeModel/*=NULL*/, bool isMotionLoop/*=false*/)
 {
	 APP_ASSERT(mpCharaBase != NULL,
				"mpCharaBase is NULL\n");
	 APP_ASSERT_PRINT((modelIdx < mModelNum),
					  "set modelIdx %d is over max %d \n", modelIdx, mModelNum);
	 //	CharaBase* p_pmd;
	 CharaNpc* p_pmd;
	 p_pmd = &mpCharaBase[modelIdx];

	 p_pmd->LinkModel(strName);
	 p_pmd->Position() = ZERO_VEC;
	 p_pmd->Scale() = VGet(10.0f, 10.0f, 10.0f);
	 p_pmd->Setup(true);

	 if (pInsertBeforeModel == NULL) {
		 GraphicManager::GetInstance()->EntryDrawModel(p_pmd);
	 } else if ((unsigned int)pInsertBeforeModel == EVENT_MANAGER_AFTER_EFFECT_DRAW) {
		 GraphicManager::GetInstance()->EntryAfterDrawModel(p_pmd);
	 } else {
		 GraphicManager::GetInstance()->InsertDrawModel(p_pmd, pInsertBeforeModel);
	 }

	 p_pmd->StartMotion(motionIdx, isMotionLoop, 0.5f, NULL, 1.0f);
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 CharaNpc* const
 EventManager::GetModelPmd(int modelIdx)
 {
	 APP_ASSERT(mpCharaBase != NULL,
				"mpCharaBase is NULL\n");
	 APP_ASSERT_PRINT((modelIdx < mModelNum),
					  "set modelIdx %d is over max %d \n", modelIdx, mModelNum);

	 // �݊����̂��߁A�����ăL���X�g���Ă���
	 return &mpCharaBase[modelIdx];
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 LayoutBase*
 EventManager::GetLayoutBase(int layoutIdx)
 {
	 APP_ASSERT_PRINT((layoutIdx < kEVTMGR_LAYOUT_NUM),
					  "set layoutIdx, %d is over max %d \n", layoutIdx, kEVTMGR_LAYOUT_NUM);
	 return &mpAssignLayout[layoutIdx];
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 LayoutBase*
 EventManager::GetSystemLayoutBase(int layoutIdx)
 {
	 APP_ASSERT_PRINT((layoutIdx < kEVTMGR_LAYOUT_NUM),
					  "set layoutIdx, %d is over max %d \n", layoutIdx, kEVTMGR_LAYOUT_NUM);
	 return &mpAssignSystemLayout[layoutIdx];
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 void
 EventManager::EnableOffsetCamera(float setPosX, float offsetPosY, float setPosZ, float setRot)
 {
	 mIsOffsetCamera = true;
	 mSetCamPosX			= setPosX;
	 mOffsetCamPosY		= offsetPosY;
	 mSetCamPosZ			= setPosZ;
	 mOffsetCamRot		= setRot;
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 bool
 EventManager::EqualPhaseFrame(float frame)
 {
	 return ( ( (frame >= mFrameCounter) && (frame < mFrameCounter + kADD_FRAME_COUNTER_VALUE)) ? true : false);
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 bool
 EventManager::BetweenPhaseFrame(float secMin, float secMax)
 {
	 return (( ( (secMin) <= mFrameCounter) && (secMax > mFrameCounter) ) ? true : false);
 }

 //---------------------------------------------------
 /**

  */
 //---------------------------------------------------
 void
 EventManager::UpdateEventAnimation(void)
 {
	 // ���f���̃A�j���[�V����
	 for (int iI = 0; iI < mModelNum; iI ++) {
		 tModelAnimInfo* p_work = &mpModelAnimWork[iI];
		 if (p_work->mAnimState != 0) {

			 if (this->BetweenPhaseFrame(p_work->mStartFrame, p_work->mEndFrame)) {
				 CharaNpc* p_chara = &mpCharaBase[iI];
				 // �t�F�[�h�̃A�j���[�V��������
				 if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_FADE_IN) {
					 float set_alpha = (mFrameCounter - p_work->mStartFrame) / (p_work->mEndFrame - p_work->mStartFrame);
					p_chara->SetAlpha(set_alpha);
				} else if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_FADE_OUT) {
					float set_alpha = (1.0f - ((mFrameCounter - p_work->mStartFrame) / (p_work->mEndFrame - p_work->mStartFrame)));
					p_chara->SetAlpha(set_alpha);
				}
				// �x����]�̃A�j���[�V��������
				if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_ROTATION) {
					p_chara->Rotation().y += p_work->mRotationSpeed;
				}
				// �g��k���̃A�j���[�V��������
				if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_SCALE) {
					float scale = (0.5f * sin(mFrameCounter / 10.0f));
					scale += p_work->mDefaultScale;
					p_chara->Scale() = VGet(scale, scale, scale);
				}
			} else {
				// �͈͊O�Ȃ̂ŏI��������
				p_work->mAnimState = 0x00;
			}
		}	
	}

	// ���C�A�E�g�̃A�j���[�V����
	for (int iI = 0; iI < kEVTMGR_LAYOUT_NUM; iI ++) {
		tLayoutAnimInfo* p_work = &mpLayoutAnimWork[iI];
		if (p_work->mAnimState != 0) {

			if (this->BetweenPhaseFrame(p_work->mStartFrame, p_work->mEndFrame)) {
				LayoutBase* p_layout = &mpAssignLayout[iI];
				if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_FADE_IN) {
					// ���T�|�[�g
					APP_ASSERT(false, "this pattern is not support \n");
				} else if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_FADE_OUT) {
					APP_ASSERT(false, "this pattern is not support \n");
				}
				// �x����]�̃A�j���[�V��������
				if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_ROTATION) {
					APP_ASSERT(false, "this pattern is not support \n");
				}
				// �g��k���̃A�j���[�V��������
				if (p_work->mAnimState & kEVTMGR_ANIM_TYPE_SCALE) {
					float tmp_width, tmp_height;
					float diff_time;
					// diff-length
					tmp_width	= (float)(p_work->mLytAnmScaleInfo.mTargetWidth	- p_work->mLytAnmScaleInfo.mBaseWidth);
					tmp_height  = (float)(p_work->mLytAnmScaleInfo.mTargetHeight- p_work->mLytAnmScaleInfo.mBaseHeight);
					// diff-time
					diff_time = (float)(p_work->mEndFrame - p_work->mStartFrame);
					// length per frame
					tmp_width  = (float)tmp_width	/ diff_time;
					tmp_height = (float)tmp_height	/ diff_time;
					
					tmp_width  = p_work->mLytAnmScaleInfo.mBaseWidth  + ((mFrameCounter - p_work->mStartFrame) * tmp_width);
					tmp_height = p_work->mLytAnmScaleInfo.mBaseHeight + ((mFrameCounter - p_work->mStartFrame) * tmp_height);
					
					p_layout->SetSize((int)tmp_width, (int)tmp_height);
				}

			} else {
				// �͈͊O�Ȃ̂ŏI��������
				p_work->mAnimState = 0x00;
			}
		}	
	}

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EventManager::SetModelAnimInfoWork(int modelIdx, tModelAnimInfo* pAnimation)
{
	APP_ASSERT_PRINT((modelIdx < mModelNum),
					 "Select modelIdx is Invalid value : %d \n", modelIdx);

	memcpy(&mpModelAnimWork[modelIdx], pAnimation, sizeof(tModelAnimInfo));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EventManager::SetLayoutAnimInfoWork(int layoutlIdx, tLayoutAnimInfo* pAnimation)
{
	APP_ASSERT_PRINT((layoutlIdx < kEVTMGR_LAYOUT_NUM),
					 "Select layoutlIdx is Invalid value : %d \n", layoutlIdx);

	memcpy(&mpLayoutAnimWork[layoutlIdx], pAnimation, sizeof(tLayoutAnimInfo));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
EventManager::EventManager() : mpCharaBase()
{
	mFrameCounter = 0;
	mIsStartEvent = false;
	mIsPauseEvent = FALSE;

	mpCameraFrame = NULL;
	mCameraKeyNum = 0;
	mNextKey = 0;
	mFileHandle = -1;

	mIsEndCamera = false;

	mModelNum		= 0;
	mpCharaBase		= NULL;

	//	memset(mpAssignLayout, 0, sizeof(mpAssignLayout));
	mpAssignLayout = NULL;
	mpAssignSystemLayout = NULL;

	mpModelAnimWork	= NULL;

	mpLayoutAnimWork = NULL;

	mpIndex = NULL;

	//	mOffsetCamPos = ZERO_VEC;
	mSetCamPosX		= 0.0f;
	mOffsetCamPosY	= 0.0f;
	mSetCamPosZ		= 0.0f;
	mOffsetCamRot	= 0.0f;

	mpCsvAccesser	= NULL;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
EventManager::~EventManager()
{
	if (mpAssignLayout != NULL) {
		for (int iI = 0; iI < kEVTMGR_LAYOUT_NUM; iI ++) {
			GraphicManager::GetInstance()->RemoveDrawLayout(&mpAssignLayout[iI]);
		}
		delete[] mpAssignLayout;
		mpAssignLayout = NULL;
	}

	if (mpAssignSystemLayout != NULL) {
		for (int iI = 0; iI < kEVTMGR_LAYOUT_NUM; iI ++) {
			GraphicManager::GetInstance()->RemoveSystemLayout(&mpAssignSystemLayout[iI]);
		}
		delete[] mpAssignSystemLayout;
		mpAssignSystemLayout = NULL;
	}

	this->EndEvent();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EventManager::getFrameCamPosTar(CameraFrame* pFrame, VECTOR* pPos, VECTOR* pTar)
{
	MATRIX mtx = MGetIdent();
	float dist;
	MATRIX tmp;
	//	float set_scale = 10.0f;
	float set_scale = mCameraScale;

	dist = pFrame->distance * set_scale;
	APP_ASSERT((dist < 0),"Camera Distance is under 0 \n");

	*pTar = VGet(pFrame->x, pFrame->y, pFrame->z);
	*pTar = VScale(*pTar, set_scale);

	tmp = MGetRotX(-pFrame->rx);
	mtx = MMult(mtx, tmp);
	tmp = MGetRotY(-pFrame->ry);
	mtx = MMult(mtx, tmp);
	tmp = MGetRotZ(-pFrame->rz);
	mtx = MMult(mtx, tmp);

	tmp = MGetScale(VGet(dist, dist, dist));
	mtx = MMult(mtx, tmp);

	*pPos = VTransform(VGet(0.0f, 0.0f, 1.0f), mtx);
	*pPos = VAdd(*pPos, *pTar);

	// �V�t�g���W��������
	*pTar = VAdd(*pTar, mShifCamPos);
	*pPos = VAdd(*pPos, mShifCamPos);

}

 /**** end of file ****/
