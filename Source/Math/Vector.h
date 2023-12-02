#pragma once

#include <iostream>
#include <ostream>
#include <initializer_list>

namespace Math {
template <typename T>
class base_vector3 final {
public:
	T x, y, z;

	base_vector3() :x(0), y(0), z(0) {
	}

	base_vector3(T _x, T _y, T _z) :x(_x), y(_y), z(_z) {
	}

	base_vector3(const std::initializer_list<T>& initList) {
		auto it = initList.begin();
		x = *it++;
		y = *it++;
		z = *it;
	}

	base_vector3(std::initializer_list<T>&& initList) noexcept {
		auto it = initList.begin();
		x = *it++;
		y = *it++;
		z = *it;
	}

	base_vector3(const base_vector3& vec) :x(vec.x), y(vec.y), z(vec.z) {
	}

	base_vector3(base_vector3&& vec) :x(vec.x), y(vec.y), z(vec.z) noexcept {
	}

	base_vector3& operator = (const base_vector3& vec) {
		x = vec.x; y = vec.y; z = vec.z;
		return *this;
	}

	base_vector3& operator = (base_vector3&& vec) noexcept {
		x = vec.x; y = vec.y; z = vec.z;
		return *this;
	}

	base_vector3 operator + (const base_vector3& vec) {
		return base_vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	base_vector3 operator + (base_vector3&& vec) noexcept {
		return base_vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	base_vector3 operator - (const base_vector3& vec) {
		return base_vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	base_vector3 operator - (base_vector3&& vec) noexcept {
		return base_vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	base_vector3 operator * (const base_vector3& vec) {
		return base_vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	base_vector3 operator * (base_vector3&& vec) noexcept {
		return base_vector3(x * vec.x, y * vec.y, z * vec.z);
	}

	base_vector3 operator / (const base_vector3& vec) {
		return base_vector3(x / vec.x, y / vec.y, z / vec.z);
	}

	base_vector3 operator / (base_vector3&& vec) noexcept {
		return base_vector3(x / vec.x, y / vec.y, z / vec.z);
	}

	base_vector3& operator + (const T value) {
		return base_vector3(x + value, y + value, z + value);
	}

	base_vector3 operator - (const T value) {
		return base_vector3(x - value, y - value, z - value);
	}

	base_vector3 operator * (const T value) {
		return base_vector3(x * value, y * value, z * value);
	}

	base_vector3 operator / (const T value) {
		return base_vector3(x / value, y / value, z / value);
	}

	base_vector3& operator += (const base_vector3& vec) {
		x += vec.x; y += vec.y; z += vec.z;
		return *this;
	}

	base_vector3& operator += (base_vector3&& vec) noexcept  {
		x += vec.x; y += vec.y; z += vec.z;
		return *this;
	}

	base_vector3& operator -= (const base_vector3& vec) {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return *this;
	}

	base_vector3& operator -= (base_vector3&& vec) noexcept {
		x -= vec.x; y -= vec.y; z -= vec.z;
		return *this;
	}

	base_vector3& operator *= (const base_vector3& vec) {
		x *= vec.x; y *= vec.y; z *= vec.z;
		return *this;
	}

	base_vector3& operator *= (base_vector3&& vec) noexcept {
		x *= vec.x; y *= vec.y; z *= vec.z;
		return *this;
	}

	base_vector3& operator /= (const base_vector3& vec) {
		x /= vec.x; y /= vec.y; z /= vec.z;
		return *this;
	}

	base_vector3& operator /= (base_vector3&& vec) noexcept {
		x /= vec.x; y /= vec.y; z /= vec.z;
		return *this;
	}

	base_vector3& operator += (const T value) {
		x += value; y += value; z += value;
		return *this;
	}

	base_vector3& operator -= (const T value) {
		x -= value; y -= value; z -= value;
		return *this;
	}

	base_vector3& operator *= (const T value) {
		x *= value; y *= value; z *= value;
		return *this;
	}

	base_vector3& operator /= (const T value) {
		x /= value; y /= value; z /= value;
		return *this;
	}

	T length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	base_vector3& normalize() {
		T length = std::sqrt(x * x + y * y + z * z);
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}

	T& operator [] (size_t index) {
		if (index == 0) {
			return x;
		} else if (index == 1) {
			return y;
		} else if (index == 2) {
			return z;
		}
		static T defaultValue = 0;
		return defaultValue;
	}

	T operator [] (size_t index) const {
		if (index == 0) {
			return x;
		}
		else if (index == 1) {
			return y;
		}
		else if (index == 2) {
			return z;
		}
		return 0;
	}

	friend
		std::ostream& operator << (std::ostream& out, const base_vector3& vec) {
		out << vec.x << ' ' << vec.y << ' ' << vec.z;
		return out;
	}
};

template<typename T>
T length(const base_vector3<T>& vec) {
	return vec.length();
}

template<typename T>
T length(base_vector3<T>&& vec) {
	return vec.length();
}

template<typename T>
base_vector3<T> normalize(const base_vector3<T>& vec) {
	base_vector3<T> resVec(vec);
	return resVec.normalize();
}

template<typename T>
base_vector3<T> normalize(base_vector3<T>&& vec) {
	base_vector3<T> resVec(vec);
	return resVec.normalize();
}

using Vector3 = base_vector3<float>;
using VectorD3 = base_vector3<double>;
}
