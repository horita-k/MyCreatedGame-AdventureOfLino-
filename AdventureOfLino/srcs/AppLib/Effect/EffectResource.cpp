/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �G�t�F�N�g�Ǘ��N���X
 * @author SPATZ.
 * @data   2014/01/28 23:12:38
 */
//---------------------------------------------------
/*=====================================*
 * include
 *=====================================*/
#include "AppLib/Basic/Basic.h"
#include "AppLib/Graphic/CameraManager.h"

#include "EffectResource.h"
#include "AppLib/Graphic/EffectType.h"

#ifdef _DEBUG
#include "Charactor/CharaBase.h"
#include "AppLib/Graphic/GraphicManager.h"
#endif // _DEBUG

#ifdef _DEBUG
#define PRINT_CONSOLE_EFF(str, ...)		PRINT_CONSOLE(str, __VA_ARGS__);
#else 
#define PRINT_CONSOLE_EFF(str, ...)
#endif // _DEBUG

/*=====================================*
 * static 
 *=====================================*/
static const BYTE kSTENCIL_INFO_NUM = 8;		//<! �����̒l�� mStencilHandleUseBit(8bit) �ƘA�����Ă���ׁA
												//<! 9�ȏ�ɂ���ꍇ�� mStencilHandleUseBit �̌^�� WORD / DWORD �ɕύX���邱�ƁI

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
EffectResource::EffectResource()
{
	// �����o�ϐ�������
	initParameters();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
EffectResource::~EffectResource()
{
	Destroy();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::Create(const char* effectName, const char* modelName)
{
	bool result = false;
	D3DXEFFECT_DESC		effect_desc;
	D3DXPARAMETER_DESC	param_desc;
	D3DXTECHNIQUE_DESC	technique_desc;

	LPSTR				tex_name = {0};

	char res_name[128] = {0};
	char root_dir[128] = {0};
	// ���f���̓ǂݍ���

	// ���[�g�f�B���N�g���̐ݒ�
	//<! �����ł� res_name �� root_dir �����ׂ̈̂����̃o�b�t�@�Ƃ��Ďg�p���Ă�̂Œ��ӁI
	int path_len = strlen(effectName);
	for (int iI = (path_len - 1); iI >= 0; iI -- ) {
		if ((effectName[iI] == '/') || (effectName[iI] == '\\')) {
			memcpy(res_name, effectName, iI);
			break;
		}
	}
	sprintf_s(root_dir, sizeof(root_dir), "object/Effect/%s", res_name);

	// �G�t�F�N�g�t�@�C���̓ǂݍ���
	memset(res_name, 0, sizeof(res_name));
	sprintf_s(res_name, sizeof(res_name), "object/Effect/%s", effectName);
	result = loadEff(res_name);
	SYSTEM_ERROR((result != false),
				 "\n %s�̃G�t�F�N�g�t�@�C���ǂݍ��݂Ɏ��s���܂����B\n�Q�[�����I�����܂��B", res_name);
	
	// ���f���t�@�C���̓ǂݍ���
	if (modelName != NULL) {
		memset(res_name, 0, sizeof(res_name));
		sprintf_s(res_name, sizeof(res_name), "object/Effect/%s", modelName);
	} else {
		int effname_len = strlen(res_name);
		res_name[effname_len -2] = 'x';
		res_name[effname_len -1] = NULL;
	}
	result = loadMdl(res_name);
	SYSTEM_ERROR((result != false),
				 "\n %s�̃��f���t�@�C���ǂݍ��݂Ɏ��s���܂����B\n�Q�[�����I�����܂��B", res_name);

	// �G�t�F�N�g�̏����擾
	EffectResource::getEffectInfo(&effect_desc);

	// �����o�ϐ��ɃG�t�F�N�g����W�J
	mParameterNum	= effect_desc.Parameters;
	mTechniqueNum	= effect_desc.Techniques;
	mFunctionNum	= effect_desc.Functions;
	mpPassNum = new BYTE[mTechniqueNum];
	APP_ASSERT( (mpPassNum != NULL), "Failed new mpPassNum\n");
	// �p�X���̎擾
	for (int iI = 0; iI < mTechniqueNum; iI ++) {
		EffectResource::getTechInfo(&technique_desc, iI);
		mpPassNum[iI] = technique_desc.Passes;
	}

#ifdef _DEBUG
	LogoutEffectInfo();
#endif // _DEBUG

	// �p�����[�^���̎擾
	D3DXHANDLE parameter_handle= NULL;
	for (BYTE param_cnt = 0; param_cnt < mParameterNum; param_cnt ++) {
		parameter_handle = EffectResource::getParamInfo(&param_desc, param_cnt);
		
		// �p�����[�^���Z�}���e�B�b�N���̏ꍇ
		if (param_desc.Semantic != NULL) {
			addSemanticInstance(param_cnt, &param_desc);
		// �p�����[�^���e�N�X�`�����̏ꍇ���A�Z�}���e�B�b�N���w�肳��Ă��Ȃ��p�����[�^�݂̂��e�N�X�`���t�@�C���Ǝw�肷��
		} else if ((param_desc.Type == D3DXPT_TEXTURE)     ||
				   (param_desc.Type == D3DXPT_TEXTURE1D)   || 
				   (param_desc.Type == D3DXPT_TEXTURE2D)   ||
				   (param_desc.Type == D3DXPT_TEXTURE3D)) {
			if (param_desc.Semantic == NULL) {
				// �A�m�e�[�V���������擾
				tAnnotationWork annowork[1] = {"ResourceName", D3DXPT_STRING, (void *)&tex_name,};
				getAnnotationParam(param_cnt, 1, annowork, (sizeof(annowork) / sizeof(tAnnotationWork)));
				
				memset(res_name, 0x00, sizeof(res_name));
				sprintf_s(res_name, sizeof(res_name), "%s/%s", root_dir, tex_name);
				result = loadTex(res_name, parameter_handle);
				SYSTEM_ERROR((result != false),
							 "\n %s�̃e�N�X�`���t�@�C���ǂݍ��݂Ɏ��s���܂����B\n�Q�[�����I�����܂��B", res_name);
			}
		}
	}
	
	int tech_index = 0;
	EffectResource::getTechInfo(&technique_desc, tech_index);
	// �e�N�j�b�N�̃X�N���v�g���̐���
	if (technique_desc.Annotations != 0) {
		setupTechniqueScript(tech_index);
	}

	// �p�X�̕`����̐���
	setupDrawTypeByPass(tech_index);

}

//---------------------------------------------------
/**
 * Technique �̃X�N���v�g�����擾����
 */
//---------------------------------------------------
void
EffectResource::setupTechniqueScript(int techIndex)
{
	LPSTR tech_script = NULL;
	char* p_script_buf = NULL;

	char* tokptr = NULL;
	int script_kind = 0;
	char* tok_tmp = NULL;

	D3DXPARAMETER_DESC anno_desc = {0};

	D3DXHANDLE hAnno;
	hAnno = EffectResource::getTechAnnotationInfo(&anno_desc, techIndex);
	mpEffect->GetString(hAnno, (LPCSTR *)&tech_script);

	int script_len = 0;
	script_len = strlen(tech_script);
	p_script_buf = new char[(script_len + 1)];
	memset(p_script_buf, 0, (script_len + 1));

	strcpy_s(p_script_buf, (script_len + 1), (char *)tech_script);

	tScriptWork* p_script = NULL;
	char* access_ptr;

	access_ptr = p_script_buf;

	for (;;) {

		/**** �R�}���h���̎擾 ****/
		tokptr = strtok(access_ptr, "=");
		if (tokptr == NULL) {
			break;
		}

		// ���[�N�̊m��
		p_script = new tScriptWork;

		for (script_kind = 0; script_kind < eSCRIPT_MAX; script_kind ++) {
			if (strcmp(tokptr, strScriptName[script_kind]) == 0) {
				break;
			}
		}
		p_script->mScriptKind		= (eScriptKind)script_kind;

		access_ptr = NULL;

		/**** �p�����[�^�̎擾 ****/
		char check_word = tokptr[strlen(tokptr)+1];
		// Script�̕������ = �� ; ���������Ă�ꍇ�̍l��
		memset(p_script->mParamString, 0, sizeof(p_script->mParamString));
		if (check_word != ';') {
			tokptr = strtok(access_ptr,  ";");
			strcpy_s(p_script->mParamString, sizeof(p_script->mParamString), tokptr);
		} else {
			access_ptr = tokptr + (strlen(tokptr) + 1 + 1);
			strcpy_s(p_script->mParamString, sizeof(p_script->mParamString), "");
		}

		/**** �p�����[�^������������ ****/
		if (p_script->mScriptKind == eSCRIPT_Pass) {
			D3DXHANDLE hPass = NULL;
			D3DXPASS_DESC pass_desc;
			// �p�X�̏ꍇ�́A���O�ɕR�Â� Pass���ɕύX����
			for (int iI = 0; iI < mpPassNum[techIndex]; iI ++) {
				EffectResource::getPassInfo(&pass_desc, techIndex, iI);

				if (strcmp(pass_desc.Name, p_script->mParamString) == 0) {
					memset(p_script->mParamString, 0, sizeof(p_script->mParamString));
					sprintf_s(p_script->mParamString, sizeof(p_script->mParamString), "%d", iI);
					break;
				}
			}
		}
		mScriptList.push_back(p_script);
	}

	if (p_script_buf != NULL) {
		delete[] p_script_buf;
		p_script_buf = NULL;
	}

}

//---------------------------------------------------
/**
 * Pass�̕`��^�C�v��Geometory��Buffer���̏����Z�b�g�A�b�v����
 */
//---------------------------------------------------
void
EffectResource::setupDrawTypeByPass(int techIndex)
{
	D3DXPASS_DESC		pass_desc;
	D3DXPARAMETER_DESC	anno_desc;
	LPSTR draw_type_string = NULL;
	
	ePassDrawType pass_draw_type;

	// �p�X�̏ꍇ�́A���O�ɕR�Â� Pass���ɕύX����
	for (int iI = 0; iI < mpPassNum[techIndex]; iI ++) {

		// �f�t�H���g�ł� Geometory ��ݒ肵�Ă���
		pass_draw_type = ePASS_DRAW_TYPE_GEOMETORY;

		// �p�X���擾
		EffectResource::getPassInfo(&pass_desc, techIndex, iI);

		if (pass_desc.Annotations > 0) {
			
			D3DXHANDLE hAnno;
			hAnno = EffectResource::getPassAnnotationInfo(&anno_desc, iI, 0);
			if (strcmp(anno_desc.Name, "Script") == 0) {
				if (anno_desc.Type == D3DXPT_STRING) {
					mpEffect->GetString(hAnno, (LPCSTR *)&draw_type_string);
					if (strcmp(draw_type_string, "Draw=Buffer;") == 0) {
						pass_draw_type = ePASS_DRAW_TYPE_BUFFER;
					}
				}
			}
		}

		mPassDrawTypeList.push_back(pass_draw_type);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::Destroy(void)
{
	SAFE_RELEASE(mpEffect);
	SAFE_RELEASE(mpBuffer);

	// �X�e���V�����̔j��
	if (mpStencilInfo != NULL) {
		for (int iI = 0; iI < kSTENCIL_INFO_NUM; iI ++) {
			for (int iJ = 0; iJ < eRENDER_COLOR_TARGET_MAX; iJ ++) {
				SAFE_RELEASE(mpStencilInfo[iI].mpRenderColorTarget[iJ]);
			}
			SAFE_RELEASE(mpStencilInfo[iI].mpRenderDepthStencilTarget);
		}
		delete[] mpStencilInfo;
		mpStencilInfo = NULL;
	}

	{
		std::vector<IDirect3DTexture9 *>::iterator ite;
		for (ite = mTextureList.begin(); ite != mTextureList.end(); ite ++) {
			SAFE_RELEASE(*ite);
		}
		mTextureList.clear();
	}
	{
		std::vector<tSemanticInfo *>::iterator ite;
		for (ite = mSemanticList.begin(); ite != mSemanticList.end(); ite ++) {
			delete (*ite);
		}
		mSemanticList.clear();
	}
	{
		std::vector<tScriptWork *>::iterator ite;
		for (ite = mScriptList.begin(); ite != mScriptList.end(); ite ++) {
			tScriptWork* p_script = (*ite);
			if (p_script != NULL) {
				delete p_script;
			}
		}
		mScriptList.clear();
	}

	mPassDrawTypeList.clear();

	if (mModelWork.mpMesh != NULL) {
		mModelWork.mpMesh->Release();
		mModelWork.mpMesh = NULL;
	}

	if (mModelWork.mpAdjucency != NULL) {
		mModelWork.mpAdjucency->Release();
		mModelWork.mpAdjucency = NULL;
	}
	if (mModelWork.mpMaterialBuffer != NULL) {
		mModelWork.mpMaterialBuffer->Release();
		mModelWork.mpMaterialBuffer = NULL;
	}
	if (mModelWork.mpEffectInstance != NULL) {
		mModelWork.mpEffectInstance->Release();
		mModelWork.mpEffectInstance = NULL;
	}
	mModelWork.mMaterialsNum = 0;

	if (mpPassNum != NULL) {
		delete[] mpPassNum;
		mpPassNum = NULL;
	}

	// �����o�ϐ�������
	initParameters();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
EffectResource::GetSemanticNum(void)
{
	return mSemanticList.size();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
EffectResource::GetSemanticNum(eSmtKind semanticKind)
{
	int num = 0;

	std::vector<tSemanticInfo *>::iterator ite;
	for (ite = mSemanticList.begin(); ite != mSemanticList.end(); ite ++) {
		if ( (*ite)->mSemKind == semanticKind) {
			num ++;
		}
	}
	return num;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
eSmtKind
EffectResource::GetSemanticType(int index)
{
	return mSemanticList.at(index)->mSemKind;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::GetSemanticName(eSmtKind semanticKind, int index)
{
	int num = 0;
	D3DXEFFECT_DESC		effect_desc		= {0};
	D3DXHANDLE			hParam			= NULL;
	D3DXPARAMETER_DESC	param_desc		= {0};

	std::vector<tSemanticInfo *>::iterator ite;
	for (ite = mSemanticList.begin(); ite != mSemanticList.end(); ite ++) {
		if ( (*ite)->mSemKind == semanticKind) {

			if (index == num) {

				// �G�t�F�N�g�̏����擾
				mpEffect->GetDesc(&effect_desc);
				
				// �p�����[�^���̎擾
				hParam = mpEffect->GetParameter(NULL, (*ite)->mSemIndex);
				mpEffect->GetParameterDesc(hParam, &param_desc);
				return param_desc.Name;
			}
			num ++;
		}
	}

	return NULL;

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
EffectResource::GetScriptNum(void)
{
	return mScriptList.size();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
EffectResource::tScriptWork*
EffectResource::GetScriptWork(int index)
{
	return mScriptList.at(index);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
DWORD
EffectResource::GetMaterialNum(void)
{
	return mModelWork.mMaterialsNum;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::DrawModel(int materialIndex)
{
	mModelWork.mpMesh->DrawSubset(materialIndex);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::GetCloneEffect(ID3DXEffect** ppEffect)
{
	HRESULT hr;

	// ���\�[�X�̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 p_device = NULL;

	hr = mpEffect->GetDevice(&p_device);
	APP_ASSERT(hr != D3DERR_INVALIDCALL,
			   "Failed ID3DXEffect::GetDevice()\n");
	hr = mpEffect->CloneEffect(p_device, ppEffect);
	APP_ASSERT(hr != D3DERR_INVALIDCALL,
			   "Failed ID3DXEffect::CloneEffect() : Invalid Call\n");
	APP_ASSERT(hr != D3DXERR_INVALIDDATA,
			   "Failed ID3DXEffect::CloneEffect() : Invalid Data\n");
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::GetSemanticHandle(ID3DXEffect* pEffect, int index)
{
	return pEffect->GetParameter(NULL, mSemanticList.at(index)->mSemIndex);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
EffectResource::ePassDrawType
EffectResource::GetPassDrawType(int index)
{
	return mPassDrawTypeList.at(index);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
EffectResource::IsExistStencilSurface(void)
{
	return (mpStencilInfo == NULL) ? false : true;
}

//---------------------------------------------------
/**
 * ���g�p�̃X�e���V���T�[�t�F�C�X�̃n���h�����������Ă��̒l��Ԃ�
 * ���S�Ďg�p����Ă���ꍇ��0��Ԃ��B(�d�l�Ƃ���)
 */
//---------------------------------------------------
void
EffectResource::HoldStencilSurfaceHandle(BYTE& rGetHandle)
{
	BYTE check_bit = 0;

	for (int iI = 0; iI < kSTENCIL_INFO_NUM; iI ++) {
		check_bit = (BYTE)(1 << iI);
		if (!(mStencilHandleUseBit & check_bit ) ) {
			rGetHandle = iI;
			mStencilHandleUseBit |= check_bit;
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::ReleaseStencilSurfaceHandle(BYTE& rGetHandle)
{
	BYTE check_bit = 0;

	check_bit = (BYTE)(1 << rGetHandle);
	mStencilHandleUseBit &= ~check_bit;

	rGetHandle = 0xFF;	//<! �֐������Ńn���h��������������
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
IDirect3DTexture9*
EffectResource::GetRenderColorTargetTexture(BYTE StencilSurfaceHandle, int index)
{
	return mpStencilInfo[StencilSurfaceHandle].mpRenderColorTarget[index];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
IDirect3DSurface9*
EffectResource::GetRenderDepthStencilSurface(BYTE StencilSurfaceHandle)
{
	return mpStencilInfo[StencilSurfaceHandle].mpRenderDepthStencilTarget;
}

#ifdef _DEBUG
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::LogoutEffectInfo(void)
{
	/* nop. */
}
#endif // _DEBUG

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::initParameters(void)
{
	mpEffect	= NULL;
	mpBuffer	= NULL;
	memset(&mModelWork, 0, sizeof(mModelWork));

	mParameterNum	= 0;
	mTechniqueNum	= 0;
	mFunctionNum	= 0;
	mpPassNum		= NULL;

	mTextureList.clear();
	mSemanticList.clear();
	mScriptList.clear();
	mPassDrawTypeList.clear();
	mStencilHandleUseBit = 0x00;

	mpStencilInfo = NULL;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
EffectResource::loadEff(const char* fileName)
{
	HRESULT hr;

	// �t�@�C������G�t�F�N�g�����[�h
	hr = D3DXCreateEffectFromFile(GET_D3DDEV(),
								  fileName,
								  NULL,
								  NULL,
								  //								  0,
								  D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
								  NULL,
								  &mpEffect,
								  &mpBuffer);
	
    if ( FAILED(hr) ) {
		// �t�@�C�������݂��Ȃ�
		if (mpBuffer == NULL) {
			PRINT_CONSOLE("%s(L:%d) : mpBuffer is NULL \n", __FILE__, __LINE__);
			return false;
		}
        //�G���[���b�Z�[�W�\��(�����R�[�h��ANSI)
        ::MessageBoxA(NULL,(LPCSTR)(mpBuffer->GetBufferPointer()),"",MB_OK);
        mpBuffer->Release();
		PRINT_CONSOLE("%s(L:%d) : FAILED(hr) \n", __FILE__, __LINE__);
		return false;
	}
	if (hr != D3D_OK) {
		PRINT_CONSOLE("%s(L:%d) : hr != D3D_OK \n", __FILE__, __LINE__);
		return false;
	}

	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
EffectResource::loadMdl(const char* fileName)
{
	HRESULT hr;
	tModelWork* p_work = &mModelWork;

	hr = D3DXLoadMeshFromX(fileName,					// pFilename
//						   D3DXMESH_MANAGED,			// Options
							0,
						   GET_D3DDEV(),				// pDevice
						   &p_work->mpAdjucency,		// ppAdjacency
						   &p_work->mpMaterialBuffer,	// ppMaterials
						   &p_work->mpEffectInstance,	// ppEffectInstances
						   &p_work->mMaterialsNum,		// pNumMaterials
						   &p_work->mpMesh);			// ppMesh

	if (hr != D3D_OK) {
		if (hr == D3DERR_INVALIDCALL) {
			PRINT_CONSOLE("%s(L:%d) : hr == D3DERR_INVALIDCALL \n", __FILE__, __LINE__);
			return false;
		}
		if (hr == E_OUTOFMEMORY) {
			PRINT_CONSOLE("%s(L:%d) : hr == E_OUTOFMEMORY \n", __FILE__, __LINE__);
			return false;
		}
		PRINT_CONSOLE("%s(L:%d) : hr != D3D_OK \n", __FILE__, __LINE__);
		return false;
	}

	if (p_work->mpMaterialBuffer == NULL) {
		PRINT_CONSOLE("%s(L:%d) : p_work->mpMaterialBuffer is NULL \n", __FILE__, __LINE__);
		return false;
	}

	// �}�e���A�����ƃe�N�X�`������ݒ�
	D3DXMATERIAL* p_material = (D3DXMATERIAL *)p_work->mpMaterialBuffer->GetBufferPointer();
	if (p_material == NULL) {
		PRINT_CONSOLE("%s(L:%d) : p_material is NULL \n", __FILE__, __LINE__);
		return false;
	}

	//<! �@���x�N�g�����ݒ肳��Ă��Ȃ��ꍇ�A
	//<! �@���x�N�g����ݒ肵�A�V���� LPD3DXMESH �𕡐�����
	if ((p_work->mpMesh->GetFVF() & D3DFVF_NORMAL) == false) {
		ID3DXMesh* p_tmp_mesh = NULL;
		p_work->mpMesh->CloneMeshFVF(p_work->mpMesh->GetOptions(),
									 p_work->mpMesh->GetFVF()|D3DFVF_NORMAL,
									 GET_D3DDEV(),
									 &p_tmp_mesh);
		p_work->mpMesh->Release();
		p_work->mpMesh = NULL;
		p_work->mpMesh = p_tmp_mesh;
	}

	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
EffectResource::loadTex(const char* fileName, D3DXHANDLE hParam)
{
	IDirect3DTexture9* p_texture = NULL;

	HRESULT hr;
	DWORD width = 256;
	DWORD height = 256;
	DWORD miplevel = 0;
	// �e�N�X�`���̐�����m�ۂ��郁�����̏ꏊ���w�肷��I�v�V����
	DWORD usage = D3DUSAGE_AUTOGENMIPMAP;
	DWORD filter_flag = ( D3DX_FILTER_POINT, 
						  D3DX_FILTER_LINEAR, 
						  (D3DX_FILTER_TRIANGLE | D3DX_FILTER_DITHER));

	/* �@�t�@�C���𖼑O�Œ��ړǂݍ��ޏꍇ */
	hr = D3DXCreateTextureFromFile(GET_D3DDEV(),
								   fileName,
								   &p_texture);
	if (hr != D3D_OK) {
		PRINT_CONSOLE("%s(L:%d) : hr = 0x%x \n", __FILE__, __LINE__, hr);
		return false;
	}

	// �e�N�X�`���ƃG�t�F�N�g�̊֘A�t��
	hr = mpEffect->SetTexture(hParam, p_texture);

	if (hr != D3D_OK) {
		PRINT_CONSOLE("%s(L:%d) : hr = 0x%x \n", __FILE__, __LINE__, hr);
	}

	mTextureList.push_back(p_texture);

	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::getAnnotationParam(int paramIndex, int annoNum, tAnnotationWork* pAnnoWork, int annoWorkNum)
{
	HRESULT hr;
	D3DXHANDLE hAnno;
	D3DXPARAMETER_DESC	anno_desc = {0};
	tAnnotationWork*	p_annowork = NULL;

	for (int iI = 0; iI < annoNum; iI ++) {
		hAnno = EffectResource::getParamAnnotationInfo(&anno_desc, paramIndex, iI);

		for (int iJ = 0; iJ < annoWorkNum; iJ ++) {
			p_annowork = &pAnnoWork[iJ];
			if (strcmp(anno_desc.Name, p_annowork->mAnnoName) == 0) {
				
				switch (p_annowork->mParamType) {
				case D3DXPT_INT:
					hr = mpEffect->GetInt(hAnno, (INT *)p_annowork->mpParam);
					break;
				case D3DXPT_STRING:
					hr = mpEffect->GetString(hAnno, (LPCSTR *)p_annowork->mpParam);
					break;

					//<! ����͂��ꂾ���A�@���Ɏg�������ꍇ�͊g�����ĂˁI

				default:
					APP_ASSERT_PRINT(NULL, 
									 "Not Support annotation param type %d \n", p_annowork->mParamType);
					break;
				};
			}
			APP_ASSERT(hr == D3D_OK, "Failed getAnnotationParam() \n");

		}
	}	
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
DWORD
EffectResource::getD3DFORMAT_FromString(char* format)
{
	if (strcmp(format, "A8R8G8B8") == 0) {
		return D3DFMT_A8R8G8B8;
	} else if (strcmp(format, "A32B32G32R32F") == 0) {
		return D3DFMT_A32B32G32R32F;
	} else if (strcmp(format, "D24S8") == 0) {
		return D3DFMT_D24S8;
	} else if (strcmp(format, "D3DFMT_A8R8G8B8") == 0){
		return D3DFMT_A8R8G8B8;
	}else{
		APP_ASSERT_PRINT(NULL,
						 "Failed getD3DFORMAT_FromString() Unsupport format %d \n", format);
	}

	return 0xFFFFFFFF;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::getEffectInfo(D3DXEFFECT_DESC* pDesc)
{
	mpEffect->GetDesc(pDesc);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::getTechInfo(D3DXTECHNIQUE_DESC* pDesc, int techIndex)
{
	D3DXHANDLE hTechnique = NULL;
	hTechnique = mpEffect->GetTechnique(techIndex);
	mpEffect->GetTechniqueDesc(hTechnique, pDesc);

	return hTechnique;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::getTechAnnotationInfo(D3DXPARAMETER_DESC* pDesc, int techIndex)
{
	D3DXHANDLE hTechnique;
	hTechnique = mpEffect->GetTechnique(techIndex);
	D3DXHANDLE hAnno;
	hAnno = mpEffect->GetAnnotation(hTechnique, 0);

	mpEffect->GetParameterDesc(hAnno, pDesc);

	return hAnno;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::getPassInfo(D3DXPASS_DESC* pDesc, int techIndex, int passIndex)
{
	D3DXHANDLE hTechnique, hPass;
	hTechnique	= mpEffect->GetTechnique(techIndex);
	hPass		= mpEffect->GetPass(hTechnique, passIndex);
	mpEffect->GetPassDesc(hPass, pDesc);

	return hPass;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::getPassAnnotationInfo(D3DXPARAMETER_DESC* pDesc, int techIndex, int passIndex)
{
	D3DXHANDLE hTechnique, hPass, hAnno;
	hTechnique	= mpEffect->GetTechnique(techIndex);
	hPass		= mpEffect->GetPass(hTechnique, passIndex);
	hAnno		= mpEffect->GetAnnotation(hPass, 0);

	mpEffect->GetParameterDesc(hAnno, pDesc);

	return hAnno;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::getParamInfo(D3DXPARAMETER_DESC* pDesc, int paramIndex)
{
	D3DXHANDLE handle = NULL;
	handle = mpEffect->GetParameter(NULL, paramIndex);
	mpEffect->GetParameterDesc(handle, pDesc);

	return handle;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
D3DXHANDLE
EffectResource::getParamAnnotationInfo(D3DXPARAMETER_DESC* pAnnoDesc, int paramIndex, int annoIndex)
{
	D3DXHANDLE hParam, hAnno;
	hParam = mpEffect->GetParameter(NULL, paramIndex);
	hAnno  = mpEffect->GetAnnotation(hParam, annoIndex);
	mpEffect->GetParameterDesc(hAnno, pAnnoDesc);

	return hAnno;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
EffectResource::addSemanticInstance(int paramIndex, D3DXPARAMETER_DESC* pParamDesc)
{
	char sementic_name[32] = {0};

	// �Z�}���e�B�b�N����啶���ɂ���
	strcpy_s(sementic_name, sizeof(sementic_name), pParamDesc->Semantic);
	for (unsigned int iI = 0; iI < strlen(sementic_name); iI ++) {
		sementic_name[iI] = toupper( sementic_name[iI] );
	}

	int sem_kind = 0;
	for (sem_kind = 0; sem_kind < eSMT_MAX; sem_kind ++) {
		if (strcmp(sementic_name, strSmtName[sem_kind]) == 0) {
			break;
		}
	}
	APP_ASSERT_PRINT(sem_kind != eSMT_MAX,
					 "semantic %s is Unsupport name \n", pParamDesc->Semantic);

#ifdef _DEBUG
	if (sem_kind == eSMT_MAX) {
		sem_kind = eSMT_DUMMY;
	}
#endif // _DEBUG

	tSemanticInfo* p_info = new tSemanticInfo();
	p_info->mSemKind	= (eSmtKind)sem_kind;
	p_info->mSemIndex	= paramIndex;

	/**** �Z�}���e�B�b�N�ŗL�̃��\�[�X���m�ۂ��� ****/
	switch (sem_kind) {
	case eSMT_RENDERCOLORTARGET:
		{
			// �X�e���V�����𐶐�����Ă��Ȃ��ꍇ�A��������
			if (mpStencilInfo == NULL) {
				mpStencilInfo = new tStencilInfo[kSTENCIL_INFO_NUM];
				APP_ASSERT( (mpStencilInfo != NULL),
							"Failed new tStencilInfo[] \n");
				memset(mpStencilInfo, 0, (sizeof(tStencilInfo) * kSTENCIL_INFO_NUM) );
			}

			// Texture ��񂪐ݒ肳��Ă��Ȃ� RENDERCOLORTARGET �̔ԍ���T��
			int target_index;
			for (target_index = 0; target_index < eRENDER_COLOR_TARGET_MAX; target_index++) {
				if (mpStencilInfo[0].mpRenderColorTarget[target_index] == NULL) {
					break;
				}
			}
			APP_ASSERT( (target_index < eRENDER_COLOR_TARGET_MAX), "no empty RenderColorTarget Texture \n");

			// �w��� RENDERCOLORTARGET? �ɑ΂��āA������ kSTENCIL_INFO_NUM �̗v�f���e�N�X�`���̐������s��
			resourceSMT_RENDERCOLORTARGET(target_index, paramIndex, pParamDesc, p_info);
		}
		break;

	case eSMT_RENDERDEPTHSTENCILTARGET:
		{
			
			// �w��� RENDERDEPTHSTENCILTARGET �ɑ΂��āA������ kSTENCIL_INFO_NUM �̗v�f���T�[�t�F�C�X�̐������s��
			resourceSMT_RENDERDEPTHSTENCILTARGET(paramIndex, pParamDesc, p_info);
		}
		break;

	default:
		break;
	};

	// �Z�}���e�B�b�N���X�g�ɒǉ�����
	mSemanticList.push_back(p_info);

	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
// �T�[�t�F�C�X�̕����擾�֐�
bool GetSurfaceWH( IDirect3DSurface9 *pSurf, UINT &uiWidth, UINT &uiHeight )
{
	if( !pSurf ) return false;

	D3DSURFACE_DESC SufDesc;
	pSurf->GetDesc( &SufDesc );
	uiWidth  = SufDesc.Width;
	uiHeight = SufDesc.Height;

	return true;
}

//---------------------------------------------------
/**
 * �Z�}���e�B�b�N�� RENDERCOLORTARGET ���w��̏ꍇ��
 * �K�v�ȃ��\�[�X(�e�N�X�`��)��p�ӂ���
 * RENDERCOLORTARGET �L�ڂ̃A�m�e�[�V�����̃p�����[�^�͈ȉ����Q��
 *		https://sites.google.com/site/mikumikumoving/manual/effect-1
 *
 �E string ResourceType
�@ �e�N�X�`���̎�ނ��w�肵�܂��B�^�Ɩ��������l�͎w��ł��܂���B
�@ "2D" "3D" "CUBE"
�@ �^��"texture"����"2D"�ȊO�̃e�N�X�`���𐶐�����ꍇ�ɂ́A�K�����̃A�m�e�[�V�������w�肵�Ă��������B
�@ �ʏ��2D�e�N�X�`���ł͏ȗ��\�ł��B

�E string ResourceName
�@ �e�N�X�`���摜�t�@�C�����w�肵�܂��B
�@ �T�|�[�g���Ă���t�@�C���t�H�[�}�b�g�� bmp, dds, dib, jpg, png, tga �ł��B
�@ ���΃p�X�Ńt�@�C�����w�肵���ꍇ�A�G�t�F�N�g�t�@�C���������Ă���t�H���_����ɂȂ�܂��B

�E int Width
�E int Height
�E int Depth
�@ �e�N�X�`���̕��A�����A�[�����s�N�Z���P�ʂŎw�肵�܂��B
�@ Dimensions, ViewportRatio �Ƃ͓����Ɏw��ł��܂���B
�@ �f�t�H���g�l��64�ł��BResourceName���w�肳��Ă���ꍇ�́A�摜�t�@�C�����玩���I�Ɏ擾����܂��B

�E int2 or int3 Dimensions
�@ �e�N�X�`���̕��A�����A�[�����s�N�Z���P�ʂŎw�肵�܂��B
�@ ViewportRatio, Width, Height, Depth �Ɠ����ɂ͎w��ł��܂���B

�E float2 ViewportRatio
�@ �e�N�X�`���̕��ƍ������A�X�N���[���T�C�Y�̔�Ŏw�肵�܂��B
�@ �X�N���[���Ɠ����T�C�Y�̃e�N�X�`���𐶐�����ɂ́A"float2 ViewportRatio = {1.0, 1.0};" �Ǝw�肵�܂��B
�@ Dimensions, Width, Height, Depth�Ƃ͓����Ɏw��ł��܂���B

�E string Format
�@ �e�N�X�`���̃t�H�[�}�b�g���w�肵�܂��B
�@ �ȗ������ꍇ�A"A8R8G8B8"���g�p����܂��B
�@ �w��\�ȃt�H�[�}�b�g�́AD3DFORMAT�ihttp://msdn.microsoft.com/ja-jp/library/bb172558(v=VS.85).aspx)���Q�Ƃ��Ă��������B
�@ "A8R8G8B8"�A"FMT_A8R8G8B8"�A"D3DFMT_A8R8G8B8"�̂�����̏����ł��w��ł��܂��B

�E int Miplevels
�@ �~�b�v�}�b�v���w�肵�����x���Ő������܂��B
�@ �ȗ����邩0���w�肵���ꍇ�A���S�ȃ~�b�v�}�b�v�`�F�[�����쐬����܂��B
�@ 1���w�肷��ƁA�~�b�v�}�b�v�͐�������܂���B

�E int Levels
�@ Miplevels�̕ʖ��ł��B
 */
//---------------------------------------------------
void
EffectResource::resourceSMT_RENDERCOLORTARGET(int targetIndex, int paramIndex, D3DXPARAMETER_DESC* pParamDesc, tSemanticInfo* pSmtInfo)
{
	IDirect3DTexture9* p_texture = NULL;
	HRESULT	hr = NULL;

	// �����e�N�X�`���̃p�����[�^�̃f�t�H���g�l
	float	ViewportRatio[2]= {1.0f, 1.0f};
	INT		Width			= 0;
	INT		Height			= 0;
	char*	Format			= "A8R8G8B8";

	GetWindowSize((int *)&Width, (int *)&Height);
	tAnnotationWork anno_work[3] = {
		{"Width",		D3DXPT_INT,		(void *)&Width},
		{"Height",		D3DXPT_INT,		(void *)&Height},
		{"Format",		D3DXPT_STRING,	(void *)&Format},
	};


	getAnnotationParam(paramIndex, pParamDesc->Annotations, anno_work, (sizeof(anno_work))/ sizeof(tAnnotationWork)) ;

	D3DFORMAT format_value = (D3DFORMAT)0;
	format_value = (D3DFORMAT)getD3DFORMAT_FromString(Format);

	for (int stencil_index = 0; stencil_index < kSTENCIL_INFO_NUM; stencil_index ++) {

		hr = D3DXCreateTexture (GET_D3DDEV(),			// 
								Width,					// UINT Width,
								Height,					// UINT Height,
								1,						// UINT Levels,
								D3DUSAGE_RENDERTARGET,	// DWORD Usage,
								format_value,			// D3DFORMAT Format,
								D3DPOOL_DEFAULT,		// D3DPOOL Pool,
								&p_texture);			// IDirect3DTexture9** pp_texture,
		APP_ASSERT((hr == D3D_OK), "Failed D3DXCreateTexture()\n");
		
		mpStencilInfo[stencil_index].mpRenderColorTarget[targetIndex] = p_texture;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
EffectResource::resourceSMT_RENDERDEPTHSTENCILTARGET(int paramIndex, D3DXPARAMETER_DESC* pParamDesc, tSemanticInfo* pSmtInfo)
{
	HRESULT	hr = NULL;

	INT Width = 0;
	INT Height = 0;
	char* Format = NULL;
	D3DFORMAT format_value = (D3DFORMAT)0;

	tAnnotationWork anno_work[3] = {
		{"Width",		D3DXPT_INT,		(void *)&Width},
		{"Height",		D3DXPT_INT,		(void *)&Height},
		{"Format",		D3DXPT_STRING,	(void *)&Format},
	};

	getAnnotationParam(paramIndex, pParamDesc->Annotations, anno_work, (sizeof(anno_work) / sizeof(tAnnotationWork)));
	format_value = (D3DFORMAT)getD3DFORMAT_FromString(Format);

	IDirect3DTexture9* p_texture = NULL;
	// Z�l�e�N�X�`���̕�����ێ�
	UINT uiZTexWidth = 0;
	UINT uiZTexHeight = 0;
	// Z�l�e�N�X�`���T�[�t�F�C�X��ێ�
	IDirect3DSurface9* pZTexSurf = NULL;

	for (int stencil_index = 0; stencil_index < kSTENCIL_INFO_NUM; stencil_index ++) {

		// RENDERCOLORTARGET0 �̃e�N�X�`�������擾
		p_texture = mpStencilInfo[stencil_index].mpRenderColorTarget[0];
		p_texture->GetSurfaceLevel( 0, &pZTexSurf );
		GetSurfaceWH( pZTexSurf, uiZTexWidth, uiZTexHeight );

		// �[�x�o�b�t�@�T�[�t�F�C�X�̍쐬
		hr = GET_D3DDEV()->CreateDepthStencilSurface(uiZTexWidth,
													 uiZTexHeight, 
													 format_value, // D3DFMT_D24S8
													 D3DMULTISAMPLE_NONE, 
													 0, 
													 FALSE, 
													 &mpStencilInfo[stencil_index].mpRenderDepthStencilTarget,
													 NULL);
		
		APP_ASSERT((hr == D3D_OK), "Failed CreateDepthStencilSurface() \n");
	}

}

/**** end of file ****/
