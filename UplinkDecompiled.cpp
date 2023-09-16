#include <iostream>
#include <cassert>
#include <cstdint>
#include <sys/mman.h>
#include <unistd.h>
#include "Util.hpp"
#include "App.hpp"

uintptr_t PAGESIZE = 0;

inline uintptr_t GetPageFromAddress(uintptr_t address)
{
	return address & ~(PAGESIZE - 1);
}

void Write8(uintptr_t address, uint8_t value)
{
	const auto ptr = (uint8_t*)address;

	const auto page = GetPageFromAddress(address);

	mprotect((void*)page, PAGESIZE, PROT_WRITE);

	*ptr = value;

	mprotect((void*)page, PAGESIZE, PROT_EXEC | PROT_READ);
}

void Write16(uintptr_t address, uint16_t value)
{
	const auto ptr = (uint16_t*)address;

	const auto page1 = GetPageFromAddress(address);
	const auto page2 = GetPageFromAddress(address + 1);

	mprotect((void*)page1, PAGESIZE, PROT_WRITE);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_WRITE);

	*ptr = value;

	mprotect((void*)page1, PAGESIZE, PROT_EXEC | PROT_READ);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_EXEC | PROT_READ);
}

void Write32(uintptr_t address, uint32_t value)
{
	const auto ptr = (uint32_t*)address;

	const auto page1 = GetPageFromAddress(address);
	const auto page2 = GetPageFromAddress(address + 3);

	mprotect((void*)page1, PAGESIZE, PROT_WRITE);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_WRITE);

	*ptr = value;

	mprotect((void*)page1, PAGESIZE, PROT_EXEC | PROT_READ);

	if (page1 != page2)
		mprotect((void*)page2, PAGESIZE, PROT_EXEC | PROT_READ);
}

void OverwriteCall(uintptr_t instr, uintptr_t target)
{
	Write32(instr + 1, target - instr - 5);
}

void WriteJump(uintptr_t instr, uintptr_t target)
{
	Write8(instr, 0xE9);
	Write32(instr + 1, target - instr - 5);
}

__attribute__((constructor))
static void Init()
{
	PAGESIZE = sysconf(_SC_PAGE_SIZE);
	WriteJump(0x080508E0, (uint32_t)GetFilePath);
}

__attribute__((destructor))
static void Fini()
{

}
