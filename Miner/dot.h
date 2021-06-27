#ifndef DOT_H
# define DOT_H
# include <QPushButton>

//! одна клетка поля
class Dot : public QPushButton {
  Q_OBJECT
public:
  Dot(int i, int j, QWidget *parent = 0);
  int i();
          //!< возвращает индекс строки клетки
  int j();
          //!< возвращает индекс столбца клетки
  bool isMine();
          //!< проверяет есть ли в клетке мина
  void setMine();
          //!< ставит в клетку мину
  void incValue();
          //!< увеличивает счетчик мин
  int value();
          //!< возвращает счетчик минz
  void setOpen();
          //!< открывает клетку
  bool isOpen();
          //!< проверяет открыта ли клетка
  void swapFlag();
          //!< инвертирует флаг
signals:
  void clicked_left();
  void clicked_right();
protected:
  virtual void paintEvent(QPaintEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *e);

  int m_i, m_j;
          //!< координаты клетки
  int m_value;
          //!< значение, хранимое в клетке
  bool m_isMine;
          //!< является ли клетка миной?
  bool m_isOpen;
          //!< открыта ли клетка?
  bool m_isFlag;
          //!< помечена ли клетка флагом
};

#endif // DOT_H
