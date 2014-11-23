#ifndef _KERN_INCLUDE_TESTS_TESTS_H_
#define _KERN_INCLUDE_TESTS_TESTS_H_

#include <stdint.h>

namespace iposix {
namespace tests {

bool process_test();

bool i_fs_test();

bool mount_test();

bool elf_loader_test();

bool memory_test();

void all_tests();

} //namespace tests
} //namespace iposix

#endif /* !_KERN_INCLUDE_TESTS_TESTS_H_ */
