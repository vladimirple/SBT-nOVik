#ifndef FIELD_H
# define FIELD_H
# include <QWidget>

class Dot;

//! игровое поле
class Field :public QWidget {
  Q_OBJECT
public:
  Field(QWidget *parent = 0);
protected:
  void openAll();
          //!< открывает все клетки поля
  bool isValidCoord(int i, int j);
          //!< проверяет является ли i,j координатами
  void openDot(int i, int j);
          //!< открывает клетку и все клетки вокруг
  bool is_win();
          //!< проверяет выигрыш
  const static int m_n = 10;
          //!< размер поля
  const static int m_mineNumber = 10;
          //!< количество мин на поле
  Dot* m_field[m_n][m_n];
          //!< игровое поле
  bool m_isGameActive;
          //!< состояние игрового поля
protected slots:
  void on_DotClickedLeft();
          //!< слот обработки нажатия клетки левой кнопкой мыши
  void on_DotClickedRight();
          //!< слот обработки нажатия клетки правой кнопкой мыши
signals:
  void finished();
          //!< сигнал завершения игры
};

#endif // FIELD_H
