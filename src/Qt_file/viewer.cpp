#include "viewer.h"

#include "ui_viewer.h"

Viewer::Viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::Viewer) {
  ui->setupUi(this);
  ui->lineEdit_Slider_1->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_2->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_3->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_7->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_8->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_9->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_10->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_11->setValidator(new QIntValidator(this));
  ui->lineEdit_Slider_12->setValidator(new QIntValidator(this));
  openGLWidget = new OpenGl(this);
  openGLWidget->move(371, 12);
  openGLWidget->resize(830, 830);
  openGLWidget->show();
  openGLWidget->startFrame = &ui->frame_13;
  openGLWidget->startLabel = &ui->label_18;
  openGLWidget->buttonGif = &ui->pushButton_3;
  connect(ui->radioButton, &QRadioButton::clicked, openGLWidget,
          &OpenGl::setProjectionParallel);
  connect(ui->radioButton_2, &QRadioButton::clicked, openGLWidget,
          &OpenGl::setProjectionCentral);
  connect(ui->radioButton_7, &QRadioButton::clicked, openGLWidget,
          &OpenGl::setLineSolid);
  connect(ui->radioButton_8, &QRadioButton::clicked, openGLWidget,
          &OpenGl::setLineDotted);
  connect(ui->radioButton_3, &QRadioButton::clicked, openGLWidget,
          &OpenGl::setPointNone);
  connect(ui->radioButton_4, &QRadioButton::clicked, openGLWidget,
          &OpenGl::setPointCube);
  connect(ui->radioButton_5, &QRadioButton::clicked, openGLWidget,
          &OpenGl::setPointCircle);
  connect(this, &Viewer::sendColor, openGLWidget, &OpenGl::setColor);
  connect(this, &Viewer::saveImage, openGLWidget, &OpenGl::saveImage);
  connect(this, &Viewer::startTimer, openGLWidget, &OpenGl::startTimer);
  allDisabled(false);
}

Viewer::~Viewer() {
  if (openGLWidget->changeFile == 5) {
    free(openGLWidget->coords);
    free(openGLWidget->lines);
  }
  delete openGLWidget;
  delete ui;
}

void Viewer::on_pushButton_clicked() {
  QString fileNames;
  QFileDialog fileDialog;
  fileDialog.setWindowTitle(tr("Выберите файл с форматом \".obj\""));
  fileDialog.setNameFilter(tr("*.obj"));
  fileDialog.setViewMode(QFileDialog::Detail);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setOption(QFileDialog::DontUseNativeDialog, true);
  if (fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles().join("");
    int countName = fileNames.length() - 1;
    while (fileNames[countName - 1] != '/') countName--;
    ui->lineEdit->setText("");
    while (countName != fileNames.length()) {
      ui->lineEdit->setText(ui->lineEdit->text() + fileNames[countName]);
      countName++;
    }
    if (openGLWidget->changeFile == 5) {
      ui->pushButton_5->clicked();
      free(openGLWidget->coords);
      free(openGLWidget->lines);
    }
    parsing_obj_file(fileNames.toStdString().c_str(), &openGLWidget->coords,
                     &openGLWidget->lines, openGLWidget->MinMax);
    openGLWidget->changeFile = 5;
    ui->label_2->setText(QString::number((openGLWidget->coords[0] - 3) / 3));
    ui->label_4->setText(QString::number(openGLWidget->coords[1] / 2));
    allDisabled(true);
  }
}

void Viewer::on_horizontalSlider_10_valueChanged() {
  connect(ui->horizontalSlider_10, &QSlider::valueChanged, openGLWidget,
          &OpenGl::sizeImage);
  ui->lineEdit_Slider_10->setText(
      QString::number(ui->horizontalSlider_10->value()));
}

void Viewer::on_lineEdit_Slider_10_editingFinished() {
  if (ui->lineEdit_Slider_10->text().toInt() > 1000)
    ui->lineEdit_Slider_10->setText("1000");
  if (ui->lineEdit_Slider_10->text().toInt() < -1000)
    ui->lineEdit_Slider_10->setText("-1000");
  ui->horizontalSlider_10->setValue(ui->lineEdit_Slider_10->text().toInt());
}

void Viewer::on_horizontalSlider_valueChanged() {
  connect(ui->horizontalSlider, &QSlider::valueChanged, openGLWidget,
          &OpenGl::setSizePoint);
  ui->lineEdit_Slider_11->setText(
      QString::number(ui->horizontalSlider->value()));
}

void Viewer::on_lineEdit_Slider_11_editingFinished() {
  if (ui->lineEdit_Slider_11->text().toInt() > 100)
    ui->lineEdit_Slider_11->setText("100");
  if (ui->lineEdit_Slider_11->text().toInt() < 0)
    ui->lineEdit_Slider_11->setText("0");
  ui->horizontalSlider->setValue(ui->lineEdit_Slider_11->text().toInt());
}

void Viewer::on_horizontalSlider_4_valueChanged() {
  connect(ui->horizontalSlider_4, &QSlider::valueChanged, openGLWidget,
          &OpenGl::setLineSize);
  ui->lineEdit_Slider_12->setText(
      QString::number(ui->horizontalSlider_4->value()));
}

void Viewer::on_lineEdit_Slider_12_editingFinished() {
  if (ui->lineEdit_Slider_12->text().toInt() > 100)
    ui->lineEdit_Slider_12->setText("100");
  if (ui->lineEdit_Slider_12->text().toInt() < 1)
    ui->lineEdit_Slider_12->setText("1");
  ui->horizontalSlider_4->setValue(ui->lineEdit_Slider_12->text().toInt());
}

void Viewer::on_pushButton_6_clicked() { setColor(ui->frame_11, 1); }

void Viewer::on_pushButton_7_clicked() { setColor(ui->frame_10, 2); }

void Viewer::on_pushButton_9_clicked() { setColor(ui->frame_12, 3); }

void Viewer::setColor(QFrame *frame, int colorConfig) {
  QColor pointColor = QColorDialog::getColor(Qt::white, this, "Укажите цвет",
                                             QColorDialog::DontUseNativeDialog);
  if (pointColor.isValid()) {
    frame->setStyleSheet(QString("background-color: rgb(%1,%2,%3);")
                             .arg(pointColor.red())
                             .arg(pointColor.green())
                             .arg(pointColor.blue()));
    emit sendColor(pointColor, colorConfig);
  }
}

void Viewer::on_pushButton_10_clicked() {
  QString fileNames;
  QFileDialog dialogPhoto(this);
  dialogPhoto.setWindowTitle(
      tr("Выберите папку и имя файла для сохраннения изображения"));
  dialogPhoto.setNameFilter(tr("*.jpeg"));
  dialogPhoto.setAcceptMode(QFileDialog::AcceptSave);
  dialogPhoto.setOption(QFileDialog::DontUseNativeDialog, true);
  if (dialogPhoto.exec()) {
    fileNames = dialogPhoto.selectedFiles().join("") + ".jpeg";
    emit saveImage(fileNames);
  }
}

void Viewer::on_pushButton_11_clicked() {
  QString fileNames;
  QFileDialog dialogPhoto(this);
  dialogPhoto.setWindowTitle(
      tr("Выберите папку и имя файла для сохраннения изображения"));
  dialogPhoto.setNameFilter(tr("*.bmp"));
  dialogPhoto.setAcceptMode(QFileDialog::AcceptSave);
  dialogPhoto.setOption(QFileDialog::DontUseNativeDialog, true);
  if (dialogPhoto.exec()) {
    fileNames = dialogPhoto.selectedFiles().join("") + ".bmp";
    emit saveImage(fileNames);
  }
}

void Viewer::on_pushButton_3_clicked() {
  ui->frame_13->setStyleSheet("background-color: orange;border-radius: 10px;");
  ui->label_18->setText("Подготовка к записи");
  QFileDialog dialogGif(this);
  dialogGif.setWindowTitle(
      tr("Выберите папку и имя файла для сохраннения анимации"));
  dialogGif.setNameFilter(tr("*.gif"));
  dialogGif.setAcceptMode(QFileDialog::AcceptSave);
  dialogGif.setOption(QFileDialog::DontUseNativeDialog, true);
  if (dialogGif.exec()) {
    openGLWidget->adressGif = dialogGif.selectedFiles().join("") + ".gif";
    emit startTimer();
    ui->pushButton_3->setEnabled(false);
  } else {
    ui->frame_13->setStyleSheet("background-color: red;border-radius: 10px;");
    ui->label_18->setText("Запись не ведется");
  }
}

void Viewer::on_horizontalSlider_1_valueChanged() {
  connect(ui->horizontalSlider_1, &QSlider::valueChanged, openGLWidget,
          &OpenGl::moveX);
  ui->lineEdit_Slider_1->setText(
      QString::number(ui->horizontalSlider_1->value()));  //-- сдвиг по Х
}

void Viewer::on_lineEdit_Slider_1_editingFinished() {
  if (ui->lineEdit_Slider_1->text().toInt() > 1000)
    ui->lineEdit_Slider_1->setText("1000");
  if (ui->lineEdit_Slider_1->text().toInt() < -1000)
    ui->lineEdit_Slider_1->setText("-1000");
  ui->horizontalSlider_1->setValue(ui->lineEdit_Slider_1->text().toInt());
}

void Viewer::on_horizontalSlider_2_valueChanged() {
  connect(ui->horizontalSlider_2, &QSlider::valueChanged, openGLWidget,
          &OpenGl::moveY);  //-- сдвиг по Y
  ui->lineEdit_Slider_2->setText(
      QString::number(ui->horizontalSlider_2->value()));
}

void Viewer::on_lineEdit_Slider_2_editingFinished() {
  if (ui->lineEdit_Slider_2->text().toInt() > 1000)
    ui->lineEdit_Slider_2->setText("1000");
  if (ui->lineEdit_Slider_2->text().toInt() < -1000)
    ui->lineEdit_Slider_2->setText("-1000");
  ui->horizontalSlider_2->setValue(ui->lineEdit_Slider_2->text().toInt());
}

void Viewer::on_horizontalSlider_3_valueChanged() {
  connect(ui->horizontalSlider_3, &QSlider::valueChanged, openGLWidget,
          &OpenGl::moveZ);  //-- сдвиг по Z
  ui->lineEdit_Slider_3->setText(
      QString::number(ui->horizontalSlider_3->value()));
}

void Viewer::on_lineEdit_Slider_3_editingFinished() {
  if (ui->lineEdit_Slider_3->text().toInt() > 1000)
    ui->lineEdit_Slider_3->setText("1000");
  if (ui->lineEdit_Slider_3->text().toInt() < -1000)
    ui->lineEdit_Slider_3->setText("-1000");
  ui->horizontalSlider_3->setValue(ui->lineEdit_Slider_3->text().toInt());
}

void Viewer::allDisabled(int on) {
  ui->frame->setEnabled(on);
  ui->frame_2->setEnabled(on);
  ui->frame_3->setEnabled(on);
  ui->frame_4->setEnabled(on);
  ui->frame_5->setEnabled(on);
  ui->frame_6->setEnabled(on);
  ui->frame_7->setEnabled(on);
  ui->frame_8->setEnabled(on);
  if (!on) {
    ui->frame_10->setStyleSheet("background-color: lightGray;");
    ui->frame_11->setStyleSheet("background-color: lightGray;");
    ui->frame_12->setStyleSheet("background-color: lightGray;");
    ui->frame_13->setStyleSheet(
        "background-color: lightGray;border-radius: 10px;");
  } else {
    ui->frame_10->setStyleSheet("background-color: rgb(0,255,0);");
    ui->frame_11->setStyleSheet("background-color: rgb(255,85,255);");
    ui->frame_12->setStyleSheet("background-color: rgb(1,1,1);");
    ui->frame_13->setStyleSheet("background-color: red;border-radius: 10px;");
  }
  ui->pushButton_2->setEnabled(on);
  ui->pushButton_3->setEnabled(on);
  ui->pushButton_4->setEnabled(on);
  ui->pushButton_5->setEnabled(on);
  ui->pushButton_10->setEnabled(on);
  ui->pushButton_11->setEnabled(on);
  ui->lineEdit->setReadOnly(true);
}

void Viewer::on_horizontalSlider_7_valueChanged() {
  connect(ui->horizontalSlider_7, &QSlider::valueChanged, openGLWidget,
          &OpenGl::rotateX);  //-- поворот по X
  ui->lineEdit_Slider_7->setText(
      QString::number(ui->horizontalSlider_7->value()));
}

void Viewer::on_lineEdit_Slider_7_editingFinished() {
  if (ui->lineEdit_Slider_7->text().toInt() > 360)
    ui->lineEdit_Slider_7->setText("360");
  if (ui->lineEdit_Slider_7->text().toInt() < 0)
    ui->lineEdit_Slider_7->setText("0");
  ui->horizontalSlider_7->setValue(ui->lineEdit_Slider_7->text().toInt());
}

void Viewer::on_horizontalSlider_8_valueChanged() {
  connect(ui->horizontalSlider_8, &QSlider::valueChanged, openGLWidget,
          &OpenGl::rotateY);  //-- поворот по Y
  ui->lineEdit_Slider_8->setText(
      QString::number(ui->horizontalSlider_8->value()));
}

void Viewer::on_lineEdit_Slider_8_editingFinished() {
  if (ui->lineEdit_Slider_8->text().toInt() > 360)
    ui->lineEdit_Slider_8->setText("360");
  if (ui->lineEdit_Slider_8->text().toInt() < 0)
    ui->lineEdit_Slider_8->setText("0");
  ui->horizontalSlider_8->setValue(ui->lineEdit_Slider_8->text().toInt());
}

void Viewer::on_horizontalSlider_9_valueChanged() {
  connect(ui->horizontalSlider_9, &QSlider::valueChanged, openGLWidget,
          &OpenGl::rotateZ);  //-- поворот по Z
  ui->lineEdit_Slider_9->setText(
      QString::number(ui->horizontalSlider_9->value()));
}

void Viewer::on_lineEdit_Slider_9_editingFinished() {
  if (ui->lineEdit_Slider_9->text().toInt() > 360)
    ui->lineEdit_Slider_9->setText("360");
  if (ui->lineEdit_Slider_9->text().toInt() < 0)
    ui->lineEdit_Slider_9->setText("0");
  ui->horizontalSlider_9->setValue(ui->lineEdit_Slider_9->text().toInt());
}

void Viewer::on_pushButton_5_clicked() {
  QString path =
      QApplication::applicationDirPath() + "/../Configs/default.conf";
  if (!cryptCheck(path)) {
    readConf(path);
    setConf();
    openGLWidget->update();
  }
}

void Viewer::on_pushButton_4_clicked() {
  if (!cryptCheck(QApplication::applicationDirPath() +
                  "/../Configs/custom.conf")) {
    readConf(QApplication::applicationDirPath() + "/../Configs/custom.conf");
    setConf();
    openGLWidget->update();
  }
}

void Viewer::readConf(QString adress) {
  QFile file(adress);
  if (file.exists()) {
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    for (int i = 1; i < 22; i++) {
      QString lineFile = file.readLine();
      QStringList lst = lineFile.split("=");
      QStringList lst2 = lst[1].split("\n");
      massConf[i] = lst2[0].toInt();
    }
    file.close();
  }
}

void Viewer::setConf() {
  ui->lineEdit_Slider_1->setText(QString::number(massConf[1]));
  ui->lineEdit_Slider_1->editingFinished();
  openGLWidget->moveX(-(openGLWidget->posXprev - massConf[1]) +
                      openGLWidget->posXprev);
  openGLWidget->posXprev = massConf[1];
  ui->lineEdit_Slider_2->setText(QString::number(massConf[2]));
  ui->lineEdit_Slider_2->editingFinished();
  openGLWidget->moveY(-(openGLWidget->posYprev - massConf[2]) +
                      openGLWidget->posYprev);
  openGLWidget->posYprev = massConf[2];
  ui->lineEdit_Slider_3->setText(QString::number(massConf[3]));
  ui->lineEdit_Slider_3->editingFinished();
  openGLWidget->moveZ(-(openGLWidget->posZprev - massConf[3]) +
                      openGLWidget->posZprev);
  openGLWidget->posZprev = massConf[3];
  ui->lineEdit_Slider_7->setText(QString::number(massConf[4]));
  ui->lineEdit_Slider_7->editingFinished();
  openGLWidget->rotateX(massConf[4]);
  ui->lineEdit_Slider_8->setText(QString::number(massConf[5]));
  ui->lineEdit_Slider_8->editingFinished();
  openGLWidget->rotateY(massConf[5]);
  ui->lineEdit_Slider_9->setText(QString::number(massConf[6]));
  ui->lineEdit_Slider_9->editingFinished();
  openGLWidget->rotateZ(massConf[6]);
  ui->lineEdit_Slider_10->setText(QString::number(massConf[7]));
  ui->lineEdit_Slider_10->editingFinished();
  openGLWidget->sizeImage(-(openGLWidget->sizePrev - massConf[7]) +
                          openGLWidget->sizePrev);
  openGLWidget->sizePrev = massConf[7];

  if (!massConf[8])
    ui->radioButton->click();
  else
    ui->radioButton_2->click();
  if (!massConf[9])
    ui->radioButton_3->click();
  else if (massConf[9] == 1)
    ui->radioButton_4->click();
  else
    ui->radioButton_5->click();
  ui->lineEdit_Slider_11->setText(QString::number(massConf[10]));
  ui->lineEdit_Slider_11->editingFinished();
  openGLWidget->setSizePoint(massConf[10]);
  if (!massConf[11])
    ui->radioButton_8->click();
  else
    ui->radioButton_7->click();
  ui->lineEdit_Slider_12->setText(QString::number(massConf[12]));
  ui->lineEdit_Slider_12->editingFinished();
  openGLWidget->setLineSize(massConf[12]);
  ui->frame_11->setStyleSheet(QString("background-color:rgb(%1,%2,%3);")
                                  .arg(massConf[13])
                                  .arg(massConf[14])
                                  .arg(massConf[15]));
  openGLWidget->setColor(QColor(massConf[13], massConf[14], massConf[15]), 1);
  ui->frame_10->setStyleSheet(QString("background-color:rgb(%1,%2,%3);")
                                  .arg(massConf[16])
                                  .arg(massConf[17])
                                  .arg(massConf[18]));
  openGLWidget->setColor(QColor(massConf[16], massConf[17], massConf[18]), 2);
  ui->frame_12->setStyleSheet(QString("background-color:rgb(%1,%2,%3);")
                                  .arg(massConf[19])
                                  .arg(massConf[20])
                                  .arg(massConf[21]));
  openGLWidget->setColor(QColor(massConf[19], massConf[20], massConf[21]), 3);
}

void Viewer::on_pushButton_2_clicked() {
  QFile file(QApplication::applicationDirPath() + "/../Configs/custom.conf");
  if (file.open(QIODevice::WriteOnly)) {
    QTextStream out(&file);
    out << "posX=" << ui->horizontalSlider_1->value() << "\n";
    out << "posY=" << ui->horizontalSlider_2->value() << "\n";
    out << "posZ=" << ui->horizontalSlider_3->value() << "\n";
    out << "rotateX=" << ui->horizontalSlider_7->value() << "\n";
    out << "rotateY=" << ui->horizontalSlider_8->value() << "\n";
    out << "rotateZ=" << ui->horizontalSlider_9->value() << "\n";
    out << "sizeModel=" << ui->horizontalSlider_10->value() << "\n";
    if (ui->radioButton->isChecked())
      out << "projection=" << 0 << "\n";
    else
      out << "projection=" << 1 << "\n";
    if (ui->radioButton_3->isChecked())
      out << "pointVisual=" << 0 << "\n";
    else if (ui->radioButton_4->isChecked())
      out << "pointVisual=" << 1 << "\n";
    else
      out << "pointVisual=" << 2 << "\n";
    out << "sizePoint=" << ui->horizontalSlider->value() << "\n";
    if (ui->radioButton_8->isChecked())
      out << "lineStripple=" << 0 << "\n";
    else
      out << "lineStripple=" << 1 << "\n";
    out << "sizeLine=" << ui->horizontalSlider_4->value() << "\n";
    out << "colorPointRed="
        << ui->frame_11->palette().color(QWidget::backgroundRole()).red()
        << "\n";
    out << "colorPointGreen="
        << ui->frame_11->palette().color(QWidget::backgroundRole()).green()
        << "\n";
    out << "colorPointBlue="
        << ui->frame_11->palette().color(QWidget::backgroundRole()).blue()
        << "\n";
    out << "colorLineRed="
        << ui->frame_10->palette().color(QWidget::backgroundRole()).red()
        << "\n";
    out << "colorLineGreen="
        << ui->frame_10->palette().color(QWidget::backgroundRole()).green()
        << "\n";
    out << "colorLineBlue="
        << ui->frame_10->palette().color(QWidget::backgroundRole()).blue()
        << "\n";
    out << "colorBackgroundRed="
        << ui->frame_12->palette().color(QWidget::backgroundRole()).red()
        << "\n";
    out << "colorBackgroundGreen="
        << ui->frame_12->palette().color(QWidget::backgroundRole()).green()
        << "\n";
    out << "colorBackgroundBlue="
        << ui->frame_12->palette().color(QWidget::backgroundRole()).blue()
        << "\n";
    file.close();
    file.open(QIODevice::ReadOnly);
    QString s = file.readAll();
    QByteArray ba = s.toUtf8();
    QString key =
        QCryptographicHash::hash(ba, QCryptographicHash::Sha256).toHex();
    file.close();
    file.setFileName(QApplication::applicationDirPath() +
                     "/../Configs/crypt_custom.txt");
    if (file.open(QIODevice::WriteOnly)) {
      QTextStream outt(&file);
      out << key;
      file.close();
    }
  }
}

int Viewer::cryptCheck(QString adress) {
  QString key1, key2;
  int success = 1;
  QFile file(adress);
  if (file.exists()) {
    file.open(QIODevice::ReadOnly);
    QString s = file.readAll();
    QByteArray ba = s.toUtf8();
    key1 = QCryptographicHash::hash(ba, QCryptographicHash::Sha256).toHex();
    file.close();
  }
  int ok = 0;
  QString pattern = "/default.conf";
  for (int i = 12; i > 0; i--) {
    if (adress[adress.length() - 13 + i] != pattern[i]) ok++;
  }
  if (!ok)
    file.setFileName(QApplication::applicationDirPath() +
                     "/../Configs/crypt_default.txt");
  else
    file.setFileName(QApplication::applicationDirPath() +
                     "/../Configs/crypt_custom.txt");
  if (file.exists()) {
    file.open(QIODevice::ReadOnly);
    key2 = file.readAll();
    file.close();
  }
  if (key1 == key2) success = 0;
  return success;
}
