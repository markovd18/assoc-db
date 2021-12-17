/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 17.12.21.
 */

#pragma once

#include <db/db.h>
#include <ostream>
#include <vector>
#include <algorithm>
#include <variant>

namespace print {

    void Print_Query_Result(const db::TDB_Query_Result& query_result,
                            std::ostream& output_stream,
                            const std::function<void()>& on_success) {
        if (query_result.success) {
            on_success();
        } else {
            output_stream << "ERROR!\n" << std::endl;
        }
    }

    void Print_OK_With_Affected_Row_Count(const size_t affected_row_count, std::ostream& output_stream) {
        output_stream << "OK\n" << affected_row_count << " rows.\n";
    }

    void Print_Element(const db::TDb_Element &element, std::ostream &output_stream,
                       const std::function<void(std::ostream&)> &print_after_item) {
        std::visit([&output_stream, &print_after_item](const auto& item) {
            using T = std::decay_t<decltype(item)>;
            if constexpr(std::is_same_v<T, std::shared_ptr<db::CPair>>) {
                output_stream << *item;
            } else {
                output_stream << item;
            }

            if (print_after_item) {
                print_after_item(output_stream);
            }
        }, element);
    }

    void Print_Affected_Row(const db::TDb_Entry& affected_row, std::ostream& output_stream) {
        const db::TDb_Element row_key = affected_row.first;
        const std::vector<db::TDb_Element> row_values = affected_row.second;

        Print_Element(row_key, output_stream, nullptr);
        output_stream << " - ";
        std::for_each(row_values.cbegin(), row_values.cend() - 1, [&output_stream](const auto& value) {
            Print_Element(value, output_stream, [](std::ostream& out) { out << ", "; });
        });
        Print_Element(row_values.back(), output_stream, nullptr);
        output_stream << '\n';
    }

    void Print_Affected_Rows(const db::TDB_Query_Result& query_result, std::ostream& output_stream) {
        Print_OK_With_Affected_Row_Count(query_result.affected_row_count, output_stream);
        const bool no_affected_rows = !query_result.affected_rows.has_value();
        if (no_affected_rows) {
            return;
        }

        const std::vector<db::TDb_Entry> affected_rows = query_result.affected_rows.value();
        for (const auto &affected_row: affected_rows) {
            Print_Affected_Row(affected_row, output_stream);
        }
    }

}

namespace db {

    std::ostream &operator<<(std::ostream &output_stream, const db::CPair &pair) {
        print::Print_Element(pair.Get_First(), output_stream, nullptr);
        output_stream << " - ";
        print::Print_Element(pair.Get_Second(), output_stream, nullptr);
        return output_stream;
    }
}

