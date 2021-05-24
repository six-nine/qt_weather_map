#pragma once
#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QPalette>
#include <QTextFormat>
class LoadingWidget : public QWidget {
 private:
  QMovie *animation;
  QLabel *label, *txt;
 public:
  LoadingWidget(QWidget *parent = nullptr);
  ~LoadingWidget();
};
