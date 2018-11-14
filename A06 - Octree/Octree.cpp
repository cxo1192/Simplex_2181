#include "Octree.h"
using namespace Simplex;
//  Octree
void Octree::Init(void)
{
	m_nData = 0;
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	std::vector<MyEntity> l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = l_Entity_List.size();
	std::vector<vector3> v3MaxMin_list;

	for (uint i = 0; i < iEntityCount; ++i)
	{
		MyRigidBody* pRB = l_Entity_List[i].GetRigidBody();
		vector3 v3Min = pRB->GetMinGlobal();
		vector3 v3Max = pRB->GetMaxGlobal();
		v3MaxMin_list.push_back(v3Min);
		v3MaxMin_list.push_back(v3Max);
		/*vector3 v3Position = pRB->GetCenterGlobal();

		if (v3Position.x < 0.0f)
		{
		if (v3Position.x < -17.0f)
		m_pEntityMngr->AddDimension(-1, 1);
		else
		m_pEntityMngr->AddDimension(-1, 2);
		}
		else if (v3Position.x > 0.0f)
		{
		if (v3Position.x > 17.0f)
		m_pEntityMngr->AddDimension(-1, 3);
		else
		m_pEntityMngr->AddDimension(-1, 4);
		}*/
		for (size_t i = 0; i < 8; i++)
		{
			m_pChild[i] = nullptr;
		}
		m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	}
}
void Octree::Swap(Octree& other)
{
	std::swap(m_nData, other.m_nData);
	std::swap(m_lData, other.m_lData);
}
void Octree::Release(void)
{
	m_lData.clear();
}
void Octree::Display(void)
{
	//m_pRigidBody->AddToRenderList();
	m_pMeshMngr->AddWireCubeToRenderList(glm::scale(vector3(70)), C_BLUE);
}
void Octree::IsColliding(void)
{
	std::vector<MyEntity> l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = l_Entity_List.size();


	for (uint i = 0; i < iEntityCount; ++i)
	{
		MyRigidBody * pRB = l_Entity_List[i].GetRigidBody();
		if (pRB->IsColliding(m_pRigidBody)) {
			l_Entity_List[i].AddDimension(m_ID);
		}
	}
}
void Octree::SubDivide(void)
{
	//needs to know if its a root or not 
	for (size_t i = 0; i < 8; i++)
	{
		m_pChild[i] = new Octree;
	}
}

//The big 3
Octree::Octree() { Init(); }
Octree::Octree(Octree const& other)
{
	m_nData = other.m_nData;
	m_lData = other.m_lData;
}
Octree& Octree::operator=(Octree const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octree temp(other);
		Swap(temp);
	}
	return *this;
}
Octree::~Octree() { Release(); };
//Accessors
void Octree::SetData(int a_nData) { m_nData = a_nData; }
int Octree::GetData(void) { return m_nData; }
void Octree::SetDataOnVector(int a_nData) { m_lData.push_back(a_nData); }
int& Octree::GetDataOnVector(int a_nIndex)
{
	int nIndex = static_cast<int>(m_lData.size());
	assert(a_nIndex >= 0 && a_nIndex < nIndex);
	return m_lData[a_nIndex];
}
//--- Non Standard Singleton Methods

