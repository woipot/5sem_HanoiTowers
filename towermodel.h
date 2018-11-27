#pragma once
#include "diskstate.h"
#include "disc.h"
#include <vector>
#include <array>
#include <iostream>
#include <QObject>
#include <QString>

using size_t = unsigned int;

struct DISC_STATE_;
struct DISC_;

class tower_model: public QObject
{
   Q_OBJECT

private:
    std::vector<DISC_STATE_> stack_;

    std::array<std::vector<DISC_>, 3> current_state_;

    size_t tower_size_;

public:
    QString next_step();
    QString back_step();

    tower_model(size_t tower_size);

    void print_current_state();

    std::array<std::vector<DISC_>, 3> get_state();
signals:
    void model_updated(std::array<std::vector<DISC_>, 3> model_state);

private:
    void update_model(size_t src, size_t dest);
};

