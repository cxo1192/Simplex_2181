#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(2.0f, C_BROWN);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{

	static float xchange = 0.1f;
	xchange += 0.01;

	float xshapechange = 0;

	float yshapechange = 0;


	// Clear the screen
	ClearScreen();

	int alienArrayPos[8][11] = { {0,0,1,0,0,0,0,0,1,0,0},
								 {0,0,0,1,0,0,0,1,0,0,0},
								 {0,0,1,1,1,1,1,1,1,0,0},
								 {0,1,1,0,1,1,0,1,1,0,0},
								 {1,1,1,1,1,1,1,1,1,1,1},
								 {1,0,1,1,1,1,1,1,1,0,1},
								 {1,0,1,0,0,0,0,0,1,0,1},
								 {0,0,0,1,1,0,1,1,0,0,0} };


	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 11; j++)
		{
			if (alienArrayPos[i][j] == 1) {
				m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3(xshapechange + xchange, yshapechange, 0.0f)));
			}
			xshapechange += 1;
		}
		xshapechange = 0;

		yshapechange -= 1;
	}


	//m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	//m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( xchange, 0.0f, 0.0f)));



	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}