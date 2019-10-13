//
// Created by FredericDT on 10/5/2019.
//

#include "arguments_parser.h"

namespace fdt {
    namespace lexical_parser {
        fdt::lexical_parser::parse_result parse_arguments(int argc, char **argv) {
            bool analysis = false;
            bool verbose = false;
            bool include_comments = false;
            cxxopts::Options options(argv[0], " - lexical parser for c89 by fdt");
            options
                    .allow_unrecognised_options()
                    .add_options()
                            ("a,analysis", "Print analytical information to STDOUT", cxxopts::value<bool>(analysis))
                            ("f,file", "Input file, required", cxxopts::value<std::string>(), "FILE")
                            ("o,output", "Output file", cxxopts::value<std::string>(), "PATH")
                            ("v,verbose", "Print verbose", cxxopts::value<bool>(verbose))
                            ("c, comments", "Result include comments", cxxopts::value<bool>(include_comments))
                            ("h,help", "Print help");
            try {
                auto result = options.parse(argc, argv);

                if (result.count("help")) {
                    std::cout << options.help({""}) << std::endl;
                    exit(0);
                }

                std::string file;
                std::string output;

                if (result.count("file")) {
                    file = result["file"].as<std::string>();
                } else {
                    throw cxxopts::option_required_exception("f, file");
                }

                if (result.count("output")) {
                    output = result["output"].as<std::string>();
                }

                return fdt::lexical_parser::parse_result(file, analysis, output, verbose, include_comments);

            } catch (const cxxopts::OptionException &e) {
                std::cout << "error parsing options: " << e.what() << std::endl;
                std::cout << options.help({""}) << std::endl;
                exit(1);
            }
        }
    };
};