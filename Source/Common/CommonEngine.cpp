
#include <iostream>
#include <string>

#include "CommonEngine.h"
#include "TemplateEngine.h"

void CommonEngineFun() {
	{
		auto res = TemplateEngineFun(std::string("qwe"), std::string("dsa"));
		std::cout << "res: " << res << std::endl;
	}
	{
		auto res = TemplateEngineFun(3, 7);
		std::cout << "res: " << res << std::endl;
	}
}
