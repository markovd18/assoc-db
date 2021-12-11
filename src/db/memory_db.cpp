/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 11.12.21.
 */

#include <db/memory_db.h>
#include <algorithm>

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

    if (!values.has_value()) {      /// There's no passed value - deleting the entire entry
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
    entry_values.erase(entry_values.begin(), result_it);

    /// If all the values were deleted, deleting the entire entry
    if (entry_values.empty()) {
        m_storage.erase(key);
    }

    return { true, values_vector.size(), std::nullopt };
}

db::TDB_Op_Result db::CMemory_Database::Key_Equals(const db::TDb_Element &key) {
    return {};
}

db::TDB_Op_Result db::CMemory_Database::Key_Greater(const db::TDb_Element &key) {
    return {};
}

db::TDB_Op_Result db::CMemory_Database::Key_Less(const db::TDb_Element &key) {
    return {};
}

db::TDB_Op_Result db::CMemory_Database::Find_Value(const db::TDb_Element &value) {
    return {};
}

db::TDB_Op_Result db::CMemory_Database::Average(const std::optional<TDb_Element> &key,
                                                double &storage) {
    return {};
}

db::TDB_Op_Result db::CMemory_Database::Min(const std::optional<TDb_Element> &key,
                                            db::TDb_Element &storage) {
    return {};
}

db::TDB_Op_Result db::CMemory_Database::Max(const std::optional<TDb_Element> &key,
                                            db::TDb_Element &storage) {
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