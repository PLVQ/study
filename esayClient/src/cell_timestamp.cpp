#include "cell_timestamp.h"

cellTimeStamp::cellTimeStamp()
{
    m_begin = high_resolution_clock::now();
}

cellTimeStamp::~cellTimeStamp()
{
}

void cellTimeStamp::update()
{
    m_begin = high_resolution_clock::now();
}

double cellTimeStamp::getElapsedSecond()
{
    return getElapsedSecondInMilliSec() * 0.001;
}

double cellTimeStamp::getElapsedSecondInMilliSec()
{
    return getElapsedSecondInMicroSec() * 0.001;
}

int64_t cellTimeStamp::getElapsedSecondInMicroSec()
{
    return duration_cast<microseconds>(high_resolution_clock::now() - m_begin).count();
}