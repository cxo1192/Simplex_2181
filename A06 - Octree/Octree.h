#pragma once
#include <vector>
#include "MyEntity.h"
//#include "Definitions.h"
using namespace std;
using namespace Simplex;
class Octree
{
public:
	vector<int> containedEntitiesByIndex;
	vector3 maxCorner;
	vector3 minCorner;
	uint entityCount = 0;

	Octree();
	virtual ~Octree();
};

