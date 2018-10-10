/*
Cameron Ostrom
DSA2 Section 1
class used to render spheres as they move from one vertex to another via lerp along an orbit of varinig radi and number of verticies
*/
#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Cameron Ostrom - cxo1192@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits
	float fRadius = 0.95f; //keeps track of radius of each torus 

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		

		//generate stoplist
		std::vector<vector3> stopList; //list of stops that each individule sphere will stop at

		float theta = (360 / i)* PI / 180; //angle of subdivision between each stop in the torus

		for (int j = 0; j <= i; j++) { //generate a vertex one subdivision away from previous vertex

			float x = fRadius * cos(theta * j);

			float y = fRadius * sin(theta * j);

			vector3 thisVertex = vector3(x, y, 0);

			stopList.push_back(thisVertex); //add the found vertex to the list of stops
		}

		allStops.push_back(stopList); //add the stop list for the individule sphere to a list of all the spheres stop lists
	


		//add a start and end index for each individule sphere to the respective vector
		startIndexs.push_back(0); 
		endIndexs.push_back(1);

		fSize += 0.5f; //increment the size for the next orbit
		fRadius += 0.5f; //incriment the radius of the next orbit 
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}

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


	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer


	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	m4Offset = glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Z);

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos;

		//vectors to store the current spheres start and endpoints
		static vector3 start; 
		static vector3 end;

		//sets the start and endpoint to lerp inbetween for each sphere
		start = allStops[i][startIndexs[i]];
		end = allStops[i][endIndexs[i]];
		
		float fpercent = static_cast<float>(MapValue(fTimer, 0.0f, 0.5f, 0.0f, 1.0f)); //percent of the way to lerp based on time mapped to half a second
		
		v3CurrentPos = glm::lerp(start, end, fpercent); //set spheres current position as a lerp between start and end based on fpercent


		if (fpercent >= 1.0f) { //if on or beyond the end point reset the start and endpoint and the timer used to develop the fpercent
		
			for (int k = 0; k < startIndexs.size(); k++) { //reset every start and end point index as they all arrive at the same time
				startIndexs[k] = startIndexs[k] + 1;
				endIndexs[k] = endIndexs[k] + 1;

				//if the sphere reaches the last stop the new end is set to the begining
				if (endIndexs[k] + 1>= allStops[k].size()) { //if on the first point again reset the switch statement 
					endIndexs[k] = 0;
				}
				//if the sphere reaches the first stop the new start is set to the begining again
				if (startIndexs[k] + 1 >= allStops[k].size()) { //if on the first point again reset the switch statement 
					startIndexs[k] = 0;
				}
			}

			fTimer = m_pSystem->GetDeltaTime(uClock);
		}
		
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.2)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}