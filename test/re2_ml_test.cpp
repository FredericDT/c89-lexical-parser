//
// Created by FredericDT on 10/6/2019.
//

#include <iostream>

#include "re2/re2.h"

int main() {

    std::string s = "<a.h>";

    std::string r;

    const RE2 regex("^((<([^>]+)>)|(\"([^\"]+)\"))");
    std::cout << std::boolalpha << RE2::PartialMatch(s, regex, &r) << std::endl;

    std::cout << r << std::endl;


}