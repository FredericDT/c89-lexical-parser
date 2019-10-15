//
// Created by FredericDT on 10/5/2019.
//

#ifndef LEXC89_PARAMETER_PARSER_H
#define LEXC89_PARAMETER_PARSER_H

#include <cxxopts.hpp>

namespace fdt {
    namespace lexical_parser {
        /**
         * Result for argument parse function, in which, 
         * 'file_path' is the input target path,
         * 'analysis' decides wether print the analytical information to the output stream,
         * 'output_file' is the destination for the lexical parse result. Tokens or exception information, if presents, 
         * is in this file.
         * 'verbose' decides wether print the debug information to output stream.
         * 'include_comments' decides wether includes comments tokens in result vector
         *
         */
        class parse_result {
        public:
            std::string file_path;
            bool analysis;
            std::string output_file;
            bool verbose;
            bool include_comments;

            parse_result(const std::string &file_path, bool analysis, const std::string &output_file, bool verbose, bool include_comments) : file_path(file_path),
                                                                                            analysis(analysis),
                                                                                            output_file(output_file), verbose(verbose), include_comments(include_comments) {}

            parse_result() : file_path(""), analysis(false), output_file(""), verbose(false), include_comments(false) {}
        };

        /**
         * Actual cli argument parser function,
         * designed to parse the raw parameters from main function, then 
         * a structured parse result is returned.
         *
         * @param argc please pass the parameter just liek the raw input from main function
         * @param argv please pass the parameter just liek the raw input from main function
         * @return structured argument parse result
         */
        fdt::lexical_parser::parse_result parse_arguments(int argc, char **argv);
    }
}

#endif //LEXC89_PARAMETER_PARSER_H
