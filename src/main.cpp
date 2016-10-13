#include <iostream>
#include <vector>

#include "dictionary.h"
#include "template.h"
#include "lookup.h"

void printHelp() {
    std::cout << "Usage:" << std::endl;
    std::cout << "\tblueprint <template> <dictionary>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printHelp();
        return 1;
    }
    std::string templatePath = argv[1];
    std::string dictionaryPath = argv[2];

    std::cout << "Reading dictionary from " << dictionaryPath << std::endl;
    Dictionary d = Dictionary::read(dictionaryPath);
    d.log(std::cout);
    std::cout << "Done." << std::endl;

    std::cout << "Reading template from " << templatePath << std::endl;
    Template t = Template::read(templatePath);
    t.log("Initial", std::cout);
    std::cout << "Done." << std::endl;

//    Dictionary::Cursor cursor;
//    std::string pattern = "?ake";
//    std::vector<std::string> all;
//
//    while(d.get(pattern, cursor)) {
//        all.push_back(*cursor);
//    }

    lookup(d, t);

    return 0;
}