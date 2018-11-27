#include "towermodel.h"

tower_model::tower_model(size_t tower_size)
{
    tower_size_ = tower_size;
    stack_ = std::vector<DISC_STATE_>();
    current_state_ = std::array<std::vector<DISC_>, 3>();

    for (size_t i = 0 ; i < tower_size; i++)
    {
        auto mass = tower_size - i;
        DISC_ disc = DISC_(mass);
        current_state_[0].push_back(disc);
    }

    DISC_STATE_ state = DISC_STATE_(tower_size, 1, 3, 2);

    stack_.push_back(state);
}

void tower_model::print_current_state()
{
    for(size_t i = 0; i < 3; i++)
    {
        std::cout << "column #" << i + 1 << "\n";
        if(current_state_[i].size() != 0)
        {
            for(int j = current_state_[i].size() - 1; j >= 0 ; --j)
            {
                std::cout << current_state_[i][j].mass << "\n";
            }
        }else
        {
            std::cout << "-------\n";
        }
    }
}

std::array<std::vector<DISC_>, 3> tower_model::get_state()
{
    return current_state_;
}


QString tower_model::next_step()
{
    QString model_is_updated = "";

    auto is_end = false;
    for(auto i = 0;  i < stack_.size(); i++)
    {
        if(stack_[i].step != 2)
        {
            is_end = true;
            break;
        }
    }

    while (model_is_updated.isEmpty() && stack_.size() != 0 && is_end)
    {
        DISC_STATE_ &state = stack_.back();

        if(state.step == 0)
        {
            state.step = 1;

            if(state.n != 1)
            {
                DISC_STATE_ newState = DISC_STATE_(state.n - 1, state.src, state.tmp, state.dest);
                stack_.push_back(newState);
            }
        }
        else if (state.step == 1)
        {
            update_model(state.src, state.dest);
            model_is_updated = "[" + QString::number(state.src) + "]->[" + QString::number(state.dest)+"]";
            state.step = 2;

            if(state.n != 1)
            {
                DISC_STATE_ newState = DISC_STATE_(state.n - 1, state.tmp, state.dest, state.src);
                stack_.push_back(newState);
            }

        }
        else if (state.step == 2)
        {
            stack_.pop_back();
        }

    }


    return  model_is_updated;
}

QString tower_model::back_step()
{
    QString model_is_updated = "";

    bool is_end = false;

    if (current_state_[0].size() == tower_size_)
    {
        is_end = true;
    }


    while (model_is_updated.isEmpty() && stack_.size() != 0 && !is_end)
    {
        DISC_STATE_ &state = stack_.back();

        if (state.step == 2)
        {
            state.step = 1;
            model_is_updated = model_is_updated = "[" + QString::number(state.dest) + "]->[" + QString::number(state.src)+"]";
            update_model(state.dest, state.src);

        }
        else if (state.step == 1)
        {

            auto copy_n = state.n;

            bool is_happened = false;
            auto state_copy = state;
            bool first_generate = true;
            for (auto n = copy_n; n > 1; n--)
            {
                if(first_generate)
                {
                    DISC_STATE_ newState = DISC_STATE_(state_copy.n - 1, state_copy.src, state_copy.tmp, state_copy.dest, 2);
                    stack_.push_back(newState);
                }
                else
                {
                    DISC_STATE_ newState = DISC_STATE_(state_copy.n - 1, state_copy.tmp, state_copy.dest, state_copy.src, 2);
                    stack_.push_back(newState);
                }

                state_copy = stack_.back();
                first_generate = false;
                is_happened = true;
            }

            if(!is_happened)
                --state.step;

        }
        else if (state.step == 0)
        {

            stack_.pop_back();

            try {
                auto &two_state = stack_.back();

                if (two_state.step != 2)
                     two_state.step = two_state.step - 1;

            } catch (...)
            {

            }

        }
    }

    return  model_is_updated;
}

void tower_model::update_model(size_t src, size_t dest)
{
    DISC_ &moved_disc = current_state_[src-1].back();
    current_state_[src-1].pop_back();

    current_state_[dest-1].push_back(moved_disc);

    emit model_updated(current_state_);
}
