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
  void onButtonClicked() {
    paintArea->setVisible(false);
    loadingArea->setVisible(true);
    area->changeMode();
    if (area->mode == RenderArea::TEMPERATURE) {
      modeLabel->setText("Текущий режим:\nтемпература");
    } else {
      modeLabel->setText("Текущий режим:\nдавление");
    }
    area->buildPoints(*parser);
    area->drawMap(*parser);
    area->update();
    loadingArea->setVisible(false);
    paintArea->setVisible(true);
  };
 public:
  MainWindow(QWidget *parent = nullptr) {
    setFixedSize(1200, 700);
    QString jsonName = "../China.json";
    parser = new MapParser(jsonName);
    QVector<QPolygon> polygons = parser->getPoints();
    area = new RenderArea(polygons);
    area->buildPoints(*parser);
    area->drawMap(*parser);
    paintArea = area;
    paintArea->setParent(this);

    modeLabel = new QLabel(this);
    modeLabel->setText("Текущий режим:\nтемпература");
    modeLabel->setGeometry(1050, 580, 110, 50);

    btn = new QPushButton(this);
    btn->setText("Сменить режим");
    btn->setGeometry(1050, 620, 140, 70);
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked()));

    loading = new LoadingWidget();
    loadingArea = new QWidget();
    loadingArea = loading;
    loadingArea->setParent(this);
    loadingArea->setVisible(false);
  }
 protected:

};
