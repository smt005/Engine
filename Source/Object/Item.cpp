#include "Item.h"

const Item Item::defaultItem;
const Item::Ptr Item::defaultItemPtr;
std::string Item::defaultName;

const std::string& Item::GetName() const {
	return defaultName;
};
