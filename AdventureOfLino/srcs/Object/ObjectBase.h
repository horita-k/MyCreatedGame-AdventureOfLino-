/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �I�u�W�F�N�g�x�[�X�N���X
 * @author SPATZ.
 * @data   2012/10/01 23:27:16
 */
//---------------------------------------------------
#ifndef __OBJECT_BASE_H__
#define __OBJECT_BASE_H__

#include "AppLib/Graphic/ModelCollision.h"
#include "Scene/TotalInfo.h"

typedef void (*GET_ITEM_HANDLER)(eItemKind);

/*=====================================*
 * class
 *=====================================*/
class ObjectBase : public ModelCollision {
 public:

	/* ��` */
	enum {
		eITEM_STATE_IDLE = 0,

		eITEM_STATE_MAX,
	};

	enum eObjectType {
		eOBJECT_TYPE_ERROR = -1,

		eOBJECT_TYPE_TREASURE = 0,		// ��
		eOBJECT_TYPE_SWITCH,			// �X�C�b�`
		eOBJECT_TYPE_ITEM,				// �A�C�e��
		eOBJECT_TYPE_HAND,				// �莝���A�C�e��
		eOBJECT_TYPE_WEAPON,			// ����

		eOBJECT_TYPE_MAX,
	};

	// �I�u�W�F�N�g�̒��׏������L����
	enum eObjectCheck {
		eOBJECT_CHECK_ENABLE = 0,
		eOBJECT_CHECK_DISABLE,
		
		eOBJECT_CHECK_MAX,
	};

	struct tObjectBaseParam {
		const char*		mResourceName;
		float			mObjectSize;
		float			mScale;
		eObjectCheck	mObjectCheck;

	};

	/* �֐� */
	ObjectBase();
	~ObjectBase();

	static void					LoadAllObjectBase(void);
	static void					SetGetItemHandler(GET_ITEM_HANDLER handler);
	static GET_ITEM_HANDLER		GetGetItemHandler(void);
	static ObjectBase*			CreateInstance(USHORT objectKind, USHORT stageIndex);
	static tObjectBaseParam*	GetObjectBaseParam(int eObjectKind);

	// ActiveObjectList �֘A
	static std::vector<ObjectBase *>&	GetActiveObjectListRef(void);
	static void					AddActiveObjectList(ObjectBase* pObject);
	static void					RemoveActiveObjectList(ObjectBase* pObject);

	virtual void		Setup(int objectIdx, eItemKind itemIdx, VECTOR* pSetPos, int rotDeg);
	virtual void		Update(void);
	virtual	bool		CheckDamageCollNormal(tCheckDamageWork* pParam);
	virtual	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType);
	virtual void		ForciblySetPositionByCentering(void);

	void				GetBodyCapsule(ShapeCapsule* pCapsule);
	void				SetNowGravity(float gravity);

	virtual void		SetUsed(void) = 0;
	virtual void		StartCheckEvent(void)	{}

	void				HideTreasure(int hideEventIndex);
	void				DeHideTreasure(void);
	int					GetHideEventIndex(void)				{ return mHideEventIndex; }
	void				InsertDrawModelSelf(void);

	bool		IsUsed(void)					{ return mIsUsed; }
	int			GetItemKind(void)				{ return mItemKind; }
	int			GetObjectKind(void)				{ return mObjectIndex; }
	eObjectType	GetObjectType(void)				{ return mObjectType; }

 protected:
	eObjectType	mObjectType;

	int			mObjectIndex;
	eItemKind	mItemKind;
	bool		mIsUsed;

	/*�ϐ�*/
	int			mHideEventIndex;
	bool		mIsHide;
	ModelBase*	mpEntryTargetModel;

	/*�ϐ�*/
};

#endif // __OBJECT_BASE_H__

 /**** end of file ****/

