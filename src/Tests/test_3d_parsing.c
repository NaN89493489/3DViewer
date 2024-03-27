#include "test.h"

START_TEST(test1_3d_parsing) {
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  const char *file = "/Obj_files/cube.obj";
  float *mass1 = NULL;
  unsigned int *mass2 = NULL;
  float MinMax[6] = {0};
  float vertex[] = {27.000000, 48.000000, 0.000000,  0.500000,  0.500000,
                    -0.500000, 0.500000,  -0.500000, -0.500000, 0.500000,
                    0.500000,  0.500000,  0.500000,  -0.500000, 0.500000,
                    -0.500000, 0.500000,  -0.500000, -0.500000, -0.500000,
                    -0.500000, -0.500000, 0.500000,  0.500000,  -0.500000,
                    -0.500000, 0.500000};
  float facets[] = {1, 5, 5, 7, 7, 3, 3, 1, 4, 3, 3, 7, 7, 8, 8, 4,
                    8, 7, 7, 5, 5, 6, 6, 8, 6, 2, 2, 4, 4, 8, 8, 6,
                    2, 1, 1, 3, 3, 4, 4, 2, 6, 5, 5, 1, 1, 2, 2, 6};
  parsing_obj_file(strcat(cwd, file), &mass1, &mass2, MinMax);
  ck_assert_float_eq(mass1[0], 27.0);
  ck_assert_float_eq(mass1[1], 48.0);
  ck_assert_float_eq(MinMax[0], -0.5);
  ck_assert_float_eq(MinMax[1], 0.5);
  ck_assert_float_eq(MinMax[2], -0.5);
  ck_assert_float_eq(MinMax[3], 0.5);
  ck_assert_float_eq(MinMax[4], -0.5);
  ck_assert_float_eq(MinMax[5], 0.5);
  for (int i = 0; i < 27; i++) {
    ck_assert_float_eq(mass1[i], vertex[i]);
  }
  for (int i = 0; i < 48; i++) {
    ck_assert_int_eq(mass2[i], facets[i]);
  }
  free(mass1);
  free(mass2);
}
END_TEST

START_TEST(test2_3d_parsing) {
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  const char *file = "/Obj_files/octahedron.obj";
  float *mass1 = NULL;
  unsigned int *mass2 = NULL;
  float MinMax[6] = {0};
  float vertex[] = {21.000000, 48.000000, 0.000000,  0.000000,  0.500000,
                    -0.500000, 0.500000,  0.000000,  -0.500000, 0.000000,
                    0.000000,  -1.000000, -0.500000, 0.000000,  -0.500000,
                    0.000000,  0.000000,  0.000000,  0.000000,  -0.500000,
                    -0.500000};
  float facets[] = {2, 3, 3, 1, 1, 2, 3, 4, 4, 1, 1, 3, 4, 5, 5, 1,
                    1, 4, 1, 5, 5, 2, 2, 1, 6, 3, 3, 2, 2, 6, 6, 4,
                    4, 3, 3, 6, 6, 5, 5, 4, 4, 6, 6, 2, 2, 5, 5, 6};
  parsing_obj_file(strcat(cwd, file), &mass1, &mass2, MinMax);
  ck_assert_float_eq(mass1[0], 21);
  ck_assert_float_eq(mass1[1], 48);
  ck_assert_float_eq(MinMax[0], -0.5);
  ck_assert_float_eq(MinMax[1], 0.5);
  ck_assert_float_eq(MinMax[2], -0.5);
  ck_assert_float_eq(MinMax[3], 0.5);
  ck_assert_float_eq(MinMax[4], -1);
  ck_assert_float_eq(MinMax[5], 0);
  for (int i = 0; i < 21; i++) {
    ck_assert_float_eq(mass1[i], vertex[i]);
  }
  for (int i = 0; i < 48; i++) {
    ck_assert_int_eq(mass2[i], facets[i]);
  }
  free(mass1);
  free(mass2);
}
END_TEST

START_TEST(test3_3d_parsing) {
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));
  const char *file = "/Obj_files/MapleTree.obj";
  float *mass1 = NULL;
  unsigned int *mass2 = NULL;
  float MinMax[6] = {0};
  parsing_obj_file(strcat(cwd, file), &mass1, &mass2, MinMax);
  ck_assert_float_eq(mass1[0], 106899);
  ck_assert_float_eq(mass1[1], 180400);
  ck_assert_float_eq_tol(MinMax[0], -0.278162, 0.000001);
  ck_assert_float_eq_tol(MinMax[1], 0.278162, 0.000001);
  ck_assert_float_eq_tol(MinMax[2], -0.5, 0.000001);
  ck_assert_float_eq_tol(MinMax[3], 0.5, 0.000001);
  ck_assert_float_eq_tol(MinMax[4], -0.540509, 0.000001);
  ck_assert_float_eq_tol(MinMax[5], -0.0175656, 0.000001);
  free(mass1);
  free(mass2);
}
END_TEST

Suite *test_3d_parsing(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("\033[43m-=S21_3D_PARSING=-\033[0m.");
  /* Core test case */
  tc = tcase_create("add_tc");

  tcase_add_test(tc, test1_3d_parsing);
  tcase_add_test(tc, test2_3d_parsing);
  tcase_add_test(tc, test3_3d_parsing);

  suite_add_tcase(s, tc);

  return s;
}
