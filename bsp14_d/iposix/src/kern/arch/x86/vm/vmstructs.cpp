#include "mm/kmalloc.h"
#include "config/memory.h"

namespace iposix {
namespace arch {
namespace x86 {
namespace vm {

using hlmemory::kmalloc;

page_table_entry *
create_page_table()
{
	scoped_paging_off paging_lock( true );

	void * mem = kmalloc( sizeof( page_table_entry ) * 1024, true, true );
	if( !mem )
	{
		return 0;
	}

	return new( mem ) page_table_entry[1024];
}

page_dir_entry *
create_page_directory_table()
{	
	scoped_paging_off paging_lock( true );

	void * mem = kmalloc( sizeof( page_dir_entry ) * 1024, true, true );
	if( !mem )
	{
		return 0;
	}

	return new( mem ) page_dir_entry[1024];
}

void
delete_page_table( page_table_entry * pt )
{
	scoped_paging_off paging_lock( true );

	delete[] pt;
}

void
delete_page_directory_table( page_dir_entry * pdt )
{
	scoped_paging_off paging_lock( true );

	delete[] pdt;
}

}}}} //iposix::arch::x86::vm
