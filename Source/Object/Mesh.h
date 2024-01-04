#pragma once

class Shape;

class Mesh
{
	friend Shape;

private:
	unsigned int _countVertex = 0;
	float* _aVertex = nullptr;
	float* _aNormal = nullptr;
	float* _aTexCoord = nullptr;

	unsigned int _countIndex = 0;
	unsigned int* _aIndex = nullptr;

	bool _hasVBO = false;
public:
	unsigned int _VAO; // Vertex Array Object
	unsigned int _VBO;
	unsigned int _TBO;
	unsigned int _EBO;
	unsigned int _buffer[4];

public:
	inline const unsigned int countVertex()	{ return _countVertex; }
	inline const unsigned int countIndex() const { return _countIndex; }

	//inline const float* const vertexes()       const { return _aVertex; }
	//inline const float* const normals()        const { return _aNormal; }
	inline float* vertexes()       const { return _aVertex; }
	inline float* normals()        const { return _aNormal; }
	inline const float* const texCoords()      const { return _aTexCoord; }
	inline const unsigned int* const indexes() const { return _aIndex; }

	inline bool hasVBO()						{ return _hasVBO; }
	inline const unsigned int* const buffers()	{ return _buffer; }

	inline unsigned int bufferVertexes()	{ return _buffer[0]; }
	inline unsigned int bufferTexCoords()	{ return _buffer[1]; }
	inline unsigned int bufferNormals()		{ return _buffer[2]; }
	inline unsigned int bufferIndexes()		{ return _buffer[3]; }

	void copy(const Mesh& mesh);

	void setData(	float* const aVertex,
					float* const aNormal,
					float* const aTexCoord,
					const unsigned int countVertex,
					unsigned int* const aIndex,
					const unsigned int countIndex);

public:
	virtual ~Mesh();
	bool initVBO2();
	bool initVBO();

	static void MakeRectangle(Mesh& mesh);
};
