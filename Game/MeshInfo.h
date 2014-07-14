#pragma once

struct Float3
{
	Float3(float _x, float _y, float _z)
	:x(_x), y(_y), z(_z){}

	float x, y, z;
};

struct Float2
{
	Float2(float _x, float _y)
	:x(_x), y(_y){}

	float x, y;
};

struct MeshInfo
{
	Float3* vertices;
	Float3* normals;
	Float2* texCoords;
};