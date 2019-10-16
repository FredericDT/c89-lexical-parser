//
// Created by FredericDT on 10/5/2019.
//

#include "lexical_parser.h"

#include "re2/re2.h"

#include <iostream>
#include <sstream>

namespace fdt {
    namespace lexical_parser {

        const std::string KEYWORD_STRING = "keyword";
        const std::string IDENTIFIER_STRING = "identifier";
        const std::string FLOATING_CONSTANT_STRING = "floating_constant";
        const std::string INTEGER_CONSTANT_STRING = "integer_constant";
        const std::string ENUMERATION_CONSTANT_STRING = "enumeration_constant";
        const std::string CHARACTER_CONSTANT_STRING = "character_constant";
        const std::string STRING_LITERAL_STRING = "string_literal";
        const std::string OPERATOR_STRING = "operator";
        const std::string PUNCTUATOR_STRING = "punctuator";
        const std::string HEADER_NAME = "header_name";
        const std::string PREPROESSING_NUMBER_STRING = "preprocessing_number";
        const std::string COMMENT_STRING = "comment";
        const std::string UNKNOWN_STRING = "unknown";

        inline const std::string to_string(const enum lexical_element::type &v) {
            switch (v) {
                case lexical_element::KEYWORD:
                    return KEYWORD_STRING;
                case lexical_element::IDENTIFIER:
                    return IDENTIFIER_STRING;
                case lexical_element::FLOATING_CONSTANT:
                    return FLOATING_CONSTANT_STRING;
                case lexical_element::INTEGER_CONSTANT:
                    return INTEGER_CONSTANT_STRING;
                case lexical_element::ENUMERATION_CONSTANT:
                    return ENUMERATION_CONSTANT_STRING;
                case lexical_element::CHARACTER_CONSTANT:
                    return CHARACTER_CONSTANT_STRING;
                case lexical_element::STRING_LITERAL:
                    return STRING_LITERAL_STRING;
                case lexical_element::OPERATOR:
                    return OPERATOR_STRING;
                case lexical_element::PUNCTUATOR:
                    return PUNCTUATOR_STRING;
                case lexical_element::HEADER_NAME:
                    return HEADER_NAME;
                case lexical_element::PREPROESSING_NUMBER:
                    return PREPROESSING_NUMBER_STRING;
                case lexical_element::COMMENT:
                    return COMMENT_STRING;
                default:
                    return UNKNOWN_STRING;
            }
        }

        const std::string lexical_element::to_string() const {
            return "<" + lexical_parser::to_string(this->type) + ", " + this->property + ">";
        }

        enum lexical_element::type lexical_element::get_type() const {
            return type;
        }

        lexical_element &lexical_element::set_type(enum lexical_element::type type) {
            lexical_element::type = type;
            return *this;
        }

        const std::string &lexical_element::get_raw() const {
            return raw;
        }

        lexical_element &lexical_element::set_raw(const std::string &raw) {
            lexical_element::raw = raw;
            return *this;
        }

        const std::string &lexical_element::get_property() const {
            return property;
        }

        lexical_element &lexical_element::set_property(const std::string &property) {
            lexical_element::property = property;
            return *this;
        }

        const RE2 KEYWORD_REGEX(
                "^(auto|break|case|char|const|continue|default|do|double|int|else|long|enum|register|extern|return|float|short|for|signed|goto|sizeof|if|static|struct|switch|typedef|union|unsigned|void|volatile|while)");

        const RE2 IDENTIFIER_REGEX("^([_a-zA-Z][_a-zA-Z0-9]*)");

        const RE2 FLOATING_CONSTANT_REGEX(
                "^((([0-9]*\\.[0-9]+|[0-9]+)([eE][+-]?[0-9]+)?|[0-9]+([eE][+-]?[0-9]+))([flFL])?)");

        const RE2 INTEGER_CONSTANT_REGEX(
                "^((([1-9][0-9]*)|(0[xX][0-9a-fA-F]+)|(0([0-7])*))(([uU][lL]?)|([lL][uU]?))?)");

        const RE2 ENUMERATION_CONSTANT_REGEX(
                "^([_a-zA-Z][_a-zA-Z0-9]*)"); // same as identifier, see reference http://www.open-std.org/Jtc1/sc22/wg14/www/docs/n1124.pdf

        const RE2 CHARACTER_CONSTANT_REGEX(
                "^(L?'([^'\\\n]|\\\\['\"?\\abfnrtv]|\\\\[0-7]{1,3}|\\\\x[0-9a-fA-F]+)')");

        const RE2 STRING_LITERAL_REGEX(
                "^(L?\"([^\"\\\\\\n]|(\\\\['\"?\\\\abfnrtv]|\\\\[0-7]{1,3}|\\\\x[0-9a-fA-F]+)+)*\")");

        const RE2 OPERATOR_REGEX(
                "^((\\[)|(\\])|(\\()|(\\))|(\\.)|->|(\\+\\+)|(--)|&|(\\*)|(\\+)|-|~|!|(sizeof)|(\\/)|%|(<<)|(>>)|<|>|(<=)|(>=)|(==)|(!=)|\\^|(\\|)|(&&)|(\\|\\|)|(\\?)|:|=|(\\*=)|(\\/=)|(%=)|(\\+=)|(-=)|(<<=)|(>>=)|(&=)|(\\^=)|\\|=|,|#{1,2})");

        const RE2 PUNCTUATOR_REGEX("^(\\[|\\]|\\(|\\)|\\{|\\}|\\*|,|:|=|;|\\.\\.\\.|#)");

        const RE2 HEADER_NAME_REGEX("^((<([^>]+)>)|(\"([^\"]+)\"))");

        const RE2 PREPROCESSING_NUMBER_REGEX("^([0-9]|\\.[0-9])([0-9a-zA-Z_]|e[+-]|E[+-])?");

        const RE2 COMMENT_START_REGEX("^(\\/\\*)");

        const RE2 COMMENT_REGEX("(?s)^(\\/\\*(.*)\\*\\/)");

        const RE2 INCLUDE_PREPROCESSING_REGEX("^(#include)");

        const RE2 UNKNOWN_REGEX("^(\\\\)");

        const std::string to_string(const std::vector<lexical_element> &v) {
            std::stringstream ss;
            for (auto i = v.begin(); i != v.end(); ++i) {
                ss << i->to_string() << std::endl;
            }
            return std::string(ss.str());
        }

        long get_identifier_id(std::map<std::string, long> &map, std::string &id) {
            if (!map[id]) {
                map[id] = map.size();
            }
            return map[id];
        }

        void parse_match_result(std::string &buffer, std::string &match, std::vector<lexical_element> &result,
                                lexical_element &e, enum lexical_element::type type,
                                std::map<std::string, long> &identifier_map, bool &excep,
                                bool verbose, long &line_count, long &char_count, bool include_comments) {
            excep = false;
            e.set_type(type);

            std::string property = match;
            if (type == lexical_element::IDENTIFIER) {
//                property = "id" + std::to_string(get_identifier_id(identifier_map, match));
                property = match;
            }

            e.set_property(property);
            e.set_raw(match);

            if (type == lexical_element::COMMENT) {
                long tmp_line_count = 0;
                long last_line_char_index = 0;
                for (long i = 0; i < match.length(); ++i) {
                    if (match[i] == '\n') {
                        last_line_char_index = i + 1;
                        ++tmp_line_count;
                    }
                }
                char_count = match.length() - last_line_char_index;
                line_count += tmp_line_count;
            } else {
                char_count += match.length();
            }

            buffer = buffer.substr(match.length(), buffer.length() - match.length());
            while (buffer[0] == '\n' || buffer[0] == '\t' || buffer[0] == ' ') {
                if (buffer[0] == '\n') {
                    char_count = 0;
                    ++line_count;
                } else {
                    ++char_count;
                }
                buffer = buffer.substr(1);
            }

            if (type != lexical_element::COMMENT || include_comments) {
                result.emplace_back(e);
            }

            if (verbose) {
                std::cout << __FILE__ << ":" << __LINE__ << " match:" << match << std::endl;
                std::cout << __FILE__ << ":" << __LINE__ << " e:" << e.to_string() << std::endl;
            }

        }

        lexical_parse_result lexical_parse(std::istream &input_stream, bool verbose, bool include_comments) {
            lexical_parse_result parse_result = lexical_parse_result();
            std::vector<lexical_element> lexical_element_vector = std::vector<lexical_element>();
            long parsed_line_count = 1;
            long parsed_char_count = 0;
            long total_line_count = 0;
            long total_char_count = 0;

            bool excep = false;
            std::string buffer;

            std::map<std::string, long> identifier_map = std::map<std::string, long>();

            enum automata_state::state automata_state = automata_state::NEUTRAL;

            do {
                if (input_stream) {
                    int t = input_stream.get();
                    switch (t) {
                        case EOF:
                            break;
                        case '\n':
                            ++total_char_count;
                            ++total_line_count;
                            if (!buffer.empty()) {
                                buffer += (char) t;
                            } else {
                                ++parsed_line_count;
                                parsed_char_count = 0;
                            }
                            break;
                        case ' ':
                        case '\t':
                            ++total_char_count;
                            if (!buffer.empty()) {
                                buffer += (char) t;
                            } else {
                                ++parsed_char_count;
                            }
                            break;
                        default:
                            input_stream.unget();
                            std::string tmp;
                            input_stream >> tmp;

                            if (verbose) {
                                std::cout << __FILE__ << ":" << __LINE__ << " tmp:" << tmp << std::endl;
                            }

                            buffer += tmp;
                            total_char_count += tmp.length();
                    }
                }
                if (buffer.length()) {

                    if (verbose) {
                        std::cout << __FILE__ << ":" << __LINE__ << " buffer:" << buffer << std::endl;
                    }

                    lexical_element e = lexical_element();
                    std::string re_match;

                    if (automata_state == automata_state::COMMENT) {
                        if (RE2::PartialMatch(buffer, COMMENT_REGEX, &re_match)) {
                            automata_state = automata_state::NEUTRAL;
                            if (verbose) {
                                std::cout << __FILE__ << ":" << __LINE__ << " exited COMMENT mode"
                                          << std::endl;
                            }
                            parse_match_result(buffer, re_match, lexical_element_vector, e, lexical_element::COMMENT,
                                               identifier_map,
                                               excep, verbose, parsed_line_count, parsed_char_count, include_comments);
                        }
                    } else {
                        std::string int_re_match;
                        std::string comment_re_match;
                        std::string header_name_re_match;
                        std::string include_re_match;
                        std::string identifier_re_match;

                        if (RE2::PartialMatch(buffer, KEYWORD_REGEX, &re_match) |
                            RE2::PartialMatch(buffer, IDENTIFIER_REGEX, &identifier_re_match)) {
                            if (identifier_re_match.length() > re_match.length()) {
                                parse_match_result(buffer, identifier_re_match, lexical_element_vector, e,
                                                   lexical_element::IDENTIFIER,
                                                   identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                                   include_comments);
                            } else {
                                parse_match_result(buffer, re_match, lexical_element_vector, e,
                                                   lexical_element::KEYWORD,
                                                   identifier_map,
                                                   excep, verbose, parsed_line_count, parsed_char_count,
                                                   include_comments);
                            }
                        } else if (RE2::PartialMatch(buffer, FLOATING_CONSTANT_REGEX, &re_match) |
                                   RE2::PartialMatch(buffer, INTEGER_CONSTANT_REGEX, &int_re_match)) {
                            if (int_re_match.length() >= re_match.length()) {
                                parse_match_result(buffer, int_re_match, lexical_element_vector, e,
                                                   lexical_element::INTEGER_CONSTANT,
                                                   identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                                   include_comments);
                            } else {
                                parse_match_result(buffer, re_match, lexical_element_vector, e,
                                                   lexical_element::FLOATING_CONSTANT,
                                                   identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                                   include_comments);
                            }
                        } else if (RE2::PartialMatch(buffer, ENUMERATION_CONSTANT_REGEX, &re_match)) {
                            // TODO: figure out what is this
                            parse_match_result(buffer, re_match, lexical_element_vector, e,
                                               lexical_element::ENUMERATION_CONSTANT,
                                               identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                               include_comments);
                        } else if (RE2::PartialMatch(buffer, CHARACTER_CONSTANT_REGEX, &re_match)) {
                            parse_match_result(buffer, re_match, lexical_element_vector, e,
                                               lexical_element::CHARACTER_CONSTANT,
                                               identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                               include_comments);
                        } else if (automata_state == automata_state::NEUTRAL &&
                                   RE2::PartialMatch(buffer, STRING_LITERAL_REGEX, &re_match)) {
                            parse_match_result(buffer, re_match, lexical_element_vector, e,
                                               lexical_element::STRING_LITERAL,
                                               identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                               include_comments);
                        } else if (automata_state == automata_state::INCLUDE &&
                                   RE2::PartialMatch(buffer, HEADER_NAME_REGEX, &header_name_re_match)) {
                            automata_state = automata_state::NEUTRAL;
                            if (verbose) {
                                std::cout << __FILE__ << ":" << __LINE__ << " exited INCLUDE mode"
                                          << std::endl;
                            }
                            parse_match_result(buffer, header_name_re_match, lexical_element_vector, e,
                                               lexical_element::HEADER_NAME,
                                               identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                               include_comments);
                        } else if (RE2::PartialMatch(buffer, COMMENT_START_REGEX, &comment_re_match)
                                   | RE2::PartialMatch(buffer, OPERATOR_REGEX, &re_match)
                                   | RE2::PartialMatch(buffer, INCLUDE_PREPROCESSING_REGEX, &include_re_match)
                                ) {

                            if (include_re_match.length() && automata_state == automata_state::NEUTRAL) {
                                automata_state = automata_state::INCLUDE;
                                if (verbose) {
                                    std::cout << __FILE__ << ":" << __LINE__ << " entered INCLUDE mode"
                                              << std::endl;
                                }
                                parse_match_result(buffer, re_match, lexical_element_vector, e,
                                                   lexical_element::PUNCTUATOR,
                                                   identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                                   include_comments);
                                RE2::PartialMatch(buffer, IDENTIFIER_REGEX, &re_match);
                                parse_match_result(buffer, re_match, lexical_element_vector, e,
                                                   lexical_element::IDENTIFIER,
                                                   identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                                   include_comments);

                            } else if (comment_re_match.length() >= re_match.length()) {
                                automata_state = automata_state::COMMENT;
                                if (verbose) {
                                    std::cout << __FILE__ << ":" << __LINE__ << " matched: " << comment_re_match
                                              << std::endl;
                                    std::cout << __FILE__ << ":" << __LINE__ << " entered COMMENT mode"
                                              << std::endl;
                                }
//                                    parse_match_result(buffer, comment_re_match, lexical_element_vector, e, lexical_element::COMMENT,
//                                                       identifier_map, line_count, char_count, verbose);
                            } else {
                                parse_match_result(buffer, re_match, lexical_element_vector, e,
                                                   lexical_element::OPERATOR,
                                                   identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                                   include_comments);
                            }
                        } else if (RE2::PartialMatch(buffer, PUNCTUATOR_REGEX, &re_match)) {
                            parse_match_result(buffer, re_match, lexical_element_vector, e, lexical_element::PUNCTUATOR,
                                               identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                               include_comments);
                        } else if (RE2::PartialMatch(buffer, PREPROCESSING_NUMBER_REGEX, &re_match)) {
                            parse_match_result(buffer, re_match, lexical_element_vector, e,
                                               lexical_element::PREPROESSING_NUMBER,
                                               identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                               include_comments);
                        } else if (RE2::PartialMatch(buffer, UNKNOWN_REGEX, &re_match)) {
                            // This category should belong to preprocessing-token
                            parse_match_result(buffer, re_match, lexical_element_vector, e, lexical_element::UNKNOWN,
                                               identifier_map, excep, verbose, parsed_line_count, parsed_char_count,
                                               include_comments);
                        } else if (!input_stream) {
                            break;
                        }
                    }

                }

                if (verbose) {
                    std::cout << __FILE__ << ":" << __LINE__ << " line_count:" << parsed_line_count << std::endl;
                    std::cout << __FILE__ << ":" << __LINE__ << " char_count:" << parsed_char_count << std::endl;
                    std::cout << __FILE__ << ":" << __LINE__ << " automate_state:" << automata_state << std::endl;
                }

            } while (input_stream || buffer.length());

//            if (buffer.length()) {
//                std::cout << "Parse exception at " << line_count << ":" << char_count <<
//                          std::endl;
//                std::cout << buffer <<
//                          std::endl;
//            }

            parse_result.set_lexical_element_vector(lexical_element_vector);
            parse_result.set_exception_occur(!buffer.empty());
            parse_result.set_total_lines(total_line_count);
            parse_result.set_total_chars(total_char_count);
            parse_result.set_exception_line(parsed_line_count);
            parse_result.set_exception_char_index(parsed_char_count);

            return parse_result;
        }

        void
        output_lexical_element_vector_to_output_stream(const std::vector<lexical_element> &v, std::ostream &stream) {
            stream << to_string(v);
        }

        void output_lexical_parse_result_to_output_stream(const lexical_parse_result &v, std::ostream &stream) {
            if (v.is_exception_occur()) {
                stream << "Parse exception at " << v.get_exception_line() << ":" << v.get_exception_char_index() <<
                       std::endl;
            } else {
                output_lexical_element_vector_to_output_stream(v.get_lexical_element_vector(), stream);
            }
        }

        const std::vector<lexical_element> &lexical_parse_result::get_lexical_element_vector() const {
            return lexical_element_vector;
        }

        lexical_parse_result &
        lexical_parse_result::set_lexical_element_vector(const std::vector<lexical_element> &lexical_element_vector) {
            lexical_parse_result::lexical_element_vector = lexical_element_vector;
            return *this;
        }

        bool lexical_parse_result::is_exception_occur() const {
            return exception_occur;
        }

        lexical_parse_result &lexical_parse_result::set_exception_occur(bool exception_occur) {
            lexical_parse_result::exception_occur = exception_occur;
            return *this;
        }

        long lexical_parse_result::get_exception_line() const {
            return exception_line;
        }

        lexical_parse_result &lexical_parse_result::set_exception_line(long exception_line) {
            lexical_parse_result::exception_line = exception_line;
            return *this;
        }

        long lexical_parse_result::get_exception_char_index() const {
            return exception_char_index;
        }

        lexical_parse_result &lexical_parse_result::set_exception_char_index(long exception_char_index) {
            lexical_parse_result::exception_char_index = exception_char_index;
            return *this;
        }

        long lexical_parse_result::get_total_lines() const {
            return total_lines;
        }

        lexical_parse_result &lexical_parse_result::set_total_lines(long total_lines) {
            lexical_parse_result::total_lines = total_lines;
            return *this;
        }

        long lexical_parse_result::get_total_chars() const {
            return total_chars;
        }

        lexical_parse_result &lexical_parse_result::set_total_chars(long total_chars) {
            lexical_parse_result::total_chars = total_chars;
            return *this;
        }

        lexical_parse_result::lexical_parse_result(const std::vector<lexical_element> &lexical_element_vector,
                                                   bool exception_occur, long exception_line, long exception_char_index,
                                                   long total_lines, long total_chars) : lexical_element_vector(
                lexical_element_vector), exception_occur(exception_occur), exception_line(exception_line),
                                                                                         exception_char_index(
                                                                                                 exception_char_index),
                                                                                         total_lines(total_lines),
                                                                                         total_chars(total_chars) {}

        lexical_parse_result::lexical_parse_result() : lexical_element_vector(
                {}), exception_occur(false), exception_line(-1), exception_char_index(-1),
                                                       total_lines(-1),
                                                       total_chars(-1) {}
    }
}
