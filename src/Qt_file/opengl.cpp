#include "opengl.h"

OpenGl::OpenGl(QWidget *parent) : QOpenGLWidget(parent) {}

void OpenGl::initializeGL() { glEnable(GL_DEPTH_TEST); }

void OpenGl::resizeGL(int width, int height) {
  glViewport(0, 0, width, height);
}

void OpenGl::paintGL() {
  if (typeLine == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 240);
  }
  glLineWidth(lineSize);
  glEnableClientState(GL_VERTEX_ARRAY);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(MassColorBackground[0], MassColorBackground[1],
               MassColorBackground[2], 1);  // настройка цвета фона
  glColor3d(MassColorLine[0], MassColorLine[1],
            MassColorLine[2]);  // настройка цвета линий
  usePoint(pointVisual);
  useProjection(projection);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (changeFile) {
    glVertexPointer(3, GL_FLOAT, 0, coords);
    glDrawElements(GL_LINES, coords[1], GL_UNSIGNED_INT, lines);
  } else {
    glVertexPointer(3, GL_FLOAT, 0, startCoordinatImage);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, startIndexImage);
  }
  glDisableClientState(GL_VERTEX_ARRAY);

  if (typeLine == 1) glDisable(GL_LINE_STIPPLE);
}

void OpenGl::useProjection(int projection) {
  double H = 0.1;
  double left = -1.0;
  double right = 1.0;
  double bottom = -1.0;
  double top = 1.0;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (projection == 0) {
    glOrtho(left * 4.0, right * 4.0, bottom * 4.0, top * 4.0, -10.0,
            10.0);  //Параллельное
  } else {
    glFrustum(-H, H, -H, H, 0.01, 100.0);  //Перспективное
  }
  update();
}

void OpenGl::usePoint(int pointVisual) {
  glPointSize(pointSize + 2);
  if (pointVisual) {
    glColor3d(MassColorPoint[0], MassColorPoint[1],
              MassColorPoint[2]);  // настройка цвета углов
    if (pointVisual == 1) glEnable(GL_POINT_SMOOTH);
    if (pointVisual > 1) glDisable(GL_POINT_SMOOTH);
    glDrawArrays(GL_POINTS, 1, coords[0] / 3 - 1);
    glColor3d(MassColorLine[0], MassColorLine[1],
              MassColorLine[2]);  // настройка цвета линий
  }
}

void OpenGl::setProjectionParallel(int on) {
  on = 0 * on;
  projection = on;
}

void OpenGl::setProjectionCentral(int on) { projection = on; }

void OpenGl::setLineDotted(int on) { typeLine = 0 * on; }

void OpenGl::setLineSolid(int on) { typeLine = on; }

void OpenGl::setSizePoint(int sizePoint) { pointSize = sizePoint; }

void OpenGl::setLineSize(int on) { lineSize = on; }

void OpenGl::setPointNone(int on) { pointVisual = 0 * on; }

void OpenGl::setPointCube(int on) { pointVisual = on; }

void OpenGl::setPointCircle(int on) { pointVisual = on + 3; }

void OpenGl::setColor(QColor colorInput, int config) {
  if (config == 1) {
    MassColorPoint[0] = colorInput.redF();
    MassColorPoint[1] = colorInput.greenF();
    MassColorPoint[2] = colorInput.blueF();
  }
  if (config == 2) {
    MassColorLine[0] = colorInput.redF();
    MassColorLine[1] = colorInput.greenF();
    MassColorLine[2] = colorInput.blueF();
  }
  if (config == 3) {
    MassColorBackground[0] = colorInput.redF();
    MassColorBackground[1] = colorInput.greenF();
    MassColorBackground[2] = colorInput.blueF();
  }
}

void OpenGl::saveImage(QString adressImage) {
  grabFramebuffer().save(adressImage);
}

void OpenGl::startTimer() {
  timerForGif = new QTimer(this);
  timerForGif->start(100);
  connect(timerForGif, &QTimer::timeout, this, &OpenGl::makeGif);
}

void OpenGl::makeGif() {
  if (countScreenForGif / 10 < 6)
    (*startLabel)
        ->setText(QString("Запись начнется через %1 сек")
                      .arg(5 - countScreenForGif / 10));
  if (countScreenForGif > 50 && countScreenForGif <= 100) {
    (*startLabel)
        ->setText(QString("Идет запись, осталось %1 сек")
                      .arg(10 - countScreenForGif / 10));
    (*startFrame)
        ->setStyleSheet("background-color: green;border-radius: 10px;");
    QPixmap pixmap = this->grab();
    QImage image = pixmap.toImage();
    image = image.scaled(640, 480);
    frames[countScreenForGif - 51] = (image);
  }
  countScreenForGif++;
  if (countScreenForGif > 100) {
    countScreenForGif = 0;
    timerForGif->stop();
    (*startLabel)->setText("Запись не ведется");
    (*startFrame)->setStyleSheet("background-color: red;border-radius: 10px;");
    QGifImage gif(QSize(640, 480));
    gif.setDefaultDelay(100);
    for (int i = 0; i < 50; i++) gif.addFrame(frames[i], 0);
    gif.save(adressGif);
    countScreenForGif = 0;
    (*buttonGif)->setEnabled(true);
  }
}

void OpenGl::sizeImage(int scrollSize) {
  moveToCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  scaling(&coords, -(sizePrev - scrollSize), MinMax);
  moveFromCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  sizePrev = scrollSize;
}

void OpenGl::moveX(int value) {
  move_coord(&coords, -(posXprev - value), 0, MinMax);
  posXprev = value;
}

void OpenGl::moveY(int value) {
  move_coord(&coords, -(posYprev - value), 1, MinMax);
  posYprev = value;
}

void OpenGl::moveZ(int value) {
  move_coord(&coords, -(posZprev - value), 2, MinMax);
  posZprev = value;
}

void OpenGl::rotateX(int value) {
  moveToCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  rotateXX(&coords, -(rotateXprev - value), MinMax);
  moveFromCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  rotateXprev = value;
}

void OpenGl::rotateY(int value) {
  moveToCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  rotateYY(&coords, -(rotateYprev - value), MinMax);
  moveFromCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  rotateYprev = value;
}

void OpenGl::rotateZ(int value) {
  moveToCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  rotateZZ(&coords, -(rotateZprev - value), MinMax);
  moveFromCentre(&coords, posXprev, posYprev, posZprev, MinMax);
  rotateZprev = value;
}
