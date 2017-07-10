/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �R���W�������胂�f��
 * @author SPATZ.
 * @data   2012/12/29 19:02:56
 */
//---------------------------------------------------
#ifndef __MODEL_COLLISION_H__
#define __MODEL_COLLISION_H__

#include <d3dx9.h>
#include "DxLib.h"
#include "AppLib/Graphic/Shape.h"
#include "ModelPmd.h"
#include "AppLib/Collision/CollisionManager.h"
#include "EffectType.h"

/*=====================================*
 * enum
 *=====================================*/
// ���݂͂��񂾂��₯�ǁA��X�g���\��
enum eAttackType {
	eATTACK_TYPE_NORMAL = 0,
	eATTACK_TYPE_ROLLING,
	eATTACK_TYPE_JUMP,
	eATTACK_TYPE_LONGHOOK,
	eATTACK_TYPE_BOOMERANG,
	eATTACK_TYPE_BOMB,
	eATTACK_TYPE_HAND,
	eATTACK_TYPE_ARROW,
	eATTACK_TYPE_MISSILE,
	eATTACK_TYPE_ENEMY_NORMAL,
	eATTACK_TYPE_BREAK_GUARD,
	eATTACK_TYPE_COPY_ROD,				// �R�s�[���b�h
	eATTACK_TYPE_FLAME,					// ��
	eATTACK_TYPE_FLAME_BREAK_GUARD,		// ���E�K�[�h�s��
	eATTACK_TYPE_THUNDER,				// ��
	eATTACK_TYPE_TORNADE,				// ����
	eATTACK_TYPE_GUARDIAN,				// �K�[�f�B�A��

	eATTACK_TYPE_MAX,
};

/*=====================================*
 * class
 *=====================================*/
class ModelCollision : public ModelPmd
{
public:
	/* �\���� */
	struct tCheckDamageWork {
		VECTOR	mPlayerPos;
		VECTOR	mWeaponTopPos;
		VECTOR	mWeaponBtmPos;
		float	mWeaponLength;
	};

	ModelCollision();
	virtual ~ModelCollision();

	virtual void		Setup(void);
	virtual void		Update(void);
	virtual void		Destroy(void);

	virtual bool		GetIsRetire(void)						{ return false; }
	virtual bool&		IsDamage(void)							{ return mIsDamage; }
	virtual int			GetMaxHitpoint(void)					{ return 1; }
	virtual int			GetHitpoint(void)						{ return 0; }

	virtual char*		GetModelName(void)						{ return "�I�u�W�F�N�g"; }

	virtual	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType);
	virtual void		GetBodyCapsule(ShapeCapsule* pCapsule)=0;
	virtual void		GetBodyPillar(CollisionManager::tPillarWork* pPillar);
	virtual void		GetBodyFrameSphere(CollisionManager::tSphereWork* pSphere, const char* strFrameName, float sphereSize);

	virtual void		SetNowGravity(float gravity)=0;
	virtual void		GetLockonPosition(VECTOR* pPosition)	{ *pPosition = mPosition; }

	virtual bool		CheckBodyCollision(CollisionManager::tSphereWork& rOtherSphere, VECTOR& rVec, float& rDist);
	virtual bool		CheckEnableAttack(void);
	virtual BOOL		GetIsEnemy(void)						{ return FALSE; }
	
	VECTOR&				MoveVec(void)							{ return mMoveVec; }
	float&				BodySize(void)							{ return mBodySize; }
	float&				BodyHeight(void)						{ return mBodyHeight; }

	virtual bool CheckDamageCollNormal(tCheckDamageWork* pParam);
	virtual bool CheckDamageCollRolling(tCheckDamageWork* pParam);
	virtual bool CheckDamageCollJump(tCheckDamageWork* pParam);
	virtual bool CheckDamageCollLonghook(tCheckDamageWork* pParam);
	virtual bool CheckDamageCollBoomerang(tCheckDamageWork* pParam);
	virtual bool CheckDamageCollBomb(ShapeCapsule* pParam);
	virtual bool CheckDamageCollHand(ShapeCapsule* pParam);
	virtual bool CheckDamageCollArrow(tCheckDamageWork* pParam);
	virtual bool CheckDamageCollEnemyNormal(tCheckDamageWork* pParam);

protected:
	virtual void		updateAllObject(void);	// ���̃I�u�W�F�N�g�Ƃ̓����蔻��
	bool				checkLine2CharaBoard(tCheckDamageWork* pParam);
	void				setupBodyHeightSize(void);

	/* �ϐ� */
	bool				mIsDamage;
	VECTOR				mMoveVec;
	float				mBodySize;
	float				mBodyHeight;

};

#endif // __MODEL_COLLISION_H__

/**** end of file ****/

