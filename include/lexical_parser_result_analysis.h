//
// Created by FredericDT on 10/7/2019.
//

#ifndef LEXC89_LEXICAL_PARSER_RESULT_ANALYSIS_H
#define LEXC89_LEXICAL_PARSER_RESULT_ANALYSIS_H

#include "lexical_parser.h"

namespace fdt {
    namespace lexical_parser {
        class analysis_result {
        private:
            long total_line_count;
            long total_char_count;
            long lexical_element_type_count[lexical_element::TYPE_ENUM_LENGTH];

        public:

            analysis_result();

            long get_total_line_count() const;

            analysis_result &set_total_line_count(long total_line_count);

            long get_total_char_count() const;

            analysis_result &set_total_char_count(long total_char_count);

            const long *get_lexical_element_type_count() const;

            std::string to_string();

            static analysis_result build_from_lexical_parse_result(lexical_parser::lexical_parse_result & v);

            void print_to_ostream(std::ostream &stream);
        };
    };
};

#endif //LEXC89_LEXICAL_PARSER_RESULT_ANALYSIS_H
