/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �V�[�������}�l�[�W���N���X
 * @author SPATZ.
 * @data   2011/11/21 00:29:49
 */
//---------------------------------------------------
/*=====================================*
 * include
 *=====================================*/
#include "../AppLib/Input/InputManager.h"
#include "../AppLib/Graphic/GraphicManager.h"
#include "../AppLib/Graphic/RenderBloom.h"
#include "../AppLib/Graphic/CameraManager.h"
#include "../AppLib/Graphic/EffekseerManager.h"
#include "../AppLib/Collision/CollisionManager.h"
#include "AppLib/Sound/SoundManager.h"
#include "AppLib/Resource/ResourceLayout.h"
#include "Utility/Utility.h"
#include "AppLib/Basic/Basic.h"

#include "SceneManager.h"
#include "SceneInfo.h"

#include "SceneMainGame.h"
#include "EffectTest.h"
#include "SceneTitle.h"
#include "SceneSelectFile.h"
#include "SceneEvent.h"

#include "DataDefine/VoiceDef.h"

#include "AppLib/Input/InputInfo.h"

#ifdef _DEBUG
#include "External/PIC/PIC.h"
#include "AppLib/Basic/Counter.h"
#endif // _DEBUG

/*=====================================*
 * CheckLoopFunc
 *=====================================*/
/* static */SceneManager::CheckLoopFunc
SceneManager::mCheckLoopFunc[SceneManager::eSCENE_TYPE_MAX] = {
	&SceneManager::checkLoopFuncGame,
	&SceneManager::checkLoopFuncMovie,
};

/*=====================================*
 * UpdateFunc
 *=====================================*/
/* static */SceneManager::UpdateFunc
SceneManager::mUpdateFunc[SceneManager::eSCENE_TYPE_MAX] = {
	&SceneManager::updateFuncGame,
	&SceneManager::updateFuncMovie,
};

/*=====================================*
 * PhaseFunc 
 *=====================================*/
/* static */SceneManager::PhaseFunc
SceneManager::mPhaseFunc[SceneManager::ePHASE_MAX] = {
	&SceneManager::phaseLoadResource,
	&SceneManager::phaseSelectMenu,
	&SceneManager::phaseDebugDoing,
};

enum {
	eSTATIC_LY_FADE_BOARD = 0,
	eSTATIC_LY_NOW_LOADING,
	eSTATIC_LY_LINO_RUN_00,

	eSTATIC_LY_INPUT_MANUAL,

	eSTATIC_LY_INFO,

	eSTATIC_LY_MAX,
};

static int				MovieGraphHandle = -1;
static LayoutBase		sStaticLayout[eSTATIC_LY_MAX];

#ifndef __LOAD_D3DX9LIB__
#define __LOAD_D3DX9LIB__
#pragma comment(lib, "d3dx9.lib")
#endif // __LOAD_D3DX9LIB__

#ifndef __LOAD_EFFEKSEER__
#define __LOAD_EFFEKSEER__
#ifdef _DEBUG
//#pragma comment(lib, "Effekseer.Debug.lib" )
//#pragma comment(lib, "EffekseerRendererCommon.Debug.lib" )
//#pragma comment(lib, "EffekseerRendererDX9.Debug.lib" )
//#pragma comment(lib, "EffekseerManagerLib.lib")

#pragma comment(lib, "Effekseer_vs2012_x86_d.lib" )
#pragma comment(lib, "EffekseerForDXLib_vs2012_x86_d.lib" )
#pragma comment(lib, "EffekseerRendererDX9_vs2012_x86_d.lib" )

#else
//#pragma comment(lib, "Effekseer.Release.lib" )
//#pragma comment(lib, "EffekseerRendererCommon.Release.lib" )
//#pragma comment(lib, "EffekseerRendererDX9.Release.lib" )
#endif // _DEBUG

#endif // __LOAD_EFFEKSEER__

#ifdef _DEBUG
/**** �v���Z�X�ԒʐM���� ****/
#pragma comment(lib, "PIC_Lib.lib" )
#define LOL_PIC_PIPE_NAME		("\\\\.\\pipe\\LOL_host")
#define DBG_PIC_PIPE_NAME		("\\\\.\\pipe\\spatz_host")
#endif // _DEBUG

#define RES_LYT_MANUAL_JOY		("../../�������(joypad).jpg")
#define RES_LYT_MANUAL_KEY		("../../�������(keyboard).jpg")

/*=====================================*
 * ���܂�B�B�����ɒu�������Ȃ�����..
 *=====================================*/
/*static*/int	SceneBase::mPhase		= -1; 
/*static*/bool	SceneBase::mIsEndScene	= false;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/SceneManager * const
SceneManager::GetInstance(void)
{
	static SceneManager sInstance;
	return &sInstance;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
SceneManager::SceneManager()
{
	mSceneType		= eSCENE_TYPE_GAME;

	mNowPhase		= 0;

#ifdef _DEBUG
	mNowMenu		= 0;
#else
	mNowMenu		= eMENU_TITLE;
#endif // _DEBUG

	mMenuArg		= 22;

	mpMainSceneInst = NULL;
	mRequestChangeTask = -1;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
SceneManager::~SceneManager()
{
	if (MovieGraphHandle != -1) {
		DeleteGraph(MovieGraphHandle);
		MovieGraphHandle = -1;
	}

	// �^�X�N����O��
	RemoveDebugTask();

	for (int iI = 0; iI < eSTATIC_LY_MAX; iI ++) {
		GraphicManager::GetInstance()->RemoveSystemLayout(&sStaticLayout[iI]);
	}
#ifdef _DEBUG
	Basic::CloseDebugLog();
#endif // _DEBUG

#ifdef _DEBUG
	// �v���Z�X�ԒʐM�̃p�C�v��j��
	PIC_LIB::CloseNamedPipe();
#endif // _DEBUG
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneManager::Create(void)
{
#ifdef _DEBUG
	Basic::OpenDebugLog();
	PRINT_LOG("PRINT_LOG test is OK");
#endif // _DEBUG
	InputManager::GetInstance()->Create();
	CameraManager::GetInstance()->Create();
	GraphicManager::GetInstance()->Create();
	EffekseerManager::GetInstance()->Create();

#ifdef _DEBUG
	// �v���Z�X�ԒʐM�̃p�C�v��ڑ�
	bool ret;
	ret = PIC_LIB::CreatePICPipe(LOL_PIC_PIPE_NAME);
	if (ret == true) {
		// �T�[�o�[�̃p�C�v���I�[�v��
		ret = PIC_LIB::OpenPICPipe(DBG_PIC_PIPE_NAME);
		if (ret == true) {
			// �N���C�A���g���ڑ�����̂�҂�
			PIC_LIB::WaitConnectClient();
		}
	}
#endif // _DEBUG
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneManager::Setup(void)
{
	if (mSceneType == eSCENE_TYPE_GAME) {
		// �`���O���t�B�b�N�̈�̎w��
		SetDrawScreen(DX_SCREEN_BACK);

		InputManager::GetInstance()->Setup();
		CameraManager::GetInstance()->Setup();
		GraphicManager::GetInstance()->Setup();
		EffekseerManager::GetInstance()->Setup();

		LayoutBase::SetupLayoutInfo(GraphicManager::GetInstance()->GetAspectRate(),
									GraphicManager::GetInstance()->GetLayoutScaleRateX(),
									GraphicManager::GetInstance()->GetLayoutScaleRateY());

		/**** �t�H���g�f�[�^�̐��� ****/
		int reso_type = TotalInfo::GetInstance()->GetConfigVideoSize();
		const tDisplaySetting* p_setting = NULL;
		if (reso_type > (sizeof(sDisplaySetting) / sizeof(tDisplaySetting))) {
			reso_type = 1; // �K����
		}
		p_setting = &sDisplaySetting[reso_type];
		static const float FONT_RATE		= 28.4f;
		int font_size = 0;/*36:default*/

		float video_rate = 0;
		video_rate = TotalInfo::GetInstance()->GetConfigResolutionRateFromGraphQuality();

		font_size = (int)((float)p_setting->mResolusionX * video_rate / FONT_RATE);
		GraphicManager::GetInstance()->CreateFontData(font_size);


	} else if (mSceneType == eSCENE_TYPE_MOVIE) {

		// �`���O���t�B�b�N�̈�̎w��
		SetDrawScreen(DX_SCREEN_FRONT);

		// ���[�r�[�t�@�C�������[�h���܂��B
		if (MovieGraphHandle == -1) {
			MovieGraphHandle = LoadGraph("data/Movie/Opening.mp4");
		}
		if (MovieGraphHandle == -1) {
			return;
		}
		// ���[�r�[���Đ���Ԃɂ��܂�
		PlayMovieToGraph( MovieGraphHandle ) ;

		mRequestChangeTask = eMENU_TITLE;

	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneManager::Update(void)
{
	int now_type = mSceneType;
	(this->*mUpdateFunc[now_type])();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneManager::Destroy(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
SceneManager::CheckMainLoop(void)
{
	return (this->*mCheckLoopFunc[mSceneType])();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
LayoutBase*
SceneManager::GetFadeLayout(void)
{
	return &sStaticLayout[eSTATIC_LY_FADE_BOARD];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
LayoutBase*
SceneManager::GetInfoLayout(void)
{
	return &sStaticLayout[eSTATIC_LY_INFO];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneManager::RequestChangeTask(int setTask)
{
	mRequestChangeTask = setTask;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
SceneManager::ChangeLoadingImage(eLoadImage loadImage)
{
	LayoutBase* p_layout = NULL;
	p_layout = &sStaticLayout[eSTATIC_LY_INPUT_MANUAL];

	if (loadImage == eLOAD_IMAGE_JOY) {
		p_layout->LinkLayout(RES_LYT_MANUAL_JOY);
	} else if (loadImage == eLOAD_IMAGE_KEY) {
		p_layout->LinkLayout(RES_LYT_MANUAL_KEY);
	}
}

//---------------------------------------------------
/**
   �f�o�b�O�̃^�X�N���G���g���[���鏈��
 */
//---------------------------------------------------
void
SceneManager::EntryDebugTask(void)
{
	if (mpMainSceneInst != NULL) {
		delete mpMainSceneInst;
		mpMainSceneInst = NULL;
	} else if (mpMainSceneInst == NULL) {
		switch (mNowMenu) {
		case eMENU_MERGE:
			mpMainSceneInst = new SceneMainGame();		break;
		case eMENU_EFFECT_TEST:
			mpMainSceneInst = new EffectTest();			break;
		case eMENU_TITLE:
			mpMainSceneInst = new SceneTitle();			break;
		case eMENU_SELECT_FILE:
			mpMainSceneInst = new SceneSelectFile();	break;
		case eMENU_EVENT:
			mpMainSceneInst = new SceneEvent();			break;
		default:
			break;
		};
		mpMainSceneInst->Create(mMenuArg);
	}
}

//---------------------------------------------------
/**
   �f�o�b�O�^�X�N���^�X�N���X�g����O��
 */
//---------------------------------------------------
void
SceneManager::RemoveDebugTask(void)
{
	if (mpMainSceneInst != NULL) {
		// �^�X�N����O��
		delete mpMainSceneInst;
		mpMainSceneInst = NULL;
	}
}

//---------------------------------------------------
/**
 * �Q�[���������̏I�����菈��
 */
//---------------------------------------------------
bool
SceneManager::checkLoopFuncGame(void)
{
	if ((ProcessMessage() == 0) && CHECK_PRESS_KEY(eKEY_ESCAPE) == FALSE) {
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
 * ���[�r�[�������̏I�����菈��
 */
//---------------------------------------------------
bool
SceneManager::checkLoopFuncMovie(void)
{
	if ((ProcessMessage() == 0) && (GetMovieStateToGraph( MovieGraphHandle ) == 1) && (CHECK_PRESS_KEY(eKEY_ESCAPE) == FALSE))	{
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
 * �Q�[�������X�V
 */
//---------------------------------------------------
void
SceneManager::updateFuncGame(void)
{
#ifdef _DEBUG
	static ULONG sDbgFrameCounter = 0;
	sDbgFrameCounter ++;

#endif // _DEBUG

	RenderBloom::ClearScreen();

#ifdef _DEBUG
	Counter::StartCounter(Counter::eCOUNT_TYPE_MAIN);
#endif // _DEBUG
	/**** *****/

	GraphicManager::GetInstance()->Refresh();

#ifdef _DEBUG
	if (CHECK_PRESS_KEY(eKEY_PGUP)) {
		GraphicManager::GetInstance()->DbgSetRenderString(true);
	}
	if (CHECK_PRESS_KEY(eKEY_PGDN)) {
		GraphicManager::GetInstance()->DbgSetRenderString(false);
	}
	if (CHECK_PRESS_KEY(eKEY_HOME)) {
		GraphicManager::GetInstance()->DbgSetRenderGraph(true);
	}
	if (CHECK_PRESS_KEY(eKEY_END)) {
		GraphicManager::GetInstance()->DbgSetRenderGraph(false);
	}
#endif // _DEBUG

	if (TotalInfo::GetInstance()->IsVisibleFPS() ) {
		char print_string[40] = {0};
		sprintf_s(print_string, sizeof(print_string), "%s (FPS:%.2f)", GAME_TITLE, Utility::GetFPS());
		SetMainWindowText(print_string);
	}

	InputManager::GetInstance()->Update();

	CollisionManager::GetInstance()->Refresh();

	SoundManager::GetInstance()->Update();

	(this->*mPhaseFunc[mNowPhase])();

	TotalInfo::GetInstance()->Update();

	CameraManager::GetInstance()->Update();

	GraphicManager::GetInstance()->Update();

	GraphicManager::GetInstance()->PostUpdate();

	/**** *****/
#ifdef _DEBUG
	Counter::StopCounter(Counter::eCOUNT_TYPE_MAIN);
	static int sCnt = 0;
	if (sCnt ++ > 60) {
		PRINT_CONSOLE("**** Main Loop Sec : %lld ns, (each count %lld ns, %.2f %%) \n", 
					  Counter::GetCountNanoSec(Counter::eCOUNT_TYPE_MAIN),
					  Counter::GetCountNanoSec(Counter::eCOUNT_TYPE_LOAD_EACH),
					  ( (float)( Counter::GetCountNanoSec(Counter::eCOUNT_TYPE_LOAD_EACH) /  (float)Counter::GetCountNanoSec(Counter::eCOUNT_TYPE_MAIN)) * 100.0f) );
		sCnt = 0;
	}

	Counter::UpdateCounter();

#endif // _DEBUG
	ScreenFlip();
}

//---------------------------------------------------
/**
 * ���[�r�[�����X�V
 */
//---------------------------------------------------
void
SceneManager::updateFuncMovie(void)
{
	InputManager::GetInstance()->Update();

	int org_graph_size_x, org_graph_size_y;
	int wnd_size_x, wnd_size_y;
	float ext_rate_x, ext_rate_y;
	float ext_graph_size_x, ext_graph_size_y;
	float ext_graph_posA_x, ext_graph_posA_y;
	float ext_graph_posB_x, ext_graph_posB_y;

	GetGraphSize(MovieGraphHandle, &org_graph_size_x, &org_graph_size_y);
	wnd_size_x = 1024;
	wnd_size_y = 768;
	
	// �䗦�͇]���ɂ��킹��
	ext_rate_x = (float)wnd_size_x / (float)org_graph_size_x;
	ext_rate_y = (float)wnd_size_y / (float)org_graph_size_y;
	ext_rate_x = 1.5f;
	
	ext_graph_size_x = ((float)org_graph_size_x) * ext_rate_x;
	ext_graph_size_y = ((float)org_graph_size_y) * ext_rate_x;

	ext_graph_posA_x = ((float)wnd_size_x - ext_graph_size_x) / 2.0f;
	ext_graph_posA_y = ((float)wnd_size_y - ext_graph_size_y) / 2.0f;
	ext_graph_posB_x = ext_graph_posA_x + ext_graph_size_x;
	ext_graph_posB_y = ext_graph_posA_y + ext_graph_size_y;
	
	DrawExtendGraph((int)ext_graph_posA_x,
					(int)ext_graph_posA_y,
					(int)ext_graph_posB_x,
					(int)ext_graph_posB_y,
					MovieGraphHandle, FALSE);

	// �E�G�C�g�������܂��A���܂葬���`�悷��Ɖ�ʂ����������ł�
	WaitTimer( 17 ) ;

	if ((CHECK_TRIG_ANY_JOY()) || (CHECK_TRIG_KEY(eKEY_RETURN))) {
		PauseMovieToGraph(MovieGraphHandle);

		/* ���̂Q�̏��ԕύX�s�� */
		mSceneType = eSCENE_TYPE_GAME;
		this->Setup();
	}
}

//---------------------------------------------------
/**
   �t�F�[�Y�؂�ւ�
 */
//---------------------------------------------------
void
SceneManager::changePhase(int setPhase)
{
	mNowPhase = setPhase;
}

//---------------------------------------------------
/**
   �K�v�ȃt�@�C���̓ǂݍ���
 */
//---------------------------------------------------
void
SceneManager::phaseLoadResource(void)
{
	TotalInfo::GetInstance()->LoadTotalInfoLayout();

	ResourceLayout::GetInstance()->LoadLayoutResource("NowLoading.png");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun000.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun001.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun002.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun003.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun004.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun005.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun006.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun007.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun008.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun009.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun010.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun011.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun012.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun013.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun014.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("LinoRun015.jpg");
	ResourceLayout::GetInstance()->LoadLayoutResource("fade_board.png");

	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_MANUAL_JOY);
	ResourceLayout::GetInstance()->LoadLayoutResource(RES_LYT_MANUAL_KEY);

	LayoutBase* p_layout = NULL;

	p_layout = &sStaticLayout[eSTATIC_LY_NOW_LOADING];
	p_layout->LinkLayout("NowLoading.png");
	p_layout->SetPositionByCentering(640 - 5*32, 480 - 48);
	p_layout->SetSize(256, 64);
	p_layout->SetAlpha(255);

	p_layout = &sStaticLayout[eSTATIC_LY_LINO_RUN_00];
	p_layout->LinkLayout("LinoRun000.jpg");
	// ���W�ݒ�͕ʓr
	p_layout->SetSize(128, 128);
	p_layout->SetAlpha(255);

	p_layout = &sStaticLayout[eSTATIC_LY_FADE_BOARD];
	p_layout->LinkLayout("fade_board.png");
	float aspect_rate = GraphicManager::GetInstance()->GetAspectRate();
	p_layout->SetPositionByCentering(320, 240);
	p_layout->SetSize( (int)(642.0f / aspect_rate), 480 );
	p_layout->SetColor(0, 0, 0);
	p_layout->SetAlpha(255);

	p_layout = &sStaticLayout[eSTATIC_LY_INPUT_MANUAL];
	p_layout->LinkLayout(RES_LYT_MANUAL_KEY);
	p_layout->SetPositionByCentering(320, 240);
	p_layout->SetSize(384, 192);
	p_layout->SetAlpha(255);

	p_layout = &sStaticLayout[eSTATIC_LY_INFO];
	//	p_layout->LinkLayout("");
	p_layout->SetPositionByCentering(320, 240);
	p_layout->SetSize(512, 128);
	p_layout->SetAlpha(0);
	
	if (GraphicManager::GetInstance()->CheckEntrySystemLayout(&sStaticLayout[eSTATIC_LY_FADE_BOARD]) == false) {
		GraphicManager::GetInstance()->EntrySystemLayout(&sStaticLayout[eSTATIC_LY_FADE_BOARD]);
		GraphicManager::GetInstance()->AssignFadeboardSystemLayout(&sStaticLayout[eSTATIC_LY_FADE_BOARD]);
	}

	if (GraphicManager::GetInstance()->CheckEntrySystemLayout(&sStaticLayout[eSTATIC_LY_INFO]) == false) {
		GraphicManager::GetInstance()->EntrySystemLayout(&sStaticLayout[eSTATIC_LY_INFO]);
	}

	// �T�E���h��ǂݍ���
	SoundManager::GetInstance()->LoadSoundComponent(eSE_MAX);
	SoundManager::GetSndComponent()->SetVolumeScale(TotalInfo::GetInstance()->GetConfigMusicVolumeScale());

	changePhase(ePHASE_SELECT_MENU);
}

//---------------------------------------------------
/**
   ���j���[�I�����̏���
 */
//---------------------------------------------------
void
SceneManager::phaseSelectMenu(void)
{
	//<! ���j���[�̑I��
	if (CHECK_TRIG_JOY(JOY_INPUT_UP)) {
		mNowMenu = (mNowMenu - 1 < 0) ? (eMENU_MAX - 1) : (mNowMenu - 1);
	}	
	if (CHECK_TRIG_JOY(JOY_INPUT_DOWN)) {
		mNowMenu = (mNowMenu + 1 > (eMENU_MAX - 1)) ? 0 : (mNowMenu + 1);
	}
	if (CHECK_TRIG_JOY(JOY_INPUT_LEFT)) {
		mMenuArg = (mMenuArg - 1 < 0) ? 0 : (mMenuArg - 1);
	}	
	if (CHECK_TRIG_JOY(JOY_INPUT_RIGHT)) {
		mMenuArg = mMenuArg + 1;
	}
	if (CHECK_TRIG_KEY(eKEY_0)) {
		mMenuArg = 0;
	}
	if (CHECK_TRIG_KEY(eKEY_1)) {
		mMenuArg = 10;
	}
	if (CHECK_TRIG_KEY(eKEY_2)) {
		mMenuArg = 20;
	}
	if (CHECK_TRIG_KEY(eKEY_3)) {
		mMenuArg = 30;
	}
	if (CHECK_TRIG_KEY(eKEY_4)) {
		mMenuArg = 40;
	}
	if (CHECK_TRIG_KEY(eKEY_5)) {
		mMenuArg = 50;
	}
	if (CHECK_TRIG_KEY(eKEY_6)) {
		mMenuArg = 60;
	}
	if (CHECK_TRIG_KEY(eKEY_7)) {
		mMenuArg = 70;
	}
	if (CHECK_TRIG_KEY(eKEY_8)) {
		mMenuArg = 80;
	}
	if (CHECK_TRIG_KEY(eKEY_9)) {
		mMenuArg = 90;
	}


#ifdef _DEBUG
	if (CHECK_TRIG_KEY(eKEY_F4)) {
		// �f�o�b�O�p���[�h
		tSaveWork load_work[3];
		memset(&load_work, 0, sizeof(load_work));

		TotalInfo::eLoadStatus load_status = TotalInfo::eLOAD_FAILED;
		while (true) {
			load_status = TotalInfo::GetInstance()->LoadDataToBuffer(load_work);
			
			if (load_status == TotalInfo::eLOAD_SUCEESS) {
				SYSTEM_MESSAGE("Load", "Load Success");
				break;
			}
			else if (load_status == TotalInfo::eLOAD_FAILED) {
				SYSTEM_MESSAGE("Load", "Load Failed");
				break;
			}
			APP_ASSERT_PRINT((load_status == TotalInfo::eLOAD_REQUEST_RELOAD),
							 "Invalid load_status : %d \n", load_status);
		}

		TotalInfo::GetInstance()->UpdateLoadDataFromBuffer(load_work[0]);

	}
#endif // _DEBUG

#ifdef _DEBUG
	if (CHECK_TRIG_ANY_JOY() || (mRequestChangeTask != -1 ) ) {
#else
	if (TRUE) {
#endif // _DEBUG
		
		PRINT_CONSOLE("Enter DebugDoing\n");

		/* �w�肳��Ă����^�X�N�ɕύX */
		if (mRequestChangeTask != -1) {
			mNowMenu = mRequestChangeTask;
			mRequestChangeTask = -1;
		}

		LayoutBase* p_layout = NULL;
		p_layout = &sStaticLayout[eSTATIC_LY_FADE_BOARD];
		p_layout->SetAlpha(255);

		EntryDebugTask();

		changePhase(ePHASE_DEBUG_DOING);
	}
	
	char print_string[64] = {0};

	int col;
	for (int iI = 0; iI < eMENU_MAX; iI ++) {
		col = GetColor(0, 0, 255);
		if (iI == mNowMenu) {
			col = GetColor(255, 0, 0);
		}
		memset(print_string, 0x00, sizeof(print_string));
		sprintf_s(print_string, sizeof(print_string), "%s\t%d", sDebugMenuName[iI], mMenuArg);
#ifdef _DEBUG
		GraphicManager::GetInstance()->DbgRenderString(print_string, col);
#endif // _DEBUG
	}

	if (mNowMenu == eMENU_MERGE) {
#ifdef _DEBUG
		GraphicManager::GetInstance()->DbgRenderString((char *)SceneMainGame::GetStageName(mMenuArg), col);
#endif // _DEBUG
	}
}

//---------------------------------------------------
/**
   �f�o�b�O���̏���
 */
//---------------------------------------------------
void
SceneManager::phaseDebugDoing(void)
{
	static const char* strLinoAnimTable[] = {
		"LinoRun000.jpg",
		"LinoRun001.jpg",
		"LinoRun002.jpg",
		"LinoRun003.jpg",
		"LinoRun004.jpg",
		"LinoRun005.jpg",
		"LinoRun006.jpg",
		"LinoRun007.jpg",
		"LinoRun008.jpg",
		"LinoRun009.jpg",
		"LinoRun010.jpg",
		"LinoRun011.jpg",
		"LinoRun012.jpg",
		"LinoRun013.jpg",
		"LinoRun014.jpg",
		"LinoRun015.jpg",
	};
	static const char kLINO_ANIM_NUM = (sizeof(strLinoAnimTable) / sizeof(const char*));

	// �f�o�b�O���j���[���̃t�F�[�Y���擾
	int now_phase = mpMainSceneInst->GetPhase();

	static int sCnt = 0;

	switch (mNowMenu) {
	case eMENU_MERGE:
		if (now_phase == SceneMainGame::ePHASE_LOAD_BEFORE) {
			GraphicManager::GetInstance()->EntrySystemLayout(&sStaticLayout[eSTATIC_LY_INPUT_MANUAL]);
			GraphicManager::GetInstance()->EntrySystemLayout(&sStaticLayout[eSTATIC_LY_NOW_LOADING]);
			GraphicManager::GetInstance()->EntrySystemLayout(&sStaticLayout[eSTATIC_LY_LINO_RUN_00]);
			sStaticLayout[eSTATIC_LY_LINO_RUN_00].SetPositionByCentering(640 - 96, 400 - 64);
			sCnt = 0;
		}
		if (now_phase == SceneMainGame::ePHASE_LOADING) {
			sCnt ++;

			/* �ς�ς�A�j�� */
			sStaticLayout[eSTATIC_LY_LINO_RUN_00].LinkLayout(strLinoAnimTable[(sCnt/2)%kLINO_ANIM_NUM]);
			int load_percent = mpMainSceneInst->GetLoadingPercent();
			int posX = (int)(544.0f - ((544.0f - 96.0f) / 100.0f * (float)load_percent));
			sStaticLayout[eSTATIC_LY_LINO_RUN_00].SetPositionByCentering(posX, 400 - 64);

			PRINT_SCREEN(GetColor(255, 255, 0), "Loading Percent : %d", load_percent);
		}

		if (now_phase == SceneMainGame::ePHASE_SETUP) {
			GraphicManager::GetInstance()->RemoveSystemLayout(&sStaticLayout[eSTATIC_LY_INPUT_MANUAL]);
			GraphicManager::GetInstance()->RemoveSystemLayout(&sStaticLayout[eSTATIC_LY_NOW_LOADING]);
			GraphicManager::GetInstance()->RemoveSystemLayout(&sStaticLayout[eSTATIC_LY_LINO_RUN_00]);
		}
		break;
		
	default:
		break;
	};

	/* �ړ� */
	// �e�X�g���ł̃t�F�[�Y���Ăяo����������
	mpMainSceneInst->Update();

	//<! �^�X�N������̏I������҂�
	if (mpMainSceneInst->CheckEndScene()) {
		PRINT_CONSOLE("Exit DebugTest\n");
		RemoveDebugTask();
		changePhase(ePHASE_SELECT_MENU);

#ifdef _DEBUG
#else
		mNowMenu		= eMENU_TITLE;	// �����[�X����
#endif // _DEBUG
	}
}


 /**** end of file ****/
