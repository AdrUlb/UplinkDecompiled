#include <iostream>
#include <cassert>
#include <cstdint>
#include <sys/mman.h>
#include <unistd.h>
#include "Include/Util.hpp"
#include "Include/App.hpp"
#include "Include/DArray.hpp"
#include "Include/Options.hpp"
#include "Include/RedShirt.hpp"
#include "Include/Bungle.hpp"
#include "Include/Button.hpp"

uintptr_t PAGESIZE = 0;

static inline uintptr_t GetPageFromAddress(uintptr_t address)
{
	return address & ~(PAGESIZE - 1);
}

template<typename T>
static void Write(uintptr_t address, T value)
{
	const auto ptr = (T*)address;

	const auto page1 = GetPageFromAddress(address);
	const auto page2 = GetPageFromAddress(address + sizeof(T) - 1);

	mprotect((void*)page1, PAGESIZE, PROT_WRITE);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_WRITE);

	*ptr = value;

	mprotect((void*)page1, PAGESIZE, PROT_EXEC | PROT_READ);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_EXEC | PROT_READ);
}

static void WriteJump(uintptr_t from, uintptr_t to)
{
#if UPLINKDECOMP32
	Write<uint8_t>(from, 0xE9);
	Write<uint32_t>(from + 1, to - from - 5);
#elif UPLINKDECOMP64
	Write<uint8_t>(from++, 0xFF);
	Write<uint8_t>(from++, 0x25);
	Write<uint8_t>(from++, 0x00);
	Write<uint8_t>(from++, 0x00);
	Write<uint8_t>(from++, 0x00);
	Write<uint8_t>(from++, 0x00);
	Write<uint64_t>(from, to);
#else
#error UNKNOWN BUILD ARCHITECTURE
#endif
}

int main(int argc, char* argv[]);

#if UPLINKDECOMP32
constexpr uintptr_t uplinkMainAddr = 0x080FF260;
#elif UPLINKDECOMP64
constexpr uintptr_t uplinkMainAddr = 0x004A12BA;
#else
#error UNKNOWN BUILD ARCHITECTURE
#endif
__attribute__((constructor))
static void Init()
{
	PAGESIZE = sysconf(_SC_PAGE_SIZE);

	WriteJump(uplinkMainAddr, (uintptr_t)main);
}
