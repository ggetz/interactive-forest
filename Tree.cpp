#include "Tree.h"
#include <math.h>


Tree::Tree()
{

}

Tree::Tree(TreeProperties properties)
{
	_properties = properties;
	_root = new Branch();
	_root->length = properties.initialBranchLength;

	_verts = vector<vec4>();
	_faces = vector<int>();
	_normals = vector<vec3>();
	_uvs = vector<vec2>();

	_vertsTwigs = vector<vec4>();
	_facesTwigs = vector<int>();
	_normalsTwigs = vector<vec3>();
	_uvsTwigs = vector<vec2>();
	
	//_root->split();
	//createForks();
	//createTwigs();
	//doFaces(_root);
	//calcNormals();
}


Tree::~Tree()
{
	Tree(TreeProperties());
}


void Tree::calcNormals()
{
	vector<vector<vec3>> allNormals = vector<vector<vec3>>();
	
	for (int i = 0; i < _verts.size(); ++i)
	{
		allNormals.push_back(vector<vec3>());
	}

	// Calculate normal for each face
	for (int i = 0; i < _faces.size(); i += 3)
	{
		vec4 a = _verts[_faces[i]];
		vec4 b = _verts[_faces[i + 1]];
		vec4 c = _verts[_faces[i + 2]];
		vec3 norm = normalize(cross(b - c, b - a));

		allNormals[_faces[i]].push_back(norm);
		allNormals[_faces[i + 1]].push_back(norm);
		allNormals[_faces[i + 2]].push_back(norm);
	}

	// Averages all the normals found for this vert
	for (int i = 0; i < _verts.size(); ++i)
	{
		vec3 total = vec3();
		int length = allNormals[i].size();
		for (int j = 0; j < length; ++j)
		{
			total += allNormals[i][j] / length;
		}

		_normals[i] = total;
	}
}

void Tree::doFaces(Branch* branch)
{
	if (branch->parent == NULL)
	{
		for (int i = 0; i < _verts.size(); ++i)
		{
			_uvs.push_back(vec2());
		}

		vec3 a = branch->child1->head - branch->head;
		vec3 b = branch->child2->head - branch->head;
		vec3 tangent = normalize(cross(a, b));

		vec3 normal = normalize(branch->head);
		vec3 left = vec3(-1, 0, 0);
		float angle = acos(dot(tangent, left));

		if (dot(cross(left, tangent), normal) > 0)
		{
			angle = 2 * M_PI - angle;
		}

		float segOffset = roundf(angle / M_PI / 2 * _properties.segments);
		for (int i = 0; i < _properties.segments; ++i)
		{
			// TODO
		}
	}

	// TODO
}

void Tree::createTwigs(Branch* branch)
{
	if (branch->child1 == NULL)
	{
		vec3 a = branch->parent->child1->head - branch->parent->head;
		vec3 b = branch->parent->child2->head - branch->parent->head;
		vec3 tangent = normalize(cross(a, b));

		vec3 binormal = normalize(branch->head - branch->parent->head);

		vec3 normal = cross(tangent, binormal);

		int index = _vertsTwigs.size();

		// verts
		vec4 twig = branch->head + tangent * _properties.twigScale + binormal * (_properties.twigScale * 2 - branch->length);
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);

		twig = branch->head + tangent * -_properties.twigScale + binormal * (_properties.twigScale * 2 - branch->length);
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);

		twig = branch->head + tangent * -_properties.twigScale + binormal * -branch->length;
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);

		twig = branch->head + tangent * _properties.twigScale + binormal * -branch->length;
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);


		twig = branch->head + tangent * _properties.twigScale + binormal * -branch->length;
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);

		twig = branch->head + tangent * -_properties.twigScale + binormal * -branch->length;
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);

		twig = branch->head + tangent * -_properties.twigScale + binormal * (_properties.twigScale * 2 - branch->length);
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);

		twig = branch->head + tangent * _properties.twigScale + binormal * (_properties.twigScale * 2 - branch->length);
		twig.z = 1.0;
		_vertsTwigs.push_back(twig);

		// faces
		_facesTwigs.push_back(index);
		_facesTwigs.push_back(index + 1);
		_facesTwigs.push_back(index + 2);

		_facesTwigs.push_back(index + 3);
		_facesTwigs.push_back(index);
		_facesTwigs.push_back(index + 2);

		_facesTwigs.push_back(index + 5);
		_facesTwigs.push_back(index + 6);
		_facesTwigs.push_back(index + 7);

		_facesTwigs.push_back(index + 5);
		_facesTwigs.push_back(index + 7);
		_facesTwigs.push_back(index + 4);

		// normals
		a = _vertsTwigs[index] - _vertsTwigs[index + 2];
		b = _vertsTwigs[index + 1] - _vertsTwigs[index + 2];
		normal = normalize(cross(a, b));

		for (int i = 0; i < 4; ++i)
		{
			_normalsTwigs.push_back(normal);
		}

		a = _vertsTwigs[index + 6] - _vertsTwigs[index + 5];
		b = _vertsTwigs[index + 7] - _vertsTwigs[index + 5];
		normal = normalize(cross(a, b));

		for (int i = 0; i < 4; ++i)
		{
			_normalsTwigs.push_back(normal);
		}

		// uvs
		_uvsTwigs.push_back(vec2(0, 1));
		_uvsTwigs.push_back(vec2(1, 1));
		_uvsTwigs.push_back(vec2(1, 0));
		_uvsTwigs.push_back(vec2(0, 0));

		_uvsTwigs.push_back(vec2(0, 1));
		_uvsTwigs.push_back(vec2(1, 1));
		_uvsTwigs.push_back(vec2(1, 0));
		_uvsTwigs.push_back(vec2(0, 0));
	}
	else
	{
		createTwigs(branch->child1);
		createTwigs(branch->child2);
	}
}

vec3 vecAxisAngle(vec3 vec, vec3 axis, float angle)
{
	float cosr = cos(angle);
	float sinr = sin(angle);
	return vec * cosr + cross(axis, vec) * sinr + axis * (dot(axis, vec) * (1 - cosr));
}

void Tree::createForks(Branch* branch, float radius)
{
	branch->radius = radius;

	if (radius > branch->length)
	{
		radius = branch->length;
	}

	float segmentAngle = M_PI * 2 / _properties.segments;

	if (branch->parent == NULL)
	{
		// Create vertex locations for root of tree
		branch->root = vector<int>();
		vec3 axis = vec3(0, 1, 0);
		vec3 left = vec3(-1, 0, 0);
		for (int i = 0; i < _properties.segments; ++i)
		{
			vec3 vec = vecAxisAngle(left, axis, -segmentAngle*i);
			branch->root.push_back(_verts.size());
			_verts.push_back(vec * (radius / _properties.radiusFalloffRate));
		}
	}

	if (branch->child1 != NULL)
	{
		vec3 axis;
		if (branch->parent)
		{
			axis = normalize(branch->head - branch->parent->head);
		}
		else
		{
			axis = normalize(branch->head);
		}

		vec3 a = normalize(branch->head - branch->child1->head);
		vec3 b = normalize(branch->head - branch->child2->head);
		vec3 tangent = normalize(cross(a, b));
		branch->tangent = tangent;

		vec3 c = normalize(cross(tangent, normalize(-a - b)));
		vec3 dir = vec3(a.x, 0, b.z);
		vec3 centerLoc = branch->head + dir * -_properties.maxRadius / 2;

		float scale = _properties.radiusFalloffRate;
	}
}