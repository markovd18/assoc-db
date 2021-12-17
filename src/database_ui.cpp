/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
 */

#include <database_ui.h>
#include <filesystem.h>
#include <utils/string_utils.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <query_handler.h>

namespace app {

    /// Command which signalizes end of interactive mode
    static const char* Exit_Command = "EXIT";

    bool Is_Input_Path_Valid(const filesystem::path& path) {
        return !path.empty() && filesystem::exists(path) && !filesystem::is_directory(path);
    }

    bool Is_Output_Path_Valid(const filesystem::path& path) {
        return !path.empty() & !filesystem::is_directory(path);
    }

    bool Is_Input_Command_Read(std::istream& input_stream, std::string& line) {
        if (!std::getline(input_stream, line)) {
            return false;
        }

        Remove_Trailing_Carriage_Return(line);
        return true;
    }

    /**
     * @brief Processes command from input string and prints the outcome information into the @a output_stream.
     * @param line input string containing command (query) to be processed
     * @param query_handler instance of query handler
     * @param output_stream output stream where the result will be printed into
     */
    void Process_Command(const std::string &line, const query::CQuery_Handler& query_handler, std::ostream &output_stream);
    /// Creates vector of strings from colon separated arguments
    std::vector<std::string> Parse_Arguments(const std::string& arguments_line);

    void Greet_User() {
        std::cout << "Welcome to KIV/CPP semester work - memory database\nPlease, enter your query." << std::endl;
    }

    void Print_Prompt() {
        std::cout << "> ";
    }

	void CDatabase_UI_App::Run_Interactive_Mode() {
		Greet_User();

        query::CQuery_Handler query_handler;
		std::string line;
        while (true) {
            Print_Prompt();
            std::getline(std::cin, line);
            if (!std::cin) {
                std::cout << "Error while reading user input! Shutting down...";
                return;
            }

            if (line == Exit_Command) {
                break;
            }

            Process_Command(line, query_handler, std::cout);
        }

        std::cout << "Bye." << std::endl;
	}

    void CDatabase_UI_App::Run_Batch_Mode(const filesystem::path& input_file, const filesystem::path& output_file) {
        if (!Is_Input_Path_Valid(input_file)) {
            throw std::invalid_argument("Path to the input file has to be valid path to an existing file! Not a folder!");
        }

        if (!Is_Output_Path_Valid(output_file)) {
            throw std::invalid_argument("Path to the output file has to be valid path to a file! Not a folder!");
        }

        std::string line;
        std::ofstream output_stream(output_file);
        std::ifstream input_stream(input_file);
        query::CQuery_Handler query_handler;
        while (Is_Input_Command_Read(input_stream, line)) {
            output_stream << line << std::endl;
            Process_Command(line, query_handler, output_stream);
        }
	}

    void Process_Command(const std::string &line, const query::CQuery_Handler& query_handler, std::ostream &output_stream) {
        /// pattern is <QUERY>(<parameter1>, <parameter2>, ...) eg. INSERT("hello", 1, 2)
        static const std::regex command_regex(R"(\w+\ *\((\d+(\.\d+)?|(\"[^\"]*\"))?(\,\ *((\d+(\.\d+)?)|(\"[^\"]*\"))\ *)*\))");
        if (!std::regex_match(line, command_regex)) {
            output_stream << "Unknown command pattern!\nPlease enter command in following format:\n"
                        << "<QUERY_NAME>(<ARGUMENTS_SPLIT_BY_COLON>)" << std::endl;
            return;
        }

        const std::size_t first_brace_index = line.find('(');
        const std::size_t second_brace_index = line.find(')');
        const std::string query_name = trim_copy(line.substr(0, first_brace_index));
        const std::string passed_arguments = line.substr(first_brace_index + 1, second_brace_index - first_brace_index - 1);
        const std::vector<std::string> arguments = Parse_Arguments(passed_arguments);

        try {
            query_handler.Handle_Query(query_name, arguments, output_stream);
        } catch(const std::exception& e) {
            output_stream << e.what() << std::endl;
        }
    }

    std::vector<std::string> Parse_Arguments(const std::string &arguments_line) {
        return split(arguments_line, ',');
    }
}