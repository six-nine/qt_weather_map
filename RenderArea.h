#pragma once

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <vector>
#include <iostream>
#include <QMap>
#include "MapParser.h"

class RenderArea : public QWidget {
 Q_OBJECT
 protected:
  void paintEvent(QPaintEvent *event) override;
 private:
  QVector<QPolygon> polygons;
  QVector<QPoint> debugPoints;
  QVector<QPoint> drawingPoints;
  QVector<QColor> drawingPointsColors;
  struct Point {
    double lon, lat;
    double value;
  };
  std::vector<Point> points;
  const QMap<double, QColor> temperatureColors = {
      {-20, QColor(156, 0, 145)},
      {-15, QColor(47, 10, 110)},
      {-10, QColor(92, 67, 177)},
      {-5, QColor(56, 115, 196)},
      {-0, QColor(0, 180, 150)},
      {5, QColor(0, 210, 59)},
      {10, QColor(170, 225, 0)},
      {15, QColor(225, 210, 0)},
      {20, QColor(230, 160, 20)},
      {25, QColor(230, 100, 70)},
      {30, QColor(200, 15, 100)},
      {40, QColor(115, 0, 35)},
  };

  const QMap<double, QColor> pressureColors = {
      {930, QColor(156, 0, 145)},
      {956, QColor(47, 10, 110)},
      {964, QColor(92, 67, 177)},
      {973, QColor(56, 115, 196)},
      {981, QColor(0, 180, 150)},
      {989, QColor(0, 210, 59)},
      {997, QColor(170, 225, 0)},
      {1005, QColor(225, 210, 0)},
      {1013, QColor(230, 160, 20)},
      {1021, QColor(230, 100, 70)},
      {1030, QColor(200, 15, 100)},
      {1054, QColor(115, 0, 35)},
  };
 public:
  enum Mode{
    TEMPERATURE,
    PRESSURE
  };
  Mode mode;

 public:
  explicit RenderArea(QVector<QPolygon> &polygons, Mode mode = TEMPERATURE, QWidget *parent = nullptr);
  void drawPoint(int x, int y);
  void buildPoints(MapParser &parser);
  double getDataFromApi(double lon, double lat);
  void drawMap(MapParser &parser);
  double countData(double lon, double lat);
  QColor getColor(double value, QMap<double, QColor> mp);
  void debugPoint(QPoint point);
  void changeMode();
};
