//
// Created by FredericDT on 10/5/2019.
//

#ifndef LEXC89_LEXICAL_PARSER_H
#define LEXC89_LEXICAL_PARSER_H

#include <string>
#include <vector>

namespace fdt {
    namespace lexical_parser {
        class lexical_element {
        public:
            enum type {
                KEYWORD,
                IDENTIFIER,
                FLOATING_CONSTANT,
                INTEGER_CONSTANT,
                ENUMERATION_CONSTANT,
                CHARACTER_CONSTANT,
                STRING_LITERAL,
                OPERATOR,
                PUNCTUATOR,
                HEADER_NAME,
                PREPROESSING_NUMBER,
                COMMENT,
                UNKNOWN
            };

            const std::string to_string() const;

            enum type get_type() const;

            lexical_element &set_type(enum type type);

            const std::string &get_raw() const;

            lexical_element &set_raw(const std::string &raw);

            const std::string &get_property() const;

            lexical_element &set_property(const std::string &property);

        private:
            std::string raw;
            std::string property;
            enum type type;
        };

        class lexical_parse_result {
        private:
            std::vector<lexical_element> lexical_element_vector;
            bool exception_occur;
            long exception_line;
            long exception_char_index;
            long total_lines;
            long total_chars;

        public:
            const std::vector<lexical_element> &get_lexical_element_vector() const;

            lexical_parse_result& set_lexical_element_vector(const std::vector<lexical_element> &lexical_element_vector);

            bool is_exception_occur() const;

            lexical_parse_result& set_exception_occur(bool exception_occur);

            long get_exception_line() const;

            lexical_parse_result& set_exception_line(long exception_line);

            long get_exception_char_index() const;

            lexical_parse_result& set_exception_char_index(long exception_char_index);

            long get_total_lines() const;

            lexical_parse_result& set_total_lines(long total_lines);

            long get_total_chars() const;

            lexical_parse_result& set_total_chars(long total_chars);

            lexical_parse_result(const std::vector<lexical_element> &lexical_element_vector, bool exception_occur,
                                 long exception_line, long exception_char_index, long total_lines, long total_chars);

            lexical_parse_result();
        };

        const std::string to_string(const enum lexical_element::type &v);

        lexical_parse_result lexical_parse(std::istream &input_stream, bool verbose);

        const std::string to_string(const std::vector<lexical_element> &v);

        void output_lexical_element_vector_to_output_stream(const std::vector<lexical_element> &v, std::ostream &stream);

        void output_lexical_parse_result_to_output_stream(const lexical_parse_result &v, std::ostream &stream);

        class automata_state {
        public:
            enum state {
                NEUTRAL, COMMENT, INCLUDE
            };
        };

    };

};

#endif //LEXC89_LEXICAL_PARSER_H
