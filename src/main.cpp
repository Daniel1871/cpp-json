#include "reader.h"
enum class Status {
    ACTUAL,
    IRRELEVANT,
    BANNED
};

template <class T1, class T2> 
void assert(const T1& t1, const T2& t2, const std::string_view& t1_str, const std::string_view& t2_str, const std::string_view& file, const unsigned& line,const std::string_view& func, const std::string_view& hint="") { 
    if (t1 != t2) { 
        std::cout << file << "(" << line << "): " << func << ": "; 
        std::cout << "ASSERT(" << t1_str << ", " << t2_str << ") failed: "; 
        std::cout << t1 << " != " << t2 << "."; 
        if (!hint.empty()) {
            std::cout << " Hint: " << hint; 
        } 
        std::cout << std::endl;
    } 
} 
 
#define ASSERT(a, b) assert(a, b, #a, #b, __FILE__, __LINE__, __FUNCTION__) 
#define ASSERT_HINT(a, b, hint) assert(a, b, #a, #b, __FILE__, __LINE__, __FUNCTION__, hint) 


int main() {
    /*Json::Value jsonVec;
    Json::Reader reader;

    reader.parse("tasks.json", jsonVec);
    reader.printFile();
    std::cout << "\n--------------------------------------------------------------------------------\nResult after reading json file:\n\n";
    reader.print(jsonVec);
    
    std::cout << "\n--------------------------------------------------------------------------------\nSorted jsonVec:\n\n";
    sortTasks(jsonVec);
    reader.print(jsonVec);*/
    
    ASSERT_HINT(1,2, "ggg");
    ASSERT(1,2.5);
    
    Json::Task t1(2023, 1, 1, 1, 1, "gg");
    Json::Task t2(2023, 1, 1, 1, 1, "ggg");
    std::cout << std::boolalpha << (t1 == t2);
    
    
        
    return 0;
}
