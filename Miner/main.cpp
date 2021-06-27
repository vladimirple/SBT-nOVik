#include <QApplication>
#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include "mediator.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QMainWindow mainWindow; // окно программы
  Mediator field; // игровое поле
  QMenuBar *menuBar; // меню

  mainWindow.setCentralWidget(&field);
  menuBar = mainWindow.menuBar();

  QAction *newAct = new QAction(QObject::tr("&Easy game"), &mainWindow);
  QObject::connect(newAct, SIGNAL(triggered()), &field, SLOT(on_gameReset()));
  menuBar->addAction(newAct);

  QAction *newAct2 = new QAction(QObject::tr("&Hard game"), &mainWindow);
  QObject::connect(newAct2, SIGNAL(triggered()), &field, SLOT(on_gameReset2()));
  menuBar->addAction(newAct2);
  mainWindow.show();

  return a.exec();
}
