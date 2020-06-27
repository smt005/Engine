#pragma once

class Shape;

class Mesh
{
	friend Shape;

private:
	unsigned short int _countVertex = 0;
	float* _aVertex = nullptr;
	float* _aNormal = nullptr;
	float* _aTexCoord = nullptr;

	unsigned short int _countIndex = 0;
	unsigned short* _aIndex = nullptr;

	bool _hasVBO = false;
	unsigned int _buffer[4];

public:
	inline const unsigned short int countVertex()	{ return _countVertex; }
	inline const unsigned short int countIndex()	{ return _countIndex; }

	inline const float* const vertexes()			{ return _aVertex; }
	inline const float* const normals()				{ return _aNormal; }
	inline const float* const texCoords()			{ return _aTexCoord; }
	inline const unsigned short* const indexes()	{ return _aIndex; }

	inline bool hasVBO()						{ return _hasVBO; }
	inline const unsigned int* const buffers()	{ return _buffer; }

	inline unsigned int bufferVertexes()	{ return _buffer[0]; }
	inline unsigned int bufferNormals()		{ return _buffer[1]; }
	inline unsigned int bufferTexCoords()	{ return _buffer[2]; }
	inline unsigned int bufferIndexes()		{ return _buffer[3]; }

	void copy(const Mesh& mesh);

	void setData(	float* const aVertex,
					float* const aNormal,
					float* const aTexCoord,
					const unsigned short int countVertex,
					unsigned short* const aIndex,
					const unsigned short int countIndex);

public:
	virtual ~Mesh();
	bool initVBO();
};
