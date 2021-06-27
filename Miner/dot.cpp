#include "dot.h"
#include <QPainter>
#include <QMouseEvent>
#include <QString>

Dot::Dot(int i, int j, QWidget *parent) //Задаём свойства и параметры клетки
  : QPushButton(parent), m_i(i), m_j(j), m_value(0), m_isMine(false)
  , m_isOpen(false), m_isFlag(false) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setFixedSize(40, 40);
}

int Dot::i() { return m_i; }
int Dot::j() { return m_j; }

int Dot::value() { return m_value; }
void Dot::incValue() { ++m_value; }

bool Dot::isMine() { return m_isMine; }
void Dot::setMine() { m_isMine = true; repaint(); }

bool Dot::isOpen() { return m_isOpen; }
void Dot::setOpen() { m_isOpen = true; repaint(); }

void Dot::swapFlag() { m_isFlag = !m_isFlag; repaint(); }

/*virtual*/ void Dot::paintEvent(QPaintEvent *event) { //окрашивание мин(эллипсов) в красный и синий цвет
  if (isOpen() && isMine() == false && 0 == m_value)
    return;
  QPushButton::paintEvent(event);
  QPainter p(this);
  if (isOpen()) {
    if (isMine()) {
      p.setBrush(QBrush(Qt::red, Qt::Dense4Pattern));
      p.drawEllipse(2, 2, width() - 5, height() - 5);
      return;
    }
    p.setPen(QPen(QColor(255, 255, 255))); //окрашивание текста и задание шрифта
    QFont font = QFont("Light");
    font.setPixelSize(24);
    p.setFont(QFont(font));
    p.drawText(QPoint(14,28), QString::number(m_value));
    return;
  }
  if (m_isFlag) {
    p.setBrush(QBrush(Qt::blue, Qt::Dense4Pattern));
    p.drawEllipse(2, 2, width() - 5, height() - 5);
  }
}


/*virtual*/ void Dot::mouseReleaseEvent(QMouseEvent *e) {
 if (e->button() == Qt::LeftButton) emit clicked_left();
 if (e->button() == Qt::RightButton) emit clicked_right();
}
