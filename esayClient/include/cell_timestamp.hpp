#ifndef _CELL_TIMESTAMP_
#define _CELL_TIMESTAMP_
#include <chrono>
using namespace std::chrono;
class cellTimeStamp {
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

cellTimeStamp::cellTimeStamp() {
    m_begin = high_resolution_clock::now();
}

cellTimeStamp::~cellTimeStamp() {
}

void cellTimeStamp::update() {
    m_begin = high_resolution_clock::now();
}

double cellTimeStamp::getElapsedSecond() {
    return getElapsedSecondInMilliSec() * 0.001;
}

double cellTimeStamp::getElapsedSecondInMilliSec() {
    return getElapsedSecondInMicroSec() * 0.001;
}

int64_t cellTimeStamp::getElapsedSecondInMicroSec() {
    return duration_cast<microseconds>(high_resolution_clock::now() - m_begin).count();
}

#endif