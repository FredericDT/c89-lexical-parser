//
// Created by FredericDT on 10/5/2019.
//

#ifndef LEXC89_LEXICAL_PARSER_H
#define LEXC89_LEXICAL_PARSER_H

#include <string>
#include <vector>

namespace fdt {
    namespace lexical_parser {
        /**
         * Logical storage of the lexical element, also known as token
         */
        class lexical_element {
        public:
            /**
             * Enumeration for classifying lexical elements into serval types
             * IMPORTANT: TYPE_ENUM_LENGTH should always be the last one of enum type
             */
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
                UNKNOWN,
                TYPE_ENUM_LENGTH // this value should be at the end of the type enumeration
            };

            /**
             * Helper function to build human readable information string about this element
             *
             * @return human friendly string contains information about this element
             */
            const std::string to_string() const;

            /**
             * Getter function
             *
             * @return type enumeration of this lexical element
             */
            enum type get_type() const;

            lexical_element &set_type(enum type type);

            /**
             * Getter funtion
             *
             * @return matched raw string from the input stream
             */
            const std::string &get_raw() const;

            lexical_element &set_raw(const std::string &raw);

            /**
             * Getter function
             *
             * @return property of this lexical element
             */
            const std::string &get_property() const;

            lexical_element &set_property(const std::string &property);

        private:
            std::string raw;
            std::string property;
            enum type type;
        };

        /**
         * Structured lexical parse result, when exception occured, the lexical element vector should not be trusted
         *
         */
        class lexical_parse_result {
        private:
            std::vector<lexical_element> lexical_element_vector;
            bool exception_occur;
            long exception_line;
            long exception_char_index;
            long total_lines;
            long total_chars;

        public:
            /**
             * Getter functio, get immutable lexical element vector
             * Notice! when exception occured, this result should not be trusted.
             *
             * @return lexical element vector
             */
            const std::vector<lexical_element> &get_lexical_element_vector() const;

            lexical_parse_result& set_lexical_element_vector(const std::vector<lexical_element> &lexical_element_vector);

            /**
             * Getter function, get wether exception is occured
             *
             * @return true when exceptions occur
             */
            bool is_exception_occur() const;

            lexical_parse_result& set_exception_occur(bool exception_occur);

            /**
             * Getter function, valid only when exception occured
             *
             * @return exception line position
             */
            long get_exception_line() const;

            lexical_parse_result& set_exception_line(long exception_line);

            /**
             * Getter function, valid only when exception occured
             *
             * @return exception character index of the line
             */
            long get_exception_char_index() const;

            lexical_parse_result& set_exception_char_index(long exception_char_index);

            /**
             * Getter function
             *
             * @return total line count of the provided input stream
             */
            long get_total_lines() const;

            lexical_parse_result& set_total_lines(long total_lines);

            /**
             * Getter function
             *
             * @return total character count of the provided input stream
             */
            long get_total_chars() const;

            lexical_parse_result& set_total_chars(long total_chars);

            lexical_parse_result(const std::vector<lexical_element> &lexical_element_vector, bool exception_occur,
                                 long exception_line, long exception_char_index, long total_lines, long total_chars);

            lexical_parse_result();
        };

        /**
         * Helper function to get human readable string from lexical element type enumeration
         *
         * @param v target lexical element type enumeration
         * @return human friendly string contains information about the lexical element type enumeration, which provided by parameter v
         */
        const std::string to_string(const enum lexical_element::type &v);

        /**
         * Actual parse function, call it to parse the input stream, with controll parameter verbose and include comments
         * a structured lexical parse result is returned
         *
         * @see lexical_parse_result
         *
         * @param input_stream provide a input stream to parse with
         * @param verbose controll wether printing debug information to stdout or not
         * @param include_comments controll wether includes comments in parse result vector
         * @return structured lexical parse result
         */
        lexical_parse_result lexical_parse(std::istream &input_stream, bool verbose, bool include_comments);

        /**
         * Get human friendly information about privided vector
         *
         * @param v provide lexical_element vactor
         * @return human friendly string
         */
        const std::string to_string(const std::vector<lexical_element> &v);

        /**
         * Helper function to print the to_string() result of provided vector to target ostream
         *
         * @param v provide vector
         * @param stream target output stream
         */
        void output_lexical_element_vector_to_output_stream(const std::vector<lexical_element> &v, std::ostream &stream);

        /**
         * Helper function to print the to_string() result of provided lexical_parse_result 
         * to target ostream
         *
         * @param v provide lexical_parse_result
         * @param stream target output stream
         */
        void output_lexical_parse_result_to_output_stream(const lexical_parse_result &v, std::ostream &stream);

        /**
         * Acts like a namespace, representing the current state of the parsing automata
         */
        class automata_state {
        public:
            enum state {
                NEUTRAL, COMMENT, INCLUDE
            };
        };

    };

};

#endif //LEXC89_LEXICAL_PARSER_H
