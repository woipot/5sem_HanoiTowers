#pragma once

#include <QMainWindow>
#include "towerpresenter.h"
#include "QDialog"
#include "QFileDialog"
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui_;
    tower_presenter *presenter_;

private slots:
    void next_step_click();
    void back_step_click();
    void switch_timer_click();
    void create_new_model_click();
    void switch_direction();

    void clear_logs_click();
    void save_logs_click();


    void time_changed(int value);
};

