#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include "mediator.h"
#include <QMessageBox>
#include <QWidget>
#include <QPoint>
#include "clock.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Mediator field; // игровое поле
  QMenuBar *menuBar; // меню
  QMainWindow mainWindow; // главное окно
  Clock m_time;

  mainWindow.setStyleSheet("background-color: rgba(24, 140, 134, 200)"); //установка цвета для главного окна и поля с клетками
  mainWindow.show();
  field.setStyleSheet("background-color: rgba(173, 217, 195, 1)");
  mainWindow.setCentralWidget(&field); // центральный виджет для field

  menuBar = mainWindow.menuBar(); //верхнее меню

  QAction *newAct = new QAction(QObject::tr("&NewGame"), &mainWindow); // кнопка для новой игры
  QObject::connect(newAct, SIGNAL(triggered()), &field, SLOT(on_gameReset())); //связь через нажатие, и функцию класса Mediator(см. void Mediator::on_gameReset())
  menuBar->addAction(newAct);

  QAction *newAct2 = new QAction(QObject::tr("&About Qt"), &mainWindow); // кнопка для показа версии Qt
  QObject::connect(newAct2, SIGNAL(triggered()), &field, SLOT(on_gameReset2())); // связь через нажатие, и функцию класса Mediator (см. void Mediator::on_gameReset2())
  menuBar->addAction(newAct2);

  QAction *newAct3 = new QAction(QObject::tr("&Exit"), &mainWindow); // кнопка для выхода
  QObject::connect(newAct3, SIGNAL(triggered()), &field, SLOT(exit())); // связь через нажатие, и функцию класса Mediator (см. void Mediator::exit())
  menuBar->addAction(newAct3);

  QAction *newAct4 = new QAction(QObject::tr("&Rules"), &mainWindow); // правила игры
  QObject::connect(newAct4, SIGNAL(triggered()), &field, SLOT(rules())); // связь через нажатие, и функцию класса Mediator (см. void Mediator::rules())
  menuBar->addAction(newAct4);



  return a.exec();
}
