#include "BsMainEditorWindow.h"
#include "BsDockManager.h"
#include "BsGUIMenuBar.h"
#include "BsCamera.h"
#include "CmSceneObject.h"
#include "CmRenderTexture.h"

// DEBUG ONLY
#include "CmTestTextSprite.h"
#include "CmDebugCamera.h"
#include "BsDrawHelper2D.h"
#include "BsDrawHelper3D.h"
#include "CmFRect.h"

using namespace CamelotFramework;
using namespace BansheeEngine;

namespace BansheeEditor
{
	MainEditorWindow::MainEditorWindow(CM::RenderWindowPtr renderWindow)
		:EditorWindowBase(renderWindow), mDockManager(cm_new<DockManager>(mGUI.get())), mMenuBar(cm_new<GUIMenuBar>(mGUI.get()))
	{
		UINT32 widgetWidth = (UINT32)std::max(0, (INT32)getWidth() - 2);
		UINT32 widgetHeight = (UINT32)std::max(0, (INT32)getHeight() - 2);

		UINT32 menuBarHeight = 20;
		mMenuBar->setArea(1, 1, widgetWidth, menuBarHeight);

		UINT32 dockHeight = (UINT32)std::max(0, (INT32)widgetHeight - (INT32)menuBarHeight);
		mDockManager->setArea(1, menuBarHeight + 1, widgetWidth, dockHeight);

		// DEBUG ONLY

		HSceneObject sceneCameraGO = SceneObject::create("SceneCamera");
		HCamera sceneCamera = sceneCameraGO->addComponent<Camera>();

		RenderTexturePtr sceneRenderTarget = RenderTexture::create(TEX_TYPE_2D, 800, 600);

		sceneCamera->initialize(sceneRenderTarget, 0.0f, 0.0f, 1.0f, 1.0f, 0);
		sceneCamera->setPriority(-1);
		sceneCameraGO->setPosition(Vector3(0,50,1240));
		sceneCameraGO->lookAt(Vector3(0,50,-300));
		sceneCamera->setNearClipDistance(5);
		sceneCamera->setAspectRatio(800.0f / 600.0f);

		GameObjectHandle<DebugCamera> debugCamera = sceneCameraGO->addComponent<DebugCamera>();

		GameObjectHandle<TestTextSprite> textSprite = mSceneObject->addComponent<TestTextSprite>();
		textSprite->initialize(mCamera->getViewport().get(), renderWindow.get());

		textSprite->init(sceneCamera, "Testing in a new row, does this work?", sceneRenderTarget);

		//DrawHelper2D::instance().drawQuad(sceneCamera, FRect(0.0f, 0.2f, 0.75f, 0.5f), Color::White, DebugDrawCoordType::Normalized, 250.0f);
		//DrawHelper2D::instance().drawQuad(sceneCamera, FRect(50.0f, 50.0f, 100.0f, 50.0f), Color::Blue, DebugDrawCoordType::Pixel, 250.0f);

		//DrawHelper2D::instance().drawLine_Pixel(sceneCamera, Vector2(0, 0), Vector2(20, 20), Color::Blue, DebugDrawCoordType::Pixel, 250.0f);

		//DrawHelper2D::instance().drawLine_AA(sceneCamera, Vector2(100, 10), Vector2(120, 40), 1.0f, 1.0f, Color::Blue, DebugDrawCoordType::Pixel, 250.0f);

		AABox dbgBox(Vector3(-300, -200, 1000), Vector3(300, 300, 1500));
		DrawHelper3D::instance().drawAABox(sceneCamera, dbgBox, Color::Green, 250.0f);
	}

	MainEditorWindow::~MainEditorWindow()
	{
		cm_delete(mDockManager);
		cm_delete(mMenuBar);
	}

	void MainEditorWindow::movedOrResized()
	{
		EditorWindowBase::movedOrResized();

		UINT32 widgetWidth = (UINT32)std::max(0, (INT32)getWidth() - 2);
		UINT32 widgetHeight = (UINT32)std::max(0, (INT32)getHeight() - 2);

		mDockManager->setArea(1, 1, widgetWidth, widgetHeight);
	}
}