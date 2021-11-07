/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
 */
#pragma once
#include <filesystem.h>

namespace app {

	/**
	 * @brief Wrapper class around the starter logic of the database UI application. Allows
	 * running in interactive or batch mode.
	*/
	class CDatabase_UI_App {
	public:
		/**
		 * @brief Runs an interactive mode of the application. User is assumed to enter query commands through the CLI
		 * to modify the contents of the database.
		*/
		static void Run_Interactive_Mode();

		/**
		 * @brief Runs a batch mode of the application. Query commands will be loaded from the @a input_file and output
		 * will be redirected into the @a output_file.
		 *
		 * @param input_file path to an input file with queries listed
		 * @param output_file path to an output file for redirected output of the program
		*/
		static void Run_Batch_Mode(const filesystem::path& input_file, const filesystem::path& output_file);
	};
}