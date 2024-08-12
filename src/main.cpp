#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <map>
#include <charconv> // std::from_chars (либо не через string_view принимать)
#include <utility> // std::pair (возврат значений из функции)
// #include "json.h"

void retrieve_task(std::string_view, std::string::iterator&, std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>&);
std::pair<std::string_view, std::string_view> retrieve_pair(std::string_view, std::string::iterator&);
size_t toNumber(std::string_view);
void printJsonMap(const std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>&);
void error();

int main() {
	std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>> jsonMap;
	
	// jsonMap[2024][5][29] = {{"05:02", "Go home"}, {"06:03", "Go to village"}};
	// jsonMap[2024][5][29].emplace("05:02", "Sidi doma");
	// printJsonMap(jsonMap);	
	
	
    std::ifstream stream("tasks_ex.json");
    if(!stream) {
    	std::cerr << "Impossible to open the file" << std::endl;
    	return 1;
    }
    
    std::string string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    std::cout << string << "\n--------------------------------------------------------------------------------\n";
        
    std::string::iterator it = string.begin();
    if(*it != '{') error;
    
    while(it != string.end())
    	retrieve_task(string, ++it, jsonMap);
    
    //printJsonMap(jsonMap);
    
    stream.close();
    return 0;
}


void retrieve_task(std::string_view string, std::string::iterator& it, std::unordered_map<size_t, std::unordered_map<size_t, std::unordered_map<size_t, std::multimap<std::string, std::string>>>>& jsonMap) {	
	while(*it == ' ' || *it == '\n')
		++it;
		
	if(*it != '"') error;
	
	std::string::iterator first(++it);
	while(*it != '"') ++it; // !
	std::string_view key(first, it++);
	
	while(*it == ':' || *it == ' ' || *it == '\n' || *it == '[' || *it == '{') ++it;
	
	size_t year = toNumber(key), month, day;
	std::string_view time, task;
	
	
	std::string_view keys[] = {"month", "day", "time", "task"};
	for(const auto& sv : keys) {
		auto [key, value] = retrieve_pair(string, it);
		
		if(key == "month") month = toNumber(value);
		else if(key == "day") day = toNumber(value);
		else if(key == "time") time = value;
		else if(key == "task") task = value;
		else error;
		// std::cout << "\n\"" << key << "\": \"" << value << "\"\n" << *it << *(it+1) << *(it+2) << std::endl;
	}
	
	// std::cout << key << year << " -> " << month << " -> " << day << " -> " << time << " -> " << task << std::endl;
	
	jsonMap[year][month][day].emplace(time, task);
	printJsonMap(jsonMap);
	exit(5);
}


std::pair<std::string_view, std::string_view> retrieve_pair(std::string_view string, std::string::iterator& it) {
	if(*it != '"') error;
	
	std::string::iterator first(++it);
	while(*it != '"') ++it; // !
	
	std::string_view key(first, it++), value;
	
	while(*it == ':' || *it == ' ')
		++it;
		
	if(*it == '"'){ // Значение - строка
		first = ++it;
		while(*it != '"') ++it;
		value = std::string_view(first, it++);
	} else { // Значение - size_t
		first = it;
		while(*it != ',') it++;
		value = std::string_view(first, it);
	}
	
	while(*it == ' ' || *it == '\n' || *it == ',' || *it == ']' || *it == '}')
		++it;
	
	// std::cout << "\n\"" << key << "\": \"" << value << *it<< *(it+1)<< "\"\n";
	return std::make_pair(key, value);
}

size_t toNumber(std::string_view string) {
    size_t number = 0;
    if(std::from_chars(string.data(), string.data() + string.size(), number).ec == std::errc())
    	return number;
  	std::cerr << "Invalid conversion to size_t" << std::endl;
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
	std::cerr << "Incorrect format of json file" << std::endl;
    exit(1); 
}

