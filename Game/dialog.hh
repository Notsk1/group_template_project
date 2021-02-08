#ifndef DIALOG_HH
#define DIALOG_HH

#include <QDialog>
#include <QTime>

const int BUTTON_WIDTH = 84;
const int BUTTON_HEIGHT = 28;
const int DIALOG_WIDTH = 360;
const int DIALOG_HEIGHT = 300;

namespace Ui {
class Dialog;
}

namespace StudentSide
{

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

signals:
    void settingsSelected(unsigned int min, unsigned int sec, bool manualTime, QTime gameTime, QString playerName);

private slots:
    void on_continueButton_clicked();

private:
    Ui::Dialog *ui;
};

}

#endif // DIALOG_HH
