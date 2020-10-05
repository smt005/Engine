
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "json/json.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <corecrt_math_defines.h>

namespace help
{
	bool loadJson(const std::string& fileName, Json::Value& value);
	bool saveJson(const std::string& fileName, const Json::Value& value, const std::string& indentation = "\t");
	std::string stringFroJson(const Json::Value& value, const std::string& indentation = "\t");

	bool intersection(glm::vec3 start1, glm::vec3 end1, glm::vec3 start2, glm::vec3 end2, glm::vec3& out_intersection);

	/*
	v1 = rand() % 100;         // v1 in the range 0 to 99
	v2 = rand() % 100 + 1;     // v2 in the range 1 to 100
	v3 = rand() % 30 + 1985;   // v3 in the range 1985-2014
	*/

	inline float random_f(const float& min = 0.0f, const float& max = 1.0f, const int& accuracy = 1000) {
		int var = rand() % accuracy;
		float k = static_cast<float>(var) / static_cast<float>(accuracy);
		float range = max - min;
		return min + range * k;
	}

	inline int random_i(const int& min = 0, const int& max = 1) {
		const int range = max - min;
		int var = rand() % range;
		float k = static_cast<float>(var) / static_cast<float>(range);
		return min + range * static_cast<int>(k);
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
		return glm::length(vec);
	}

	template <class VecT>
	inline float distXY(const VecT& pos_0, const VecT& pos_1 = VecT(0.0f)) {
		VecT vec((pos_0.x - pos_1.x), (pos_0.y - pos_1.y), 0.0f);
		return glm::length(vec);
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

	template <class TextT>
	void log(const TextT& text) {
		std::cout << "LOG: " << text << std::endl;
	}
}; // help
