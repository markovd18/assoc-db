/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 11.12.21.
 */

#include <db/memory_db.h>
#include <algorithm>
#include <map_utils.h>
#include <execution>

db::CMemory_Database s_Memory_Database;

db::TDB_Op_Result db::CMemory_Database::Insert(db::TDb_Element key,
                                               std::vector<TDb_Element> values) {
    const auto it = m_storage.find(key);
    if (it != m_storage.end()) {    /// If key does not exist, returns error
        return {false , 0, std::nullopt };
    }

    if (values.empty()) {       /// We need some values to save, cannot be empty
        return {false , 0, std::nullopt };
    }

    m_storage.insert(TDb_Entry(std::move(key), std::move(values)));
    return { true, 1, std::nullopt};
}

db::TDB_Op_Result db::CMemory_Database::Delete(const db::TDb_Element& key,
                                               const std::optional<std::vector<TDb_Element>> &values) {
    const auto it = m_storage.find(key);
    if (it == m_storage.end()) {        /// If key does not exist, returns error
        return { false , 0, std::nullopt };
    }

    std::vector<TDb_Element>& entry_values = it->second;

    if (!values.has_value() || values.value().empty()) {      /// There's no passed value - deleting the entire entry
        if (m_storage.erase(key) != 1) {
            return {false, 0, std::nullopt };
        }
        return { true, 1, std::nullopt };
    }
    /// We need to check if all values for deletion actually exist
    for (const TDb_Element& value : values.value_or(std::vector<TDb_Element>())) {
        if (std::none_of(entry_values.begin(), entry_values.end(), [&value](const TDb_Element& element) { return element == value; })) {
            return {false , 0, {} };    /// Value is not present, we return an error
        }
    }

    const auto& values_vector = values.value();
    /// Removing all given values from database
    const auto result_it = std::remove_if(entry_values.begin(), entry_values.end(), [&values_vector](const TDb_Element& element) {
        return std::find(values_vector.begin(), values_vector.end(), element) != values_vector.end();
    });
    entry_values.erase(result_it, entry_values.end());

    /// If all the values were deleted, deleting the entire entry
    if (entry_values.empty()) {
        m_storage.erase(key);
    }

    return { true, values_vector.size(), std::nullopt };
}

db::TDB_Op_Result db::CMemory_Database::Key_Equals(const db::TDb_Element &key) const {
    auto it = m_storage.find(key);
    if (it == m_storage.end()) {
        return { true, 0, std::nullopt };
    }

    return { true, 1, { { *it } }};
}

db::TDB_Op_Result db::CMemory_Database::Key_Greater(const db::TDb_Element &key) const {
    std::vector<TDb_Entry> result = map::Find_Key_In(m_storage, [&key](const TDb_Element& element) { return element > key; });
    if (result.empty()) {
        return { true, 0, std::nullopt };
    }

    return { true, result.size(), std::move(result) };
}

db::TDB_Op_Result db::CMemory_Database::Key_Less(const db::TDb_Element &key) const {
    std::vector<TDb_Entry> result = map::Find_Key_In(m_storage, [&key](const TDb_Element& element) { return element < key; });
    if (result.empty()) {
        return {true, 0, std::nullopt };
    }

    return { true, result.size(), std::move(result) };
}

db::TDB_Op_Result db::CMemory_Database::Find_Value(const db::TDb_Element &value) const {
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

db::TDB_Op_Result db::CMemory_Database::Average(const std::optional<TDb_Element> &key,
                                                double &storage) const {
    size_t element_count = 0;
    if (key.has_value()) {
        auto it = m_storage.find(key.value());
        if (it == m_storage.end()) {
            return { false, 0, std::nullopt };
        }

        Accumulate_Average(it->second, element_count, storage);

        storage /= reinterpret_cast<decltype(storage)>(element_count);
        return { true, 1, std::nullopt };
    }

    size_t entry_count = 0;
    for (const auto& pair : m_storage) {
        size_t previous_element_count = element_count;
        Accumulate_Average(pair.second, element_count, storage);
        if (element_count > previous_element_count) {
            entry_count++;
        }
    }

    storage /= static_cast<double>(element_count);
    return {  true, entry_count, std::nullopt };
}

db::TDB_Op_Result db::CMemory_Database::Min(const std::optional<TDb_Element> &key,
                                            db::TDb_Element &storage) const {
    if (key.has_value()) {

    }
    return {};
}

db::TDB_Op_Result db::CMemory_Database::Max(const std::optional<TDb_Element> &key,
                                            db::TDb_Element &storage) const {
    return {};
}

db::TDB_Op_Result db::CMemory_Db_Interface::Search_Key(const TDb_Element &key, CMemory_Db_Interface::NDb_Operation db_operation) const {
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