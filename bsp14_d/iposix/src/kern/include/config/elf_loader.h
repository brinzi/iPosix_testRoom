#ifndef _KERN_INCLUDE_CONFIG_ELF_LOADER_H_
#define _KERN_INCLUDE_CONFIG_ELF_LOADER_H_

#include "config/config.h"

#ifdef _IPOSIX_ARCHITECTURE_X86_
#	include "config/x86/elf_loader.h"
#endif //_IPOSIX_ARCHITECTURE_X86_

#endif /* !_KERN_INCLUDE_CONFIG_ELF_LOADER_H_ */
