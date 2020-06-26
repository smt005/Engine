#pragma once

#include <string>
#include <map>
#include <memory>

#include "Object/Identify.h"

using namespace std;

template <class ObjectT>
#define ObjectPtrT shared_ptr<ObjectT>

class DataClass : public Name
{
public:
	DataClass() {};
	virtual ~DataClass() {};

	virtual bool create(const string &name);
	virtual void setDefault(const string &name);

private:
	static map<string, ObjectPtrT> _map;
	static ObjectT _default;

public:
	static ObjectPtrT& getByName(const string& name);
	static bool hasByName(const string& name);

	static void erase(const string& name);
	static void clear(bool onlyUnused = true);
};

template <class ObjectT>
map<string, ObjectPtrT> DataClass<ObjectT>::_map;

template <class ObjectT>
ObjectT DataClass<ObjectT>::_default;

template <class ObjectT>
bool DataClass<ObjectT>::create(const string &name) {
	setName(name);
	return true;
};

template <class ObjectT>
void DataClass<ObjectT>::setDefault(const string &name) {
	setName(name);
}

template <class ObjectT>
ObjectPtrT& DataClass<ObjectT>::getByName(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end()) {
		return it->second;
	}

	ObjectT* newItem = new ObjectT();

	if (!newItem->create(name)) {
		newItem->setDefault(name);
	}
	
	ObjectPtrT newItemPtr(newItem);
	return _map[name] = newItemPtr;
}

template <class ObjectT>
bool DataClass<ObjectT>::hasByName(const string& name) {
	auto it = _map.find(name);
	return it != _map.end() ? true : false;
}

template <class ObjectT>
void DataClass<ObjectT>::erase(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end()) {
		_map.erase(it);
	}
}

template <class ObjectT>
void DataClass<ObjectT>::clear(bool onlyUnused)
{
	if (onlyUnused = false)	{
		_map.clear();
	} else {
		remove(_map.begin(), _map.end(), [](ObjectPtrT& itemPtr) { return itemPtr.use_count <= 0 ? true : false; });
	}
}
