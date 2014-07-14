#pragma once

struct MeshInfo;
class IMesh
{
public:
	IMesh(){}
	~IMesh(){}

	virtual void Initialize(const MeshInfo& info) = 0;
	virtual void Draw() = 0;
};