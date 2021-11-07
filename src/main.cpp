﻿/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
 */

#include <iostream>
#include <database_ui.h>

constexpr uint8_t Interactive_Mode_Param_Count = 1;
constexpr uint8_t Batch_Mode_Param_Count = 3;

int main(int argc, char** argv) {
    switch (argc) {
        case Interactive_Mode_Param_Count:
            app::CDatabase_UI_App::Run_Interactive_Mode();
            break;
        case Batch_Mode_Param_Count:
            app::CDatabase_UI_App::Run_Batch_Mode(argv[1], argv[2]);
            break;
        default:
            std::cout << "Invalid parameter count! Please, pass no parameters to run in interactive mode or two parameters - "
            << "input file path and output file path to run in batch mode." << std::endl;
    }

	return 0;
}
