#include "field.h"
#include "dot.h"
#include <QGridLayout>
#include <QMessageBox>


bool Field::isValidCoord(int i, int j) {
  return i >= 0 && j >= 0 && i < m_n && j < m_n; //проверка на координаты
}

bool Field::is_win() { //проверка на победу
  int n = m_n * m_n - m_mineNumber; //перебираем все значения
  for (int i = 0; i < m_n; ++i)
    for (int j = 0; j < m_n; ++j)
      n -= m_field[i][j]->isOpen(); //вычитаем m_field и n и обращение в isOpen
  return 0 == n;
}

void Field::openDot(int i, int j) { //открытие клеток
  if (false == isValidCoord(i, j)) return; //проверка на координаты
  Dot *p = m_field[i][j]; //отдаём в Dot координаты
  if (p->isOpen()) //проверка на isOpen
    return;
  p->setOpen();
  if (p->value()) return;
  openDot(i - 1, j); openDot(i + 1, j);
  openDot(i, j - 1); openDot(i, j + 1);
}

void Field::on_DotClickedRight() { //клик правой кнопкой мыши
  if (false == m_isGameActive) return;
  Dot *t = reinterpret_cast<Dot*>(sender());
  if (t->isOpen()) return;
  t->swapFlag();
}

void Field::openAll() { //открытие всего поля
  for (int i = 0; i < m_n; ++i)
    for (int j = 0; j < m_n; ++j)
      m_field[i][j]->setOpen();
}

void Field::on_DotClickedLeft() { //клик левой кнопкой мыши
  if (false == m_isGameActive)
    return;
  Dot *t = reinterpret_cast<Dot*>(sender());
  if (t->isOpen()) //задаём некоторую проверку на мину или чистую клетку
    return;
  if (t->isMine()) {
    m_isGameActive = false;
    emit finished();
    QMessageBox::information(0, "Message", "Вы проиграли");
    openAll();
    return;
  }
  openDot(t->i(), t->j());
  if (is_win()) { //проверка на победу и вывод сообщения
    m_isGameActive = false;
    emit finished();
    QMessageBox::information(0, "Message", "Вы выйграли");
    openAll();
  }
}

Field::Field(QWidget *parent)
  : QWidget(parent) {
  m_isGameActive = true;
  QGridLayout *layout = new QGridLayout(this); //задаём новый layout и поле с клетками
  for (int i = 0; i < m_n; ++i) {
    for (int j = 0; j < m_n; ++j) {
      m_field[i][j] = new Dot(i, j, this);
      layout->addWidget(m_field[i][j], i, j, 1, 1);
      connect(m_field[i][j], SIGNAL(clicked_left()), this, SLOT(on_DotClickedLeft())); //связь поля с клетками с кнопками мыши
      connect(m_field[i][j], SIGNAL(clicked_right()), this, SLOT(on_DotClickedRight()));
    }
  }

  for (int i = 0; i < m_mineNumber;) { //расставляем мины случайиным образом
    Dot *p = m_field[qrand() % m_n][qrand() % m_n];
    if (p->isMine())
      continue;
    else {
      p->setMine();
      ++i;
    }
  }

  for (int i = 0; i < m_n; ++i) { //расставляем значения возле мин
    for (int j = 0; j < m_n; ++j) {
      if (m_field[i][j]->isMine()) {
        if (isValidCoord(i - 1, j - 1)) m_field[i - 1][j - 1]->incValue();
        if (isValidCoord(i - 1, j)) m_field[i - 1][j]->incValue();
        if (isValidCoord(i - 1, j + 1)) m_field[i - 1][j + 1]->incValue();
        if (isValidCoord(i, j - 1)) m_field[i][j - 1]->incValue();
        if (isValidCoord(i, j + 1)) m_field[i][j + 1]->incValue();
        if (isValidCoord(i + 1, j - 1)) m_field[i + 1][j - 1]->incValue();
        if (isValidCoord(i + 1, j)) m_field[i + 1][j]->incValue();
        if (isValidCoord(i + 1, j + 1)) m_field[i + 1][j + 1]->incValue();
      }
    }
  }
}
