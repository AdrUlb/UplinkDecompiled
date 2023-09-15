#include <iostream>
#include <cassert>
#include "App.hpp"

__attribute__((constructor))
static void Init()
{
	std::cout << sizeof(App) << std::endl;
	assert(sizeof(App) == 0x5C4);
}

__attribute__((destructor))
static void Fini()
{

}