/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
 */

#include <database_ui.h>
#include <filesystem.h>
#include <iostream>
#include <string>

namespace app {

    static inline const char* EXIT_COMMAND = "EXIT";

    void Greet_User() {
        std::cout << "Welcome to KIV/CPP semestral work - memory database\nPlease, enter your query." << std::endl;
    }

	void CDatabase_UI_App::Run_Interactive_Mode() {
		Greet_User();

		std::string line;
        while (line != EXIT_COMMAND) {
            std::getline(std::cin, line, '\n');
            if (!std::cin) {
                std::cout << "Error while reading user input! Shutting down...";
                return;
            }
        }
	}

	void CDatabase_UI_App::Run_Batch_Mode(const filesystem::path& input_file, const filesystem::path& output_file) {
        if (input_file.empty() || !filesystem::exists(input_file) || filesystem::is_directory(input_file)) {
            std::cout << "Path to the input file has to be valid path to an existing file! Not a folder!" << std::endl;
            return;
        }

        if (output_file.empty() || filesystem::is_directory(output_file)) {
            std::cout << "Path to the output file has to be valid path to a file! Not a folder!" << std::endl;
            return;
        }
	}
}