#ifndef TOPWINDOW_H
#define TOPWINDOW_H

#include <QDialog>
#include <fstream>
#include <iostream>
namespace Ui {
class Topwindow;
}

namespace StudentSide {

const std::string TOP10_PATH = "top10.txt";

class Topwindow : public QDialog
{
  Q_OBJECT

public:
  explicit Topwindow(QWidget *parent = nullptr);
  ~Topwindow();

private:
  Ui::Topwindow *ui;
  std::multimap<int, std::string, std::greater<int>> getScore();
};

}

#endif // TOPWINDOW_H
