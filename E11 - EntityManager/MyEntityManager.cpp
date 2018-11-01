#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
}
void MyEntityManager::Release(void)
{
	for (size_t i = 0; i < m_uEntityCount; i++)
	{
		SafeDelete(m_entityList[i]);
	}

	SafeDelete(m_pInstance);
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	SafeDelete(m_pInstance);
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (size_t i = 0; i < m_uEntityCount; i++)
	{
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			return i;
	}
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount && a_uIndex > -1)
		return m_entityList[a_uIndex]->GetModel();
	
	return nullptr;
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	int index = GetEntityIndex(a_sUniqueID);
	
	if(index > -1)
		return m_entityList[index]->GetModel();
	
	
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount && a_uIndex > -1)
		return m_entityList[a_uIndex]->GetRigidBody();

	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	int index = GetEntityIndex(a_sUniqueID);

	if (index > -1)
		return m_entityList[index]->GetRigidBody();


	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount && a_uIndex > -1)
		return m_entityList[a_uIndex]->GetModelMatrix();


	return IDENTITY_M4;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	int index = GetEntityIndex(a_sUniqueID);

	if (index > -1)
		return m_entityList[index]->GetModelMatrix();


	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	int index = GetEntityIndex(a_sUniqueID);

	if (index > -1)
		m_entityList[index]->SetModelMatrix(a_m4ToWorld);

}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount && a_uIndex > -1)
		m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);

}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other) { SafeDelete(m_pInstance); m_pInstance = other.GetInstance();}
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { SafeDelete(m_pInstance); m_pInstance = other.GetInstance(); return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	m_uEntityCount = m_entityList.size();
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	int index = GetEntityIndex(a_sUniqueID);

	MyEntity * e = NULL;
	if (index != -1) {
		e = new MyEntity(a_sFileName, a_sUniqueID);
	}
	else {
		e = new MyEntity(a_sFileName, "");
	}

	m_entityList.push_back(e);
		



}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount && a_uIndex > -1) {
		SafeDelete(m_entityList[a_uIndex]);
		m_entityList.erase(m_entityList.begin() + a_uIndex);
	}


}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int index = GetEntityIndex(a_sUniqueID);

	if (index > -1) {
		SafeDelete(m_entityList[index]);
		m_entityList.erase(m_entityList.begin() + index);
	}
		
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	String id = NULL;

	if (a_uIndex < m_uEntityCount && a_uIndex > -1) {
		id = m_entityList[a_uIndex]->GetUniqueID();
		return id;
	}

	return "";
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	if (a_uIndex < m_uEntityCount && a_uIndex > -1) {
		MyEntity * e = m_entityList[a_uIndex];
		return e;
	}
	return nullptr;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	if (a_uIndex < m_uEntityCount && a_uIndex > -1) {
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	int index = GetEntityIndex(a_sUniqueID);

	if (index > -1) {
		m_entityList[index]->AddToRenderList(a_bRigidBody);
	}
}