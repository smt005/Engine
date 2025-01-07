#pragma once
#include <memory>

class Item {
public:
	typedef std::shared_ptr<Item> Ptr;
	virtual ~Item() = default;
	virtual void Action() = 0;
};
