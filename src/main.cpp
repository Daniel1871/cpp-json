#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
// #include <unordered_map>
// #include "json.h"
#include <iterator>

void read_object(std::string_view, std::string::iterator&);
void retrieve_pair(std::string_view, std::string::iterator&);
void to_number(std::string_view);

int main() {
    std::ifstream stream("file.json");
    if(!stream) {
    	std::cerr << "Impossible to open the file" << std::endl;
    	return 1;
    }
	
    std::string string((std::istream_iterator<char>(stream)), (std::istream_iterator<char>()));
    std::cout << string;
    
    std::string::iterator it = string.begin();
    while(it != string.end()) {   	
    	if(*it == '{')
    		read_object(string, ++it);
    	// Иначе это сразу объект - ПОТОМ
    	return 2;
   
    }
     
    
    stream.close();
    return 0;
}


void read_object(std::string_view string, std::string::iterator& it) {			
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
	
	if(*it == ':')
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
		while(*it != ',' && *it != '}') 
			value += *it++;
		
		if(value[0] == '-' || std::isdigit(value[0])){ 
			std::cout  << "\n!!!";
			to_number(value);
			
		}
	}
	
	// Проходим через запятую при ее наличии
	if(*it == ',')
		++it;
	
	std::cout << "\n\nKey |" << key << "|\nValue |" << value << "|";
}

void to_number(std::string_view string){
	// Работаем
	//int a = std::stoi(value);
			//std::cout  << a <<"\n";
}

