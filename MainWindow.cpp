#include "MainWindow.h"


void MainWindow::onButtonClicked() {
  paintArea->setVisible(false);
  loadingArea->setVisible(true);
  area->changeMode();
  if (area->mode == RenderArea::TEMPERATURE) {
    modeLabel->setText("Текущий режим:\nТемпература");
  } else {
    modeLabel->setText("Текущий режим:\nДавление");
  }
  area->buildPoints(*parser);
  area->drawMap(*parser);
  loadingArea->setVisible(false);
  paintArea->setVisible(true);
}

MainWindow::MainWindow(QWidget *parent) {
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
  modeLabel->setText("Текущий режим:\nТемпература");
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
MainWindow::~MainWindow() {
  delete area;
  delete paintArea;
  delete loadingArea;
  delete btn;
  delete parser;
  delete loading;
  delete modeLabel;
};