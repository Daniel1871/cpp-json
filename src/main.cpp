#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
// #include <unordered_map>
// #include "json.h"

void read_object(std::string_view, std::string::iterator&);
void retrieve_pair(std::string_view, std::string::iterator&);
void to_number(std::string_view);

int main() {
    std::ifstream stream("file.json");
    if(!stream) {
    	std::cerr << "Impossible to open the file" << std::endl;
    	return 1;
    }
    
    
    std::string line, string;
    while(std::getline(stream, line))
    	string += line;
    std::cout << string;
    
    
    std::string::iterator it = string.begin();
    while(it != string.end()) {
    	while(*it == ' ')
			++it;
    	
    	if(*it == '{')
    		read_object(string, ++it);
    	// Иначе это сразу объект - ПОТОМ
    	return 2;
   
    }
     
    
    stream.close();
    return 0;
}


void read_object(std::string_view string, std::string::iterator& it) {	
	while(*it == ' ')
		++it;
			
	while(*it != '}') {
		retrieve_pair(string, it); // Не пробел и не } -> " -> будем извлекать пару ключ-значение
	}
	
}


void retrieve_pair(std::string_view string, std::string::iterator& it) {
	std::string key, value;
	
	++it;
	while(*it != '"'){
		if(*it == '\\')
    		key += *it++; // При экранировании оставляем \ и экранированный символ	
		key += *it++;
	}
	++it;
	
	while(*it == ':' || *it == ' ')
		++it;
	
	if(*it == '"'){
		++it;
		while(*it != '"'){
			if(*it == '\\')
    			value += *it++;
			value += *it++;
		}
		++it;
	} else {
		while(*it != ' ' && *it != ',' && *it != '}') 
			value += *it++;
		
		if(value[0] == '-' || std::isdigit(value[0])){ 
			std::cout  << "\n!!!";
			to_number(value);
			
		}
	}
	
	// Проходим через запятую при ее наличии
	while(*it == ' ' || *it == ',')
		++it;
	
	std::cout << "\n\nKey |" << key << "|\nValue |" << value << "|";
}

void to_number(std::string_view string){
	// Работаем
	//int a = std::stoi(value);
			//std::cout  << a <<"\n";
}

