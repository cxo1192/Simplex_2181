#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Cameron Ostrom - cxo1192@rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(vector3(5.0f,3.0f,15.0f), ZERO_V3, AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");
	
	m_stopsList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_stopsList.push_back(vector3(1.0f, -2.0f, 5.0f));

	m_stopsList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_stopsList.push_back(vector3(2.0f, -1.0f, 3.0f));

	m_stopsList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_stopsList.push_back(vector3(3.0f, 0.0f, 0.0f));

	m_stopsList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_stopsList.push_back(vector3(4.0f, 1.0f, -3.0f));

	m_stopsList.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_stopsList.push_back(vector3(5.0f, 2.0f, -5.0f));

	m_stopsList.push_back(vector3(1.0f, 3.0f, -5.0f));
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
	// Clear the screen
	ClearScreen();

	// Draw the model
	m_pModel->PlaySequence();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;
	
	
	


	//your code goes here
	//v3CurrentPos = vector3(0.0f, 0.0f, 0.0f);
	//-------------------

	float fpercent = static_cast<float>(MapValue(fTimer, 0.0f, 5.0f, 0.0f, 1.0f)); //what percent do I want to lerp mapped so that the lerp wil work so that it takes 5 seconds to get from start to finish
	
	static uint route = 0; //unsigned int for dictating wich set of points to travel to and from


	static vector3 start; //starting point for each small section of the journey 
	static vector3 end; //ending point of each section


	switch (route) //sets wich point is start and wich is end
	{
	case 0:
		start = m_stopsList[0];
		end = m_stopsList[1];
		break;
	case 1:
		start = m_stopsList[1];
		end = m_stopsList[2];
		break;
	case 2:
		start = m_stopsList[2];
		end = m_stopsList[3];
		break;
	case 3:
		start = m_stopsList[3];
		end = m_stopsList[4];
		break;
	case 4:
		start = m_stopsList[4];
		end = m_stopsList[5];
		break;
	case 5:
		start = m_stopsList[5];
		end = m_stopsList[6];
		break;
	case 6:
		start = m_stopsList[6];
		end = m_stopsList[7];
		break;
	case 7:
		start = m_stopsList[7];
		end = m_stopsList[8];
		break;
	case 8:
		start = m_stopsList[8];
		end = m_stopsList[9];
		break;
	case 9:
		start = m_stopsList[9];
		end = m_stopsList[10];
		break;
	case 10:
		start = m_stopsList[10];
		end = m_stopsList[0];
		break;
	default:
		break;
	}


	v3CurrentPos = glm::lerp(start, end, fpercent); //set models current position as a lerp between start and end based on fpercent

	if (fpercent >= 1.0f) { //if on or beyond the end point reset the start and endpoint and the timer used to develop the fpercent
		route++;
		if (route > 10) { //if on the first point again reset the switch statement 
			route = 0;
		}
		fTimer = m_pSystem->GetDeltaTime(uClock);
	}



	
	matrix4 m4Model = glm::translate(v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("\nTimer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	// Draw stops
	for (uint i = 0; i < m_stopsList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_stopsList[i]) * glm::scale(vector3(0.05f)), C_GREEN, RENDER_WIRE);
	}
	
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
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}