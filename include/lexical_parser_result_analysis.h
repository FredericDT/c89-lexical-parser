//
// Created by FredericDT on 10/7/2019.
//

#ifndef LEXC89_LEXICAL_PARSER_RESULT_ANALYSIS_H
#define LEXC89_LEXICAL_PARSER_RESULT_ANALYSIS_H

#include "lexical_parser.h"

namespace fdt {
    namespace lexical_parser {
        /**
         * Structure for analysis result
         * please build from only the class method "build_from_lexical_parse_result"
         *
         */
        class analysis_result {
        private:
            long total_line_count;
            long total_char_count;
            long lexical_element_type_count[lexical_element::TYPE_ENUM_LENGTH];

        public:

            analysis_result();

            /**
             * Getter function, returns count for total lines,
             * dispite ignoring the comments or not, those lines containing only the comments
             * are counted
             *
             * @return count for total lines
             */
            long get_total_line_count() const;

            analysis_result &set_total_line_count(long total_line_count);

            /**
             * Getter function, returns count for total characters,
             * dispite ignoring the comments or not, those characters containing only the comments
             * are counted
             *
             * @return count for total characters
             */
            long get_total_char_count() const;

            analysis_result &set_total_char_count(long total_char_count);

            /**
             * Getter function, returns an immutable long array,
             * containing counts for all types of lexical elements,
             * to access count result for specified type, pass its enum type as the index
             *
             * @see lexical_element::type
             *
             * @return immutable long array of count of all type lexical elements
             */
            const long *get_lexical_element_type_count() const;

            /**
             * Likes toString() method in Java,
             * this method is intended to generate printable string for char devices
             *
             * @return human friendly information
             */
            std::string to_string();

            /**
             * Builder function to build a analysis_result from lexical_parse_result
             *
             * @see analysis_result
             * @see lexical_parse_result
             *
             * @param v lexical parse result, to which, generating analytical information
             */
            static analysis_result build_from_lexical_parse_result(lexical_parser::lexical_parse_result & v);

            /**
             * Print string result from to_string() to ostream it provided
             *
             * @param stream output stream target
             */
            void print_to_ostream(std::ostream &stream);
        };
    };
};

#endif //LEXC89_LEXICAL_PARSER_RESULT_ANALYSIS_H
