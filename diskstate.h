#pragma once


struct DISC_STATE_
{
    unsigned int n;
    unsigned int src;
    unsigned int dest;
    unsigned int tmp;
    unsigned int step;

    DISC_STATE_(unsigned int n, unsigned int src, unsigned int dest, unsigned int tmp, unsigned int step = 0)
    {
        this->n = n;
        this->src = src;
        this->dest = dest;
        this->tmp = tmp;
        this->step = step;
    }

    DISC_STATE_()
    {
        n = 0;
        src = 0;
        dest = 0;
        tmp = 0;
        step = 0;
    }

};

