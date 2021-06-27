#include "mediator.h"
#include "field.h"
#include "clock.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>

Mediator::Mediator() //задаём некоторые начальные значения
  : m_field(0), m_time(0) {
  m_layout = new QGridLayout;
  setLayout(m_layout);
  on_gameReset(); // у нас выполнится при самом первом запуске
}

void Mediator::on_gameReset() {
  if (m_field) delete m_field; // удаляем старое поле и время
  if (m_time) delete m_time;
  m_time = new Clock(this); // задааём новое время и поле (т.е. идём в clock и field)
  m_field = new Field(this);

  connect(m_field, SIGNAL(finished()), m_time, SLOT(stop())); //связь через завершение и функции класса Clock (см.void Clock::stop())

  m_layout->addWidget(m_time, 0, 0, 1, 1, Qt::AlignCenter); // установка виджета времени в определённое положение
  m_layout->addWidget(m_field, 1, 0, 1, 1); // установка виджета поля времени в определённое положение
}

void Mediator::on_gameReset2() { // функция для показа Qt версии через MesseageBox
QMessageBox::aboutQt(this, "About Qt");
}

void Mediator::exit() { // функция для выхода через Application
    QApplication::quit();
}

void Mediator::rules() { // функция для выхода через Application
    QMessageBox::information(0, "rules", "Вам дается минное поле, состоящее из ячеек."
"В каждой ячейке находится либо мина, либо число мин в соседних ячейках."
"При этом изначально ячейки закрыты, то есть вы не знаете что в них находится."
"При помощи нажатий на ячейки необходимо открыть все ячейки, в которых нет мин."
"1.Если вы открываете ячейку, в которой есть мина, то вы проигрываете."
"2.Если вы открыли все ячейки без мин и только их, то вам засчитывается победа.");
}
