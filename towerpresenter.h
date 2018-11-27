#pragma once

#include "towermodel.h"
#include "printparams.h"

#include "QPushButton"
#include "QSlider"
#include "QLabel"
#include "QTextBrowser"
#include "QImage"
#include "QPainter"
#include "QPixmap"
#include "QTimer"
#include "QFile"
#include "QTextStream"
#include "QString"

#include <QObject>

#define DEFAULTSIZE 10
#define DEFAULTHEIGHT 400
#define DEFAULTWIDTH 800
#define DEFAULTTIME 1000
#define COLUMNS_COUNT 3

class tower_presenter : public QObject
{
    Q_OBJECT

private:
    QTimer *tmr_;

    tower_model *model_;
    PRINT_PARAMS_ *params_;

    unsigned int tower_height_;

    unsigned int timer_time_;

    bool to_end_ = false;

private:
    QPushButton *new_model_button_;
    QPushButton *timer_button_;
    QPushButton *next_step_button_;
    QPushButton *back_step_button_;

    QSlider *timer_time_slider_;

    QLabel *state_output_label_;

    QTextBrowser *logs_browser_;

public:
    tower_presenter();
    tower_presenter(QLabel *label);
    ~tower_presenter();

    void next_step();
    void back_step();
    void create_new_model(unsigned int tower_height);
    void switch_timer();

    void set_timer_time(unsigned int time);
    void set_direction(bool to_end);

    void save_logs(QString patch);
    void clear_logs();

    void set_new_model_button(QPushButton *button);
    void set_timer_button(QPushButton *button);
    void set_next_step_button(QPushButton *button);
    void set_back_step_button(QPushButton *button);
    void set_state_output_label(QLabel *label);
    void set_logs_browser(QTextBrowser *browser);


public slots:
    void on_model_updated(std::array<std::vector<DISC_>, 3> model_state);


private slots:
    void update_time();

private:
    void init_print_params(unsigned int height, unsigned int width);
    void init_timer();

    void drow_base(QPainter &p);
    void drow_state(QPainter &p, const std::array<std::vector<DISC_>, 3> &model_state);
};
