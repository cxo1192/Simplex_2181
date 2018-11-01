#include "MyRigidBody.h"
using namespace Simplex;
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pMeshMngr, a_pOther.m_pMeshMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pMeshMngr = a_pOther.m_pMeshMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };
//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding as pre-test
	bool bColliding = (glm::distance(GetCenterGlobal(), a_pOther->GetCenterGlobal()) < m_fRadius + a_pOther->m_fRadius);
	
	//if they are colliding check the SAT
	if (bColliding)
	{
		if(SAT(a_pOther) != eSATResults::SAT_NONE)
			bColliding = false;// reset to false
	}

	if (bColliding) //they are colliding
	{
		this->AddCollisionWith(a_pOther);
		a_pOther->AddCollisionWith(this);
	}
	else //they are not colliding
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}

	return bColliding;
}
void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pMeshMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pMeshMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}

uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
 // local direction vectors , right, up, forward

 //currecnt object
vector3 right = glm::normalize(v3Corner[1] - v3Corner[0]);// back down right - back down left
vector3 up = glm::normalize(v3Corner[2] - v3Corner[0]); //back up left - back down left 
vector3 forward = glm::normalize(v3Corner[4] - v3Corner[0]); //front down left - back down left

vector3 otherRight = glm::normalize(a_pOther->v3Corner[1] - a_pOther->v3Corner[0]);//other object
vector3 otherUp = glm::normalize(a_pOther->v3Corner[2] - a_pOther->v3Corner[0]);
vector3 otherForward = glm::normalize(a_pOther->v3Corner[4] - a_pOther->v3Corner[0]);

// mins and maxes of points along projection line
float starterNum = 9990; //large placeholder value to be replaced later
float min, minOther; 
min = minOther = starterNum;
float max, maxOther;
max = maxOther = -starterNum;
int looper = 8;//value for looping through points


//loop to check collision
for (int i = 0; i < looper; i++)// normal checks using right up forward
{
	
	float cont = glm::dot(right, v3Corner[i]); //fill container with the projection of the corner on the right direction vector
	float otherCont = glm::dot(right, a_pOther->v3Corner[i]); //fill container with the projection of the other objects corner on the right direction vector
	
	vector4 minzMaxz = compareHelper(cont, otherCont, min, max, minOther, maxOther); //compares the values in the container to see if it is a min or max for the line
	min = minzMaxz.x;
	max = minzMaxz.y;
	minOther = minzMaxz.z;
	maxOther = minzMaxz.w;
}
if (min >= maxOther || minOther >= max) //if the lines dont overlap there is no collision
	return 1;

// Reset
min = minOther = starterNum;
max = maxOther = -starterNum;

for (int i = 0; i < looper; i++)
{
	float cont = glm::dot(otherRight, v3Corner[i]); //project corners on right vector of other object
	float otherCont = glm::dot(otherRight, a_pOther->v3Corner[i]);
	
	vector4 minzMaxz = compareHelper(cont, otherCont, min, max, minOther, maxOther);

	min = minzMaxz.x;
	max = minzMaxz.y;
	minOther = minzMaxz.z;
	maxOther = minzMaxz.w;
}
if (min >= maxOther || minOther >= max)//if the lines dont overlap there is no collision	
	return 1;

//reset
min = minOther = starterNum; 
max = maxOther = -starterNum;

for (int i = 0; i < looper; i++)
{

	float cont = glm::dot(up, v3Corner[i]); //project corners on up vector
	float otherCont = glm::dot(up, a_pOther->v3Corner[i]);

	vector4 minzMaxz = compareHelper(cont, otherCont, min, max, minOther, maxOther);

	min = minzMaxz.x;
	max = minzMaxz.y;
	minOther = minzMaxz.z;
	maxOther = minzMaxz.w;
}
if (min >= maxOther || minOther >= max)//if the lines dont overlap there is no collision
	return 1;

//reset
min = minOther = starterNum;
max = maxOther = -starterNum;

for (int i = 0; i < looper; i++)
{
	
	float cont = glm::dot(otherUp, v3Corner[i]);//project corners on up of other object
	float otherCont = glm::dot(otherUp, a_pOther->v3Corner[i]);

	vector4 minzMaxz = compareHelper(cont, otherCont, min, max, minOther, maxOther);

	min = minzMaxz.x;
	max = minzMaxz.y;
	minOther = minzMaxz.z;
	maxOther = minzMaxz.w;
}
if (min >= maxOther || minOther >= max)//if the lines dont overlap there is no collision
	return 1;

//reset
min = minOther = starterNum;
max = maxOther = -starterNum;

for (int i = 0; i < looper; i++)
{
	
	float cont = glm::dot(forward, v3Corner[i]);//project corners on forward vector
	float otherCont = glm::dot(forward, a_pOther->v3Corner[i]);

	vector4 minzMaxz = compareHelper(cont, otherCont, min, max, minOther, maxOther);

	min = minzMaxz.x;
	max = minzMaxz.y;
	minOther = minzMaxz.z;
	maxOther = minzMaxz.w;
}
if (min >= maxOther || minOther >= max)//if the lines dont overlap there is no collision
	return 1;

//reset
min = minOther = starterNum;
max = maxOther = -starterNum;

for (int i = 0; i < looper; i++)
{

	float cont = glm::dot(otherForward, v3Corner[i]); //project corners on forward of other object
	float otherCont = glm::dot(otherForward, a_pOther->v3Corner[i]);

	vector4 minzMaxz = compareHelper(cont, otherCont, min, max, minOther, maxOther);

	min = minzMaxz.x;
	max = minzMaxz.y;
	minOther = minzMaxz.z;
	maxOther = minzMaxz.w;
}
if (min >= maxOther || minOther >= max)//if the lines dont overlap there is no collision
	return 1;


// find other axis to project on via the cross product and save them 

vector3 generatedAxis[9];
generatedAxis[0] = glm::cross(right, otherRight);
generatedAxis[1] = glm::cross(right, otherUp);
generatedAxis[2] = glm::cross(right, otherForward);

generatedAxis[3] = glm::cross(up, otherRight);
generatedAxis[4] = glm::cross(up, otherUp);
generatedAxis[5] = glm::cross(up, otherForward);

generatedAxis[6] = glm::cross(forward, otherRight);
generatedAxis[7] = glm::cross(forward, otherUp);
generatedAxis[8] = glm::cross(forward, otherForward);


// Loop to check to check if objects are colliding using the cross product axis
for (int i = 0; i < 9; i++) //loops through generated axis
{
	//reset
	 min = minOther = starterNum;
	 max = maxOther = -starterNum;
	
	if (generatedAxis[i] != vector3(0)) //cant project on zero vector  
	{
		for (int j = 0; j < 8; j++) //loops through corners
		{
			
			float cont = glm::dot(generatedAxis[i], v3Corner[j]);//projects all the corners on the generated axis
			float otherCont = glm::dot(generatedAxis[i], a_pOther->v3Corner[j]);

			vector4 minzMaxz = compareHelper(cont, otherCont, min, max, minOther, maxOther);

			min = minzMaxz.x;
			max = minzMaxz.y;
			minOther = minzMaxz.z;
			maxOther = minzMaxz.w;
		}
		if (min >= maxOther || minOther >= max)//if the lines dont overlap there is no collision
			return 1;
	}
}
//there is no axis test that separates this two objects
return 0;
}

///compare passed in projected corners inside the containers to see if they are mins or maxs on the line 
vector4 MyRigidBody::compareHelper(float container, float otherContainer, float minimum, float maximum, float otherMinimum, float otherMaximum) {
	
	//replaces mins and maxs 
	if (container <= minimum)
	{
		minimum = container;
	}

	if (container >= maximum)
	{
		maximum = container;
	}


	//other object minz and maxs
	if (otherContainer <= otherMinimum)
	{
		otherMinimum = otherContainer;
	}

	if (otherContainer >= otherMaximum)
	{
		otherMaximum = otherContainer;
	}


	return vector4(minimum, maximum, otherMinimum, otherMaximum); //returns all minz and maxs 
}