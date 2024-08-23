#include "reader.h"
enum class Status {
    ACTUAL,
    IRRELEVANT,
    BANNED
};

template <class T1, class T2> 
void assert(const T1& t1, const T2& t2, const std::string_view& t1_str, const std::string_view& t2_str, const std::string_view& file, const unsigned& line,const std::string_view& func, const std::string_view& hint="") { 
    if(t1 != t2) { 
        std::cout << file << "(" << line << "): " << func << ": "; 
        std::cout << "ASSERT(" << t1_str << ", " << t2_str << ") failed:\n\n"; 
        std::cout << t1_str << ": " << t1 << "\n" << t2_str << ": " << t2; 
        if (!hint.empty()) {
            std::cout << "\nHint: " << hint; 
        } 
        std::cout << std::endl;
    } 
} 
 
#define ASSERT(a, b) assert(a, b, #a, #b, __FILE__, __LINE__, __FUNCTION__) 
#define ASSERT_HINT(a, b, hint) assert(a, b, #a, #b, __FILE__, __LINE__, __FUNCTION__, hint) 

void testParse() {
    Json::Value jsonVec;
    Json::Reader reader;
    reader.parse("test.json", jsonVec);
    
    Json::Task t1(2025, 8, 11, 1, 38, "Implement a 'smart' calculator that treats every user error as a new calculation method.");
    Json::Task t2(2025, 8, 19, 10, 6, "Create a program that randomly replaces all spaces in a text with emojis.");
    Json::Task t3(2024, 3, 2, 0, 56, "Write a program that generates random conspiracy theories based on user input.");
    Json::Value testJsonVec = {t1, t2, t3};
    
    ASSERT_HINT(jsonVec, testJsonVec, "Incorrect json parsing.");
}

void testSort() {
    Json::Task t1(2025, 8, 11, 1, 38, "Implement a 'smart' calculator that treats every user error as a new calculation method.");
    Json::Task t2(2025, 8, 19, 10, 6, "Create a program that randomly replaces all spaces in a text with emojis.");
    Json::Task t3(2024, 3, 2, 0, 56, "Write a program that generates random conspiracy theories based on user input.");
    
    Json::Value jsonVec = {t1, t2, t3};
    sortTasks(jsonVec);
    
    Json::Value sortedJsonVec = {t3, t1, t2};
    
    ASSERT_HINT(jsonVec, sortedJsonVec, "Incorrect sort of Json::Value");
}

void testReader() {
    testParse();
    testSort();
}


int main() {
    /*Json::Value jsonVec;
    Json::Reader reader;

    reader.parse("test.json", jsonVec);*/
    // reader.printFile();
    // std::cout << "\n--------------------------------------------------------------------------------\nResult after parsing json file:\n\n";
    // reader.print(jsonVec);
    
    /*std::cout << "\n--------------------------------------------------------------------------------\nSorted jsonVec:\n\n";
    sortTasks(jsonVec);
    reader.print(jsonVec);*/
    
    /*Json::Task t1(2025, 8, 11, 1, 38, "Implement a 'smart' calculator that treats every user error as a new calculation method.");
    Json::Task t2(2025, 8, 19, 10, 6, "Create a program that randomly replaces all spaces in a text with emojis.");
    Json::Task t3(2024, 3, 2, 0, 56, "Write a program that generates random conspiracy theories based on user input.");
    Json::Value testJsonVec = {t1, t2, t3};
    ASSERT_HINT(jsonVec, testJsonVec, "Incorrect json parsing.");
    
    sortTasks(jsonVec);
    Json::Value sortedJsonVec = {t3, t1, t1};
    ASSERT_HINT(jsonVec, sortedJsonVec, "Incorrect sort of Json::Value");*/
    
    testReader();
    
    
    return 0;
}
