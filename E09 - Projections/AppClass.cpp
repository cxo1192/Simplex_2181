#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Cameron Ostrom - cxo1192@rit.edu";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);
	
	//allocate the primitive
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateTorus(3.0f, 2.0f, 6, 6, C_RED);

	//allocate the primitive
	m_pMesh2 = new MyMesh();
	m_pMesh2->GenerateTorus(3.0f, 2.0f, 8, 8, C_GREEN);

	m_pMesh3 = new MyMesh();
	m_pMesh3->GenerateCone(0.5f, 1.0f, 6, C_BLUE);

	//create a new camera
	m_pCamera = new MyCamera();

	//projection used
	m_uProjection = 1;


	zNear = 0.0001f; //near clipping plane
	zFar = 1000.0f; //far clipping plane
	 

	mousePressed = false;
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
	//Clear the screen
	ClearScreen();
	
	//draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();




	

	//calculate view and projection
	switch (m_uProjection)
	{
	default:
	case 1:
		m_pCamera->ResetCamera();
		break;
	case 2:
		m_pCamera->ResetCamera();
		m_pCamera->SetPerspective(false); //makes the camera orthographic
		break;
	case 3:
		m_pCamera->ResetCamera();
		m_pCamera->SetPosition(vector3(25.0f, 0, 0)); //moves camera to the right of models
		m_pCamera->SetTarget(vector3(0.0f, 0.0f, 0.0f)); //points camera at models
		m_pCamera->SetUp(vector3(0.0f, 0.0f, -1.0f)); //orients camera so it is turned on its side
		break;
	case 4:
		m_pCamera->ResetCamera();
		m_pCamera->SetPosition(vector3(0, 0, -15.0f)); //moves camera behind models
		break;
	case 5:
		m_pCamera->ResetCamera();
		m_pCamera->SetPosition(vector3(0, 0, -15.0f)); //moves camera behind models
		zNear = 5.0f; //stores near plane far away than usual
		nF = vector2(zNear, zFar);//stores near and far 
		m_pCamera->SetNearFar(nF); //sets the near and far plane so blue pyramid cut out
		break;
	case 6:
		m_pCamera->ResetCamera();
		m_pCamera->SetPosition(vector3(0, 0, -15.0f));
		zFar = 15.0f; //stores far plane closer than usual
		nF = vector2(zNear, zFar);//stores near and far 
		m_pCamera->SetNearFar(nF); //sets the near and far plane so red torus cut out
		break;
	case 7:
		m_pCamera->ResetCamera();
		m_pCamera->SetUp(vector3(0, -1.0f, 0)); //orients camera so that it is upsidedown 
		break;
	}

	m_pCamera->CalculateProjectionMatrix();
	m_pCamera->CalculateViewMatrix();

	//draw the primitive
	m_pMesh1->Render(m_pCamera, glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh2->Render(m_pCamera, glm::translate(IDENTITY_M4, vector3(0.0f,0.0f,-5.0f)) * glm::rotate(IDENTITY_M4, 1.5708f, AXIS_X));
	m_pMesh3->Render(m_pCamera, glm::translate(vector3(0.0f, 0.0f, -10.0f)));

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
	//release primitive
	SafeDelete(m_pMesh1);
	SafeDelete(m_pMesh2);
	SafeDelete(m_pMesh3);
	
	//release the camera
	SafeDelete(m_pCamera);
	
	//release GUI
	ShutdownGUI();
}
