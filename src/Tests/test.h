#ifndef SRC_TESTS_TEST_H_
#define SRC_TESTS_TEST_H_

#include <check.h>
#include <limits.h>
#include <unistd.h>

#include "../lib_c/s21_backend.h"

Suite *test_3d_parsing(void);
Suite *test_3d_moving(void);

#endif  //  SRC_TESTS_TEST_H_
