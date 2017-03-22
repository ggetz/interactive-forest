
#include "Ground.h"

Ground::Ground() : Mesh()
{
    build();
}

void Ground::build()
{
    _verts.push_back(vec4(0, -1, 0, 1));
    _verts.push_back(vec4(1, -1, 1, 1));
    _verts.push_back(vec4(1, 0, 0, 1));
    _verts.push_back(vec4(0, 0, 0, 1));
    
    createFace(0, 1, 2, 3);
    
    calcNormals();
}

void Ground::createFace(int a, int b, int c, int d)
{
    // first triangle
    _faces.push_back(a);
    _faces.push_back(b);
    _faces.push_back(c);
    
    _uvs.push_back(vec2(0, 0));
    _uvs.push_back(vec2(1, 0));
    _uvs.push_back(vec2(1, 1));
    
    // second triangle
    _faces.push_back(c);
    _faces.push_back(d);
    _faces.push_back(a);
    
    _uvs.push_back(vec2(0, 0));
    _uvs.push_back(vec2(1, 1));
    _uvs.push_back(vec2(0, 1));
}

void Ground::calcNormals()
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
