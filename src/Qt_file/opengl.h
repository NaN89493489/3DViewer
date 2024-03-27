#ifndef OPENGL_H
#define OPENGL_H

#include <qgifimage.h>

#include <QColor>
#include <QColorDialog>
#include <QFrame>
#include <QImage>
#include <QLabel>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

#include "../lib_c/s21_backend.h"

class OpenGl : public QOpenGLWidget {
  Q_OBJECT
 public:
  OpenGl(QWidget *parent = nullptr);
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  float *coords;
  unsigned int *lines;
  int changeFile = 0;
  QString adressGif;
  QLabel **startLabel;
  QFrame **startFrame;
  QPushButton **buttonGif;
  int posXprev = 0;
  int posYprev = 0;
  int posZprev = 0;
  int rotateXprev = 0;
  int rotateYprev = 0;
  int rotateZprev = 0;
  int sizePrev = 0;
  float MinMax[6] = {0};

 public slots:
  void sizeImage(int scrollSize);
  void setSizePoint(int sizePoint);
  void setProjectionParallel(int on);
  void setProjectionCentral(int on);
  void setLineDotted(int on);
  void setLineSolid(int on);
  void setLineSize(int on);
  void setPointNone(int on);
  void setPointCube(int on);
  void setPointCircle(int on);
  void setColor(QColor colorInput, int config);
  void saveImage(QString adressImage);
  void startTimer();
  void makeGif();
  void moveX(int value);
  void moveY(int value);
  void moveZ(int value);
  void rotateX(int value);
  void rotateY(int value);
  void rotateZ(int value);

 private:
  int projection = 0;
  int typeLine = 0;
  int pointVisual = 0;
  int pointSize = 15;
  int lineSize = 1;
  int moveFromProject = 0;
  float MassColorLine[3] = {0, 1, 0};
  float MassColorPoint[3] = {1, 0.333333, 1};
  float MassColorBackground[3] = {0.00392157, 0.00392157, 0.00392157};
  void useProjection(int projection);
  void usePoint(int pointVisual);

  float startCoordinatImage[6] = {0};
  unsigned int startIndexImage[2] = {1, 2};

  QTimer *timerForGif;
  int countScreenForGif = 0;
  QImage frames[55];
};

#endif  // OPENGL_H
