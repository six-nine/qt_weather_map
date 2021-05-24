#pragma once

#include <QVector>
#include <QPoint>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QPolygon>
#include <QJsonArray>
#include <climits>
#include "PointProcessor.h"

class MapParser {
 private:
  QJsonObject jsonObject;
  double _left, _right, _top, _bottom;

  static double min(double a, double b);
  static double max(double a, double b);

  QVector<QPolygon> _polygons;
 public:
  MapParser(QString &filename);

  QVector<QPolygon> getPoints();

  double left();
  double right();
  double top();
  double bottom();

};
