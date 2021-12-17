/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 16.12.21.
 */


#include <db/query.h>
#include <db/memory_db.h>
#include <utils/print_utils.h>

bool Is_Argument_Completely_Processed(const char* invalid_character_pointer) {
    return *invalid_character_pointer == '\0';
}

db::TDb_Element Create_Element_From_String_Argument(const std::string& argument) {
    char* first_invalid_char_ptr = nullptr;
    const long long_value = std::strtol(argument.data(), &first_invalid_char_ptr, 10);
    if (Is_Argument_Completely_Processed(first_invalid_char_ptr)) {
        return int(long_value);
    }

    const double double_value = std::strtod(argument.data(), &first_invalid_char_ptr);
    if (Is_Argument_Completely_Processed(first_invalid_char_ptr)) {
        return double_value;
    }

    return argument;
}

std::vector<db::TDb_Element> Create_Elements_From_String_Arguments(const std::vector<std::string>& arguments) {
    std::vector<db::TDb_Element> created_elements;
    created_elements.reserve(arguments.size());

    for (const auto& argument : arguments) {
        const db::TDb_Element created_element = Create_Element_From_String_Argument(argument);
        created_elements.emplace_back(created_element);
    }

    return created_elements;
}

std::tuple<db::TDb_Element, std::vector<db::TDb_Element>>
Create_Key_And_Values(const std::vector<db::TDb_Element> &arguments) {
    if (arguments.empty()) {
        return {};
    }

    return { arguments[0], { arguments.cbegin() + 1, arguments.cend() } };
}

db::TDb_Element Create_Key(const std::vector<db::TDb_Element>& arguments) {
    if (arguments.empty()) {
        return {};
    }

    return arguments[0];
}

std::optional<db::TDb_Element> Create_Optional_Key(const std::vector<db::TDb_Element>& arguments) {
    if (arguments.empty()) {
        return std::nullopt;
    }

    return Create_Key(arguments);
}

void query::IQuery::Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) const {
    std::vector<db::TDb_Element> created_elements = Create_Elements_From_String_Arguments(arguments);
    Execute(created_elements, output_stream);
}

void query::CInsert_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    if (arguments.empty()) {
        output_stream << "ERROR\n" << "No key passed" << std::endl;
        return;
    }

    const auto [key, values] = Create_Key_And_Values(arguments);
    const db::TDB_Query_Result result = s_Memory_Database.Insert(key, values);
    if (result.success) {
        print::Print_OK_With_Affected_Row_Count(result.affected_row_count, output_stream);
    } else {
        output_stream << "ERROR!\n" << std::endl;
    }
}

void query::CDelete_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    if (arguments.empty()) {
        output_stream << "ERROR\n" << "No key passed" << std::endl;
        return;
    }

    const auto [key, values] = Create_Key_And_Values(arguments);
    const db::TDB_Query_Result result = s_Memory_Database.Delete(key, values);
    print::Print_Query_Result(result, output_stream, [&output_stream, &result]() {
        output_stream << "OK\n" << result.affected_row_count << " rows.\n" << std::endl;
    });
}

void query::CKey_Equals_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    if (arguments.empty()) {
        output_stream << "ERROR\n" << "No key passed" << std::endl;
        return;
    }

    const db::TDb_Element key = Create_Key(arguments);
    const db::TDB_Query_Result result = s_Memory_Database.Key_Equals(key);
    print::Print_Query_Result(result, output_stream, [&result, &output_stream]() {
        print::Print_Affected_Rows(result, output_stream); });
    output_stream << std::endl;
}

void query::CKey_Greater_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    if (arguments.empty()) {
        output_stream << "ERROR\n" << "No key passed" << std::endl;
        return;
    }

    const db::TDb_Element key = Create_Key(arguments);
    const db::TDB_Query_Result result = s_Memory_Database.Key_Greater(key);
    print::Print_Query_Result(result, output_stream, [&result, &output_stream]() {
        print::Print_Affected_Rows(result, output_stream); });
    output_stream << std::endl;
}

void query::CKey_Less_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    if (arguments.empty()) {
        output_stream << "ERROR\n" << "No key passed" << std::endl;
        return;
    }

    const db::TDb_Element key = Create_Key(arguments);
    const db::TDB_Query_Result result = s_Memory_Database.Key_Less(key);
    print::Print_Query_Result(result, output_stream, [&result, &output_stream]() {
        print::Print_Affected_Rows(result, output_stream); });
    output_stream << std::endl;
}

void query::CFind_Value_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    if (arguments.empty()) {
        output_stream << "ERROR\n" << "No value passed" << std::endl;
        return;
    }

    const db::TDb_Element sought_value = arguments[0];
    const db::TDB_Query_Result result = s_Memory_Database.Find_Value(sought_value);
    print::Print_Affected_Rows(result, output_stream);
    output_stream << std::endl;
}

void query::CAverage_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    std::optional<db::TDb_Element> key = Create_Optional_Key(arguments);

    double average = 0;
    const db::TDB_Query_Result result = s_Memory_Database.Average(key, average);
    const size_t affected_row_count = result.affected_row_count;
    print::Print_Query_Result(result, output_stream, [&output_stream, &affected_row_count, &average]() {
        print::Print_OK_With_Affected_Row_Count(affected_row_count, output_stream);
        output_stream << "AVERAGE() - " << average << '\n' << std::endl;
    });
}

void query::CMax_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    std::optional<db::TDb_Element> key = Create_Optional_Key(arguments);

    db::TDb_Element maximum;
    const db::TDB_Query_Result result = s_Memory_Database.Max(key, maximum);
    const size_t affected_row_count = result.affected_row_count;
    print::Print_Query_Result(result, output_stream, [&output_stream, &affected_row_count, &maximum]() {
        print::Print_OK_With_Affected_Row_Count(affected_row_count, output_stream);
        output_stream << "MAX() - ";
        print::Print_Element(maximum, output_stream, [](std::ostream& out) { out << '\n' << std::endl; });
    });
}

void query::CMin_Query::Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const {
    std::optional<db::TDb_Element> key = Create_Optional_Key(arguments);

    db::TDb_Element minimum;
    const db::TDB_Query_Result result = s_Memory_Database.Min(key, minimum);
    const size_t affected_row_count = result.affected_row_count;
    print::Print_Query_Result(result, output_stream, [&output_stream, &affected_row_count, &minimum]() {
        print::Print_OK_With_Affected_Row_Count(affected_row_count, output_stream);
        output_stream << "MIN() - ";
        print::Print_Element(minimum, output_stream, [](std::ostream& out) { out << '\n' << std::endl; });
    });
}
