#pragma once

struct MeshInfo;
class Loader
{
public:
	const MeshInfo* LoadObj(const wchar_t* filename) const;
};