#include "field.h"
#include "dot.h"
#include <QGridLayout>
#include <QMessageBox>

bool Field::isValidCoord(int i, int j) {
  return i >= 0 && j >= 0 && i < m_n && j < m_n;
}

bool Field::is_win() {
  int n = m_n * m_n - m_mineNumber;
  for (int i = 0; i < m_n; ++i)
    for (int j = 0; j < m_n; ++j)
      n -= m_field[i][j]->isOpen();
  return 0 == n;
}

void Field::openDot(int i, int j) {
  if (false == isValidCoord(i, j)) return;
  Dot *p = m_field[i][j];
  if (p->isOpen())
    return;
  p->setOpen();
  if (p->value()) return;
  openDot(i - 1, j); openDot(i + 1, j);
  openDot(i, j - 1); openDot(i, j + 1);
}

void Field::on_DotClickedRight() {
  if (false == m_isGameActive) return;
  Dot *t = reinterpret_cast<Dot*>(sender());
  if (t->isOpen()) return;
  t->swapFlag();
}

void Field::openAll() {
  for (int i = 0; i < m_n; ++i)
    for (int j = 0; j < m_n; ++j)
      m_field[i][j]->setOpen();
}

void Field::on_DotClickedLeft() {
  if (false == m_isGameActive)
    return;
  Dot *t = reinterpret_cast<Dot*>(sender());
  if (t->isOpen())
    return;
  if (t->isMine()) {
    m_isGameActive = false;
    emit finished();
    QMessageBox::information(0, "Information", "lost");
    openAll();
    return;
  }
  openDot(t->i(), t->j());
  if (is_win()) {
    m_isGameActive = false;
    emit finished();
    QMessageBox::information(0, "Information", "win");
    openAll();
  }
}

Field::Field(QWidget *parent)
  : QWidget(parent) {
  m_isGameActive = true;
  QGridLayout *layout = new QGridLayout(this);
  for (int i = 0; i < m_n; ++i) {
    for (int j = 0; j < m_n; ++j) {
      m_field[i][j] = new Dot(i, j, this);
      layout->addWidget(m_field[i][j], i, j, 1, 1);
      connect(m_field[i][j], SIGNAL(clicked_left()), this, SLOT(on_DotClickedLeft()));
      connect(m_field[i][j], SIGNAL(clicked_right()), this, SLOT(on_DotClickedRight()));
    }
  }

  for (int i = 0; i < m_mineNumber;) {
    Dot *p = m_field[qrand() % m_n][qrand() % m_n];
    if (p->isMine())
      continue;
    else {
      p->setMine();
      ++i;
    }
  }

  for (int i = 0; i < m_n; ++i) {
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
