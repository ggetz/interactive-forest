#pragma once
#include <math.h>

/**
	Options object used to create a Tree object.
*/
class TreeProperties
{
public:

	/**
		Creates a TreeProperties object with default values.
	*/
	TreeProperties () 
	{
		// default values
		seed = rseed = 262;
		segments = 6;
		levels = 5;
		vMultiplier = 2.36;
		twigScale = 2.0;
		initialBranchLength = 0.53;
		lengthFalloffFactor = 0.85;
		lengthFalloffPower = 0.99;
		clumpMax = 0.6;
		clumpMin = 0.4;
		branchFactor = 2.5;
		dropAmount = -0.2;
		growAmount = 0.03;
		sweepAmount = 0.01;
		maxRadius = 0.139;
		climbRate = 0.4;
		trunkKink = 0.093;
		treeSteps = 5;
		taperRate = 0.95;
		radiusFalloffRate = 0.6;
		twistRate = 0.1;
		trunkLength = 2.4;
	}

	// Number of segemnts around tree branches
	int segments;

	// Number of branch generation levels
	int levels;

	float vMultiplier;

	// Scale to determine size of twig mesh.
	float twigScale;

	float initialBranchLength;
	float lengthFalloffFactor;
	float lengthFalloffPower;

	float clumpMax;
	float clumpMin;

	float branchFactor;
	float dropAmount;
	float growAmount;

	// How far branches are swept to the side
	float sweepAmount;

	// Max branch radius
	float maxRadius;
	float climbRate;
	float trunkKink;
	int treeSteps;

	// How quickly the branch radius tapers
	float taperRate;

	float radiusFalloffRate;
	float twistRate;

	// Length of trunk before branches begin
	float trunkLength;
	
	// Random seed used to generate tree
	void setSeed(int value)
	{
		seed = rseed = value;
	}

	int seed;
	int rseed;

	float random (int a) 
	{
		return abs(cos(a + a * a));
	}

};