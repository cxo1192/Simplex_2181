#include "Octree.h"
#include "AppClass.h"
using namespace Simplex;
//  Octree
 uint Octree::octantCount = 0; //number of octants in total
 uint Octree::leavesCount = 0; //number of leaves
 uint Octree::maxLevelSub = 0; //maximum level of subdivision
 uint Octree::entityCount = 0; //number of entities
 uint Octree::m_nCount = 0;
 Octree* Octree::root = nullptr;

void Octree::Init(void)
{
	//m_nData = 0;
	//m_pMeshMngr = MeshManager::GetInstance();
	//m_pEntityMngr = MyEntityManager::GetInstance();
	//std::vector<MyEntity> l_Entity_List = m_pEntityMngr->GetEntityList();
	//uint iEntityCount = l_Entity_List.size();
	//std::vector<vector3> v3MaxMin_list;

	//for (uint i = 0; i < iEntityCount; ++i)
	//{
	//	MyRigidBody* pRB = l_Entity_List[i].GetRigidBody();
	//	vector3 v3Min = pRB->GetMinGlobal();
	//	vector3 v3Max = pRB->GetMaxGlobal();
	//	v3MaxMin_list.push_back(v3Min);
	//	v3MaxMin_list.push_back(v3Max);
	//	/*vector3 v3Position = pRB->GetCenterGlobal();

	//	if (v3Position.x < 0.0f)
	//	{
	//	if (v3Position.x < -17.0f)
	//	m_pEntityMngr->AddDimension(-1, 1);
	//	else
	//	m_pEntityMngr->AddDimension(-1, 2);
	//	}
	//	else if (v3Position.x > 0.0f)
	//	{
	//	if (v3Position.x > 17.0f)
	//	m_pEntityMngr->AddDimension(-1, 3);
	//	else
	//	m_pEntityMngr->AddDimension(-1, 4);
	//	}*/
	//	for (size_t i = 0; i < 8; i++)
	//	{
	//		m_pChild[i] = nullptr;
	//	}
	//	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	//}


	/////////////////////////////////////////////////
	//m_pEntityMngr = MyEntityManager::GetInstance();
	//m_pMeshMngr = MeshManager::GetInstance();

	//m_ID = octantCount;
	//octantCount += 1;
	////////////////////////////////////////////////
	m_nData = 0;
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
	
	//IsColliding();
	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
}

//Octree::Octree(uint maxLev, uint entCount, bool isRoot, vector3 newCent, vector3 newSize)
Octree::Octree(uint mlev) //ctor
{
	//Init();
	//if (isRoot) {
	//	root = this;
	//	maxLevelSub = maxLev;
	//	entityCount = entCount;
	//	maxCorner = minCorner = m_pEntityMngr->GetRigidBody()->GetCenterGlobal();
	//	currentLevelEntityCount = m_pEntityMngr->GetEntityCount();

	//	for (uint i = 0; i < currentLevelEntityCount; i++)
	//	{
	//		entityList.push_back(i);
	//		vector3 tempMin = m_pEntityMngr->GetRigidBody()->GetMinGlobal();
	//		vector3 tempMax = m_pEntityMngr->GetRigidBody()->GetMaxGlobal();

	//		//set min and max
	//		if (tempMin.x < minCorner.x) {
	//			minCorner.x = tempMin.x;
	//		}

	//		if (tempMax.x > maxCorner.x) {
	//			maxCorner.x = tempMax.x;
	//		}

	//		if (tempMin.y < minCorner.y) {
	//			minCorner.y = tempMin.y;
	//		}

	//		if (tempMax.y > maxCorner.y) {
	//			maxCorner.y = tempMax.y;
	//		}

	//		if (tempMin.z < minCorner.z) {
	//			minCorner.z = tempMin.z;
	//		}

	//		if (tempMax.z > maxCorner.z) {
	//			maxCorner.z = tempMax.z;
	//		}
	//	}


	//	octCenter = (minCorner + maxCorner) / 2.0f;
	//	octSize = maxCorner - minCorner;


	//	SubDivide();

	//	ConfigureEntityDimensions();

	//}
	//else {//leaves
	//	octCenter = newCent;
	//	octSize = newSize;
	//	maxCorner = newCent + newSize / 2.0f;
	//	minCorner = newCent - newSize / 2.0f;
	//}

	Init();
	m_nCount = 0;
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
	}

	m_pRigidBody = new MyRigidBody(v3MaxMin_list);
	m_pRigidBody->MakeCubic();
	m_iID = m_nCount;
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
	m_nCount++;
	m_iID = m_nCount;
	IsColliding();
}

void Octree::Release(void)
{
	if (this == root)
	{
		if (numChildren == 0)//is a Leaf
		{
			return;
		}
		else { 
			for (uint i = 0; i < numChildren; i++) //delete all children in root first
			{
				m_pChild[i]->Release();
				SafeDelete(m_pChild[i]);
			}
		}
	//	m_nCount = 0;
	}
}
void Octree::Display(uint index, bool indie)
{
	//m_pRigidBody->AddToRenderList();
	//m_pMeshMngr->AddWireCubeToRenderList(glm::scale(vector3(70)), C_YELLOW);



	//if (index >= octantCount) {
	//	DisplayHelper();
	//	return;
	//}
	//
	//	//childrenOfRoot[index]
	//DisplayHelper1();

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
				return;
			}
		}
		
		return;
	}

	m_pRigidBody->AddToRenderList();
	
	if (maxLevelSub > m_nLevel) {
		for (uint i = 0; i < 8; i++)
		{
			if (m_pChild[i])
				m_pChild[i]->Display(index, indie);
		}
	}
	
	
}
//
//void Octree::DisplayHelper() { //displays all cubes in the octree
//	if (numChildren == 0)
//	{
//		DisplayHelper1();
//	}
//	else {
//		for (uint i = 0; i < numChildren; ++i)
//		{
//			m_pChild[i]->DisplayHelper();
//		}
//	}
//}
//void Octree::DisplayHelper1() { //displays one cube
//	if (this == root) {
//		m_pMeshMngr->AddWireCubeToRenderList(/*glm::translate(IDENTITY_M4, octCenter) **/ glm::scale(IDENTITY_M4, octSize * 70.0f), C_YELLOW);
//	}
//	else {
//		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, octCenter) * glm::scale(IDENTITY_M4, octSize * 70.0f), C_YELLOW);
//	}
//	
//}
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
	MyEntity** l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = m_pEntityMngr->GetEntityCount();
	for (uint i = 0; i < iEntityCount; ++i)
	{
		MyRigidBody* pRB = l_Entity_List[i]->GetRigidBody();
		if (pRB->IsColliding(m_pRigidBody))
		{
			l_Entity_List[i]->AddDimension(m_iID);
		}
	}
}
void Octree::SubDivide(void)
{
//	//needs to know if its a root or not 
//	/*for (uint i = 0; i < 8; i++)
//	{
//		m_pChild[i] = new Octree(,,false,,);
//	}*/
//
//	if (currentLevel >= maxLevelSub) //|| !containsmorethanentitycount
//	{
//		root->childrenOfRoot.push_back(this);
//		leavesCount += 1;
//		return;
//	}
//
//	//if (numChildren == 0) //subdivide the last level
//	//{
//	//	return;
//	//}
//
//	m_pChild[0] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(-octSize.x / 4.0f, octSize.y / 4.0f, -octSize.z / 4.0f), octSize / 2.0f);
//	m_pChild[1] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(-octSize.x / 4.0f, octSize.y / 4.0f, octSize.z / 4.0f), octSize / 2.0f);
//	m_pChild[2] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(-octSize.x / 4.0f, -octSize.y / 4.0f, -octSize.z / 4.0f), octSize / 2.0f);
//	m_pChild[3] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(-octSize.x / 4.0f, -octSize.y / 4.0f, octSize.z / 4.0f), octSize / 2.0f);
//	m_pChild[4] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(octSize.x / 4.0f, -octSize.y / 4.0f, -octSize.z / 4.0f), octSize / 2.0f);
//	m_pChild[5] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(octSize.x / 4.0f, -octSize.y / 4.0f, octSize.z / 4.0f), octSize / 2.0f);
//	m_pChild[6] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(octSize.x / 4.0f, octSize.y / 4.0f, -octSize.z / 4.0f), octSize / 2.0f);
//	m_pChild[7] = new Octree(maxLevelSub, entityCount, false, octCenter + vector3(octSize.x / 4.0f, octSize.y / 4.0f, octSize.z / 4.0f), octSize / 2.0f);
//
//	numChildren = 8;
//
//	for (uint i = 0; i < numChildren; i++)
//	{
//		m_pChild[i]->m_pParent = this;
//		m_pChild[i]->currentLevel = currentLevel + 1;
//		m_pChild[i]->root = root;
//
//		for (uint j = 0; j < currentLevelEntityCount; j++)
//		{
//			if (m_pChild[i]->IsColliding(entityList[j]))
//			{
//				m_pChild[i]->entityList.push_back(entityList[j]);
//			}
//		}
//
//		m_pChild[i]->currentLevelEntityCount = m_pChild[i]->entityList.size();
//		m_pChild[i]->SubDivide();
//	}

	if (m_nLevel > maxLevelSub) //change to use maxlevel instead of magic number
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
		m_pChild[i]->SubDivide();
	}


}

//The big 3

Octree::Octree(Octree const& other)
{
	/*Init();

	currentLevel = other.currentLevel;
	octSize = other.octSize;
	octCenter = other.octCenter;
	minCorner = other.minCorner;
	maxCorner = other.maxCorner;
	m_pParent = other.m_pParent;
	Release();
	numChildren = other.numChildren;
	currentLevelEntityCount = other.currentLevelEntityCount;
	root = other.root;
	
	for (uint i = 0; i < numChildren; i++)
	{
		m_pChild[i] = new Octree(*other.m_pChild[i]);
	}

	for (uint i = 0; i < currentLevelEntityCount; i++)
	{
		entityList.push_back(other.entityList[i]);
	}

	if (this == root)
	{
		float cCount = other.childrenOfRoot.size();
		for (uint i = 0; i < cCount; i++)
		{
			childrenOfRoot.push_back(other.childrenOfRoot[i]);
		}
	}*/

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

