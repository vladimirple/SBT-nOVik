#ifndef FIELDWRAPPER_H
# define FIELDWRAPPER_H
# include <QWidget>
# include <QLabel>

class QGridLayout;
class Clock;
class Field;

//! посредник между полем и таймером
class Mediator :public QWidget {
  Q_OBJECT
public:
  Mediator();
public slots:
  void on_gameReset();
  void on_gameReset2();
protected:
  Field *m_field;
                //!< игровое поле
  Clock *m_time;
                //!< отображает время игры
  QGridLayout *m_layout;
                //!< раскладка виджета
};

#endif // FIELDWRAPPER_H
