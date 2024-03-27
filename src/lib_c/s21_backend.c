#include "./s21_backend.h"

void parsing_obj_file(const char *file, float **mass1, unsigned int **mass2,
                      float MinMax[6]) {
  setlocale(LC_ALL, "POSIX");
  int err = 0;
  data_struct array_obj = {0};
  err = count_v_and_f(file, &array_obj);
  if (err != 1) {
    s21_create_vertex(&array_obj);
    fill_struct(file, &array_obj);

    array_obj.MinMaxX[0] = min(&array_obj, 0);
    array_obj.MinMaxY[0] = min(&array_obj, 1);
    array_obj.MinMaxZ[0] = min(&array_obj, 2);

    array_obj.MinMaxX[1] = max(&array_obj, 0);
    array_obj.MinMaxY[1] = max(&array_obj, 1);
    array_obj.MinMaxZ[1] = max(&array_obj, 2);
    centering(&array_obj);
    first_scaling(&array_obj, 0.5);
    MinMax[0] = min(&array_obj, 0);
    MinMax[1] = max(&array_obj, 0);
    MinMax[2] = min(&array_obj, 1);
    MinMax[3] = max(&array_obj, 1);
    MinMax[4] = min(&array_obj, 2);
    MinMax[5] = max(&array_obj, 2);
    array_obj.vertex[0] = (int)array_obj.count_v;
    array_obj.vertex[1] = (int)array_obj.count_f;
    create_matrix_double(mass1, &array_obj);
    create_matrix_int(mass2, &array_obj);
    free(array_obj.facets);
    free(array_obj.vertex);
  }
}

void create_matrix_double(float **mass1, data_struct *array_obj) {
  *mass1 = calloc(array_obj->size_vertex, sizeof(float));
  for (int i = 0; i < array_obj->size_vertex; i++) {
    (*mass1)[i] = (float)array_obj->vertex[i];
  }
}

void create_matrix_int(unsigned int **mass2, data_struct *array_obj) {
  *mass2 = calloc(array_obj->size_facets, sizeof(unsigned int));
  for (int i = 0; i < array_obj->size_facets; i++) {
    (*mass2)[i] = (unsigned int)array_obj->facets[i];
  }
}

int count_v_and_f(const char *file, data_struct *array_obj) {
  int error = 0;
  const char *pattern_v = "^v( +-?[0-9]+((,|.)([0-9]+))?){3}";
  const char *pattern_f = "^f ";
  int count_v = 0, count_f = 0, match = 0;
  char *line = NULL;
  size_t len = 0;
  regex_t regex_v, regex_f;
  regcomp(&regex_v, pattern_v, REG_EXTENDED);
  regcomp(&regex_f, pattern_f, REG_EXTENDED);
  FILE *f = fopen(file, "r");
  if (f == NULL) {
    error = 1;
  } else {
    while (getline(&line, &len, f) != -1) {
      match = regexec(&regex_v, line, 0, NULL, 0);
      if (match == 0) {
        count_v++;
      }
      match = regexec(&regex_f, line, 0, NULL, 0);
      if (match == 0) {
        count_f++;
      }
    }
    array_obj->amount_v = count_v;
    array_obj->amount_f = count_f;
  }
  free(line);
  fclose(f);
  regfree(&regex_v);
  regfree(&regex_f);
  return error;
}

int s21_create_vertex(data_struct *array_obj) {
  int res = 1;
  array_obj->size_vertex = array_obj->amount_v * 3 + 6;
  array_obj->vertex = calloc(array_obj->size_vertex, sizeof(double));
  return res;
}

void fill_struct(const char *file, data_struct *array_obj) {
  const char *pattern_v = "^v( +-?[0-9]+((,|.)([0-9]+))?){3}";
  const char *pattern_f = "^f ";
  char *line = NULL;
  size_t len = 0;
  int match = 0;
  regex_t regex_v, regex_f;
  regcomp(&regex_v, pattern_v, REG_EXTENDED);
  regcomp(&regex_f, pattern_f, REG_EXTENDED);
  array_obj->count_v = 3;
  FILE *f = fopen(file, "r");
  while (getline(&line, &len, f) != -1) {
    match = regexec(&regex_v, line, 0, NULL, 0);
    if (match == 0) {
      fill_vertex(line, array_obj);
    }
    match = regexec(&regex_f, line, 0, NULL, 0);
    if (match == 0) {
      int count_facetes = 0;
      char *start = line;
      while (*start != '\0') {
        if ((*start >= '0' && *start <= '9') && *(start - 1) == ' ') {
          count_facetes++;
        }
        start++;
      }
      fill_facets(count_facetes, line, array_obj);
    }
  }
  free(line);
  fclose(f);
  regfree(&regex_v);
  regfree(&regex_f);
}

void fill_vertex(char *line, data_struct *array_obj) {
  int flag = 0;
  char *istr;
  istr = strtok(line, " ");
  while (istr != NULL) {
    if (flag != 0 && flag != 4) {
      double c = atof(istr);
      array_obj->vertex[array_obj->count_v] = c;
      array_obj->count_v++;
    }
    istr = strtok(NULL, " ");
    flag++;
  }
}

double min(data_struct *array_obj, int k) {
  double min = array_obj->vertex[3 + k];
  for (int i = 3 + k; i < array_obj->size_vertex; i = i + 3) {
    if (min >= array_obj->vertex[i]) {
      min = array_obj->vertex[i];
    }
  }
  return min;
}

double max(data_struct *array_obj, int k) {
  double max = array_obj->vertex[3 + k];
  for (int i = 3 + k; i < array_obj->size_vertex; i = i + 3) {
    if (max <= array_obj->vertex[i]) {
      max = array_obj->vertex[i];
    }
  }
  return max;
}

void fill_facets(int count_facets, char *line, data_struct *array_obj) {
  int flag = 0;
  double r = 0;
  int *values = array_obj->facets;
  if (count_facets > 0) {
    array_obj->size_facets = (count_facets * 2) + array_obj->size_facets;
    int *mem = (int *)realloc(values, array_obj->size_facets * sizeof(int));
    array_obj->facets = mem;
    char *start = line;
    while (*start != '\0') {
      if (*start == ' ' && (*(start + 1) >= '0' && *(start + 1) <= '9')) {
        if (flag >= 1) {
          array_obj->facets[array_obj->count_f] = strtod(start, &start);
          if (flag == 1) {
            r = array_obj->facets[array_obj->count_f];
          }
          array_obj->count_f++;
        }
        if (flag > 1) {
          array_obj->facets[array_obj->count_f] =
              array_obj->facets[array_obj->count_f - 1];
          array_obj->count_f++;
        }
      }
      start++;
      flag++;
    }
    array_obj->facets[array_obj->count_f] = r;
    array_obj->count_f++;
  }
}

void centering(data_struct *array_obj) {
  double cX = array_obj->MinMaxX[0] +
              (array_obj->MinMaxX[1] - array_obj->MinMaxX[0]) / 2;
  double cY = array_obj->MinMaxY[0] +
              (array_obj->MinMaxY[1] - array_obj->MinMaxY[0]) / 2;
  double cZ = array_obj->MinMaxY[0] +
              (array_obj->MinMaxZ[1] - array_obj->MinMaxZ[0]) / 2;

  for (int i = 3; i < array_obj->size_vertex; i = i + 3) {
    array_obj->vertex[i] -= cX;
  }

  for (int i = 4; i < array_obj->size_vertex; i = i + 3) {
    array_obj->vertex[i] -= cY;
  }

  for (int i = 5; i < array_obj->size_vertex; i = i + 3) {
    array_obj->vertex[i] -= cZ;
  }
}

void first_scaling(data_struct *array_obj, double value) {
  double x = array_obj->MinMaxX[1] - array_obj->MinMaxX[0];
  double y = array_obj->MinMaxY[1] - array_obj->MinMaxY[0];
  double z = array_obj->MinMaxZ[1] - array_obj->MinMaxZ[0];

  double maxV = x;
  if (y > maxV) {
    maxV = y;
  }
  if (z > maxV) {
    maxV = z;
  }

  double scale = (value - value * (-1)) / maxV;
  for (int i = 3; i < array_obj->size_vertex; i++) {
    array_obj->vertex[i] *= scale;
  }
}

void scaling(float **vertex, int value, float MinMax[6]) {
  float maxV = (MinMax[3] - MinMax[2]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[3] - MinMax[2])
               : (MinMax[5] - MinMax[4]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[5] - MinMax[4])
                   : (MinMax[1] - MinMax[0]);
  for (int i = 3; i < (*vertex)[0]; i++)
    (*vertex)[i] = (*vertex)[i] * (value * (maxV / 200) + 1);
}

void rotateXX(float **vertex, int value, float MinMax[6]) {
  double valueRad = value * 0.0174533;

  double cY = MinMax[2] + (MinMax[3] - MinMax[2]) / 2;
  double cZ = MinMax[4] + (MinMax[5] - MinMax[4]) / 2;

  for (int i = 3; i < (*vertex)[0]; i = i + 3) {
    double y = (*vertex)[i + 1];
    double z = (*vertex)[i + 2];
    (*vertex)[i + 1] =
        ((y - cY) * cos(valueRad) - (z - cZ) * sin(valueRad)) + cY;
    (*vertex)[i + 2] = (y - cY) * sin(valueRad) + (z - cZ) * cos(valueRad) + cZ;
  }
}

void rotateYY(float **vertex, int value, float MinMax[6]) {
  double valueRad = value * 0.0174533;

  double cX = MinMax[0] + (MinMax[1] - MinMax[0]) / 2;
  double cZ = MinMax[4] + (MinMax[5] - MinMax[4]) / 2;

  for (int i = 3; i < (*vertex)[0]; i = i + 3) {
    double x = (*vertex)[i];
    double z = (*vertex)[i + 2];
    (*vertex)[i] = (x - cX) * cos(valueRad) + (z - cZ) * sin(valueRad) + cX;
    (*vertex)[i + 2] = (z - cZ) * cos(valueRad) - (x - cX) * sin(valueRad) + cZ;
  }
}

void rotateZZ(float **vertex, int value, float MinMax[6]) {
  double valueRad = value * 0.0174533;

  double cX = MinMax[0] + (MinMax[1] - MinMax[0]) / 2;
  double cY = MinMax[2] + (MinMax[3] - MinMax[2]) / 2;

  for (int i = 3; i < (*vertex)[0]; i = i + 3) {
    double x = (*vertex)[i];
    double y = (*vertex)[i + 1];
    (*vertex)[i] = (x - cX) * cos(valueRad) - (y - cY) * sin(valueRad) + cX;
    (*vertex)[i + 1] = (y - cY) * cos(valueRad) + (x - cX) * sin(valueRad) + cY;
  }
}

void move_coord(float **vertex, int value, int k, float MinMax[6]) {
  float maxV = (MinMax[3] - MinMax[2]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[3] - MinMax[2])
               : (MinMax[5] - MinMax[4]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[5] - MinMax[4])
                   : (MinMax[1] - MinMax[0]);
  for (int i = 3; i < (*vertex)[0]; i = i + 3)
    (*vertex)[i + k] = (*vertex)[i + k] + (int)value * (maxV / 200.0);
}

void moveToCentre(float **vertex, int posXprev, int posYprev, int posZprev,
                  float MinMax[6]) {
  float maxV = (MinMax[3] - MinMax[2]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[3] - MinMax[2])
               : (MinMax[5] - MinMax[4]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[5] - MinMax[4])
                   : (MinMax[1] - MinMax[0]);
  for (int i = 3; i < (*vertex)[0]; i = i + 3) {
    (*vertex)[i] = (*vertex)[i] + (-(int)posXprev) * (maxV / 200.0);
    (*vertex)[i + 1] = (*vertex)[i + 1] + (-(int)posYprev) * (maxV / 200.0);
    (*vertex)[i + 2] = (*vertex)[i + 2] + (-(int)posZprev) * (maxV / 200.0);
  }
}

void moveFromCentre(float **vertex, int posXprev, int posYprev, int posZprev,
                    float MinMax[6]) {
  float maxV = (MinMax[3] - MinMax[2]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[3] - MinMax[2])
               : (MinMax[5] - MinMax[4]) > (MinMax[1] - MinMax[0])
                   ? (MinMax[5] - MinMax[4])
                   : (MinMax[1] - MinMax[0]);
  for (int i = 3; i < (*vertex)[0]; i = i + 3) {
    (*vertex)[i] = (*vertex)[i] + (int)posXprev * (maxV / 200.0);
    (*vertex)[i + 1] = (*vertex)[i + 1] + (int)posYprev * (maxV / 200.0);
    (*vertex)[i + 2] = (*vertex)[i + 2] + (int)posZprev * (maxV / 200.0);
  }
}
