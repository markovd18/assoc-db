/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
 */

#include <iostream>
#include <database_ui.h>
#include <db/memory_db.h>
#include <db/query.h>

constexpr uint8_t Interactive_Mode_Param_Count = 1;
constexpr uint8_t Batch_Mode_Param_Count = 3;

void Prepare_Database() {
    db::CMemory_Db_Interface memory_db_interface;
    const db::TDB_Query_Result result = memory_db_interface.Search_Key("Author", db::CMemory_Db_Interface::NDb_Operation::Equals);
    if (result.affected_row_count == 0) {
        memory_db_interface.Insert("Author", std::make_shared<db::CPair>("David", "Markov"), "A21N0059P");
    }

    const db::TDB_Query_Result ridiculous_question = memory_db_interface.Search_Key("Which type of bear is best?",
                                                                                    db::CMemory_Db_Interface::NDb_Operation::Equals);
    if (ridiculous_question.affected_row_count == 0) {
        memory_db_interface.Insert("Which type of bear is best?", "Black bear");
        memory_db_interface.Insert("Identity theft", 1'000'000, "every year");
    }
}
int main(int argc, char** argv) {
    Prepare_Database();


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
