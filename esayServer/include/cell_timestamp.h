#ifndef _CELL_TIMESTAMP_
#define _CELL_TIMESTAMP_

#include "server_message.h"

class cellTimeStamp
{
private:
    time_point<high_resolution_clock> m_begin;

public:
    cellTimeStamp();
    virtual ~cellTimeStamp();
    void update();
    double getElapsedSecond();
    double getElapsedSecondInMilliSec();
    int64_t getElapsedSecondInMicroSec();
};

#endif