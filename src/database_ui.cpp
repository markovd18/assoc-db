/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
 */

#include <database_ui.h>
#include <filesystem.h>
#include <string_utils.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

namespace app {

    /// Command which signalizes end of interactive mode
    static inline const char* Exit_Command = "EXIT";

    /**
     * @brief Processes command from input string and prints the outcome information into the @a output_stream.
     * @param line input string containing command (query) to be processed
     * @param output_stream output stream where the result will be printed into
     */
    void Process_Command(const std::string &line, std::ostream &output_stream);
    /// Creates vector of strings from colon separated arguments
    std::vector<std::string> Parse_Arguments(const std::string& arguments_line);

    void Greet_User() {
        std::cout << "Welcome to KIV/CPP semestral work - memory database\nPlease, enter your query." << std::endl;
    }

	void CDatabase_UI_App::Run_Interactive_Mode() {
		Greet_User();

		std::string line;
        while (line != Exit_Command) {
            std::getline(std::cin, line);
            if (!std::cin) {
                std::cout << "Error while reading user input! Shutting down...";
                return;
            }

            Process_Command(line, std::cout);
        }

        std::cout << "Bye." << std::endl;
	}

    void CDatabase_UI_App::Run_Batch_Mode(const filesystem::path& input_file, const filesystem::path& output_file) {
        if (input_file.empty() || !filesystem::exists(input_file) || filesystem::is_directory(input_file)) {
            throw std::invalid_argument("Path to the input file has to be valid path to an existing file! Not a folder!");
        }

        if (output_file.empty() || filesystem::is_directory(output_file)) {
            throw std::invalid_argument("Path to the output file has to be valid path to a file! Not a folder!");
        }

        std::string line;
        std::ofstream output_stream(output_file);
        std::ifstream input_stream(input_file);
        while (std::getline(input_stream, line)) {
            if (line.back() == '\r') {  /// making sure we don't have trailing CR on windows
                line = line.substr(0, line.size() - 1);
            }

            Process_Command(line, output_stream);
        }
	}

    void Process_Command(const std::string &line, std::ostream &output_stream) {
        static const std::regex command_regex(R"(\w+(\ *)?\(((\d+(\.\d+)?|(\"\w+\"))?(\,(\ *)?(\d+(\.\d+)?|(\"\w+\"))+(\ *)?)?)+\))");
        if (!std::regex_match(line, command_regex)) {
            output_stream << "Unknown command pattern!\nPlease enter command in following format:\n"
                        << "<QUERY_NAME>(<ARGUMENTS_SPLIT_BY_COLON>)" << std::endl;
            return;
        }

        const std::size_t first_brace_index = line.find('(');
        const std::size_t second_brace_index = line.find(')');
        const std::string query_name = trim_copy(line.substr(0, first_brace_index));
        // TODO find query function with given name

        const std::vector<std::string> arguments = Parse_Arguments(line.substr(first_brace_index + 1, second_brace_index - first_brace_index - 1));
        // TODO call query function with given arguments
    }

    std::vector<std::string> Parse_Arguments(const std::string &arguments_line) {
        return split(arguments_line, ',');
    }
}