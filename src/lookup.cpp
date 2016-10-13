#include "lookup.h"

#include <iostream>

bool lookup(const Dictionary& d, Template& t) {
    if (t) {
        t.log("Answer", std::cout);
        return true;
    }

    std::string pattern = t.next();

    Dictionary::Cursor c;
    while(d.get(pattern, c)) {
        t.place(*c);

// Uncomment if you want to stop aftre first match
//        if (lookup(d,t)) {
//            return true;
//        } else {
//            t.rollback();
//        }

        lookup(d, t);
        t.rollback();
    }

    return false;
}