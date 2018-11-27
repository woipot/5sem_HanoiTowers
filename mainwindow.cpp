#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

    presenter_ = new tower_presenter(ui_->stateLabel);
    presenter_->set_back_step_button(ui_->backStepButton);
    presenter_->set_next_step_button(ui_->nextStepButton);
    presenter_->set_new_model_button(ui_->newModelButton);
    presenter_->set_timer_button(ui_->timerButton);

    presenter_->set_logs_browser(ui_->logsBrowser);

    presenter_->set_timer_time(ui_->timerSlider->value());
    QObject::connect(ui_->backStepButton, SIGNAL(clicked()), this, SLOT(back_step_click()));
    QObject::connect(ui_->nextStepButton, SIGNAL(clicked()), this, SLOT(next_step_click()));
    QObject::connect(ui_->newModelButton, SIGNAL(clicked()), this, SLOT(create_new_model_click()));
    QObject::connect(ui_->timerButton, SIGNAL(clicked()), this, SLOT(switch_timer_click()));

    QObject::connect(ui_->saveButton, SIGNAL(clicked()), this, SLOT(save_logs_click()));
    QObject::connect(ui_->clearButton, SIGNAL(clicked()), this, SLOT(clear_logs_click()));

    QObject::connect(ui_->timerSlider, SIGNAL(valueChanged(int)), this, SLOT(time_changed(int)));

    QObject::connect(ui_->toEndCheck, SIGNAL(clicked()), this, SLOT(switch_direction()));
}

MainWindow::~MainWindow()
{
    delete ui_;
    delete presenter_;
}


void MainWindow::next_step_click()
{
    presenter_->next_step();
}

void MainWindow::back_step_click()
{
    presenter_->back_step();
}

void MainWindow::switch_timer_click()
{
    presenter_->switch_timer();
}

void MainWindow::create_new_model_click()
{

    Dialog* pInputDialog = new Dialog();
    if (pInputDialog->exec() == QDialog::Accepted) {
        auto new_tower_size = pInputDialog->get_result();
        presenter_->create_new_model(new_tower_size);
    }
    delete pInputDialog;

}

void MainWindow::switch_direction()
{
    presenter_->set_direction(ui_->toEndCheck->isChecked());
}

void MainWindow::clear_logs_click()
{
    presenter_->clear_logs();
}

void MainWindow::save_logs_click()
{
    QString str = QFileDialog::getSaveFileName(nullptr, "Save to", "", "*.txt");

    if(str != "")
        presenter_->save_logs(str);
}

void MainWindow::time_changed(int value)
{
    presenter_->set_timer_time((unsigned int)value);
}
