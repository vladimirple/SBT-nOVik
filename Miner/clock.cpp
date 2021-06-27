#include "clock.h"
#include <QTimer>
#include <QMessageBox>

Clock::Clock(QWidget *parent)
  :QLCDNumber(parent), m_time(0, 0, 0) { //Устанавливаем QLCD таймер
  setSegmentStyle(Filled); //Задаём стиль часов

  m_timer = new QTimer(this); //создаём таймер
  connect(m_timer, SIGNAL(timeout()), this, SLOT(on_tick())); //связываем таймер с функцией on_tick
  m_timer->start(1000);

  display(m_time.toString("hh:mm:ss")); // отображение таймера изначально с часами/минутами/секундами

  setFixedSize(200, 30); //размер окна QLCD таймера
}

void Clock::on_tick() { //задаём отображение таймера и добавелние по секунде
  m_time = m_time.addSecs(1);
  display(m_time.toString("hh:mm:ss"));
}


void Clock::stop() { //остановка m_timer
    m_timer->stop();
}
