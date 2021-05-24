#pragma once

#include <QWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMovie>
#include "RenderArea.h"
#include "LoadingWidget.h"

class MainWindow : public QWidget {
 Q_OBJECT
 private:
  RenderArea *area;
  QWidget *paintArea, *loadingArea;
  QPushButton *btn;
  MapParser *parser;
  LoadingWidget *loading;
  QLabel *modeLabel;

 public slots:
  void onButtonClicked();
 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
};
