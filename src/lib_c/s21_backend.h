#ifndef S21_BACKEND_H
#define S21_BACKEND_H

#include <locale.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data_struct {
  int amount_v;
  int amount_f;

  double MinMaxX[2];
  double MinMaxY[2];
  double MinMaxZ[2];

  double *vertex;
  int size_vertex;
  int count_v;

  int *facets;
  int size_facets;
  int count_f;
} data_struct;

#ifdef __cplusplus
extern "C" {
#endif

void parsing_obj_file(const char *file, float **mass1, unsigned int **mass2,
                      float MinMax[6]);
int count_v_and_f(const char *file, data_struct *array_obj);
int s21_create_vertex(data_struct *array_obj);
void fill_struct(const char *file, data_struct *array_obj);
void fill_facets(int count_facets, char *line, data_struct *array_obj);
void fill_vertex(char *line, data_struct *array_obj);

double min(data_struct *array_obj, int k);
double max(data_struct *array_obj, int k);

void centering(data_struct *array_obj);
void first_scaling(data_struct *array_obj, double value);

void scaling(float **vertex, int value, float MinMax[6]);

void rotateXX(float **vertex, int value, float MinMax[6]);
void rotateYY(float **vertex, int value, float MinMax[6]);
void rotateZZ(float **vertex, int value, float MinMax[6]);

void move_coord(float **vertex, int value, int k, float MinMax[6]);

void create_matrix_double(float **mass1, data_struct *array_obj);
void create_matrix_int(unsigned int **mass2, data_struct *array_obj);

void moveToCentre(float **vertex, int posXprev, int posYprev, int posZprev,
                  float MinMax[6]);
void moveFromCentre(float **vertex, int posXprev, int posYprev, int posZprev,
                    float MinMax[6]);

#ifdef __cplusplus
}
#endif
#endif  // S21_BACKEND_H
