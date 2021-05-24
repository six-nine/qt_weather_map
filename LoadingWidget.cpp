#include "LoadingWidget.h"
LoadingWidget::LoadingWidget(QWidget *parent) {
  setFixedSize(1200, 700);
  label = new QLabel(this);
  txt = new QLabel(this);
  txt->setText("Загрузка...");
  txt->setAlignment(Qt::AlignCenter);
  txt->setGeometry(600 - txt->width() / 2, 450, txt->width(), txt->height());

  animation = new QMovie("../loading.gif");
  label->setMovie(animation);
  animation->setScaledSize(QSize(200, 200));
  label->setGeometry(500, 250, 200, 200);
  setStyleSheet("background-color: white");
  animation->start();
}
LoadingWidget::~LoadingWidget() {
  delete label;
  delete animation;
}
