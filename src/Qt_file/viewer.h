#ifndef VIEWER_H
#define VIEWER_H

#include <QColor>
#include <QColorDialog>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QIntValidator>
#include <QMainWindow>
#include <QTextStream>

#include "../lib_c/s21_backend.h"
#include "opengl.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Viewer;
}
QT_END_NAMESPACE

class Viewer : public QMainWindow {
  Q_OBJECT

 public:
  Viewer(QWidget *parent = nullptr);
  ~Viewer();

 signals:
  void sendColor(QColor, int);
  void saveImage(QString);
  void massPicture(double *, unsigned int *);
  void startTimer();

 private slots:
  void on_pushButton_clicked();

  void on_horizontalSlider_10_valueChanged();

  void on_lineEdit_Slider_10_editingFinished();

  void on_horizontalSlider_valueChanged();

  void on_lineEdit_Slider_11_editingFinished();

  void on_horizontalSlider_4_valueChanged();

  void on_lineEdit_Slider_12_editingFinished();

  void on_pushButton_6_clicked();

  void setColor(QFrame *frame, int colorConfig);

  void on_pushButton_7_clicked();

  void on_pushButton_9_clicked();

  void on_pushButton_10_clicked();

  void on_pushButton_11_clicked();

  void on_pushButton_3_clicked();

  void on_horizontalSlider_1_valueChanged();

  void on_lineEdit_Slider_1_editingFinished();

  void on_horizontalSlider_2_valueChanged();

  void on_lineEdit_Slider_2_editingFinished();

  void on_horizontalSlider_3_valueChanged();

  void on_lineEdit_Slider_3_editingFinished();

  void on_horizontalSlider_7_valueChanged();

  void on_lineEdit_Slider_7_editingFinished();

  void on_horizontalSlider_8_valueChanged();

  void on_lineEdit_Slider_8_editingFinished();

  void on_horizontalSlider_9_valueChanged();

  void on_lineEdit_Slider_9_editingFinished();

  void on_pushButton_5_clicked();

  void on_pushButton_2_clicked();

  void on_pushButton_4_clicked();

 private:
  Ui::Viewer *ui;
  OpenGl *openGLWidget;
  int configWriter = 0;
  void allDisabled(int on);
  int massConf[25] = {0};
  void readConf(QString adress);
  void setConf();
  int cryptCheck(QString adress);
};

#endif  // VIEWER_H
