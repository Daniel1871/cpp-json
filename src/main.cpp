#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <charconv> // std::from_chars (либо не через string_view принимать)
#include <utility> // std::pair (возврат значений из функции)
// #include "json.h"

// #include <algorithm>

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
    };

    using Value = std::vector<Task>;

    std::ostream &operator<<(std::ostream &, const Task&);
    std::ostream &operator<<(std::ostream &, const Value&);
    
    class Reader {
    private:
        std::string string;
        std::string::iterator it;
        
        void retrieveTask(Value&);
        std::pair<std::string_view, std::string_view> retrievePair();

        size_t toNumber(std::string_view) const;
        
        void error() const;

    public:
        void readJson(const std::string&);
        void printJson() const { std::cout << string << std::endl; }
        void parse(Value&);
        void print(Value& jsonVec) const { std::cout << jsonVec; }
    };
    
    
}

void sortTasks(Json::Value&);

int main() {
	Json::Value jsonVec;
    Json::Reader reader;
    reader.readJson("tasks_ex.json");
    reader.printJson();
    std::cout << "\n--------------------------------------------------------------------------------\nResult after reading json file:\n\n";
    reader.parse(jsonVec);
    reader.print(jsonVec);
    
    std::cout << "\n--------------------------------------------------------------------------------\n";
    sortTasks(jsonVec);
    reader.print(jsonVec);
        
    return 0;
}

void sortTasks(Json::Value& jsonVec){
    std::sort(jsonVec.begin(), jsonVec.end());
}


std::ostream &Json::operator<<(std::ostream &stream, const Task &task) { 
    stream << task.year << " -> " << task.month << " -> " << task.day << " -> " << task.hour << ":" << task.min << " -> " << task.description << std::endl; 

    return stream;
}

std::ostream &Json::operator<<(std::ostream &stream, const Json::Value& jsonVec) {
	for(const auto& task : jsonVec) {
		std::cout << task;
	}

	return stream;
}

void Json::Reader::readJson(const std::string &filename) {
    std::ifstream stream(filename);
    if(!stream) {
        std::cerr << "Unable to open the json file" << std::endl;

        exit(1);
    }

    string = std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));

    stream.close();
}


void Json::Reader::parse(Json::Value& jsonVec) {
    it = string.begin();

    if(*it != '{') {
    	error();
    }

    ++it;
    
    while(*it == ' ' || *it == '\n') { 
    	++it;
    }
    
    while(it != string.end()) {
    	retrieveTask(jsonVec);
    }
}


void Json::Reader::retrieveTask(Json::Value& jsonVec) {
    if(*it != '"') {
    	error();
    }

    std::string::iterator first(++it);

    while(*it != '"'){
        if(it == string.end()) {
        	error();
        }
        ++it;
    }

    std::string_view key(first, it++);

    while(*it == ':' || *it == ' ' || *it == '\n' || *it == '[' || *it == '{') {
    	++it;
    }

    size_t year = toNumber(key);
    if(year == 0) { // Мб можно добавить ограничение сверху или снизу
    	error();
    }
    
    auto [key_month, str_month] = retrievePair();
    size_t month = toNumber(str_month);
    if(key_month != "month" || month == 0 || month > 12) { 
    	error();
    } 

    auto [key_day, str_day] = retrievePair();
    size_t day = toNumber(str_day);
    if(key_day != "day" || day == 0 || day > 31) { // Для дня можно сделать жестче проверку (учитывая номер месяца)
    	error();
    } 
    
    auto [key_time, time] = retrievePair();
    if(key_time != "time" || time.length() != 5) {
    	error();
    }
    size_t hour = toNumber(time.substr(0, 2)), min = toNumber(time.substr(3, 2));
    if(time[2] != ':' || hour > 23 || min > 59) {
    	error();
    }

    auto [key_description, description] = retrievePair();
    if(key_description != "task") {
    	error();
    }

    // std::cout << year << " -> " << month << " -> " << day << " -> " << time << " -> " << description << std::endl << std::endl;
    jsonVec.push_back(Task(year, month, day, hour, min, description));
}


std::pair<std::string_view, std::string_view> Json::Reader::retrievePair() {
    if(*it != '"') {
    	error();
    }

    std::string::iterator first(++it);

    while(*it != '"'){
        if(it == string.end()) {
        	error();
        }
        ++it;
    }

    std::string_view key(first, it++), value;

    while(*it == ':' || *it == ' ') {
    	++it;
    }

    if(*it == '"'){ // Значение - строка
        first = ++it;
        while(*it != '"'){
            if(it == string.end()) {
            	error();
            }
            if(*it == '\\') { // Экранированным мб только значение, причем именно строка (ключи заранее знаем и сверяем их в retrieveTask)
            	++it;
            } 
            ++it;
        }
        value = std::string_view(first, it++);
    } else { // Значение - size_t
        first = it;

        while(*it != ','){
            if(it == string.end()) {
            	error();
            }
            ++it;
        }

        value = std::string_view(first, it);
    }

    while(*it == ' ' || *it == '\n' || *it == ',' || *it == ']' || *it == '}') {
    	++it;
    }
    // std::cout << "\n\"" << key << "\": \"" << value << "\"\n";
    return std::make_pair(key, value);
}


size_t Json::Reader::toNumber(std::string_view string) const {
    size_t number = 0;
    if(std::from_chars(string.data(), string.data() + string.size(), number).ec == std::errc()) {
    	return number;
    }

    std::cerr << "Invalid conversion to size_t." << std::endl;
    
    exit(1);
}


void Json::Reader::error() const {
    std::cerr << "Incorrect format of the json file." << std::endl;

    exit(1); 
}


bool Json::Task::operator<(const Task& task){ 
    if(year < task.year) {
    	return true;
    }
    if(year > task.year) {
    	return false;
    }
    
    if(month < task.month) {
    	return true;
    }
    if(month > task.month) {
    	return false; 
    }
    
    if(day < task.day) {
    	return true; 
    }
    if(day > task.day) {
    	return false; 
    }
    
    if(hour < task.hour) {
    	return true; 
    }
    if(hour > task.hour) {
    	return false; 
    }
    
    if(min < task.min) {
    	return true; 
    }
    if(min > task.min) {
    	return false; 
    }
    
    return false;
}


/*void createJson(const std::string& filename) {
    // По функциям раскид потом
    std::ofstream stream(filename);
    if(!stream) {
        std::cerr << "Unable to open the json file" << std::endl;
        exit(1);
    }
    // string = std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    stream.close();
}*/


