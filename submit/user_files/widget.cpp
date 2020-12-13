#include "widget.h"
#include "ui_widget.h"

//using semi transparent color for stepping information
static QColor toggleColor[4] = {QColor(255,0,0,127),QColor(0,0,255,127),QColor(0,127,255,127),QColor(0,127,0,127)};
extern queue_t *mailbox;
extern int xPrev,yPrev;
extern data_t curr_status;
using namespace std;


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

render_line::render_line(QWidget *parent) :
        QWidget(parent)
{
  QPainter painter(this);

  this->pen.setStyle(Qt::SolidLine);
  this->pen.setWidth(3);
  this->pen.setBrush(Qt::green);
  this->pen.setCapStyle(Qt::RoundCap);
  this->pen.setJoinStyle(Qt::RoundJoin);

  painter.setPen(this->pen);

  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  this->color_id = 0;

  this->timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer->start(200);
  //use vector to store some set of points
}

render_line :: ~render_line()
{
  int count_e = segment.count();
  segment.remove(0,count_e);
}

void render_line::animate()
{
  this->update();
}
void render_line::setPen(const QPen &pen)
{
  this->pen = pen;
  update();
}

void render_line::changeColor()
{
  this->color_id = (this->color_id + 1) % 4;
  this->pen.setColor(toggleColor[this->color_id]);
}

void render_line::clearDisplay()
{
  int count_e = segment.count();
  segment.remove(0,count_e);
  QPoint temp = QPoint(xPrev,yPrev);
  for(int i = 0; i< 200 ; i++)
  {
    segment.append(temp);
  }
}

void render_line:: set_point(const int x, const int y)
{
    QPoint temp = QPoint(x,y);

    int count_e = segment.count();
    if( count_e < TRAILER)
    {
      segment.append(temp);
    }
    else
    {
      segment.push_front(temp);
      segment.pop_back();
    }
}

/* plots all the points stored in the vector to screen */
void render_line::paintEvent(QPaintEvent *pEvent)
{
 QPainter paint(this);
 QRegion reg(QRect(0,0,950,950));
 paint.setClipRegion(reg);

 if(0 != curr_status.color)
  this->changeColor();
 else if(0 != curr_status.clear)
  this->clearDisplay();

 paint.setPen(this->pen);
 paint.setBrush(this->brush);

 int count_e = segment.count();
 for(int i= 1; i < count_e; i++)
 {
  //plot all the points in the vector
  paint.save();
  paint.drawLine((QPoint)segment.at(i-1), (QPoint)segment.at(i));
  paint.restore();
 }

}

update_thread ::update_thread()
{
}

update_thread ::~update_thread()
{
}

void update_thread::run()
{
    data_t new_data;
    int ret = 0;
    cout<<"Thread 2: polling mailbox for data to be plotted on screen "<< endl;
   while(1)
   {
    do{
       ret = dequeue(mailbox, &new_data);
       SLEEP(50);
     }while(ret < 0);

    calculate_position(&new_data);
    this->set_point(xPrev,yPrev);
   }
}
