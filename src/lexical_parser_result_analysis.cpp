//
// Created by FredericDT on 10/7/2019.
//

#include <sstream>
#include "lexical_parser_result_analysis.h"

namespace fdt {
    namespace lexical_parser {

        fdt::lexical_parser::analysis_result::analysis_result() : total_char_count(-1), total_line_count(-1),
                                                                  lexical_element_type_count{0} {}

        long fdt::lexical_parser::analysis_result::get_total_line_count() const {
            return total_line_count;
        }

        fdt::lexical_parser::analysis_result &
        fdt::lexical_parser::analysis_result::set_total_line_count(long total_line_count) {
            analysis_result::total_line_count = total_line_count;
            return *this;
        }

        long fdt::lexical_parser::analysis_result::get_total_char_count() const {
            return total_char_count;
        }

        fdt::lexical_parser::analysis_result &
        fdt::lexical_parser::analysis_result::set_total_char_count(long total_char_count) {
            analysis_result::total_char_count = total_char_count;
            return *this;
        }

        const long *fdt::lexical_parser::analysis_result::get_lexical_element_type_count() const {
            return lexical_element_type_count;
        }

        std::string fdt::lexical_parser::analysis_result::to_string() {
            std::stringstream ss = std::stringstream();
            ss << "total lines: " << total_line_count << std::endl;
            ss << "total characters: " << total_char_count << std::endl;
            ss << "tokens count: " << std::endl;
            for (int i = 0; i < lexical_element::TYPE_ENUM_LENGTH; ++i) {
                ss << "\t" << lexical_parser::to_string(static_cast<enum lexical_element::type >(i)) << ": " << lexical_element_type_count[i] << std::endl;
            }

            return std::string(ss.str());
        }

        void analysis_result::print_to_ostream(std::ostream &stream) {
            stream << this->to_string();
        }

        fdt::lexical_parser::analysis_result
        analysis_result::build_from_lexical_parse_result(fdt::lexical_parser::lexical_parse_result &v) {
            analysis_result analysis = analysis_result();

            analysis.set_total_char_count(v.get_total_chars());
            analysis.set_total_line_count(v.get_total_lines());

            const std::vector<lexical_element> &lexical_element_vector = v.get_lexical_element_vector();
            for (auto i = lexical_element_vector.begin(); i != lexical_element_vector.end(); ++i) {
                ++analysis.lexical_element_type_count[i->get_type()];
            }

            return analysis;
        }

    }
}