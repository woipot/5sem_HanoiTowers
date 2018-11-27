#include "towerpresenter.h"

tower_presenter::tower_presenter()
{
    timer_time_ = DEFAULTTIME;
    tower_height_ = DEFAULTSIZE;
    model_ = nullptr;
    params_ = nullptr;
    state_output_label_ = nullptr;
    logs_browser_ = nullptr;
    create_new_model(tower_height_);

    init_timer();
}

tower_presenter::tower_presenter(QLabel *label)
{
    timer_time_ = DEFAULTTIME;
    tower_height_ = DEFAULTSIZE;
    model_ = nullptr;
    params_ = nullptr;
    logs_browser_ = nullptr;
    state_output_label_ = label;
    create_new_model(tower_height_);

    init_timer();
}

tower_presenter::~tower_presenter()
{
    delete tmr_;
    delete model_;
    delete params_;
}

void tower_presenter::next_step()
{
    QString result = model_->next_step();
    if(!result.isEmpty())
    {
        logs_browser_->append("Next step : " + result + "\n");
    }
}

void tower_presenter::back_step()
{
    QString result = model_->back_step();
    if(!result.isEmpty())
    {
        logs_browser_->append("Back step : " + result + "\n");
    }
}


void tower_presenter::create_new_model(unsigned int tower_height)
{
    tower_height_ = tower_height;

    if (state_output_label_ != nullptr)
    {
        init_print_params(state_output_label_->size().height(), state_output_label_->size().width());
    }
    else
    {
        init_print_params(DEFAULTHEIGHT, DEFAULTWIDTH);
    }

    if(model_ != nullptr)
    {
        QObject::disconnect(model_, SIGNAL(model_updated(std::array<std::vector<DISC_>, 3>)), this, SLOT(on_model_updated(std::array<std::vector<DISC_>, 3>)));
        delete model_;
    }

    model_ = new tower_model(tower_height);
    QObject::connect(model_, SIGNAL(model_updated(std::array<std::vector<DISC_>, 3>)), this, SLOT(on_model_updated(std::array<std::vector<DISC_>, 3>)));

    on_model_updated(model_->get_state());

    if(logs_browser_ != nullptr)
        logs_browser_->append("@Created new model (" + QString::number(tower_height_) + ")\n");
}

void tower_presenter::switch_timer()
{
    if(tmr_->isActive())
    {
        next_step_button_->setEnabled(true);
        back_step_button_->setEnabled(true);

        tmr_->stop();

        logs_browser_->append("Stop timer\n");
    }
    else
    {
        next_step_button_->setEnabled(false);
        back_step_button_->setEnabled(false);

        if (!to_end_)
            logs_browser_->append("Start timer (next)\n");
        else
            logs_browser_->append("Start timer (back)\n");

        tmr_->start();
    }
}

void tower_presenter::set_timer_time(unsigned int time)
{
    timer_time_ = (time == 0)? 1: time;

    tmr_->setInterval(timer_time_);
}

void tower_presenter::set_direction(bool to_end)
{
    logs_browser_->append("*switch deriection to : ");
    logs_browser_->append((to_end) ? "back\n" : "next");
    to_end_ = to_end;
}


void tower_presenter::save_logs(QString patch)
{
    QFile fileOut(patch); // Связываем объект с файлом fileout.txt
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream writeStream(&fileOut);
        writeStream << logs_browser_->toPlainText() << "\n";
        fileOut.close(); // Закрываем файл

        logs_browser_->append("Seave logs in file: " + patch+ "\n");
    }
    else{
        logs_browser_->append("#Error in save logs\n");
    }
}

void tower_presenter::clear_logs()
{
    logs_browser_->clear();
}


//--------------------------------set methods
void tower_presenter::set_new_model_button(QPushButton *button)
{
    new_model_button_ = button;
}

void tower_presenter::set_timer_button(QPushButton *button)
{
    timer_button_ = button;
}

void tower_presenter::set_next_step_button(QPushButton *button)
{
    next_step_button_ = button;
}

void tower_presenter::set_back_step_button(QPushButton *button)
{
    back_step_button_ = button;
}

void tower_presenter::set_state_output_label(QLabel *label)
{
    state_output_label_ = label;
    init_print_params(state_output_label_->size().height(), state_output_label_->size().width());
}

void tower_presenter::set_logs_browser(QTextBrowser *browser)
{
    logs_browser_ = browser;
}


//---------------------------slots

void tower_presenter::on_model_updated(std::array<std::vector<DISC_>, 3> model_state)
{
    auto image = new QImage(params_->image_width, params_->image_height, QImage::Format_RGB32);
    auto painter = new QPainter(image);

    drow_base(*painter);
    drow_state(*painter, model_state);

    state_output_label_ -> setPixmap(QPixmap::fromImage(*image));

    painter->end();
    delete image;
    delete painter;
}

//-------private slots
void tower_presenter::update_time()
{
    if(!to_end_)
        next_step();
    else
        back_step();
}


//------------------------------private methods
void tower_presenter::init_print_params(unsigned int height, unsigned int width)
{
    if(params_ != nullptr)
    {
        delete params_;
    }
    params_ = new PRINT_PARAMS_();

    if(tower_height_ + 2 >= height/3)
    {
        params_->image_height = (tower_height_ + 2) * 2;
        auto ad_part_width = (tower_height_ * 2  * 3)/5;
        params_->image_width  = ad_part_width * 7;
    }
    else
    {
        params_->image_width      = width;
        params_->image_height     = height;
    }

    params_->disc_height = params_->image_height / (tower_height_ + 2);
    auto part_width  = params_->image_width / 7;
    params_->column_width = part_width * 5 / 3;
    params_->space_horizontal = part_width * 2 / 4;

    params_->column_height    = params_->disc_height * tower_height_;
    params_->disc_width_delta = params_->column_width / tower_height_;
    params_->kernel_width     = params_->disc_width_delta;
    params_->kernel_height    = params_->image_height;
    params_->base_height      = params_->disc_height;
}

void tower_presenter::init_timer()
{
    tmr_ = new QTimer();
    tmr_->setInterval(timer_time_);
    connect(tmr_, SIGNAL(timeout()), this, SLOT(update_time()));
}

void tower_presenter::drow_base(QPainter &p)
{
    p.setPen(QPen(Qt::lightGray,1,Qt::SolidLine));
    p.setBrush(Qt::blue);

    for (auto i = 0; i < 3; i++)
    {
        auto column_left_margin = params_->space_horizontal + i * (params_->column_width + params_->space_horizontal);
        auto kernel_left_margin = column_left_margin + (params_->column_width - params_->kernel_width)/2;

        p.drawRect(kernel_left_margin, 0, params_->kernel_width, params_->kernel_height);
    }

    p.drawRect(0, params_->image_height - 1 - params_->base_height, params_->image_width, params_->base_height);
}

void tower_presenter::drow_state(QPainter &p, const std::array<std::vector<DISC_>, 3> &model_state)
{
    p.setBrush(Qt::darkGreen);
    for(auto i = 0; i < 3; i++)
    {
        auto botom_margin = params_->base_height;
        auto column_left_margin = params_->space_horizontal + i * (params_->column_width + params_->space_horizontal);
        for(auto j = 0; j < model_state[i].size(); j++)
        {
            auto curent_mass = model_state[i][j].mass;

            botom_margin += params_->disc_height;

            auto disc_width = curent_mass * params_->disc_width_delta;
            auto disc_left_margin = column_left_margin + (params_->column_width - disc_width)/2;

            p.drawRect(disc_left_margin, params_->image_height - botom_margin, disc_width , params_->disc_height);
        }
    }
}

