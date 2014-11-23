#include "x86/core/local_descriptor_table.h"

namespace iposix
{
namespace arch
{
namespace x86
{

local_descriptor_table::local_descriptor_table()
{
}

local_descriptor_table::local_descriptor_table(uint16_t size)
{
}

local_descriptor_table::~local_descriptor_table()
{
}

void local_descriptor_table::activate()
{
	asm volatile("lldt %0" : : "m" (this->reg));
}

} //namespace x86
} //namespace arch
} //namespace iposix
