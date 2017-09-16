#ifndef DATAMEASUREMENT_H
#define DATAMEASUREMENT_H

#include <string>
#include "json.hpp"

using json = nlohmann::json;

class DataMeasurement {
    public:
        virtual std::string toString(std::string) const = 0;
        virtual std::string getHeader(std::string) const = 0;
        virtual json toJSON() const = 0;
};

#endif