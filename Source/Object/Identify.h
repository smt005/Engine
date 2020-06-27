#pragma once

#include <string>

class UniqueId
{
public:
	UniqueId() {
		++_counter;
		_id = _counter;
	}

	virtual ~UniqueId() {}

	inline long unsigned int getId() const {
		return _id;
	}

	inline std::string getIdAsString(const unsigned short int lenght = 0) const {
		return getIdAsStringStatic(_id, lenght);
	}

public:
	static std::string getIdAsStringStatic(const unsigned long int id, const unsigned short int lenght = 0) {
		std::string idStr = std::to_string(id);
		size_t lenghtId = idStr.length();

		if (lenghtId >= lenght) {
			return idStr;
		}

		size_t prefixLenght = lenght - lenghtId;
		std::string prefixStr;
		prefixStr.reserve(prefixLenght);
		char* prefixChar = prefixStr.data();
		size_t i = 0;
		for (i = 0; i < prefixLenght; ++i) {
			prefixChar[i] = '0';
		}
		prefixChar[i] = '\0';

		return prefixChar + idStr;
	}

private:
	long unsigned int _id;

private:
	static long unsigned int _counter;
};

class Name
{
public:
	Name() {
	}

	Name(const char* name) {
		setName(name);
	}

	Name(const std::string& nameObject) {
		setName(nameObject);
	}

	Name(const Name& name) {
		setName(name);
	}

	virtual ~Name() {}

	inline void setNameFromPtr() {
		long unsigned int intPtr = reinterpret_cast<long unsigned int>(this);
		_nameObject = std::to_string(intPtr);
	}

	inline void setName(const UniqueId& id, const unsigned short int lenght = 0) {
		_nameObject = id.getIdAsString(lenght);
	}

	inline void setName(const char* name) {
		_nameObject = name;
	}

	inline void setName(const std::string& name) {
		_nameObject = name;
	}

	inline void setName(const unsigned long int value, const unsigned short int lenght = 0) {
		_nameObject = UniqueId::getIdAsStringStatic(value, lenght);
	}

	inline void setName(const Name& name) {
		_nameObject = name._nameObject;
	}

	inline std::string getName() {
		return _nameObject;
	}

	inline const std::string& getName() const {
		return _nameObject;
	}

private:
	std::string _nameObject;
};


