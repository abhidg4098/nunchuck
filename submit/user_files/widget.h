#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QTimer>
#include <iostream>
#include <QString>
#include <QThread>
#include "queue.h"
#include "user_appli.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget {
    Q_OBJECT
public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Widget *ui;
};

//New class definition for the render
class render_line : public QWidget
{
  Q_OBJECT
public:

  void setPen(const QPen &pen);
  void changeColor();
  void clearDisplay();
  void set_point(const int x, const int y);
  void paintEvent(QPaintEvent *pEvent);
  render_line(QWidget *parent = 0);
  virtual ~render_line();
public slots:
  void animate();
private:
  int color_id;
  QPen pen;
  QBrush brush;
  QPixmap pixmap;
  QVector<QPoint> segment;
  QTimer *timer;
};

class update_thread : public QThread, public render_line
{
 public:
   virtual void run();
   update_thread();
  ~update_thread();
};
#endif // WIDGET_H
