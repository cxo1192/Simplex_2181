#include "Octree.h"
#include "AppClass.h"
using namespace Simplex;
//  Octree
 uint Octree::octantCount = 0; //number of octants in total
// uint Octree::leavesCount = 0; //number of leaves
 uint Octree::maxLevelSub = 0; //maximum level of subdivision
 uint Octree::entityCount = 0; //number of entities
 uint Octree::m_nCount = 0;
 Octree* Octree::root = nullptr;

void Octree::Init(void)
{
	
	m_nData = 0;
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_iID = m_nCount;
	m_nCount++;

	
	//IsColliding();
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}

//Octree::Octree(uint maxLev, uint entCount, bool isRoot, vector3 newCent, vector3 newSize)
Octree::Octree(uint mlev) //ctor
{
	
	Init();
	//m_nCount = 0;
	maxLevelSub = mlev;
	MyEntity** l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = m_pEntityMngr->GetEntityCount();
	std::vector<vector3> v3MaxMin_list;
	for (uint i = 0; i < iEntityCount; ++i)
	{
		MyRigidBody* pRG = l_Entity_List[i]->GetRigidBody();
		vector3 v3Min = pRG->GetMinGlobal();
		vector3 v3Max = pRG->GetMaxGlobal();
		v3MaxMin_list.push_back(v3Min);
		v3MaxMin_list.push_back(v3Max);
		//thisEntityList.push_back((l_Entity_List[i]));
		AddArr(l_Entity_List[i]);
	}

	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	m_pRigidBody->MakeCubic();
	//m_iID = m_nCount;
	SubDivide();
	IsColliding();
}

Octree::Octree(vector3 a_v3Center, float a_fSize)
{
	Init();
	std::vector<vector3> v3MaxMin_list;
	v3MaxMin_list.push_back(a_v3Center - vector3(a_fSize));
	v3MaxMin_list.push_back(a_v3Center + vector3(a_fSize));
	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	MyEntity** l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < iEntityCount; ++i)
	{
		//thisEntityList.push_back(l_Entity_List[i]);
		AddArr(l_Entity_List[i]);
	}
	//m_nCount++;
//	m_iID = m_nCount;
	IsColliding();
}

void Octree::Release(void)
{
	if (this == root)
	{
		if (numChildren == 0)//is a Leaf
		{
			m_nCount = 0;
			return;
		}
		else { 
			for (uint i = 0; i < numChildren; i++) //delete all children in root first
			{
				m_pChild[i]->Release();
				SafeDelete(m_pChild[i]);
			}
		}
		//m_nCount = 0;
		delete Arr;
	}
	
}
void Octree::AddArr(MyEntity * adder)
{
	if (Arr == nullptr) {
		Arr = new MyEntity*[1000];
		Arr[0] = adder;
		ArrCount++;
	}

	if (ArrCount + 1 > maxCount) {
		
		//int tempCount = ;
		//MyEntity** temp = new MyEntity*[ArrCount];
		MyEntity** temp = Arr;
		
		Arr = new MyEntity*[ArrCount * 2];

		for (uint i = 0; i < ArrCount; i++)
		{
			Arr[i] = temp[i];
			//temp[i] = Arr[i];
		}
		
		
		
		maxCount *= 2;

		Arr[ArrCount] = adder;
		ArrCount++;

	}
	else {
		Arr[ArrCount] = adder;
		ArrCount++;
	}
}
void Octree::Display(uint index, bool indie)
{
	if (indie) {
		//use index to display specific box
		for (uint i = 0; i < 8; i++)
		{
			if (m_pChild[i]) {
				if (m_pChild[i]->m_iID == index) {
					m_pChild[i]->m_pRigidBody->AddToRenderList();
					return;
				}
			}
				
		}

		for (uint i = 0; i < 8; i++)
		{
			if (m_pChild[i]) {
				
				m_pChild[i]->Display(index, indie);
				//return;
			}
		}
		
		return;
	}

	
	
	if (maxLevelSub > m_nLevel) {
		for (uint i = 0; i < 8; i++)
		{
			if (m_pChild[i])
				m_pChild[i]->Display(index, indie);
		}
		return;
	}
	m_pRigidBody->AddToRenderList();
	
}

void Octree::IsColliding(/*uint rBodyIndex*/) //used to see if entity is colliding with this cube
{
	////std::vector<MyEntity> l_Entity_List = m_pEntityMngr->GetEntityList();
	////uint iEntityCount = l_Entity_List.size();


	////for (uint i = 0; i < iEntityCount; ++i)
	////{
	////	MyRigidBody * pRB = l_Entity_List[i].GetRigidBody();
	////	if (pRB->IsColliding(m_pRigidBody)) {
	////		l_Entity_List[i].AddDimension(m_ID);
	////	}
	////}

	//MyRigidBody* rBody = m_pEntityMngr->GetRigidBody(rBodyIndex);
	//vector3 rBodyMax = rBody->GetMaxGlobal();
	//vector3 rBodyMin = rBody->GetMinGlobal();

	//if (rBodyMax.x > minCorner.x && rBodyMax.y > minCorner.y && rBodyMax.z > minCorner.z && rBodyMin.x < maxCorner.x && rBodyMin.y < maxCorner.y && rBodyMin.z < maxCorner.z) {
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}



	//////////////////////////////////////////////////////////////////
	/*MyEntity** l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < iEntityCount; ++i)
	{
		MyRigidBody* pRB = l_Entity_List[i]->GetRigidBody();
		if (pRB->IsColliding(m_pRigidBody))
		{
			l_Entity_List[i]->AddDimension(m_iID);
		}
	}*/

	/////////////////////////////////////////////////////
	//MyEntity** l_Entity_List = m_pEntityMngr->GetEntityList();
	//uint iEntityCount = m_pEntityMngr->GetEntityCount();
	//if (numChildren == 0) {
	//	for (uint i = 0; i < iEntityCount; i++)
	//	{
	//		//l_Entity_List[i]->AddDimension(m_iID);
	//		/*uint indx = m_pEntityMngr->GetEntityIndex(l_Entity_List[i]->GetUniqueID());
	//		m_pEntityMngr->AddDimension(indx, m_iID);*/
	//		m_pEntityMngr->AddDimension(indecies[i], m_iID);
	//	}
	//}
	//else {
	//	for (uint i = 0; i < numChildren; i++)
	//	{
	//		m_pChild[i]->IsColliding();
	//	}
	//}
	/////////////////////////////////////////////
	//Clear all collisions
	for (uint i = 0; i < ArrCount; i++)
	{
		//thisEntityList[i]->ClearCollisionList();
		Arr[i]->ClearCollisionList();
	}

	//check collisions
	for (uint i = 0; i < ArrCount - 1; i++)
	{
		for (uint j = i + 1; j < ArrCount; j++)
		{
			Arr[i]->IsColliding(Arr[j]);
		}
	}

}
void Octree::SubDivide(void)
{
	if (m_nLevel >= maxLevelSub) //change to use maxlevel instead of magic number
		return;

	vector3 v3Center = m_pRigidBody->GetCenterLocal();
	vector3 v3HalfWidth = m_pRigidBody->GetHalfWidth();
	float fSize = (v3HalfWidth.x) / 2.0f;
	float fCenters = fSize;

	m_pChild[0] = new Octree(v3Center + vector3(fCenters, fCenters, fCenters), fSize);
	m_pChild[1] = new Octree(v3Center + vector3(-fCenters, fCenters, fCenters), fSize);
	m_pChild[2] = new Octree(v3Center + vector3(-fCenters, -fCenters, fCenters), fSize);
	m_pChild[3] = new Octree(v3Center + vector3(fCenters, -fCenters, fCenters), fSize);
						 
	m_pChild[4] = new Octree(v3Center + vector3(fCenters, fCenters, -fCenters), fSize);
	m_pChild[5] = new Octree(v3Center + vector3(-fCenters, fCenters, -fCenters), fSize);
	m_pChild[6] = new Octree(v3Center + vector3(-fCenters, -fCenters, -fCenters), fSize);
	m_pChild[7] = new Octree(v3Center + vector3(fCenters, -fCenters, -fCenters), fSize);

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i]->m_nLevel = m_nLevel + 1;
		m_pChild[i]->m_pParent = this;
		//m_pChild[i].m_
		m_pChild[i]->SubDivide();
	}


}

//The big 3

Octree::Octree(Octree const& other)
{
	Init();

	m_pParent = other.m_pParent;
	Release();
	numChildren = other.numChildren;
	/*currentLevelEntityCount = other.currentLevelEntityCount;*/
	root = other.root;
	
	for (uint i = 0; i < numChildren; i++)
	{
		m_pChild[i] = new Octree(*other.m_pChild[i]);
	}

	

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
Octree::~Octree() { Release(); }
void Octree::ConfigureEntityDimensions(void)
{
	/*if (numChildren == 0)
	{
		for (uint i = 0; i < currentLevelEntityCount; ++i)
		{
			m_pEntityMngr->AddDimension(entityList[i], m_ID);
		}
	}
	else {
		for (uint i = 0; i < numChildren; ++i)
		{
			m_pChild[i]->ConfigureEntityDimensions();
		}
	}*/
}
;
//Accessors
void Octree::SetData(int a_nData) { m_nData = a_nData; }
int Octree::GetData(void) { return m_nData; }
void Octree::SetDataOnVector(int a_nData) { m_lData.push_back(a_nData); }
uint Octree::GetOctantCount(void) {
	return octantCount;
}
int& Octree::GetDataOnVector(int a_nIndex)
{
	int nIndex = static_cast<int>(m_lData.size());
	assert(a_nIndex >= 0 && a_nIndex < nIndex);
	return m_lData[a_nIndex];
}
void Octree::Swap(Octree& other)
{
	std::swap(m_nData, other.m_nData);
	std::swap(m_lData, other.m_lData);
}
//--- Non Standard Singleton Methods

