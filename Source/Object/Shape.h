#pragma once

#include "Common/DataClass.h"
#include "Mesh.h"

#include <string>

using namespace std;

class Shape;
typedef std::shared_ptr<Shape> ShapePtr;

class Shape: public Mesh, public DataClass<Shape>
{
	friend Mesh;
public:
	virtual ~Shape() = default;
    
	bool create(const string &name) override;
	bool load(const string& name);
	
	void setScale(float* scale);
	void copy(const Shape& shape) {
		Mesh::copy(shape);
	}

	bool Loaded() {
		return _loaded;
	}

private:
	void loadThread(const string& name);

private:
	bool _loaded = false;
};
