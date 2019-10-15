//
// Created by FredericDT on 10/5/2019.
//

#ifndef LEXC89_FILE_CHECK_H
#define LEXC89_FILE_CHECK_H

#include <ios>

namespace fdt {
    namespace lexical_parser {
        /**
         * A function to check wether the file provided is good,
         * exit with status code 2 when ths file is not good,
         * after printing the caller file name and line for debugging.
         *
         * @param file target file for checking
         * @param path target file path
         * @param _file please pass __FILE__ for it
         * @param _line please pass __LINE__ for it
         *
         */
        void check_file_good_or_exit_error(const std::ios &file, const std::string &path, const std::string &_file,
                                           const unsigned long _line);
    };
};

#endif //LEXC89_FILE_CHECK_H
