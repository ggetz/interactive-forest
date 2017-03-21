#pragma once

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
		seed = 10;
		segments = 6;
		levels = 5;
		vMultiplier = 0.2;
		twigScale = 2.0;
		initialBranchLength = 0.85;
		lengthFalloffFactor = 0.85;
		lengthFalloffPower = 1.0;
		clumpMax = 0.8;
		clumpMin = 0.5;
		branchFactor = 2.0;
		dropAmount = 0.0;
		growAmount = 0.0;
		sweepAmount = 0.0;
		maxRadius = 0.25;
		climbRate = 1.5;
		trunkKink = 0.0;
		treeSteps = 2;
		taperRate = 0.95;
		radiusFalloffRate = 0.6;
		twistRate = 13;
		trunkLength = 2.4;
	}

	// Random seed used to create a unique tree
	int seed;

	// Number of segemnts in tree
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
};