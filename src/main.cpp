#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <map>
#include <charconv> // std::from_chars (либо не через string_view принимать)
#include <utility> // std::pair (возврат значений из функции)
// #include "json.h"

void createJson(const std::string&, std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>&);

void readJson(const std::string&, std::string&);

void fillJsonMap(std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>&,
	std::string::iterator, std::string::iterator);

void retrieveTask(std::string::iterator&, const std::string::iterator&, 
    std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>&);
    
std::pair<std::string_view, std::string_view> retrievePair(std::string::iterator&, const std::string::iterator&);

size_t toNumber(std::string_view);

void printJsonMap(const std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>&);

void error();


int main() {
    std::string string;
    readJson("tasks_ex.json", string);
    
    std::cout << string << "\n--------------------------------------------------------------------------------\n";
    
    std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>> jsonMap;
    fillJsonMap(jsonMap, string.begin(), string.end());

    std::cout << "\nResult after reading json file:\n\n";
    printJsonMap(jsonMap);
    
    // createJson("new_json", jsonMap);
    
    return 0;
}

void createJson(const std::string& filename, std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>& jsonMap){
    // По функциям раскид потом
    std::ofstream stream(filename);
    /*if(!stream) {
        std::cerr << "Unable to open the json file" << std::endl;
        exit(1);
    }
    // string = std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));*/
    stream.close();
}


void readJson(const std::string& filename, std::string& string){
    std::ifstream stream(filename);
    if(!stream) {
        std::cerr << "Unable to open the json file" << std::endl;
        exit(1);
    }
    string = std::string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    stream.close();
}


void fillJsonMap(std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>& jsonMap, 
    std::string::iterator it, std::string::iterator end){

    if(*it != '{') error();
    ++it;
    while(*it == ' ' || *it == '\n') ++it;
    
    while(it != end)
        retrieveTask(it, end, jsonMap); 
}


void retrieveTask(std::string::iterator& it, const std::string::iterator& end, 
    std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>& jsonMap) {			
    if(*it != '"') error();
 
    std::string::iterator first(++it);

    while(*it != '"'){
        if(it == end) error();
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

    auto [key_task, task] = retrievePair(it, end);
    if(key_task != "task") error();

    // std::cout << year << " -> " << month << " -> " << day << " -> " << time << " -> " << task << std::endl << std::endl;
    jsonMap[year][month][day].emplace(time, task);
}


std::pair<std::string_view, std::string_view> retrievePair(std::string::iterator& it, const std::string::iterator& end) {
    if(*it != '"') error();

    std::string::iterator first(++it);
    while(*it != '"'){
        if(it == end) error();
        ++it;
    }

    std::string_view key(first, it++), value;

    while(*it == ':' || *it == ' ')
        ++it;

    if(*it == '"'){ // Значение - строка
        first = ++it;
        while(*it != '"'){
            if(it == end) error();
            if(*it == '\\') ++it; // Экранированным мб только значение, причем именно строка (ключи заранее знаем и сверяем их в retrieveTask)
            ++it;
        }
        value = std::string_view(first, it++);
    } else { // Значение - size_t
        first = it;
        while(*it != ','){
            if(it == end) error();
            ++it;
        }
        value = std::string_view(first, it);
    }

    while(*it == ' ' || *it == '\n' || *it == ',' || *it == ']' || *it == '}')
        ++it;

    // std::cout << "\n\"" << key << "\": \"" << value << "\"\n";
    return std::make_pair(key, value);
}


size_t toNumber(std::string_view string) {
    size_t number = 0;
    if(std::from_chars(string.data(), string.data() + string.size(), number).ec == std::errc())
        return number;
    std::cerr << "Invalid conversion to size_t." << std::endl;
    exit(1);
}


void printJsonMap(const std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>& jsonMap) {
    std::cout <<  "year -> month -> day -> time -> task" << std::endl;
    for(const auto& [year, umap] : jsonMap)
        for(const auto& [month, umap] : umap)
            for(const auto& [day, umap] : umap)
                for(const auto& [time, task] : umap)
                    std::cout << year << " -> " << month << " -> " << day << " -> " << time << " -> " << task << std::endl;
}


void error(){
    std::cerr << "Incorrect format of the json file." << std::endl;
    exit(1); 
}

