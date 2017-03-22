#include "Tree.h"
#include <math.h>

Tree::Tree()
{
	_properties = TreeProperties();
	build();
}

Tree::Tree(TreeProperties properties)
{
	_properties = properties;
	build();
}

void Tree::build()
{
	_root = new Branch(vec3(0, _properties.trunkLength, 0));
	_root->length = _properties.initialBranchLength;

	_verts = vector<vec4>();
	_faces = vector<unsigned int>();
	_normals = vector<vec3>();
	_uvs = vector<vec2>();

	_vertsTwigs = vector<vec4>();
	_facesTwigs = vector<unsigned int>();
	_normalsTwigs = vector<vec3>();
	_uvsTwigs = vector<vec2>();

	_root->split(_properties.levels, _properties.treeSteps, _properties, 1, 1);
	createForks(_root, _properties.maxRadius);
	createTwigs(_root);
	doFaces(_root);
	calcNormals();
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

		_normals.push_back(total);
	}
}

void Tree::doFaces(Branch* branch)
{
	int segments = _properties.segments;

	if (branch->parent == NULL)
	{
		for (int i = 0; i < _verts.size(); ++i)
		{
			_uvs.push_back(vec2(0, 0));
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

		int segOffset = (int) roundf(angle / M_PI / 2 * segments);
		for (int i = 0; i < segments; ++i)
		{
			int v1 = branch->ring0[i];
			int v2 = branch->root[(i + segOffset + 1) % segments];
			int v3 = branch->root[(i + segOffset) % segments];
			int v4 = branch->ring0[(i + 1) % segments];

			_faces.push_back(v1);
			_faces.push_back(v4);
			_faces.push_back(v3);

			_faces.push_back(v4);
			_faces.push_back(v2);
			_faces.push_back(v3);

			_uvs[(i + segOffset) % segments] = vec2(abs(i / (float) segments - 0.5) * 2, 0);
			float len = length(_verts[branch->ring0[i]] - _verts[branch->root[(i + segOffset) % segments]]) * _properties.vMultiplier;
			_uvs[branch->ring0[i]] = vec2(abs(i / (float) segments - 0.5) * 2, len);
			_uvs[branch->ring2[i]] = vec2(abs(i / (float) segments - 0.5) * 2, len);
		}
	}

	if (branch->child1->ring0.size() > 0)
	{
		int segOffset0 = -1;
		int segOffset1 = -1;
		int match0, match1;

		vec4 v1 = normalize(_verts[branch->ring1[0]] - branch->head);
		vec4 v2 = normalize(_verts[branch->ring2[0]] - branch->head);

		v1 = v1 * normalize(branch->child1->head - branch->head);
		v2 = v2 * normalize(branch->child2->head - branch->head);

		for (int i = 0; i < segments; i++) 
		{
			vec4 d = normalize(_verts[branch->child1->ring0[i]] - branch->child1->head);
			float l = dot(d, v1);
			if (segOffset0 == -1 || l > match0) 
			{
				match0 = l;
				segOffset0 = segments - i;
			}
			d = normalize(_verts[branch->child2->ring0[i]] - branch->child2->head);
			l = dot(d, v2);
			if (segOffset1 == -1 || l > match1) 
			{
				match1 = l;
				segOffset1 = segments - i;
			}
		}

		float UVScale = _properties.maxRadius / branch->radius;


		for (int i = 0; i < segments; i++) 
		{
			int f1 = branch->child1->ring0[i];
			int f2 = branch->ring1[(i + segOffset0 + 1) % segments];
			int f3 = branch->ring1[(i + segOffset0) % segments];
			int f4 = branch->child1->ring0[(i + 1) % segments];
			
			_faces.push_back(f1); 
			_faces.push_back(f4);
			_faces.push_back(f3);

			_faces.push_back(f4);
			_faces.push_back(f2);
			_faces.push_back(f3);

			f1 = branch->child2->ring0[i];
			f2 = branch->ring2[(i + segOffset1 + 1) % segments];
			f3 = branch->ring2[(i + segOffset1) % segments];
			f4 = branch->child2->ring0[(i + 1) % segments];

			_faces.push_back(f1);
			_faces.push_back(f2);
			_faces.push_back(f3);

			_faces.push_back(f1);
			_faces.push_back(f4);
			_faces.push_back(f2);

			float len1 = length(_verts[branch->child1->ring0[i]] - _verts[branch->ring1[(i + segOffset0) % segments]]) * UVScale;
			vec2 uv1 = _uvs[branch->ring1[(i + segOffset0 - 1) % segments]];

			_uvs[branch->child1->ring0[i]] = vec2(uv1[0], uv1[1] + len1 * _properties.vMultiplier);
			_uvs[branch->child1->ring2[i]] = vec2(uv1[0], uv1[1] + len1 * _properties.vMultiplier);

			float len2 = length(_verts[branch->child2->ring0[i]] - _verts[branch->ring2[(i + segOffset1) % segments]]) * UVScale;
			vec2 uv2 = _uvs[branch->ring2[(i + segOffset1 - 1) % segments]];

			_uvs[branch->child2->ring0[i]] = vec2(uv2.x, uv2.y + len2 * _properties.vMultiplier);
			_uvs[branch->child2->ring2[i]] = vec2(uv2.x, uv2.y + len2 * _properties.vMultiplier);
		}

		doFaces(branch->child1);
		doFaces(branch->child2);
	}
	else
	{
		for (int i = 0; i < segments; ++i)
		{
			_faces.push_back(branch->child1->end);
			_faces.push_back(branch->ring1[(i + 1) % segments]);
			_faces.push_back(branch->ring1[i]);

			_faces.push_back(branch->child2->end);
			_faces.push_back(branch->ring2[(i + 1) % segments]);
			_faces.push_back(branch->ring2[i]);

			float len = length(_verts[branch->child1->end] - _verts[branch->ring1[i]]);
			_uvs[branch->child1->end] = vec2(abs(i / (float) segments - 1 - 0.5) * 2, len * _properties.vMultiplier);
			len = length(_verts[branch->child2->end] - _verts[branch->ring2[i]]);
			_uvs[branch->child2->end] = vec2(abs(i / (float)segments - 0.5) * 2, len * _properties.vMultiplier);
		}
	}
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
		vec4 diff =_vertsTwigs[index] - _vertsTwigs[index + 2];
		a = vec3(diff.x, diff.y, diff.z);
		diff = _vertsTwigs[index + 1] - _vertsTwigs[index + 2];
		b = vec3(diff.x, diff.y, diff.z);
		normal = normalize(cross(a, b));

		for (int i = 0; i < 4; ++i)
		{
			_normalsTwigs.push_back(normal);
		}

		diff = _vertsTwigs[index + 6] - _vertsTwigs[index + 5];
		a = vec3(diff.x, diff.y, diff.z);
		diff = _vertsTwigs[index + 7] - _vertsTwigs[index + 5];
		b = vec3(diff.x, diff.y, diff.z);
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
		if (branch->parent != NULL)
		{
			axis = normalize(branch->head - branch->parent->head);
		}
		else
		{
			axis = normalize(branch->head);
		}

		vec3 axis1 = normalize(branch->head - branch->child1->head);
		vec3 axis2 = normalize(branch->head - branch->child2->head);
		vec3 tangent = normalize(cross(axis1, axis2));
		branch->tangent = tangent;

		vec3 axis3 = normalize(cross(tangent, normalize(-axis1 - axis2)));
		vec3 dir = vec3(axis1.x, 0, axis2.z);
		vec3 centerLoc = branch->head + dir * -_properties.maxRadius / 2;

		float scale = _properties.radiusFalloffRate;

		if (branch->child1->trunk || branch->trunk)
		{
			scale = 1 / _properties.taperRate;
		}

		int linch0 = _verts.size();
		branch->ring0.push_back(linch0);
		branch->ring2.push_back(linch0);
		_verts.push_back(centerLoc + tangent * -radius * scale);

		int start = _verts.size() - 1;
		vec3 d1 = vecAxisAngle(tangent, axis2, 1.57);
		vec3 d2 = normalize(cross(tangent, axis));
		float s = 1 / dot(d1, d2);

		int segments = _properties.segments;
		for (int i = segments / 2 + 1; i < segments; i++) {
			vec3 vec = vecAxisAngle(tangent, axis1, segmentAngle*i);
			branch->ring0.push_back(_verts.size());
			branch->ring2.push_back(_verts.size());
			_verts.push_back(centerLoc + vec * radius * scale);
		}

		int linch1 = _verts.size();
		branch->ring0.push_back(linch1);
		branch->ring1.push_back(linch1);
		_verts.push_back(centerLoc + tangent * -radius * scale);
		for (int i = segments / 2 + 1; i<segments; i++) {
			vec3 vec = vecAxisAngle(tangent, axis1, segmentAngle * i);
			branch->ring0.push_back(_verts.size());
			branch->ring1.push_back(_verts.size());
			_verts.push_back(centerLoc + vec * radius * scale);
		}

		branch->ring1.push_back(linch0);
		branch->ring2.push_back(linch1);
		start = _verts.size() - 1;
		for (int i = 1; i < segments / 2; i++) {
			vec3 vec = vecAxisAngle(tangent, axis3, segmentAngle*i);
			branch->ring1.push_back(start + i);
			branch->ring2.push_back(start + (segments / 2 - i));
			vec3 v = vec * radius * scale;
			_verts.push_back(centerLoc + vec * radius * scale);
		}

		float radius0 = 1 * radius * _properties.radiusFalloffRate;
		float radius1 = 1 * radius * _properties.radiusFalloffRate;
		if (branch->child1->trunk)
		{
			radius0 = radius * _properties.taperRate;
		}
		createForks(branch->child1, radius0);
		createForks(branch->child2, radius1);
	}
	else
	{
		//add points for the ends of braches
		branch->end = _verts.size();
		_verts.push_back(branch->head);
	}
}