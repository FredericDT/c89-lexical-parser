//
// Created by FredericDT on 10/5/2019.
//

#ifndef LEXC89_FILE_CHECK_H
#define LEXC89_FILE_CHECK_H

#include <ios>

namespace fdt {
    namespace lexical_parser {
        void check_file_good_or_exit_error(const std::ios &file, const std::string &path);
    };
};

#endif //LEXC89_FILE_CHECK_H
