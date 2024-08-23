// reader.h
#ifndef READER_H
#define READER_H


#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <charconv> // std::from_chars (либо не через string_view принимать)
#include <utility> // std::pair (возврат значений из функции)
#include "reader.h"

namespace Json {
    struct Task { 
        size_t year = 0;
        size_t month = 0;
        size_t day = 0;
        size_t hour = 0;
        size_t min = 0;
        std::string description;

        Task() {}
        Task(size_t year, size_t month, size_t day, size_t hour, size_t min, std::string_view description) : year(year), month(month), day(day), hour(hour), min(min), description(description) {}

        bool operator<(const Task&);
        friend bool operator==(const Task& t1, const Task& t2) { return t1.year == t2.year && t1.month == t2.month && t1.day == t2.day && t1.hour == t2.hour && t1.min == t2.min && t1.description == t2.description; }
    };

    using Value = std::vector<Task>;

    std::ostream &operator<<(std::ostream &, const Task&);
    std::ostream &operator<<(std::ostream &, const Value&);
    
    class Reader {
    private:
        std::string string;
        std::string::iterator it;
        
        void readJson(const std::string&);
        void retrieveTask(Value&);
        std::pair<std::string_view, std::string_view> retrievePair();

        size_t toNumber(std::string_view) const;
        
        void error() const;

    public:
    	void parse(const std::string&, Value&);
        void printFile() const { std::cout << string << std::endl; }
        void print(Value& jsonVec) const { std::cout << jsonVec; }
    };    
}

void sortTasks(Json::Value&);

#endif // READER_H

