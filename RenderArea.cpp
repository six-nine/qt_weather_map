#include <QPainter>
#include <QEventLoop>
#include <iomanip>
#include "RenderArea.h"

RenderArea::RenderArea(QVector<QPolygon> &polygons, Mode mode, QWidget *parent) {
  this->setFixedSize(QSize(1200, 700));
  this->polygons = polygons;
  this->mode = mode;
}

void RenderArea::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setBrush(Qt::cyan);
  painter.setPen(Qt::cyan);
  foreach(QPolygon polygon, polygons) {
    painter.drawPolygon(polygon);
  }
  for (int i = 0; i < drawingPoints.size(); i++){
    painter.setPen(drawingPointsColors[i]);
    painter.drawPoint(drawingPoints[i]);
  }
  painter.setBrush(Qt::black);
  painter.setPen(Qt::black);
  foreach (QPoint point, debugPoints){
    painter.drawRect(point.x(), point.y(), 3, 3);
  }
  int x = 20, y = 20;
  int w = 40, h = 30;
  if (mode == TEMPERATURE){
    for (auto key: temperatureColors.keys()){
      painter.setPen(QPen(temperatureColors[key]));
      painter.setBrush(QBrush(temperatureColors[key]));
      painter.drawRect(x, y, w, h);
      painter.setPen(Qt::white);
      painter.drawText(x + 5, y + h - 5, QString::number(key));
      y += h;
    }
  } else {
    for (auto key: pressureColors.keys()){
      painter.setPen(QPen(pressureColors[key]));
      painter.setBrush(QBrush(pressureColors[key]));
      painter.drawRect(x, y, w, h);
      painter.setPen(Qt::white);
      painter.drawText(x + 5, y + h - 5, QString::number(key));
      y += h;
    }
  }
}

void RenderArea::drawPoint(int x, int y) {
  drawingPoints.push_back(QPoint(x, y));
  update();
}

void RenderArea::buildPoints(MapParser &parser) {
  points.clear();
  double stepX, stepY;
  stepX = (parser.right() - parser.left()) / 9;
  stepY = (parser.bottom() - parser.top()) / 5;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 10; j++) {
      double lon = parser.left() + stepX * j;
      double lat = parser.top() + i * stepY;
      points.push_back(Point{lon, lat, getDataFromApi(lon, lat)});
      int x, y;
      PointProcessor::get_point(x, y, lon, lat);
    }
  }
}

double RenderArea::getDataFromApi(double lon, double lat) {
  QEventLoop eventLoop;
  double value = 0;
  auto *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
  QNetworkRequest request;
  QString apiKey = "5b5183425a7b61b150ec5a21dbb65162";
  QString baseUrl = "http://api.openweathermap.org/data/2.5/weather?lat="+QString::number(lat)+"&lon="+QString::number(lon)+"&appid="+apiKey;
  request.setUrl(QUrl(baseUrl));
  QNetworkReply* reply = manager->get(request);
  eventLoop.exec();
  QJsonObject jsonObj;
  QJsonDocument jsonDoc;
  QJsonParseError parseError;
  auto replyText = reply->readAll();
  jsonDoc = QJsonDocument::fromJson(replyText, &parseError);
  if (parseError.error != QJsonParseError::NoError){
    qDebug() << replyText;
    qWarning() << "Json parse error: " << parseError.errorString();
  } else {
    jsonObj  = jsonDoc.object();
    value = jsonObj.value("main").toObject().value(mode == TEMPERATURE ? "temp" : "pressure").toDouble();
    if (mode == TEMPERATURE) value -= 273;
  }
  delete reply;
  return value;
}

QColor RenderArea::getColor(double value, QMap<double, QColor> mp) {
  auto left_it = mp.begin();
  for (auto it = mp.begin(); it != mp.end(); it++){
    if (it.key() <= value){
      left_it = it;
    }
  }
  double left_bound = left_it.key();
  double right_bound = (++left_it).key();
  double k = (value - left_bound) / (right_bound - left_bound);
  return QColor(
      (1.0 - k) * mp[left_bound].red() + k * mp[right_bound].red(),
      (1.0 - k) * mp[left_bound].green() + k * mp[right_bound].green(),
      (1.0 - k) * mp[left_bound].blue() + k * mp[right_bound].blue()
  );
}

void RenderArea::drawMap(MapParser &parser) {
  QPixmap pixmap = this->grab();
  QImage img(pixmap.toImage());
  double stepX, stepY;
  int x0, y0, x1, y1;
  PointProcessor::get_point(x0, y0, parser.left(), parser.top());
  PointProcessor::get_point(x1, y1, parser.right(), parser.bottom());
  stepX = (parser.right() - parser.left()) / abs(x1 - x0);
  stepY = (parser.bottom() - parser.top()) / abs(y1 - y0);
  for (int i = 0; i < abs(x1 - x0); i++) {
    for (int j = 0; j < abs(y1 - y0); j++) {
      double lon = parser.left() + stepX * i;
      double lat = parser.top() + j * stepY;
      int x, y;
      PointProcessor::get_point(x, y, lon, lat);
      if (img.pixelColor(x, y) != img.pixelColor(0, 0)) {
        drawingPoints.push_back(QPoint(x, y));
        double value = countData(lon, lat);
        drawingPointsColors.push_back(getColor(value, mode == TEMPERATURE ? temperatureColors : pressureColors));
      }
    }
  }
  update();
}

double RenderArea::countData(double lon, double lat) {
  int index = -1;
  for (int i = 0; i < points.size(); i++){
    if (lon >= points[i].lon && lat >= points[i].lat){
      index = i;
    }
  }
  Point q[2][2];
  q[0][0] = points[index];
  q[0][1] = points[index + 1];
  q[1][0] = points[index + 10];
  q[1][1] = points[index + 11];
  double fr0 = (lon - q[0][0].lon) / (q[0][1].lon - q[0][0].lon) * q[0][1].value +
               (q[0][1].lon - lon) / (q[0][1].lon - q[0][0].lon) * q[0][0].value;
  double fr1 = (lon - q[1][0].lon) / (q[1][1].lon - q[1][0].lon) * q[1][1].value +
               (q[1][1].lon - lon) / (q[1][1].lon - q[1][0].lon) * q[1][0].value;
  return (lat - q[0][0].lat) / (q[1][0].lat - q[0][0].lat) * fr1 +
         (q[1][0].lat - lat) / (q[1][0].lat - q[0][0].lat) * fr0;
}

void RenderArea::debugPoint(QPoint point) {
  debugPoints.push_back(point);
  update();
}
void RenderArea::changeMode() {
  mode == TEMPERATURE ? mode = PRESSURE : mode = TEMPERATURE;
  update();
}
