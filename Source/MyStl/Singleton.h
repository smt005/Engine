// ◦ Xyz ◦
#pragma once

namespace mystd {
	template<typename T>
	class Singleton {
	public:
		inline static T& Instance() {
			if (!instance) {
				instance = new T();
			}
			return *instance;
		}

		void Clear() {
			if (instance) {
				delete instance;
				instance = nullptr;
			}
		}

	private:
		static T* instance;
	};

	template<typename T>
	T* Singleton<T>::instance = nullptr;
}
