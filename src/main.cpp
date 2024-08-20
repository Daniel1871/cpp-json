#include "reader.h"

int main() {
    Json::Value jsonVec;
    Json::Reader reader;

    reader.parse("tasks.json", jsonVec);
    reader.printFile();
    std::cout << "\n--------------------------------------------------------------------------------\nResult after reading json file:\n\n";
    reader.print(jsonVec);
    
    std::cout << "\n--------------------------------------------------------------------------------\nSorted jsonVec:\n\n";
    sortTasks(jsonVec);
    reader.print(jsonVec);
        
    return 0;
}
