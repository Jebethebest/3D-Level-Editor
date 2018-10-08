
#include "stdafx.h"

#include "MainGame.h"
#include "GeneralStructs.h"
#include "SceneManager.h"
#include "PhysxProxy.h"

#include "DebugRenderer.h"

#define COURSE_SCENES
//#define TEST_SCENES
//#define HUD_SCENES
//#define TERRAIN_TEST
//#define SINGLE_SCENE
//#define TEST_SCENES

#include "EditorScene.h"
//#ifdef COURSE_SCENES
//#include "CourseObjects/Week 3/Pong/PongScene.h"
//#include "CourseObjects/Week 3/TutorialScene.h"
//#include "CourseObjects/Week 4/ModelTestScene.h"
//#include "CourseObjects/Week 5/UberScene.h"
//#include "CourseObjects/Week 5/SpikeyScene.h"
//#include "CourseObjects/Week 6/PickScene.h"
//#include "CourseObjects/Week 6/SpriteTestScene.h"
//#include "CourseObjects/Week 7/SoftwareSkinningScene_1.h"
//#include "CourseObjects/Week 7/SoftwareSkinningScene_2.h"
//#include "CourseObjects/Week 7/SoftwareSkinningScene_3.h"
//#include "CourseObjects/Week 8/HardwareSkinningScene.h"
//#include "CourseObjects/Week 9/PostProcessingScene.h"
//#include "CourseObjects/Week 10/ParticleScene.h"
//#include "CourseObjects/Week 11/SkyBoxScene.h"
//#endif
//
//#ifdef TEST_SCENES
//#include "TestScene.h"
//#include "TestScenes\ClothScene.h"
//#include "TestScenes\CharacterTestScene.h"
//#include "TestScenes\PickSceneTest.h"
//#include "TestScene.h"
//#endif
//
//#ifdef HUD_SCENES
//#define SPRITE_EFFECT_SUPPORT
//#include "TestScenes\HudTestScene.h"
//#endif
//
//#ifdef TERRAIN_TEST
//#include "CourseObjects/Terrain/TerrainTestScene.h"
//#endif
//
//#ifdef SINGLE_SCENE
//#include "CourseObjects/Week 11/SkyBoxScene.h"
//#endif


MainGame::MainGame(void)
{
}


MainGame::~MainGame(void)
{
}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);

	//gameSettings.DirectX.EnableDeferredRendering = true;

	//Nothing to do atm.
}

void MainGame::Initialize()
{
	
#ifdef COURSE_SCENES
	//SceneManager::GetInstance()->AddGameScene(new UberScene());
	//SceneManager::GetInstance()->AddGameScene(new SpikeyScene());
	//SceneManager::GetInstance()->AddGameScene(new TutorialScene());
	//SceneManager::GetInstance()->AddGameScene(new PongScene());
	//SceneManager::GetInstance()->AddGameScene(new ModelTestScene());
	//SceneManager::GetInstance()->AddGameScene(new SpriteTestScene());
	SceneManager::GetInstance()->AddGameScene(new EditorScene());
	//SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_1());
	//SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_2());
	//SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_3());
	//SceneManager::GetInstance()->AddGameScene(new HardwareSkinningScene());
	//SceneManager::GetInstance()->AddGameScene(new PostProcessingScene());
	//SceneManager::GetInstance()->AddGameScene(new ParticleScene());
	//SceneManager::GetInstance()->AddGameScene(new SkyBoxScene());
#endif

#ifdef TEST_SCENES
	SceneManager::GetInstance()->AddGameScene(new ClothScene());
	SceneManager::GetInstance()->AddGameScene(new CharacterTestScene());
	SceneManager::GetInstance()->AddGameScene(new PickSceneTest());
	SceneManager::GetInstance()->AddGameScene(new TestScene());
#endif

#ifdef HUD_SCENES
	SceneManager::GetInstance()->AddGameScene(new HudTestScene());
#endif

#ifdef TERRAIN_TEST
	SceneManager::GetInstance()->AddGameScene(new TerrainTestScene());
#endif

#ifdef SINGLE_SCENE
	SceneManager::GetInstance()->AddGameScene(new SkyBoxScene());
#endif

}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
			else if (wParam == VK_F4)
			{
				DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
			else if (wParam == VK_F6)
			{
				auto activeScene = SceneManager::GetInstance()->GetActiveScene();
				activeScene->GetPhysxProxy()->NextPhysXFrame();

			}
		}
	}

	return -1;
}
