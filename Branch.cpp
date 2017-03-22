#include "Branch.h"



Branch::Branch()
{
	head = vec3(0, 0, 0);

	root = vector<int>();
	ring0 = vector<int>();
	ring1 = vector<int>();
	ring2 = vector<int>();

	trunk = false;
}

Branch::Branch(vec3 h)
{
	head = h;

	root = vector<int>();
	ring0 = vector<int>();
	ring1 = vector<int>();
	ring2 = vector<int>();

	trunk = true;
}

Branch::Branch(vec3 h, Branch* p)
{
	head = h;
	parent = p;

	root = vector<int>();
	ring0 = vector<int>();
	ring1 = vector<int>();
	ring2 = vector<int>();

	trunk = false;
}

vec3 Branch::mirrorBranch(vec3 vec, vec3 norm, TreeProperties properties)
{
	vec3 v = cross(norm, cross(vec, norm));
	float s = properties.branchFactor * dot(v, vec);
	return vec3(vec.x - v.x * s, vec.y - v.y * s, vec.z - v.z * s);
}


void Branch::split(int level, int steps, TreeProperties properties, int l1, int l2)
{
	int rLevel = properties.levels - level;
	vec3 po;

	if (parent != NULL) 
	{
		po = parent->head;
	}
	else 
	{
		po = vec3(0, 1, 0);
		trunk = true;
	}

	vec3 so = head;
	vec3 dir = normalize(so - po);

	vec3 normal = cross(dir, vec3(dir.z, dir.x, dir.y));
	vec3 tangent = cross(dir, normal);
	float r = properties.random(rLevel * 10 + l1 * 5 + l2 + properties.seed);
	float r2 = properties.random(rLevel * 10 + l1 * 5 + l2 + 1 + properties.seed);
	float clumpmax = properties.clumpMax;
	float clumpmin = properties.clumpMin;

	vec3 adj = normal * r + tangent * (1 - r);
	if (r > 0.5) 
	{
		adj = -adj;
	}

	float clump = (clumpmax - clumpmin) * r + clumpmin;
	vec3 newdir = normalize(adj * (1 - clump) + dir * clump);


	vec3 newdir2 = mirrorBranch(newdir, dir, properties);
	if (r > 0.5) 
	{
		vec3 tmp = newdir;
		newdir = newdir2;
		newdir2 = tmp;
	}
	if (steps > 0) 
	{
		float angle = steps / properties.treeSteps * 2 * M_PI * properties.twistRate;
		newdir2 = normalize(vec3(sin(angle), r, cos(angle)));
	}

	float growAmount = level * level / properties.levels * properties.levels * properties.growAmount;
	float dropAmount = rLevel * properties.dropAmount;
	float sweepAmount = rLevel * properties.sweepAmount;
	newdir = normalize(newdir + vec3(sweepAmount, dropAmount + growAmount, 0));
	newdir2 = normalize(newdir2 + vec3(sweepAmount, dropAmount + growAmount, 0));

	vec3 head0 = so + newdir * length;
	vec3 head1 = so + newdir2 * length;

	child1 = new Branch(head0, this);
	child2 = new Branch(head1, this);
	child1->length = pow(length, properties.lengthFalloffPower) * properties.lengthFalloffFactor;
	child2->length = pow(length, properties.lengthFalloffPower) * properties.lengthFalloffFactor;
	
	if (level > 0) 
	{
		if (steps > 0) 
		{
			child1->head = head + vec3((r - 0.5) * 2 * properties.trunkKink, properties.climbRate, (r - 0.5) * 2 * properties.trunkKink);
			child1->trunk = true;
			child1->length = length * properties.taperRate;
			child1->split(level, steps - 1, properties, l1 + 1, l2);
		}
		else 
		{
			child1->split(level - 1, 0, properties, l1 + 1, l2);
		}

		child2->split(level - 1, 0, properties, l1, l2 + 1);
	}
}