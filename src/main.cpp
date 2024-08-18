#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <charconv> // std::from_chars (либо не через string_view принимать)
#include <utility> // std::pair (возврат значений из функции)
// #include "json.h"

#include <vector>

struct Task { 
  size_t year = 0;
  size_t month = 0;
  size_t day = 0;
  std::string time;
  std::string description;
  Task() {}
  Task(size_t year, size_t month, size_t day, std::string_view time, std::string_view description) : year(year), month(month), day(day), time(time), description(description) {}
  void print() const { std::cout << year << " -> " << month << " -> " << day << " -> " << time << " -> " << description << std::endl; }
};

void printTasks(std::vector<Task>& jsonVec){
	for(const auto& task : jsonVec) task.print();
}

void createJson(const std::string&, std::vector<Task>&);

void readJson(const std::string&, std::string&);

void filljsonVec(std::vector<Task>&, std::string::iterator, std::string::iterator);

void retrieveTask(std::string::iterator&, const std::string::iterator&, std::vector<Task>&);
    
std::pair<std::string_view, std::string_view> retrievePair(std::string::iterator&, const std::string::iterator&);

size_t toNumber(std::string_view);

void printjsonVec(const std::vector<Task>&);

void error();


int main() {
    std::string string;
    readJson("tasks_ex.json", string);
    
    std::cout << string << "\n--------------------------------------------------------------------------------\n";
    
    std::vector<Task> jsonVec;
    filljsonVec(jsonVec, string.begin(), string.end());

    std::cout << "\nResult after reading json file:\n\n";
    
    printTasks(jsonVec);
    
    // createJson("new_json", jsonVec);
    
    return 0;
}


void readJson(const std::string& filename, std::string& string) {
    std::ifstream stream(filename);
    if(!stream) {
        std::cerr << "Unable to open the json file" << std::endl;
        exit(1);
    }
    string = std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    stream.close();
}


void filljsonVec(std::vector<Task>& jsonVec, 
    std::string::iterator it, std::string::iterator end) {

    if(*it != '{') error();
    ++it;
    while(*it == ' ' || *it == '\n') ++it;
    while(it != end) retrieveTask(it, end, jsonVec); 
}


void retrieveTask(std::string::iterator& it, const std::string::iterator& end, std::vector<Task>& jsonVec) {	
    		
    if(*it != '"') error();
    std::string::iterator first(++it);
    while(*it != '"'){
        if(it == end)
        	error();
        ++it;
    }
    std::string_view key(first, it++);

    while(*it == ':' || *it == ' ' || *it == '\n' || *it == '[' || *it == '{') ++it;

    size_t year = toNumber(key);
    if(year == 0) error(); // Мб можно добавить ограничение сверху или сниpу
    
    auto [key_month, str_month] = retrievePair(it, end);
    size_t month = toNumber(str_month);
    if(key_month != "month" || month == 0 || month > 12) error(); 

    auto [key_day, str_day] = retrievePair(it, end);
    size_t day = toNumber(str_day);
    if(key_day != "day" || day == 0 || day > 31) error(); // Для дня можно сделать жестче проверку (учитывая номер месяца)
    
    auto [key_time, time] = retrievePair(it, end);
    if(key_time != "time" || time.length() != 5) error();
    size_t hour = toNumber(time.substr(0, 2)), min = toNumber(time.substr(3, 2));
    if(time[2] != ':' || hour > 23 || min > 59) error();

    auto [key_description, description] = retrievePair(it, end);
    if(key_description != "task") error();

    std::cout << year << " -> " << month << " -> " << day << " -> " << time << " -> " << description << std::endl << std::endl;
    jsonVec.push_back(Task(year, month, day, time, description));
}


std::pair<std::string_view, std::string_view> retrievePair(std::string::iterator& it, const std::string::iterator& end) {
    if(*it != '"') error();
    std::string::iterator first(++it);
    while(*it != '"'){
        if(it == end)
        	error();
        ++it;
    }
    std::string_view key(first, it++), value;

    while(*it == ':' || *it == ' ') ++it;

    if(*it == '"'){ // Значение - строка
        first = ++it;
        while(*it != '"'){
            if(it == end)
            	error();
            if(*it == '\\')
            	++it; // Экранированным мб только значение, причем именно строка (ключи заранее знаем и сверяем их в retrieveTask)
            ++it;
        }
        value = std::string_view(first, it++);
    } else { // Значение - size_t
        first = it;
        while(*it != ','){
            if(it == end)
            	error();
            ++it;
        }
        value = std::string_view(first, it);
    }

    while(*it == ' ' || *it == '\n' || *it == ',' || *it == ']' || *it == '}') ++it;
    // std::cout << "\n\"" << key << "\": \"" << value << "\"\n";
    return std::make_pair(key, value);
}


size_t toNumber(std::string_view string) {
    size_t number = 0;
    if(std::from_chars(string.data(), string.data() + string.size(), number).ec == std::errc()) return number;
    std::cerr << "Invalid conversion to size_t." << std::endl;
    exit(1);
}


void error(){
    std::cerr << "Incorrect format of the json file." << std::endl;
    exit(1); 
}


/*void createJson(const std::string& filename, std::vector<Task>& jsonVec) {
    // По функциям раскид потом
    std::ofstream stream(filename);
    if(!stream) {
        std::cerr << "Unable to open the json file" << std::endl;
        exit(1);
    }
    // string = std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    stream.close();
}*/

