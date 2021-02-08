#include "topwindow.h"
#include "ui_topwindow.h"
#include <QDebug>


namespace StudentSide {

Topwindow::Topwindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Topwindow)
{
  ui->setupUi(this);
  setWindowTitle("Top 10 scores");
  std::multimap<int, std::string, std::greater<int>> players = getScore();
  int amount = 0;
  for(auto key : players){
    if(amount <10){
      ui->textBrowserPlayer->append(QString::fromStdString(key.second));
      ui->textBrowser_2->append(QString::fromStdString(std::to_string(key.first)));
    }
    else{
      break;
    }
    amount++;
  }



}

Topwindow::~Topwindow()
{
  delete ui;

}

std::multimap<int, std::string, std::greater<int>> Topwindow::getScore(){
  std::ifstream infile;
  infile.open(TOP10_PATH);
  std::multimap<int, std::string, std::greater<int>> players;
  std::string line;
  while (std::getline(infile, line)){
    std::vector<std::string> words;
    std::string word = "";

    for (auto x : line) {
      if (x == ':'){
        words.push_back(word);
        word = "";
      }
      else {
        if(x != ' '){
           word = word + x;
        }
      }
    }
    words.push_back(word);
    qDebug() << QString::fromStdString(words.at(0)) << " " << QString::fromStdString(words.at(1));
    players.insert(std::pair <int,std::string>(stoi(words.at(1)),words.at(0)));

  }


  return players;
}


}
