#ifndef CUMULATIVE_MEASUREMENTS_H
#define CUMULATIVE_MEASUREMENTS_H

#include "processedmeasurement.h"
#include "cumulativethermalinfo.h"
#include "cumulativepackageinfo.h"
#include "cumulativebatteryinfo.h"
#include "cumulativediskinfo.h"
#include "cumulativenetworkinfo.h"

class CumulativeMeasurement: public ProcessedMeasurement 
{

    public:
        CumulativeMeasurement(const ProcessedMeasurement* pMeasurement, int interval);
        CumulativeMeasurement();
        void insert(const ProcessedMeasurement* pMeasurement, int interval) const;
        void setTotals(const CumulativeMeasurement* prev) const;
        ~CumulativeMeasurement();
};


#endif