/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * PMD���f���N���X
 * @author SPATZ.
 * @data   2011/11/20 23:08:14
 */
//---------------------------------------------------
#include "AppLib/Basic/Basic.h"
#include "ModelPmd.h"
#include "Scene/TotalInfo.h"
#include "Utility/Utility.h"

#ifdef _DEBUG
#include "AppLib/Graphic/GraphicManager.h"
#endif // _DEBUG

/*=====================================*
 * static
 *=====================================*/
/*static*/
ModelPmd::PHASE_ANIMATION
ModelPmd::mPhaseAnimation[ModelPmd::eANIMATION_TYPE_MAX] = {
	&ModelPmd::phaseAnimationMotion,
	&ModelPmd::phaseAnimationMorphing,
	&ModelPmd::phaseAnimationNone,
};

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ModelPmd::ModelPmd()
{
	// initialize
	memset(&mMotion, 0x00, sizeof(mMotion));
	mMotion.mpHandle	= NULL;
	mMotion.mIndex		= -1;
	mMotion.mPrevIndex	= -1;
	mMotion.mSpeed		= 0.5f;
	mMotion.mPauseSpeed = 0.0f;
	mMotion.mBlendSpeed = 0.0f;

	this->DeattachModelAll();

	mAnimationType = eANIMATION_TYPE_MOTION;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ModelPmd::~ModelPmd()
{
	Destroy();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::Create(void)
{
	ModelBase::Create();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::Setup(bool isDefaultMotioon/*=false*/)
{
	//	if (mIsSetup) return;

	ModelBase::Setup();
	mMotion.mIndex = -1;

	// �A�j���[�V�����^�C�v�����[�V�����ɏ����ݒ�
	mAnimationType = eANIMATION_TYPE_MOTION;

	int anim_num = MV1GetAnimNum(mModelHandle);
	if (anim_num == 0) {
		// �A�j���[�V�����Ȃ����f���Ƃ��ēK�p����
		mAnimationType = eANIMATION_TYPE_NONE;
	} else {
		if (mMotion.mpHandle == NULL) {
			mMotion.mpHandle = new int[anim_num];
			APP_ASSERT( (mMotion.mpHandle != NULL),
						"Failed new mMotion.mpHandle \n");

			for (int iI = 0; iI < anim_num; iI ++) {
				mMotion.mpHandle[iI] = MV1AttachAnim(mModelHandle, iI, -1, FALSE);
				if (mMotion.mpHandle[iI] == -1) {
					PRINT_LOG_PRINT("Attach Error ");
				}
				PRINT_LOG_PRINT("MV1SetAttachAnimBlendRate = {%d, mpHandle[%d] = %d ", mModelHandle, iI, mMotion.mpHandle[iI]);
				MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[iI], 0.0f);
			}
		}
	}

	// 	���b�V���̒��_�f�B�t���[�Y�J���[���}�e���A���̃f�B�t���[�Y�J���[�̑���Ɏg�p���邩�ǂ�����ݒ肷��
	MV1SetUseVertDifColor( mModelHandle, FALSE);
	MV1SetUseVertSpcColor( mModelHandle, TRUE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::Update(void)
{
	ModelBase::Update();

	APP_ASSERT((((unsigned)mAnimationType) < eANIMATION_TYPE_MAX),
			   "Invalid mAnimationType \n");
	(this->*mPhaseAnimation[mAnimationType])();

	// �A�^�b�`�����g�̏���
	for (char iI = 0; iI < kATTACH_MODEL_NUM; iI ++) {
		if (mAttach[iI].mIsActive == TRUE) {
			MATRIX mtx = MV1GetFrameLocalWorldMatrix(mModelHandle, mAttach[iI].mFrameIndex);
			mAttach[iI].mpAttachedModel->Matrix() = mtx;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::Destroy(void)
{
	ModelBase::Destroy();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::DeleteModel(void)
{
	int ret = 0;

	if (mModelHandle != -1) {
		int anim_num = MV1GetAnimNum(mModelHandle);
		if (mMotion.mpHandle != NULL) {
			for (int iI = 0; iI < anim_num; iI ++) {
				ret = MV1DetachAnim(mModelHandle, mMotion.mpHandle[iI]);
				APP_ASSERT(ret != -1, 
						   "Failed MV1DetachAnim()\n");
				mMotion.mpHandle[iI] = -1;
			}
		}
	}

	if (mMotion.mpHandle != NULL) {
		delete[] mMotion.mpHandle;
		mMotion.mpHandle = NULL;
	}
	
	mAnimationType = eANIMATION_TYPE_NONE;

	ModelBase::DeleteModel();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::SetupMorphType(float startBlend)
{
	mMotion.mBlendRate = startBlend;

	// ���[�t�B���O�ɐݒ�
	mAnimationType = eANIMATION_TYPE_MORPHING;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::StartMorph(int animIndexA, int animIndexB, float blendSpeed)
{
	if (blendSpeed == mMotion.mBlendSpeed) return;

	APP_ASSERT(mModelHandle != -1,
			   "ModelPmd::mModelHandle is -1\n");

	mMotion.mPrevIndex	= (SHORT)animIndexA;
	mMotion.mIndex		= (SHORT)animIndexB;
	mMotion.mBlendSpeed	= blendSpeed;

	mMotion.mPrevBlendRate		= 0.0f;
	mMotion.mNowTime			= 0.0f;
	mMotion.mTotalTime			= 0.0f;
	mMotion.mIsLoop				= false;
	mMotion.mSpeed				= 0.0f;
	mMotion.mIsEnd				= false;
	mMotion.mIsBlend			= true;
	mMotion.mPauseSpeed			= 0.0f;
	mMotion.mpNextMotion		= NULL;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::StartMotion(int animIndex, bool isLoop, float animSpeed, const tMotionParam* pNextMotion/*=NULL*/, float blendSpeed/*=0.1f*/)
{
	if (mMotion.mIndex == animIndex) return;
	if (animIndex >= MV1GetAnimNum(mModelHandle) ) return;
	
	StartMotionForcibly(animIndex, isLoop, animSpeed, pNextMotion, blendSpeed);
}

//---------------------------------------------------
/**
   �������[�V�����ŋ����I�ɍĐ�������
 */
//---------------------------------------------------
void
ModelPmd::StartMotionForcibly(int animIndex, bool isLoop, float animSpeed, const tMotionParam* pNextMotion/*=NULL*/, float blendSpeed/*=0.1f*/)
{
	APP_ASSERT(mModelHandle != -1,
			   "ModelPmd::mModelHandle is -1\n");
	APP_ASSERT( (mMotion.mpHandle != NULL),
				"mMotion.mpHandle is NULL \n");
	MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[mMotion.mPrevIndex], 0.0f);

	mMotion.mPrevBlendRate	= mMotion.mBlendRate;
	mMotion.mPrevIndex		= mMotion.mIndex;

	mMotion.mBlendRate	= 0.0f;
	mMotion.mIndex		= animIndex;
	mMotion.mNowTime	= 0.0f;
	mMotion.mTotalTime	= MV1GetAttachAnimTotalTime(mModelHandle, mMotion.mpHandle[mMotion.mIndex]);
	mMotion.mIsLoop		= isLoop;
	mMotion.mSpeed		= animSpeed;
	mMotion.mBlendSpeed	= blendSpeed;
	mMotion.mIsEnd		= false;
	mMotion.mIsBlend	= true;
	mMotion.mPauseSpeed	= 0.0f;
	mMotion.mpNextMotion= (tMotionParam *)pNextMotion;
	
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::StartMotionEx(const tMotionParam* pParam)
{
	StartMotion(pParam->mIndex, pParam->mIsLoop, pParam->mSpeed, pParam->mpNextMotion);
};

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::PauseMotion(void)
{
	mMotion.mPauseSpeed = mMotion.mSpeed;
	mMotion.mSpeed = 0.0f;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::ResumeMotion(void)
{
	mMotion.mSpeed = mMotion.mPauseSpeed;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::ReplayMotion(void)
{
	mMotion.mNowTime	= 0.0f;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
ModelPmd::GetIsEndMotion(void)
{
	if (mMotion.mNowTime >= mMotion.mTotalTime) {
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
ModelPmd::GetMotionTimePercent(void)
{
	return (int)((mMotion.mNowTime / mMotion.mTotalTime) * 100.0f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::AttachModel(ModelBase* pAttachedModel, eAttachedModelType attachedModelType, int frameIndex)
{
	// ���ɓo�^����Ă��邩���`�F�b�N
	for (char iI = 0; iI < kATTACH_MODEL_NUM; iI ++) {
		if (mAttach[iI].mpAttachedModel == pAttachedModel) {
			// ���ɓo�^����Ă��郂�f���̏ꍇ�͏����𔲂���
			return;
		}
	}
	for (char iI = 0; iI < kATTACH_MODEL_NUM; iI ++) {
		if (mAttach[iI].mpAttachedModel == NULL) {
			pAttachedModel->SetMatrixType(ModelBase::eMATRIX_TYPE_MATRIX);

			mAttach[iI].mIsActive			= TRUE;
			mAttach[iI].mpAttachedModel		= pAttachedModel;
			mAttach[iI].mAttachedModelType	= attachedModelType;
			mAttach[iI].mFrameIndex			= frameIndex;
			mAttachCount ++;
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::ChangeAttachModelFrameIndex(int attachIndex, int frameIndex)
{
	APP_ASSERT_PRINT( (attachIndex < kATTACH_MODEL_NUM),
					  "Invalid attachIndex %d \n", attachIndex);
	APP_ASSERT_PRINT( (frameIndex > 0),
					  "Invalid frameIndex %d \n", frameIndex);

	mAttach[attachIndex].mFrameIndex = frameIndex;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::ChangeAttachModelPtr(int attachIndex, ModelBase* pModel)
{
	APP_ASSERT_PRINT( (attachIndex < kATTACH_MODEL_NUM),
					  "Invalid attachIndex %d \n", attachIndex);
	
	mAttach[attachIndex].mpAttachedModel = pModel;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::DeattachModelAll(void)
{
	for (char iI = 0; iI < kATTACH_MODEL_NUM; iI ++) {
		mAttach[iI].mIsActive			= FALSE;
		mAttach[iI].mpAttachedModel		= NULL;
		mAttach[iI].mFrameIndex			= -1;
	}
	mAttachCount = 0;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
ModelPmd::EqualPhaseFrame(float frame)
{
	return ( ( (frame >= mMotion.mNowTime) && (frame < mMotion.mNowTime + mMotion.mSpeed)) ? true : false);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
ModelPmd::BetweenPhaseFrame(float frameMin, float frameMax)
{
	return (( ( (frameMin) <= mMotion.mNowTime) && (frameMax > mMotion.mNowTime) ) ? true : false);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::PauseAttach(int attachIndex)
{
	mAttach[attachIndex].mIsActive = FALSE;
	mAttach[attachIndex].mpAttachedModel->SetMatrixType(ModelBase::eMATRIX_TYPE_TRS);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::ResumeAttach(int attachIndex)
{
	mAttach[attachIndex].mIsActive = true;
	mAttach[attachIndex].mpAttachedModel->SetMatrixType(ModelBase::eMATRIX_TYPE_MATRIX);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
ModelPmd::GetFrameIndexFromName(const char* frameName)
{
	return MV1SearchFrame(mModelHandle, frameName);
}

//---------------------------------------------------
/**
 * �w��t���[�����̍s��擾
 */
//---------------------------------------------------
bool
ModelPmd::GetFrameMatrixFromName(MATRIX* pOut, const char* frameName)
{
	if (pOut == NULL) return false;

	int frame_index = this->GetFrameIndexFromName(frameName);

	if (frame_index >= 0) {
		MATRIX mtx = MV1GetFrameLocalWorldMatrix(mModelHandle, frame_index);
		*pOut = mtx;
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
 * �w��t���[�����̍s��擾
 */
//---------------------------------------------------
bool
ModelPmd::GetFrameMatrixFromNameByScale(MATRIX* pOut, const char* frameName, float setScale)
{
	if (pOut == NULL) return false;
	
	int frame_index = this->GetFrameIndexFromName(frameName);

	if (frame_index >= 0) {
		MATRIX node_mtx = MV1GetFrameLocalWorldMatrix(mModelHandle, frame_index);

		MATRIX tmp;
		
		*pOut = MGetIdent();

		// �g��k���s��
		tmp = MGetScale(VGet(setScale, setScale, setScale));
		*pOut = MMult(*pOut, tmp);

		// ��]�s��
		Utility::ExtractRotationMatrix(&tmp, &node_mtx, 1.0f);
		*pOut = MMult(*pOut, tmp);

		// �ړ��s��
		VECTOR trans_pos;
		trans_pos.x = node_mtx.m[3][0];
		trans_pos.y = node_mtx.m[3][1];
		trans_pos.z = node_mtx.m[3][2];

		tmp = MGetTranslate(trans_pos);
		*pOut = MMult(*pOut, tmp);

		return true;
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
ModelPmd::GetFrameMatrixFromIndex(MATRIX* pOut, BYTE frameIndex)
{
	MATRIX mtx = MV1GetFrameLocalWorldMatrix(mModelHandle, (int)frameIndex);
	*pOut = mtx;

	return true;
}

//---------------------------------------------------
/**
   �w��t���[�����̍��W�擾
 */
//---------------------------------------------------
bool
ModelPmd::GetFramePosFromName(VECTOR* pOut, const char* frameName)
{
	if (pOut == NULL) return false;

	MATRIX mtx;
	bool ret;

	ret = this->GetFrameMatrixFromName(&mtx, frameName);
	PRINT_ASSERT(ret == true,
				 "Failed GetFramePosFromName : %s\n", frameName);
	pOut->x = mtx.m[3][0];
	pOut->y = mtx.m[3][1];
	pOut->z = mtx.m[3][2];
	
	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
ModelPmd::GetFramePosFromIndex(VECTOR* pOut, BYTE frameIndex)
{
	if (pOut == NULL) return false;

	MATRIX mtx;

	this->GetFrameMatrixFromIndex(&mtx, frameIndex);
	pOut->x = mtx.m[3][0];
	pOut->y = mtx.m[3][1];
	pOut->z = mtx.m[3][2];
	
	return true;
}

//---------------------------------------------------
/**
 * �w��̃t���[���i�m�[�h�j�̍s��Ɍʂ̃p�����[�^��ݒ肷��
 */
//---------------------------------------------------
void
ModelPmd::SetFrameUserMatrix(char* frameName, VECTOR* pTrans, VECTOR* pRotPos, VECTOR* pRotTar, VECTOR* pScale)
{
	int frame_index = 0;
	MATRIX mtx, tmp;

	/**** �w��̃t���[�����w��̃}�g���b�N�X�̏ꏊ�ɐݒ肳���� ****/
	// �w��̃}�g���b�N�X mtx �𐶐�����
	mtx = MGetIdent();
	/* �g�k�s��̌v�Z */
	tmp = MGetIdent();
	tmp = MGetScale(VGet(pScale->x, pScale->y, pScale->z));
	mtx = MMult(mtx, tmp);

	/* ��]�s��̌v�Z */
	tmp = MGetIdent();
	Utility::MTXLookAtMatrix(&tmp, pRotTar, pRotPos, &VGet(0.0f, 1.0f, 0.0f));
	mtx = MMult(mtx, tmp);

	/* �ړ��s��̌v�Z */
	tmp = MGetIdent();
	tmp = MGetTranslate(*pTrans);
	mtx = MMult(mtx, tmp);

	/* ���߂��s��̓��[���h�s��̒l�����A�ݒ肷��֐��̓��[�J���ϐ��ł̎w��ׁ̈A
	   ���[���h�s��̋t�s��������ă��[�J���s��ɔ��f�����Ă���ݒ肷�� */
	MATRIX world_mtx, inv_mtx;
	world_mtx = MV1GetFrameLocalWorldMatrix(mModelHandle, 0);
	CreateInverseMatrix(&inv_mtx, &world_mtx);
	mtx = MMult(mtx, inv_mtx);	// ���[���h���烍�[�J���֕ϊ�

	// �w��̃t���[���ɑ΂��ă��[�J���s���ݒ肷��
	frame_index = this->GetFrameIndexFromName(frameName);
	MV1SetFrameUserLocalMatrix(mModelHandle, frame_index, mtx);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::SetFrameUserRotation(char* frameName, VECTOR* pRotTar, float sensitiveRate)
{
	SetFrameUserRotation(frameName, pRotTar, &VGet(0.0f, 1.0f, 0.0f), sensitiveRate, NULL);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::SetFrameUserRotation(char* frameName, VECTOR* pRotTar, VECTOR* pUpVec, float sensitiveRate, MATRIX* pOutParentInvMtx/*=NULL*/)
{
	int frame_index = 0;
	MATRIX target_mtx, tmp, world, set_mtx;
	VECTOR rot_pos;
	MATRIX world_mtx, inv_mtx;

	frame_index = this->GetFrameIndexFromName(frameName);

	MATRIX local_mtx;
	local_mtx = MV1GetFrameLocalMatrix(mModelHandle, frame_index);

	// �w��̃}�g���b�N�X mtx �𐶐�����
	target_mtx = MGetIdent();
	/* �g�k�s��̌v�Z */
	tmp = MGetIdent();
	tmp = MGetScale(VGet(mScale.x, mScale.y, mScale.z));
	target_mtx = MMult(target_mtx, tmp);

	/* ��]�s��̌v�Z */
	tmp = MGetIdent();
	world = MV1GetFrameLocalWorldMatrix(mModelHandle, frame_index);
	Utility::ExtractMatrixPosition(&rot_pos, &world);

	// ��]�s��̌v�Z
	Utility::MTXLookAtMatrix(&tmp, pRotTar, &rot_pos, pUpVec);
	/* ���߂��s��̓��[���h�s��̒l�����A�ݒ肷��֐��̓��[�J���ϐ��ł̎w��ׁ̈A
	   ���[���h�s��̋t�s��������ă��[�J���s��ɔ��f�����Ă���ݒ肷�� */
	world_mtx = MV1GetFrameLocalWorldMatrix(mModelHandle, 0);
	CreateInverseMatrix(&inv_mtx, &world_mtx);
	tmp = MMult(tmp, inv_mtx);	// ���[���h���烍�[�J���֕ϊ�
	target_mtx = MMult(target_mtx, tmp);

	if (pOutParentInvMtx != NULL) {
		*pOutParentInvMtx = inv_mtx;
	}

	/* �ړ��s��̌v�Z */
	target_mtx.m[3][0] = local_mtx.m[3][0];
	target_mtx.m[3][1] = local_mtx.m[3][1];
	target_mtx.m[3][2] = local_mtx.m[3][2];

	set_mtx = MAdd(MScale(target_mtx,sensitiveRate * 2.0f) , 
				   MScale(local_mtx, (1.0f - sensitiveRate) * 2.0f));
	set_mtx = MScale(set_mtx, 0.5f);

	// �w��̃t���[���ɑ΂��ă��[�J���s���ݒ肷��
	MV1SetFrameUserLocalMatrix(mModelHandle, frame_index, set_mtx);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::SetFrameUserRotationEx(int childFrameIndex, VECTOR& rTargetRot, VECTOR& rUpVec, float sensitiveRate, MATRIX& rInvMatrix)
{
	// �e�̃t���[���ԍ����擾
	int parent_frame_index = MV1GetFrameParent(mModelHandle, childFrameIndex);
	
	// �e�Ǝq�̃m�[�h�Ԃ̋��������߂�
	float node_length = 30.0f;

	// �e�s��̏����擾
	MATRIX parent_world_mtx, parent_local_mtx;
	parent_world_mtx = MV1GetFrameLocalWorldMatrix(mModelHandle, parent_frame_index);
	parent_local_mtx = MV1GetFrameLocalMatrix(mModelHandle, parent_frame_index);
	
	// �q�̃��[���h�s���e�s��̏�񂩂�v�Z���ċ��߂�
	VECTOR child_world_pos;
	{
		// ���[���h���W�Őe�̈ړ���̍��W�����߂�
		VECTOR parent_world_pos;
		Utility::ExtractMatrixPosition(&parent_world_pos, &parent_world_mtx);

		// ���[���h�̐e�m�[�h�̉�]�x�N�g�������߂�
		VECTOR parent_world_rot;
		parent_world_rot = VNorm(VTransformSR(VGet(0.0f, 0.0f, 1.0f), parent_world_mtx) );
		
		// �q�m�[�h�̈ʒu�����߂�
		child_world_pos = VAdd(parent_world_pos, 
							   VScale(parent_world_rot, (node_length * mScale.x) ) );
	}
	
	//<! �q�̃��[���h�s������߂�
	MATRIX child_world_mtx, tmp_mtx;
	{
		child_world_mtx = MGetIdent();

		// �g�k
		tmp_mtx = MGetScale(VGet(mScale.x, mScale.y, mScale.z) );
		child_world_mtx = MMult(child_world_mtx, tmp_mtx);

		// ��]
		Utility::MTXLookAtMatrix(&tmp_mtx, &rTargetRot, &child_world_pos, &rUpVec);
		child_world_mtx = MMult(child_world_mtx, tmp_mtx);

		// �ړ�
		tmp_mtx = MGetTranslate(child_world_pos);
		child_world_mtx = MMult(child_world_mtx, tmp_mtx);
	}

	// �q�̃��[�J���s������߂�
	MATRIX child_local_mtx;
	{
		// �v�Z��A��x���[�J�����W�Ƀ��[���h�l��ݒ肷��
		child_local_mtx = child_world_mtx;

		MATRIX root_mtx, inv_mtx;
		MATRIX calc_mtx;
		calc_mtx = rInvMatrix;
		
		// �e�̃��[�J���s��̋t�s������߁A������
		root_mtx = MV1GetFrameLocalMatrix(mModelHandle, parent_frame_index);
		CreateInverseMatrix(&inv_mtx, &root_mtx);
		calc_mtx = MMult(calc_mtx, inv_mtx);

		child_local_mtx = MMult(child_local_mtx, calc_mtx);

		rInvMatrix = calc_mtx;
	}
	
	MATRIX set_mtx, org_local_mtx;
	{
		org_local_mtx = MV1GetFrameLocalMatrix(mModelHandle, childFrameIndex); 
		
		set_mtx = MAdd(MScale(child_local_mtx, sensitiveRate * 2.0f),
					   MScale(org_local_mtx, (1.0f - sensitiveRate) * 2.0f) );
		set_mtx = MScale(set_mtx, 0.5f);
	}

	MV1SetFrameUserLocalMatrix(mModelHandle, childFrameIndex, set_mtx);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::ResetFrameUserMatrix(char* frameName)
{
	int frame_index = 0;
	frame_index = this->GetFrameIndexFromName(frameName);
	
	MV1ResetFrameUserLocalMatrix(mModelHandle, frame_index);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::ResetMotion(void)
{
	mMotion.mIndex = 0;
	mMotion.mPrevIndex = 1;
	mMotion.mIsBlend = true;

	mMotion.mBlendRate = 0.1f;
	mMotion.mSpeed = 0.5f;

	int anim_num = MV1GetAnimNum(mModelHandle);
	for (int iI = 0; iI < anim_num; iI ++) {
		MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[iI], 0);
	}
	MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[mMotion.mIndex], 1);
	MV1SetAttachAnimTime(mModelHandle, 
						 mMotion.mpHandle[mMotion.mIndex], 
						 0);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
ModelPmd::GetAttachedNum(void)
{
	return mAttachCount;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ModelPmd::tAttachWork*
ModelPmd::GetAttachAddr(int attachedIndex)
{
	return &mAttach[attachedIndex];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ModelPmd::DumpParam(void)
{
	PRINT_LOG_PRINT("mMotion.mPrevBlendRate = %.2f", mMotion.mPrevBlendRate);
	PRINT_LOG_PRINT("mMotion.mPrevIndex = %d ", mMotion.mPrevIndex);
	PRINT_LOG_PRINT("mMotion.mBlendRate = %.2f ",mMotion.mBlendRate );
	PRINT_LOG_PRINT("mMotion.mIndex = %d ", mMotion.mIndex);
	PRINT_LOG_PRINT("mMotion.mNowTime = %.2f ", mMotion.mNowTime);
	PRINT_LOG_PRINT("mMotion.mTotalTime = %.2f ", mMotion.mTotalTime);
	PRINT_LOG_PRINT("mMotion.mIsLoop = %d ", mMotion.mIsLoop);
	PRINT_LOG_PRINT("mMotion.mSpeed = %.2f ", mMotion.mSpeed);
	PRINT_LOG_PRINT("mMotion.mBlendSpeed = %.2f ", mMotion.mBlendSpeed);
	PRINT_LOG_PRINT("mMotion.mIsEnd = %d ", mMotion.mIsEnd);
	PRINT_LOG_PRINT("mMotion.mIsBlend = %d ", mMotion.mIsBlend);
	PRINT_LOG_PRINT("mMotion.mPauseSpeed = %.2f ", mMotion.mPauseSpeed);
	PRINT_LOG_PRINT("mMotion.mpNextMotion = 0x%x ", (unsigned long)mMotion.mpNextMotion);
}

//---------------------------------------------------
/**
 * ���[�V�����̃A�j���[�V����������
 */
//---------------------------------------------------
void
ModelPmd::phaseAnimationMotion(void)
{
	APP_ASSERT((mModelHandle != -1), "mModelHandle is Invalid Value \n");

	// �u�����h�֘A
	if (mMotion.mIsBlend) {

		mMotion.mBlendRate += mMotion.mBlendSpeed;
		if (mMotion.mBlendRate >= 1.0f) {
			mMotion.mBlendRate = 1.0f;
			mMotion.mIsBlend = false;
		}

		MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[mMotion.mIndex],		mMotion.mBlendRate);
		MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[mMotion.mPrevIndex],	(1.0f - mMotion.mBlendRate) );
	}

	// �A�j���[�V�������Ԋ֘A
	mMotion.mNowTime += mMotion.mSpeed;
	if (mMotion.mNowTime >= mMotion.mTotalTime) {
		if (mMotion.mIsLoop) {	// ���[�v�̏ꍇ
			mMotion.mNowTime = mMotion.mNowTime - mMotion.mTotalTime;
		} else {				// ���[�v�ȊO
			if (mMotion.mpNextMotion != NULL) {
				StartMotionEx(mMotion.mpNextMotion);
			} else {
				mMotion.mNowTime = mMotion.mTotalTime;
				mMotion.mIsEnd = true;
			}
		}
	}
	MV1SetAttachAnimTime(mModelHandle, mMotion.mpHandle[mMotion.mIndex], mMotion.mNowTime);
}

//---------------------------------------------------
/**
 * ���[�V�����̃A�j���[�V����������
 */
//---------------------------------------------------
void
ModelPmd::phaseAnimationMorphing(void)
{
	APP_ASSERT((mModelHandle != -1), "mModelHandle is Invalid Value \n");

	// �u�����h�֘A
	if (mMotion.mIsBlend) {

		mMotion.mBlendRate += mMotion.mBlendSpeed;
		if (mMotion.mBlendRate >= 1.0f) {
			mMotion.mBlendRate = 1.0f;
			mMotion.mIsBlend = false;
		} else if (mMotion.mBlendRate <= 0.0f) {
			mMotion.mBlendRate = 0.0f;
			mMotion.mIsBlend = false;
		}

		MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[mMotion.mIndex], mMotion.mBlendRate);
		MV1SetAttachAnimBlendRate(mModelHandle, mMotion.mpHandle[mMotion.mPrevIndex], 1.0f - mMotion.mBlendRate);
	}
}

//---------------------------------------------------
/**
 * ���[�V�����̃A�j���[�V����������
 */
//---------------------------------------------------
void
ModelPmd::phaseAnimationNone(void)
{
	return; /* nop */
}

 /**** end of file ****/


