#pragma once
#include <iostream>

namespace mystd {

template <typename T>
class shared final {
public:
	shared()
		: _ptr(nullptr)
		, _counterPtr(nullptr)
	{}

	shared(T* ptr)
		: _ptr(ptr)
		, _counterPtr(_ptr ? new unsigned int(1) : nullptr)
	{}

	shared(const shared& ph)
		: _ptr(ph._ptr)
		, _counterPtr(ph._counterPtr)
	{
		if (_counterPtr) {
			++(*_counterPtr);
		}
	}

	shared(shared&& ph) noexcept {
		std::swap(_ptr, ph._ptr);
		std::swap(_counterPtr, ph._counterPtr);
	}

	~shared() {
		if (!_counterPtr) {
			return;
		}

		--(*_counterPtr);
		
		if (*_counterPtr == 0) {
			delete _ptr;
			delete _counterPtr;
		}
	}

	T& operator * () {
		return *_ptr;
	}

	T* operator -> () const {
		return _ptr;
	}

	T* get() {
		return _counterPtr && *_counterPtr > 0 ? _ptr : nullptr;
	}

	operator bool () const {
		return !_counterPtr || *_counterPtr == 0 || !_ptr ? false : true;
	}

	bool operator == (const shared& ph) const {
		return _ptr == ph._ptr;
	}

	bool operator == (const shared&& ph) const noexcept {
		return _ptr == ph._ptr;
	}

	shared& operator = (const shared& ph) {
		if (this == &ph || _ptr == ph._ptr) {
			return *this;
		}

		~shared();

		_ptr = ph._ptr;
		_counterPtr = ph._counterPtr;
		++(*_counterPtr);
		return *this;
	}

	shared& operator = (shared&& ph) noexcept {
		std::swap(_ptr, ph._ptr);
		std::swap(_counterPtr, ph._counterPtr);
		return *this;
	}

	template <typename... Args>
	static shared make(Args&&... args) {
		return shared(new T(std::forward<Args>(args)...));
	}

	void reset() {
		if (!_counterPtr) {
			return;
		}

		~shared();
		_ptr = nullptr;
		_counterPtr = nullptr;
	}

private:
	T* _ptr = nullptr;
	unsigned int* _counterPtr = nullptr;
};

}
