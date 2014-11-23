#ifndef _KERN_INCLUDE_CONFIG_SCHEDULER_H_
#define _KERN_INCLUDE_CONFIG_SCHEDULER_H_

#include "config/config.h"

#ifdef _IPOSIX_ARCHITECTURE_X86_
#	include "config/x86/scheduler.h"
#endif //_IPOSIX_ARCHITECTURE_X86_

#define SCHEDULER_STRATEGIE ::iposix::scheduler::policies::round_robin
#include "scheduler/policies/round_robin.hpp"

#define SCHEDULER ::iposix::scheduler::i_scheduler < ARCH_SCHEDULER, SCHEDULER_STRATEGIE >
#include "scheduler/i_scheduler.h"

#endif /* !_KERN_INCLUDE_CONFIG_SCHEDULER_H_ */
