// reader.cpp
#include "reader.h"


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

void sortTasks(Json::Value& jsonVec){
    std::sort(jsonVec.begin(), jsonVec.end());
}


void Json::Reader::readJson(const std::string &filename) {
    std::ifstream stream(filename);
    if(!stream) {
        std::cerr << "Unable to open the json file" << std::endl;

        exit(1);
    }

    string = std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	it = string.begin();
	
    stream.close();
}


void Json::Reader::parse(const std::string &filename, Json::Value& jsonVec) {
    readJson(filename);

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
