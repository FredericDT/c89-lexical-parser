//
// Created by FredericDT on 10/5/2019.
//

#include <iostream>
#include <sstream>

int main(int argc, char **argv) {
    std::stringstream ss("a\n"
                         "b "
                         "c\n"
                         "1 "
                         "2\n"
                         "3 "
                         "5\n");

    std::string a;
    ss >> a;

    std::cout << a << std::endl;



    std::string b;
    ss >> b;

    std::cout << b << std::endl;

    char t;
    t = ss.get();
    std::cout << t;

    t = ss.get();
    std::cout << t;

    return 0;
}