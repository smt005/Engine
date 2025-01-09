#pragma once
#include <memory>
#include <string>

class Item {
public:
	typedef std::shared_ptr<Item> Ptr;

	virtual ~Item() = default;
	virtual void Action() {};
	virtual const std::string& GetName() const;

public:
	static const Item defaultItem;
	static const Item::Ptr defaultItemPtr;

private:
	static std::string defaultName;
};
