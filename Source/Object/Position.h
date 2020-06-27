#pragma once

#include "glm/mat4x4.hpp"
#include <glm/gtc/type_ptr.hpp>

class Position
{
public:
	Position() : _matrix(glm::mat4x4(1.0f))	{}
	Position(const glm::mat4x4& matrix) : _matrix(glm::mat4x4(matrix)) {}
	Position(const glm::vec3& pos) : _matrix(glm::mat4x4(1.0f)) {
		setPos(pos);
	}

	virtual ~Position() {}

	void setPos(const float (&pos)[3]) {
		_matrix[3][0] = pos[0];
		_matrix[3][1] = pos[1];
		_matrix[3][2] = pos[2];
	}

	void setMatrix(const float (&matrix)[16]);

	inline void setPos(const glm::vec3& pos) {
		_matrix[3][0] = pos.x;
		_matrix[3][1] = pos.y;
		_matrix[3][2] = pos.z;
	}

	inline void set(const glm::mat4x4& matrix) { _matrix = matrix; }

	inline glm::vec3 getPos() const					 { return glm::vec3(_matrix[3][0], _matrix[3][1], _matrix[3][2]); }
	inline const glm::mat4x4& getMatrix() const		 { return _matrix; }
	inline const float* const getMatrixFloat() const { return glm::value_ptr(_matrix); };

	inline Position& operator=(const Position& pos)		  { _matrix = pos._matrix; }
	inline Position& operator=(const glm::mat4x4& matrix) { _matrix = matrix; }
	inline Position& operator=(const glm::vec3& pos)	  { setPos(pos); }

protected:
	glm::mat4x4 _matrix;
};