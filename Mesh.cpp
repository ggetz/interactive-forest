#include "Mesh.h"
#include <iostream>


Mesh::Mesh()
{
	Mesh(vector<vec4>(),
		vector<unsigned int>(),
		vector<vec3>(),
		vector<vec2>());
}


Mesh::Mesh(vector<vec4> verts, vector<unsigned int> faces, vector<vec3> normals, vector<vec2> uvs)
{
	_verts = verts;
	_faces = faces;
	_normals = normals;
	_uvs = uvs;

	// set up model matrix, position at object origin
	_modelmatrix = mat4();

	// create a default material for the object
	_material = Material();
}


Mesh::~Mesh()
{
	_verts.clear();
	_faces.clear();
	_normals.clear();
	_uvs.clear();

	glDeleteBuffers(2, _buffers);
	glDeleteVertexArrays(1, &_vao);
}


void Mesh::init()
{
	_program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	_shadowProgram = InitShader("vshader02_v150.glsl", "fshader02_v150.glsl");

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(2, _buffers);

	// Bind buffer data
	glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vec4) * _verts.size() + sizeof(vec3) * _normals.size() + sizeof(vec2) * _uvs.size(),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 
		sizeof(vec4) * _verts.size(), 
		&_verts.front());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _verts.size(),
		sizeof(vec3) * _normals.size(),
		&_normals.front());
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4) * _verts.size() + sizeof(vec3) * _normals.size(),
		sizeof(vec2) * _uvs.size(),
		&_uvs.front());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _faces.size(), &_faces.front(), GL_STATIC_DRAW);

	// Bind index arrays
	glUseProgram(_program);
	GLuint vPosition = glGetAttribLocation(_program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	GLuint vNormal = glGetAttribLocation(_program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * _verts.size()));
	GLuint vUV = glGetAttribLocation(_program, "vUV");
	glEnableVertexAttribArray(vUV);
	glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4) * _verts.size() + sizeof(vec3) * _normals.size()));

	// Bind texture
	glGenTextures(1, &_texture);
	GLubyte* image0 = ppmRead(_material.texturePath, &_material.textureSize, &_material.textureSize);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _material.textureSize, _material.textureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, image0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Shadow buffer
	glUseProgram(_shadowProgram);
	GLuint shvPosition = glGetAttribLocation(_shadowProgram, "vPosition");
	glEnableVertexAttribArray(shvPosition);
	glVertexAttribPointer(shvPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}


void Mesh::draw(Camera camera, DirectionalLight light, GLuint shadowMap)
{
	glBindVertexArray(_vao);
	glUseProgram(_program);

	// material properties
	glUniform4fv(
		glGetUniformLocation(_program, "ambient"), 1,
		_material.ambient);
	glUniform4fv(
		glGetUniformLocation(_program, "diffuse"), 1,
		_material.diffuse);
	glUniform4fv(
		glGetUniformLocation(_program, "spec"), 1,
		_material.specular);
	glUniform1f(
		glGetUniformLocation(_program, "shininess"),
		_material.shininess);

	// sun properties
	glUniform3fv(
		glGetUniformLocation(_program, "light_direction"), 1,
		light.direction);
	glUniform4fv(
		glGetUniformLocation(_program, "light_ambient"), 1,
		light.ambient);
	glUniform4fv(
		glGetUniformLocation(_program, "light_diffuse"), 1,
		light.diffuse);
	glUniform4fv(
		glGetUniformLocation(_program, "light_specular"), 1,
		light.specular);

	// viewer properties
	glUniform4fv(
		glGetUniformLocation(_program, "eye"), 1,
		camera.getEye());
	glUniformMatrix4fv(
		glGetUniformLocation(_program, "model_matrix"),
		1, GL_TRUE, _modelmatrix);
	glUniformMatrix4fv(
		glGetUniformLocation(_program, "view_matrix"), 
		1, GL_TRUE, camera.getViewMatrix());
	glUniformMatrix4fv(
		glGetUniformLocation(_program, "proj_matrix"),
		1, GL_TRUE, camera.getProjMatrix());

	// fog properties
	glUniform1f(
		glGetUniformLocation(_program, "fog_start"),
		light.fogStart);
	glUniform1f(
		glGetUniformLocation(_program, "fog_end"),
		light.fogEnd);
	glUniform4fv(
		glGetUniformLocation(_program, "fog_color"), 1,
		light.fogColor);

	// texture properties
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(glGetUniformLocation(_program, "textureID"), 0);

	// shadow properties
	mat4 light_camera_matrix = LookAt(light.position, light.position - light.direction, vec4(0, 1, 0, 0));
	mat4 light_proj_matrix = Ortho(-10, 10, -10, 10, 0, 20);
	glUniformMatrix4fv(
		glGetUniformLocation(_program, "shadow_matrix"),
		1, GL_TRUE, (light_proj_matrix * light_camera_matrix * _modelmatrix));
	glUniform4fv(
		glGetUniformLocation(_program, "shadow_color"), 1,
		light.shadow);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(_program, "shadow_map"), 1);

	// draw
	glEnable(GL_DEPTH_TEST);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[1]);
	glDrawElements(GL_TRIANGLES, _faces.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}

void Mesh::drawShadowMap(DirectionalLight light)
{
	glBindVertexArray(_vao);
	glUseProgram(_shadowProgram);

	mat4 light_camera_matrix = LookAt(light.position, light.position - light.direction, vec4(0, 1, 0, 0));
	mat4 light_proj_matrix = Ortho(-10, 10, -10, 10, 0, 20);
	glUniformMatrix4fv(
		glGetUniformLocation(_shadowProgram, "MVP_matrix"),
		1, GL_TRUE, light_proj_matrix * light_camera_matrix * _modelmatrix);

	glEnable(GL_DEPTH_TEST);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers[1]);
	glDrawElements(GL_TRIANGLES, _faces.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
}


unsigned char* Mesh::ppmRead(char* filename, int* width, int* height)
{
	FILE* fp = NULL;
	int i, w, h, d;
	unsigned char* image;
	char head[70];		// max line <= 70 in PPM (per spec).
#ifdef WIN32
	fopen_s(&fp, filename, "rb");
#else
	fp = fopen(filename, "rb");
#endif
	if (fp == NULL) {
		perror(filename);
		return NULL;
	}

	// Grab first two chars of the file and make sure that it has the
	// correct magic cookie for a raw PPM file.
	fgets(head, 70, fp);
	if (strncmp(head, "P6", 2)) {
		fprintf(stderr, "%s: Not a raw PPM file\n", filename);
		return NULL;
	}

	// Grab the three elements in the header (width, height, maxval).
	i = 0;
	while (i < 3) {
		fgets(head, 70, fp);
		if (head[0] == '#')		// skip comments.
			continue;
		if (i == 0) {
#ifdef WIN32
			i += sscanf_s(head, "%d %d %d", &w, &h, &d);
#else
			i += sscanf(head, "%d %d %d", &w, &h, &d);
#endif
		}
		else if (i == 1) {
#ifdef WIN32
			i += sscanf_s(head, "%d %d", &h, &d);
#else
			i += sscanf(head, "%d %d", &h, &d);
#endif
		}
		else if (i == 2) {
#ifdef WIN32
			i += sscanf_s(head, "%d", &d);
#else
			i += sscanf(head, "%d", &d);
#endif
		}
	}

	// Grab all the image data in one fell swoop.
	image = (unsigned char*)malloc(sizeof(unsigned char) * w * h * 3);
	fread(image, sizeof(unsigned char), w * h * 3, fp);
	fclose(fp);

	*width = w;
	*height = h;
	return image;

}
