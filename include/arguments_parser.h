//
// Created by FredericDT on 10/5/2019.
//

#ifndef LEXC89_PARAMETER_PARSER_H
#define LEXC89_PARAMETER_PARSER_H

#include <cxxopts.hpp>

namespace fdt {
    namespace lexical_parser {
        class parse_result {
        public:
            std::string file_path;
            bool analysis;
            std::string output_file;
            bool verbose;

            parse_result(const std::string &file_path, bool analysis, const std::string &output_file, bool verbose) : file_path(file_path),
                                                                                            analysis(analysis),
                                                                                            output_file(output_file), verbose(verbose) {}

            parse_result() : file_path(""), analysis(false), output_file(""), verbose(false) {}
        };

        fdt::lexical_parser::parse_result parse_arguments(int argc, char **argv);
    }
}

#endif //LEXC89_PARAMETER_PARSER_H
