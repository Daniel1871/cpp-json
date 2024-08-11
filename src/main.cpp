#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
// #include <unordered_map>
// #include "json.h"

#include <charconv> // std::from_chars (либо не через string_view принимать)


void read_object(std::string_view, std::string::iterator&);
void retrieve_pair(std::string_view, std::string::iterator&);
void to_number(std::string_view);

int main() {
    std::ifstream stream("file.json");
    if(!stream) {
    	std::cerr << "Impossible to open the file" << std::endl;
    	return 1;
    }
    
    std::string string((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    std::cout << string << "\n-----------------------------\n";
        
    std::string::iterator it = string.begin();
    
    while(it != string.end()) {
    	while(*it == ' ' || *it == '\n')
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
	while(*it == ' ' || *it == '\n')
		++it;
			
	while(*it != '}') {
		retrieve_pair(string, it); // Не пробел и не } -> " -> будем извлекать пару ключ-значение
	}	
}


void retrieve_pair(std::string_view string, std::string::iterator& it) {
	std::string::iterator first(++it);
	while(*it != '"'){
		if(*it == '\\')
    		it++; // При экранировании оставляем \ и экранированный символ
		it++;
	}
	std::string_view key(first, it++), value;
	
	while(*it == ':' || *it == ' ' || *it == '\n')
		++it;
		
	if(*it == '"'){		
		first = ++it;
		while(*it != '"'){
			if(*it == '\\')
    			it++;
			it++;
		}
		value = std::string_view(first, it++);
	} else {
	    first = it;
		while(*it != ' ' && *it != '\n' && *it != ',' && *it != '}') 
			it++;
		value = std::string_view(first, it);
		
		if(value[0] == '-' || value[0] == '+' || std::isdigit(value[0])){ 
			to_number(value);
		}
	}
	
	// Проходим через запятую при ее наличии
	while(*it == ' ' || *it == '\n' || *it == ',')
		++it;
	
	std::cout << "\n\"" << key << "\": \"" << value << "\"\n";
}

void to_number(std::string_view string) {
    if(string.find('.') == std::string_view::npos){
        int number = 0;
        if(std::from_chars(string.data(), string.data() + string.size(), number).ec == std::errc()){
        	// ДЕЙСТВИЕ!!!
        } else 
        	std::cerr << "Invalid conversion to int" << std::endl;
    } else {
        double number = 0.0;
        if(std::from_chars(string.data(), string.data() + string.size(), number).ec == std::errc()) {
        	// ДЕЙСТВИЕ!!!
        } else 
        	std::cerr << "Invalid conversion to double" << std::endl;
    }
}

