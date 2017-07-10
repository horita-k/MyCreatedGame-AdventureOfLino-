/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
#ifndef __SHADOW_H__
#define __SHADOW_H__

/*=====================================*
 * include
 *=====================================*/
#include "dxlib.h"

//class CharaBase;
class ModelCollision;

/*=====================================*
 * class
 *=====================================*/
class Shadow {
	
 public:

	enum eUpdateType {
		eUPDATE_TYPE_ALWAYS = 0,
		eUPDATE_TYPE_ONCE,

		eUPDATE_TYPE_MAX,
	};

	static const int kRIPPLE_NUM = 4;			// �\�����鐅�ʂ̐�
	static const int kBACKUP_POLYGON_NUM = 8;	// Vertex�f�[�^�̍ő�ێ��� (�W�ȏ�|���S�����܂ޏꍇ�͎̂Ă�)

	struct tRippleWork {
		VECTOR		mVector[kBACKUP_POLYGON_NUM][3];
		VECTOR		mCharaOldPos;
		VECTOR		mNormal[kBACKUP_POLYGON_NUM];
		float		mSpeed;
		int			mVtxNum;
		int			mCount;
	};

	Shadow();
	~Shadow();

	void		Setup(ModelCollision* pChara, eUpdateType updateType);
	void		Update(void);
	void		UpdateNotRender(void);

	void		EnableShadow(void);
	void		DisableShadow(void);

	bool		IsRenderShadow(void)			{ return mIsRenderShadow; }
	VECTOR&		NormalVec(void)					{ return mNormalVec; }

	void		SetupOnceDrawShadow(void);
	void		SetUpdateType(eUpdateType updateType)			{ mUpdateType = updateType; }
	void		SetShadowVector(VECTOR moveVec);

	void		SetShadowHeightOffset(float offset)				{ mShadowHeightOffset = offset; }

	static	void EnableShadowMap(BOOL bEnable)					{ sEnableShadowMap = bEnable; }

 private:

	void		initialize(void);

	void		updateShadowVertexInfo(VERTEX3D* pVertex, MV1_COLL_RESULT_POLY* pHitRes, float shadowSize, VECTOR* pCharaPos);
	/* �ϐ� */
	typedef void (Shadow::*UPDATE_FUNC)(void);
	static UPDATE_FUNC mUpdateFunc[eUPDATE_TYPE_MAX];
	void		updateFuncAlways(void);
	void		updateFuncOnce(void);

	ModelCollision*		mpChara;
	bool				mIsRenderShadow;
	VECTOR				mNormalVec;
	eUpdateType			mUpdateType;

	// �g����
	float				mRippleCreateCount;		// �g�䐶���J�E���g
	tRippleWork*		mpRippleWork;			// �g����\����
	int					mRippleIndex;			// �g��̃C���f�b�N�X

	// ��x�����e��`�悷��ׂ̍\����
	tRippleWork			mOnceDrawWork;
	float				mShadowHeightOffset;
	static BYTE			sEnableShadowMap;

};

#endif // __SHADOW_H__

 /**** end of file ****/
