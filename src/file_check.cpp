//
// Created by FredericDT on 10/5/2019.
//

#include "file_check.h"
#include <iostream>

namespace fdt {
    namespace lexical_parser {
        void check_file_good_or_exit_error(const std::ios &file, const std::string &path) {
            if (!file.good()) {
                std::cout << __FILE__ << ":" << __LINE__ << "\"" << path << "\" file not good" << std::endl;
                exit(2);
            }
        }
    };
};