#pragma once
//#include <vector>
//#include "MyEntity.h"
#include "Simplex\Simplex.h"
#include "MyEntityManager.h"
//#include "Definitions.h"
using namespace std;
using namespace Simplex;
class Octree
{
	//vector<int> containedEntitiesByIndex;
	//vector3 maxCorner;
	//vector3 minCorner;
	//vector3 octCenter;
	//vector3 octSize;
	static uint octantCount; //number of octants in total
	static uint leavesCount; //number of leaves
	static uint maxLevelSub; //maximum level of subdivision
	static uint entityCount; //number of entities
	//uint currentLevel = 0; //number of levels deep
	//uint currentLevelEntityCount = 0; //number of entities
	//std::vector<uint> entityList; //entities in this octant
	//std::vector<Octree*> childrenOfRoot;
	//Octree* m_pParent = nullptr;
	//Octree* m_pChild[8];
	static Octree* root;
	//uint m_ID = 0; //current octant ID #
	uint numChildren = 0;

	////generic
	//int m_nData = 0; //Number of elements in the list of elements
	//std::vector<int> m_lData; //list of elements
	//MeshManager * m_pMeshMngr = nullptr;
	//MyEntityManager * m_pEntityMngr = nullptr;
	//MyRigidBody* m_pRigidBody = nullptr;
	int m_nData = 0; //Number of elements in the list of elements
	std::vector<int> m_lData; //list of elements
	MeshManager* m_pMeshMngr = nullptr; //Pointer to Mesh manager
	MyEntityManager* m_pEntityMngr = nullptr;
	MyRigidBody* m_pRigidBody = nullptr;
	uint m_iID = 0;
	static uint m_nCount;
	Octree* m_pParent = nullptr;
	Octree* m_pChild[8];
	uint m_nLevel = 0;

	

public:
	void Display(uint id, bool ind);
	/*void DisplayHelper(void);
	void DisplayHelper1(void);*/
	void IsColliding(uint rbi);
	void SubDivide(void);
	//void Octree(vector3 m_v3Size);
	/*
	Usage: Constructor
	Arguments: ---
	Output: class object instance
	*/
	//Octree(uint maxLev, uint entCount, bool isRoot, vector3 newCent, vector3 newSize);//root ctor
	Octree::Octree(uint mlev);
	Octree::Octree(vector3 a_v3Center, float a_fSize);
	/*
	Usage: Copy Constructor
	Arguments: class object to copy
	Output: class object instance
	*/
	Octree(Octree const& other);
	/*
	Usage: Copy Assignment Operator
	Arguments: class object to copy
	Output: ---
	*/
	Octree& operator=(Octree const& other);
	/*
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~Octree(void);

	void ConfigureEntityDimensions(void);

	/*
	Usage: Changes object contents for other object's
	Arguments: other -> object to swap content from
	Output: ---
	*/
	void Swap(Octree& other);

	/*
	Usage: Gets data member
	Arguments: ---
	Output: data
	*/
	int GetData(void);
	/*
	Usage: Sets data member
	Arguments: int a_nData = 1 -> data to set
	Output: ---
	*/
	void SetData(int a_nData = 1);
	/* Property */
	//__declspec(property(get = GetData, put = SetData)) int Data;

	/*
	Usage: adds data on vector
	Arguments: int a_nData -> data to add
	Output: ---
	*/
	void SetDataOnVector(int a_nData);
	uint GetOctantCount(void);
	/*
	Usage: gets data on vector at the specified entry
	Arguments: int a_nIndex -> entry index
	Output: entry's content
	*/
	int& GetDataOnVector(int a_nIndex);

private:
	/*
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);
	/*
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(void);
};//class

