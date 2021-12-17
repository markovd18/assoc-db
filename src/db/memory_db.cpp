/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 11.12.21.
 */

#include <db/memory_db.h>
#include <algorithm>
#include <utils/map_utils.h>
#include <execution>

db::CMemory_Database s_Memory_Database;

bool Vector_Contains_Element(const db::TDb_Element& sought_element, const std::vector<db::TDb_Element>& reference_vector) {
    return std::ranges::all_of(reference_vector, [&sought_element](const auto& element) {
        return element == sought_element;
    });
}
bool Vector_Contains_All_Elements(const std::vector<db::TDb_Element> &reference_vector,
                                  const std::vector<db::TDb_Element> &second_vector) {
    return std::ranges::all_of(second_vector, [&reference_vector](const auto& element) {
        return Vector_Contains_Element(element, reference_vector);
    });
}

void Remove_All_Elements_From_Vector(std::vector<db::TDb_Element>& reference_vector,
                                     const std::vector<db::TDb_Element>& elements_to_delete) {
    const auto result_it = std::remove_if(reference_vector.begin(), reference_vector.end(), [&elements_to_delete](const auto& element) {
        return std::find(elements_to_delete.begin(), elements_to_delete.end(), element) != elements_to_delete.end();
    });

    reference_vector.erase(result_it, reference_vector.end());
}

db::TDB_Query_Result db::CMemory_Database::Insert(db::TDb_Element key,
                                                  std::vector<TDb_Element> values) {
    const auto it = m_storage.find(key);
    if (it != m_storage.end()) {
        return {false , 0, std::nullopt };
    }

    if (values.empty()) {       /// We need some values to save, cannot be empty
        return {false , 0, std::nullopt };
    }

    m_storage.insert(TDb_Entry(std::move(key), std::move(values)));
    return { true, 1, std::nullopt};
}

db::TDB_Query_Result db::CMemory_Database::Delete(const db::TDb_Element& key,
                                                  const std::optional<std::vector<TDb_Element>> &values) {
    const auto it = m_storage.find(key);
    if (it == m_storage.end()) {
        return { false , 0, std::nullopt };
    }

    if (!values.has_value() || values.value().empty()) {      /// There's no passed value - deleting the entire entry
        if (m_storage.erase(key) != 1) {
            return {false, 0, std::nullopt };
        }
        return { true, 1, std::nullopt };
    }

    std::vector<TDb_Element>& row_values = it->second;
    const auto& values_to_delete = values.value();
    /// We need to check if all values for deletion actually exist
    if (!Vector_Contains_All_Elements(row_values, values_to_delete)){
        return { false, 0, std::nullopt };
    }

    Remove_All_Elements_From_Vector(row_values, values_to_delete);
    if (row_values.empty()) {
        m_storage.erase(key);
    }

    return { true, values_to_delete.size(), std::nullopt };
}

db::TDB_Query_Result db::CMemory_Database::Key_Equals(const db::TDb_Element &key) const {
    auto it = m_storage.find(key);
    if (it == m_storage.end()) {
        return { true, 0, std::nullopt };
    }

    return { true, 1, { { *it } }};
}

db::TDB_Query_Result db::CMemory_Database::Key_Greater(const db::TDb_Element &key) const {
    std::vector<TDb_Entry> result = map::Find_Key_In(m_storage, [&key](const TDb_Element& element) { return element > key; });
    if (result.empty()) {
        return { true, 0, std::nullopt };
    }

    return { true, result.size(), std::move(result) };
}

db::TDB_Query_Result db::CMemory_Database::Key_Less(const db::TDb_Element &key) const {
    std::vector<TDb_Entry> result = map::Find_Key_In(m_storage, [&key](const TDb_Element& element) { return element < key; });
    if (result.empty()) {
        return {true, 0, std::nullopt };
    }

    return { true, result.size(), std::move(result) };
}

db::TDB_Query_Result db::CMemory_Database::Find_Value(const db::TDb_Element &value) const {
    std::vector<TDb_Entry> result = map::Find_Value_In(m_storage, [&value](const std::vector<TDb_Element>& values) {
        return std::find(values.begin(),  values.end(), value) != values.end();
    });
    if (result.empty()) {
        return { true, 0, std::nullopt };
    }

    return { true, result.size(), std::move(result) };
}

void Accumulate_Average(const std::vector<db::TDb_Element>& values, size_t& total_count, double& total_sum) {
    std::for_each(std::execution::par_unseq, values.begin(), values.end(), [&total_sum, &total_count](const db::TDb_Element& element) {
        std::visit([&total_sum, &total_count](const auto& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                total_sum += arg;
                total_count++;
            }
        }, element);
    });
}

db::TDB_Query_Result db::CMemory_Database::Average(const std::optional<TDb_Element> &key,
                                                   double &storage) const {
    if (key.has_value()) {
        return Compute_Average_In_Row_With_Key(key, storage);
    }

    return Compute_Average_In_All_Rows(storage);

}

db::TDB_Query_Result db::CMemory_Database::Compute_Average_In_All_Rows(double &storage) const {
    size_t total_element_count = 0;
    size_t row_count = 0;
    for (const auto& row : m_storage) {
        size_t previous_element_count = total_element_count;
        const std::vector<db::TDb_Element> row_values = row.second;
        Accumulate_Average(row_values, total_element_count, storage);
        if (total_element_count > previous_element_count) {
            row_count++;
        }
    }

    storage /= static_cast<double>(total_element_count);
    return {true, row_count, std::nullopt };
}

db::TDB_Query_Result db::CMemory_Database::Compute_Average_In_Row_With_Key(const std::optional<TDb_Element> &key, double &storage) const {
    size_t element_count = 0;
    auto it = m_storage.find(key.value());
    if (it == m_storage.end()) {
        return { false, 0, std::nullopt };
    }

    Accumulate_Average(it->second, element_count, storage);

    storage /= static_cast<double>(element_count);
    return { true, 1, std::nullopt };
}

db::TDB_Query_Result db::CMemory_Database::Min(const std::optional<TDb_Element> &key,
                                               db::TDb_Element &storage) const {
    if (key.has_value()) {
        return Find_Minimum_In_Row_With_Key(key, storage);
    }

    return Find_Minimum_In_All_Rows(storage);
}

db::TDB_Query_Result db::CMemory_Database::Find_Minimum_In_All_Rows(db::TDb_Element &storage) const {
    if (m_storage.empty()) {
        return { true, 0, std::nullopt };
    }

    TDb_Element total_minimum = m_storage.begin()->second[0];
    for (const auto &row: m_storage) {
        const std::vector<TDb_Element> row_values = row.second;
        std::for_each(row_values.cbegin(),  row_values.cend(), [&total_minimum](const auto& value) {
            if (value < total_minimum) {
                total_minimum = value;
            }
        });
    }

    storage = total_minimum;
    return {true, m_storage.size(), std::nullopt };
}

db::TDB_Query_Result db::CMemory_Database::Find_Minimum_In_Row_With_Key(const std::optional<TDb_Element> &key,
                                                                        db::TDb_Element &storage) const {
    auto row = m_storage.find(key.value());
    if (row == m_storage.end()) {
        return { false, 0, std::nullopt };
    }

    const std::vector<TDb_Element> row_values = row->second;
    TDb_Element minimum = row_values[0];
    std::for_each(row_values.cbegin() + 1,  row_values.cend(), [&minimum](const auto& value) {
        if (value < minimum) {
            minimum = value;
        }
    });

    storage = minimum;
    return { true, 1, std::nullopt };
}

db::TDB_Query_Result db::CMemory_Database::Max(const std::optional<TDb_Element> &key,
                                               db::TDb_Element &storage) const {
    if (key.has_value()) {
        return Find_Maximum_In_Row_With_Key(key, storage);

    }

    return Find_Maximum_In_All_Rows(storage);
}

db::TDB_Query_Result db::CMemory_Database::Find_Maximum_In_All_Rows(db::TDb_Element &storage) const {
    if (m_storage.empty()) {
        return { true, 0, std::nullopt };
    }

    TDb_Element total_maximum = m_storage.begin()->second[0];
    for (const auto &row: m_storage) {
        const std::vector<TDb_Element> row_values = row.second;
        std::for_each(row_values.cbegin(),  row_values.cend(), [&total_maximum](const auto& value) {
            if (value > total_maximum) {
                total_maximum = value;
            }
        });
    }

    storage = total_maximum;
    return {true, m_storage.size(), std::nullopt };
}

db::TDB_Query_Result db::CMemory_Database::Find_Maximum_In_Row_With_Key(const std::optional<TDb_Element> &key,
                                                                        db::TDb_Element &storage) const {
    auto row = m_storage.find(key.value());
    if (row == m_storage.end()) {
        return { false, 0, std::nullopt };
    }

    const std::vector<TDb_Element> row_values = row->second;
    TDb_Element maximum = row_values[0];
    std::for_each(row_values.cbegin() + 1,  row_values.cend(), [&maximum](const auto& value) {
        if (value > maximum) {
            maximum = value;
        }
    });

    storage = maximum;
    return { true, 1, std::nullopt };
}


db::TDB_Query_Result db::CMemory_Db_Interface::Search_Key(const TDb_Element &key, const CMemory_Db_Interface::NDb_Operation db_operation) const {
    switch (db_operation) {
        case NDb_Operation::Equals:
            return s_Memory_Database.Key_Equals(key);
        case NDb_Operation::Greater_Than:
            return s_Memory_Database.Key_Greater(key);
        case NDb_Operation::Less_Than:
            return s_Memory_Database.Key_Less(key);
    }

    return { false, 0, std::nullopt };
}