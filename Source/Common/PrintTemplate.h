#pragma once

#include <string>
#include <iostream>

namespace help {
	template <typename T, typename D>
	void Print(T&& container, D&& devider, int countEndl = 1)
	{
		const size_t size = container.size();
		const size_t lastIndex = size - 1;

		for (size_t index = 0; index < size; ++index) {
			if (index < lastIndex) {
				cout << container[index] << devider;
				continue;
			}
			cout << container[index];
		}

		for (int i = 0; i < countEndl; ++i) {
			cout << endl;
		}
	}

	template <typename T, typename D>
	void PrintXYZ(T&& container, D&& devider, int countEndl = 1)
	{
		cout << container.x << devider << container.y << devider << container.z;

		for (int i = 0; i < countEndl; ++i) {
			cout << endl;
		}
	}

	void SPACE(int count = 1)
	{
		cout << std::string(count, ' ');
	}
}
