#include "dialog.hh"
#include "ui_dialog.h"

namespace StudentSide
{

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);
    setWindowTitle("Settings");
    connect(ui->manualTimeRadioButton, &QRadioButton::toggled, ui->timeEdit, &QTimeEdit::setEnabled);

    ui->welcomeLabel->move(30, 10);
    ui->playerNameEdit->move(210, 45);
    ui->playerNameLabel->move(52, 45);
    ui->realTimeRadioButton->setChecked(true);
    ui->realTimeRadioButton->move(30, 75);
    ui->manualTimeRadioButton->move(30, 100);
    ui->timeEdit->move(210, 100);
    ui->timeEdit->setDisabled(true);

    ui->gameLengthLabel->move(140, 160);
    ui->minutesLabel->move(50, 185);
    ui->minutesSpinBox->setRange(1, 5);
    ui->minutesSpinBox->setValue(3);
    ui->minutesSpinBox->move(110, 185);
    ui->secondsLabel->move(200, 185);
    ui->secondsSpinBox->setRange(0, 59);
    ui->secondsSpinBox->move(260, 185);
    ui->playerNameEdit->setMaxLength(8);


    ui->closeButton->move(20, DIALOG_HEIGHT - BUTTON_HEIGHT - 20);
    ui->continueButton->move(DIALOG_WIDTH - BUTTON_WIDTH - 20, DIALOG_HEIGHT - BUTTON_HEIGHT - 20);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_continueButton_clicked()
{
    unsigned int minutes = ui->minutesSpinBox->value();
    unsigned int seconds = ui->secondsSpinBox->value();
    bool manualTime = ui->manualTimeRadioButton->isChecked();
    QTime gameTime = manualTime ? ui->timeEdit->time() : QTime(0, 0);
    QString playerName = ui->playerNameEdit->text();
    emit settingsSelected(minutes, seconds, manualTime, gameTime,playerName);
}

}
