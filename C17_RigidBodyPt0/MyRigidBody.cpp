#include "MyRigidBody.h"
using namespace Simplex;
//Accessors
bool MyRigidBody::GetVisible(void) { return m_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColor(void) { return m_v3Color; }
void MyRigidBody::SetColor(vector3 a_v3Color) { m_v3Color = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void){	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix) { m_m4ToWorld = a_m4ModelMatrix; }
//Allocation
void MyRigidBody::Init(void)
{
	m_pMeshMngr = MeshManager::GetInstance();
	m_bVisible = true;

	m_fRadius = 0.0f;

	m_v3Color = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& other)
{
	std::swap(m_pMeshMngr , other.m_pMeshMngr);
	std::swap(m_bVisible , other.m_bVisible);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_v3Color , other.m_v3Color);

	std::swap(m_v3Center , other.m_v3Center);
	std::swap(m_v3MinL , other.m_v3MinL);
	std::swap(m_v3MaxL , other.m_v3MaxL);

	std::swap(m_v3MinG , other.m_v3MinG);
	std::swap(m_v3MaxG , other.m_v3MaxG);

	std::swap(m_v3HalfWidth , other.m_v3HalfWidth);

	std::swap(m_m4ToWorld , other.m_m4ToWorld);
}
void MyRigidBody::Release(void)
{
	m_pMeshMngr = nullptr;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();

	uint points = a_pointList.size();

	if (points == 0)
		return;

	m_v3MinL = a_pointList[0];
	m_v3MaxL = a_pointList[0];

	for (size_t i = 0; i < points; i++)
	{

		if (m_v3MinL.x > a_pointList[i].x)
			m_v3MinL.x = a_pointList[i].x;
		else if (m_v3MaxL.x < a_pointList[i].x)
			m_v3MaxL.x = a_pointList[i].x;

		if (m_v3MinL.y > a_pointList[i].y)
			m_v3MinL.y = a_pointList[i].y;
		else if (m_v3MaxL.y < a_pointList[i].y)
			m_v3MaxL.y = a_pointList[i].y;

		if (m_v3MinL.z > a_pointList[i].z)
			m_v3MinL.z = a_pointList[i].z;
		else if (m_v3MaxL.z < a_pointList[i].z)
			m_v3MaxL.z = a_pointList[i].z;

	}

	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f; ///centroid
	
	/*
	for (uint i = 0; i < points; i++) { //find largest radius from center point
		float fDistance = glm::distance(m_v3Center, a_pointList[i]);
		if (m_fRadius < fDistance)
			m_fRadius = fDistance;
	}
	*/


	m_fRadius = glm::distance(m_v3Center, m_v3MaxL);

	//vector3 v3Size;
	/////////////////////////////v3Size.x = glm::distance(vector3(m_v3MaxL.x, 0, 0), vector3(m_v3MinL.x, 0, 0));
	//v3Size = m_v3MaxL - m_v3MinL;
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

}
MyRigidBody::MyRigidBody(MyRigidBody const& other)
{
	m_pMeshMngr = m_pMeshMngr;
	m_bVisible = m_bVisible;

	m_fRadius = m_fRadius;

	m_v3Color = m_v3Color;

	m_v3Center = m_v3Center;
	m_v3MinL = m_v3MinL;
	m_v3MaxL = m_v3MaxL;

	m_v3MinG = m_v3MinG;
	m_v3MaxG = m_v3MaxG;

	m_v3HalfWidth = m_v3HalfWidth;

	m_m4ToWorld = m_m4ToWorld;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyRigidBody temp(other);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody(){Release();};

//--- Non Standard Singleton Methods
void MyRigidBody::AddToRenderList(void)
{
	if (!m_bVisible)
		return;

	//m_pMeshMngr->AddSphereToRenderList(m_m4ToWorld * glm::translate(m_v3Center) * glm::scale(vector3(5.0f)), C_RED, RENDER_SOLID);
	/*m_pMeshMngr->AddWireSphereToRenderList(m_m4ToWorld * 
										glm::translate(m_v3Center) * 
										glm::scale(vector3(m_fRadius)), 
										m_v3Color, RENDER_SOLID);*/

	/*
	m_pMeshMngr->AddWireCubeToRenderList(m_m4ToWorld *
											glm::translate(m_v3Center) *
											glm::scale(m_v3HalfWidth * 2.0f),
											C_YELLOW, RENDER_WIRE);


											*/


	m_pMeshMngr->AddWireCubeToRenderList(m_m4ToWorld *
											glm::translate(m_v3Center) *
											glm::scale(m_v3HalfWidth * 2.0f),
											m_v3Color, RENDER_WIRE);
}
bool MyRigidBody::IsColliding(MyRigidBody* const other)
{
	vector3 v3Max1 = m_m4ToWorld * vector4(m_v3MaxL, 1.0f);
	vector3 v3Min1 = m_m4ToWorld * vector4(m_v3MinL, 1.0f);

	vector3 v3Max2 = m_m4ToWorld * vector4(m_v3MaxL, 1.0f);
	vector3 v3Min2 = m_m4ToWorld * vector4(m_v3MinL, 1.0f);

	bool bColliding = false;
	if (v3Max1.x < v3Min2.x || v3Min1.x > v3Max2.x)
		return false;

	if (v3Max1.y < v3Min2.y || v3Min1.y > v3Max2.y)
		return false;


	if (v3Max1.z < other->m_v3MinL.z || m_v3MinL.z > other->m_v3MaxL.z)
		return false;

	return bColliding;
	

	//return (glm::distance(m_v3Center, other->m_v3Center) < m_fRadius + other->m_fRadius);
}