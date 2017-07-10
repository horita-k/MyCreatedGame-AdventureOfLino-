/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ���\�[�X���f���N���X
 * @author SPATZ.
 * @data   2011/11/27 15:30:43
 */
//---------------------------------------------------
#ifndef __RESOURCE_MODEL_H__
#define __RESOURCE_MODEL_H__

#include <vector>
#include "../Basic/Basic.h"

/*=====================================*
 * class
 *=====================================*/
class ResourceModel
{
 public:
	static ResourceModel* const GetInstance(void);

	void		Destroy(void);

	void		LoadCharaResource(char* charaName);
	void		LoadItemResource(const char* modelName);
	void		LoadGimmickResource(const char* modelName);
	void		LoadModelResource(const char* modelName);
	void		LoadStageResource(char* stageName, BOOL isExtData);
	void		LoadDragMv1Resource(char* mv1Name);

	void		UnloadResource(char* resourceName);

	bool		CheckLoadComponent(const char* cmpName);
	int			GetCloneHandle(const char* charaName);

	char*		GetResourceName(char* searchWord, int startIndex);

private:
	ResourceModel();
	~ResourceModel();

	void		loadResource(const char* resourceName, const char* format);
	void		unloadResource(const char* resourceName);

	struct tComponentWork {
		char*	mName;
		int		mHandle;
	};

	std::vector<tComponentWork> mComponentList;

};

#endif // __RESOURCE_MODEL_H__

 /**** end of file ****/

