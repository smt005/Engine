
#pragma once

#include <random>
#include <algorithm>
#include <corecrt_math_defines.h>
#include <string>
#include <iostream>
#include <vector>
#include "json/json.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"


namespace help
{
	//...
	bool loadJson(const std::string& fileName, Json::Value& value, bool toResource = true);
	bool saveJson(const std::string& fileName, const Json::Value& value, const std::string& indentation = "\t", bool toResource = true);
	std::string stringFroJson(const Json::Value& value, const std::string& indentation = "\t");

	bool intersection(glm::vec3 start1, glm::vec3 end1, glm::vec3 start2, glm::vec3 end2, glm::vec3& out_intersection);

	template <typename T>
	int random(T lowerBound, T upperBound) {
		if (lowerBound > upperBound) {
			std::swap(lowerBound, upperBound);
		}

		std::random_device rd; // Инициализация случайного устройства
		std::mt19937 gen(rd()); // Генератор с использованием устройства
		std::uniform_int_distribution<> distrib(lowerBound, upperBound); // Равномерное распределение

		return distrib(gen);
	}

	inline float areaCircle(const float radius) { return powf(radius, 2) * (float)M_PI; }
	inline 	float radiusCircle(const float volume) { return sqrtf(volume / (float)M_PI); }

	inline float volumeSphere(const float radius) { return 4.0f / 3.0f * (float)M_PI * powf(radius, 3); }
	inline float radiusSphere(const float value) {
		float valueTemp = (3.0f * value) / (4.0f * (float)M_PI);
		return powf(valueTemp, 1.0f / 3.0f);
	}

	template <class VecT>
	inline float dist(const VecT& pos_0, const VecT& pos_1 = VecT(0.0f)) {
		VecT vec = pos_0 - pos_1;
		return vec.length();
	}

	template <class VecT>
	inline float distXY(const VecT& pos_0, const VecT& pos_1 = VecT(0.0f)) {
		VecT vec((pos_0.x - pos_1.x), (pos_0.y - pos_1.y), 0.0f);
		return vec.length();
	}

	template <class ObjectT>
	ObjectT& add(std::vector<ObjectT*>& vector)
	{
		ObjectT* newItem = new ObjectT();
		vector.push_back(newItem);
		return *newItem;
	}

	template <class ObjectT>
	ObjectT& add(std::vector<ObjectT*>& vector, ObjectT* newItem)
	{
		vector.push_back(newItem);
		return *newItem;
	}

	template <class ObjectT>
	void erase(std::vector<ObjectT*>& vector, const ObjectT item)
	{
		auto it = find(vector.begin(), vector.end(), item);

		if (it != vector.end())
		{
			vector.erase(it);
		}
	}

	template <class ObjectT>
	void erase(std::vector<ObjectT*>& vector, const std::string& name)
	{
		auto it = find_if(vector.begin(), vector.end(), [name](auto item)
		{
			if (!item)
				return false;

			if (item->getName() == name)
				return true;

			return false;
		});

		if (it != vector.end())
		{
			delete it->get();
			vector.erase(it);
		}
	}

	template <class ObjectT>
	void clear(std::vector<ObjectT*>& vector)
	{
		for (auto object : vector)
		{
			delete object;
		}

		vector.clear();
	}

	template <class ObjectT>
	ObjectT& find(std::vector<ObjectT*>& vector, const std::string& name)
	{
		auto it = find_if(vector.begin(), vector.end(), [name](auto item)
		{
			if (!item)
				return false;

			if (item->getName() == name)
				return true;

			return false;
		});

		if (it != vector.end())
		{
			return *(*it);
		}

		return ObjectT::defaultItem();
	}

	template <typename Tchar, typename Tstring>
	Tchar* CopyToCharsPtr(const Tstring& text)
	{
		size_t len = text.length();
		Tchar* charsPtr = new char[len + 1];
		memcpy(charsPtr, text.c_str(), len);
		charsPtr[len] = '\0';
		return charsPtr;
	}

} // help
