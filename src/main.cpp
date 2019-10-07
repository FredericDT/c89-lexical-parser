#include <iostream>
#include <fstream>

#include "lexical_parser.h"
#include "cxxopts.hpp"
#include "arguments_parser.h"
#include "file_check.h"

int main(int argc, char *argv[]) {
    fdt::lexical_parser::parse_result arguments_parse_result = fdt::lexical_parser::parse_arguments(argc, argv);

    std::ifstream input_file = std::ifstream(arguments_parse_result.file_path);
    fdt::lexical_parser::check_file_good_or_exit_error(input_file, arguments_parse_result.file_path);

    fdt::lexical_parser::lexical_parse_result lexical_parse_result = fdt::lexical_parser::lexical_parse(input_file,
                                                                                                        arguments_parse_result.verbose);
    bool output_file_provided = !arguments_parse_result.output_file.empty();

    if (output_file_provided) {
        std::ofstream output_file = std::ofstream(arguments_parse_result.output_file);
        fdt::lexical_parser::check_file_good_or_exit_error(output_file, arguments_parse_result.output_file);
        fdt::lexical_parser::output_lexical_parse_result_to_output_stream(
                lexical_parse_result,
                output_file);
        output_file.close();
    } else {
        fdt::lexical_parser::output_lexical_parse_result_to_output_stream(
                lexical_parse_result,
                std::cout);
    }


    return 0;
}
