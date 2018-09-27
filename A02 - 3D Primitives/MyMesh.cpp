#include "MyMesh.h"
void MyMesh::GenerateCircle(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color) //function used as building block for other generate methods, from E04
{
	Release();
	Init();

	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	/*
		Calculate a_nSubdivisions number of points around a center point in a radial manner
		then call the AddTri function to generate a_nSubdivision number of faces

	*/

	float theta = (360 / a_nSubdivisions)* PI / 180;
	vector3 lastVertex = vector3(0, 0, 0);
	vector3 firstOuterVertex = vector3(0, 0, 0);

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float x = a_fRadius * cos(theta * i);

		float y = a_fRadius * sin(theta * i);

		vector3 thisVertex = vector3(x, y, 0);

		if (i == 0) {
			lastVertex = thisVertex;
			firstOuterVertex = thisVertex;
		}
		else if (i == a_nSubdivisions) {
			AddTri(vector3(0, 0, 0), lastVertex, firstOuterVertex);
		}
		else
		{
			AddTri(vector3(0, 0, 0), lastVertex, thisVertex);
			lastVertex = thisVertex;
		}
		//AddVertexPosition(vector3(x, y, 1));
	}


	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}

///Generates a cone based on a circle base conected with tris to a vertex that froms the tip
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();


	
	float theta = (360 / a_nSubdivisions)* PI / 180; //generates angle of each subdivision
	vector3 lastVertex = vector3(0, 0, 0); //the last vertex that was calculated before the current one
	vector3 firstOuterVertex = vector3(0, 0, 0); //the first vertex calculate in the whole cone
	vector3 tip = vector3(0, 0, -a_fHeight);  // the tip of the cone

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float x = a_fRadius * cos(theta * i); //rotate around the oragin by the angle of subdivison and generate a new vertex

		float y = a_fRadius * sin(theta * i);

		vector3 thisVertex = vector3(x, y, 0);

		if (i == 0) { //if it is the first vertex generated store the vertex without drawing a tri
			lastVertex = thisVertex;
			firstOuterVertex = thisVertex;
		}
		else if (i == a_nSubdivisions) { //if it is the last subdivision generate a tri using the last coordinates and the first coordinates found around the circle
			AddTri(vector3(0, 0, 0), lastVertex, firstOuterVertex);
			AddTri(firstOuterVertex, lastVertex, tip); //conect the circle to the tip with a tri to make the cones protrusion 
		}
		else
		{
			AddTri(vector3(0, 0, 0), lastVertex, thisVertex); //generate the tri in the subdivision of the circle base
			AddTri(thisVertex, lastVertex, tip); //conect the circle to the tip with a tri to make the cones protrusion 
			lastVertex = thisVertex; //store the current vertex for the next loop through
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
///generates a cylinder by creating two circles and conecting them with quads
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color) 
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();


	float theta = (360 / a_nSubdivisions)* PI / 180; //generates angele of subdivision
	vector3 lastVertex = vector3(0, 0, 0);  //stores the last vertex that was generated for the bottom circle
	vector3 firstOuterVertex = vector3(0, 0, 0); //stores the very first vertex of the bottom circle
	vector3 lastVertexTop = vector3(0, 0, a_fHeight); //the last vertex generated of the top circle
	vector3 firstOuterVertexTop = vector3(0, 0, a_fHeight); //the very first virtex generated of the top circle

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float x = a_fRadius * cos(theta * i);

		float y = a_fRadius * sin(theta * i);

		vector3 thisVertex = vector3(x, y, 0); //generate vertex on bottom circle
		vector3 thisVertexTop = vector3(x, y, a_fHeight); //generate coorisponding vertex on top circle

		if (i == 0) { //if first vertexs of each circle store the vertexs for next loop through and store them as the very first vertexes found
			lastVertex = thisVertex;
			firstOuterVertex = thisVertex;
			lastVertexTop = thisVertexTop;
			firstOuterVertexTop = thisVertexTop;
		}
		else if (i == a_nSubdivisions) { //if last subdivision connect the last vertexs to the very first vertexs found 
			AddTri(lastVertex, vector3(0, 0, 0), firstOuterVertex);
			AddTri(vector3(0, 0, a_fHeight), lastVertexTop, firstOuterVertexTop); //same tri as prior one but fliped the other direction so that it is visible from the bottom
			AddQuad(lastVertex, firstOuterVertex, lastVertexTop, firstOuterVertexTop);  //connects the two circles with quads
		}
		else
		{
			AddTri(lastVertex, vector3(0, 0, 0), thisVertex);
			AddTri(vector3(0, 0, a_fHeight), lastVertexTop, thisVertexTop);//same tri as prior one but fliped the other direction so that it is visible from the bottom
			AddQuad(lastVertex, thisVertex, lastVertexTop, thisVertexTop);  //connects the two circles with quads
			//store current vertexs for the next loop
			lastVertex = thisVertex;
			lastVertexTop = thisVertexTop;

		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
///generate tube by creating two uncaped cylinders and connecting them at the rims
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color) 
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();


	float theta = (360 / a_nSubdivisions)* PI / 180; //angle of subdivision
	vector3 lastVertex = vector3(0, 0, 0); //the last vertex generated on the outer bottom circle
	vector3 lastVertexIn = vector3(0, 0, 0); //the last vertex generated on the inner bottom circle
	vector3 firstOuterVertex = vector3(0, 0, 0); //the very first vertex generated on the outer bottom circle
	vector3 firstOuterVertexIn = vector3(0, 0, 0); //the very first vertex generated on the inner bottom circle
	vector3 lastVertexTop = vector3(0, 0, a_fHeight); //the last vertex generated on the outer top circle
	vector3 lastVertexTopIn = vector3(0, 0, a_fHeight); //the last vertex generated on the inner top circle
	vector3 firstOuterVertexTop = vector3(0, 0, a_fHeight); //the very first vertex generated on the outer top circle
	vector3 firstOuterVertexTopIn = vector3(0, 0, a_fHeight); //the very first vertex generated on the inner top circle

	for (int i = 0; i <= a_nSubdivisions; i++) {

		float x = a_fOuterRadius * cos(theta * i);

		float y = a_fOuterRadius * sin(theta * i);

		float xIn = a_fInnerRadius * cos(theta * i);

		float yIn = a_fInnerRadius * sin(theta * i);


		//gen current outer vertex bottom and top for the bottom and top circles
		vector3 thisVertex = vector3(x, y, 0);
		vector3 thisVertexTop = vector3(x, y, a_fHeight);

		//gen current inner vertex bottom and top circles
		vector3 thisVertexIn = vector3(xIn, yIn, 0);
		vector3 thisVertexTopIn = vector3(xIn, yIn, a_fHeight);



		//if first vertex generated for inner and outer edges of both top and bottom store them for later loops
		if (i == 0) {
			lastVertex = thisVertex;
			lastVertexIn = thisVertexIn;
			
			lastVertexTop = thisVertexTop;
			lastVertexTopIn = thisVertexTopIn;

			
			firstOuterVertex = thisVertex;
			firstOuterVertexIn = thisVertexIn;
			firstOuterVertexTop = thisVertexTop;
			firstOuterVertexTopIn = thisVertexTopIn;
			
		}
		else if (i == a_nSubdivisions) { //if it is the last subdivision generate quads using the last vertecies and the very first set of vertices  
			AddQuad(firstOuterVertex, lastVertex, firstOuterVertexIn, lastVertexIn); //bottom quad
			AddQuad(lastVertexTop, firstOuterVertexTop, lastVertexTopIn, firstOuterVertexTopIn ); //top quad
			AddQuad(lastVertex,firstOuterVertex,lastVertexTop,firstOuterVertexTop);//connecting quad outer
			AddQuad(firstOuterVertexIn, lastVertexIn, firstOuterVertexTopIn, lastVertexTopIn);//connecting quad inner


		}
		else
		{
			

			//rims generated at the top and bottom of the tube connecting the inner and outer rings
			AddQuad(thisVertex, lastVertex, thisVertexIn, lastVertexIn); //bottom quad
			AddQuad(lastVertexTop, thisVertexTop, lastVertexTopIn, thisVertexTopIn); //top quad

			//quads connecting the top and bottom rims facing opposite dirrections
			AddQuad(lastVertex, thisVertex, lastVertexTop, thisVertexTop);//connecting quad outer
			AddQuad(thisVertexIn, lastVertexIn, thisVertexTopIn, lastVertexTopIn);//connecting quad inner


			//storeing the current vertecies for next loop
			lastVertex = thisVertex;
			lastVertexIn = thisVertexIn;
			lastVertexTop = thisVertexTop;
			lastVertexTopIn = thisVertexTopIn; 

		}
	}
	
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
///generate sphere by connecting concentric circles displaced vertically and capping the end with tris
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();


	float theta = (360 / a_nSubdivisions)* PI / 180; //angle of subdevision of three circles
	vector3 lastVertex = vector3(0, 0, 0); //last vertex generated for this loop of center circle  
	vector3 lastLowerVertex = vector3(0, 0, (-a_fRadius/2));  //last vertex generated of lower circle 
	vector3 lastHigherVertex = vector3(0, 0, (a_fRadius / 2)); //last vertex generated of higher circle

	//very first vertecies generated of each circle
	vector3 firstOuterVertex = vector3(0, 0, 0); 
	vector3 firstOuterLowerVertex = vector3(0, 0, 0);
	vector3 firstOuterHigherVertex = vector3(0, 0, 0);

	for (int i = 0; i <= a_nSubdivisions; i++) {

		//position of verticies of center circle
		float x = a_fRadius * cos(theta * i);
		float y = a_fRadius * sin(theta * i);

		//position of verticies of top and bottom circles
		float x2 = ((2 * a_fRadius)/3) * cos(theta * i);
		float y2 = ((2 * a_fRadius)/3) * sin(theta * i);

		vector3 thisVertex = vector3(x, y, 0); 
		vector3 thisLowerVertex = vector3(x2, y2, (-a_fRadius/2)); //verticies generated higher and lower than center circle
		vector3 thisHigherVertex = vector3(x2, y2, (a_fRadius/2));



		if (i == 0) { //if it is the first vertices generated store their location for later loops
			lastVertex = thisVertex;
			firstOuterVertex = thisVertex;

			lastLowerVertex = thisLowerVertex;
			firstOuterLowerVertex = thisLowerVertex;

			lastHigherVertex = thisHigherVertex;
			firstOuterHigherVertex = thisHigherVertex;

		}
		else if (i == a_nSubdivisions) { //if it is the last subdivision connect the last verticies to the very first vertices on each circle

			AddQuad(lastVertex, firstOuterVertex, lastHigherVertex, firstOuterHigherVertex); //connect the upper and center circles' last and first vertices
			AddQuad(lastLowerVertex, firstOuterLowerVertex, lastVertex, firstOuterVertex); //connect the lower and center circles' last and first vertices


			//generates rounded ends above/below the upper and lower circles 
			AddTri(lastLowerVertex, vector3(0, 0, (-a_fRadius )), firstOuterLowerVertex); //lower
			AddTri(vector3(0, 0, (a_fRadius )), lastHigherVertex, firstOuterHigherVertex); // upper

		}
		else
		{
			//connects the three circles using the last and current verticies
			AddQuad(lastVertex, thisVertex, lastHigherVertex, thisHigherVertex);
			AddQuad(lastLowerVertex, thisLowerVertex, lastVertex, thisVertex);

			//generates rounded end above and below upper and lower circle to complete the sphere
			AddTri(lastLowerVertex, vector3(0, 0, (-a_fRadius)), thisLowerVertex);
			AddTri(vector3(0, 0, (a_fRadius )), lastHigherVertex, thisHigherVertex);
			
			//store current vertices for next loop through to connect the next subdivision 
			lastVertex = thisVertex;
			lastLowerVertex = thisLowerVertex;
			lastHigherVertex = thisHigherVertex;
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}