/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 08.12.21.
 */

#pragma once

#include <tuple>
#include <variant>
#include <vector>
#include <db/db.h>
#include <functional>
#include <concepts>
#include <map>

namespace db {

    /**
     * @brief Memory database implementation of common interface.
     */
    class CMemory_Database final : public IDatabase {
    private:
        std::map<TDb_Element, std::vector<TDb_Element>> m_storage;
    public:
        TDB_Op_Result Insert(TDb_Element key, std::vector<TDb_Element> values) override;
        TDB_Op_Result Delete(const TDb_Element& key, const std::optional<std::vector<TDb_Element>> &values) override;
        TDB_Op_Result Key_Equals(const TDb_Element &key) const override;
        TDB_Op_Result Key_Greater(const TDb_Element &key) const override;
        TDB_Op_Result Key_Less(const TDb_Element &key) const override;
        TDB_Op_Result Find_Value(const TDb_Element &value) const override;
        TDB_Op_Result Average(const std::optional<TDb_Element> &key, double &storage) const override;
        TDB_Op_Result Min(const std::optional<TDb_Element> &key, TDb_Element &storage) const override;
        TDB_Op_Result Max(const std::optional<TDb_Element> &key, TDb_Element &storage) const override;
    };

    /**
     * @brief Programmer interface for database communication. Internally transfers request into valid form
     * and sends it to the standard database interface.
     */
    class CMemory_Db_Interface final {
    public:
        /**
         * @brief Enumeration of comparison operations which can be searched by in the database interface.
         */
        enum class NDb_Operation {
            Equals,
            Greater_Than,
            Less_Than
        };

        /**
         * @brief Inserts key with given values.
         * @tparam Args variadic template argument - element types have to be of type @a TDb_Full_element
         * @param key key to store
         * @param values values to store with key - must not be empty
         * @return number of stored elements
         */
        template<typename ...Args>
        TDB_Op_Result Insert(const TDb_Element &key, const Args... values) const;

        /**
         * @brief Deletes an entry with given key. If no value is passed, deletes the entire entry.
         * If values are passed, deletes only those values from the entry.
         * @tparam Args variadic template argument - element types have to be of type @a TDb_Full_element
         * @param key key of entry to delete
         * @param values values to delete
         * @return number of deleted values
         */
        template<typename ...Args>
        TDB_Op_Result Delete(const TDb_Element &key, const Args... values) const;

        /**
         * @brief Searches a database entry which has key that is equal/lesser/greater than given key, based on
         * passed kind of operation.
         * @param key comparison key
         * @param db_operation comparison action
         * @return list of found entries
         */
        [[nodiscard]] TDB_Op_Result Search_Key(const TDb_Element &key, NDb_Operation db_operation) const;

        TDB_Op_Result Find_Value(Find_Value_Functor auto find_functor) const;
    };
}

extern db::CMemory_Database s_Memory_Database;

namespace db {

    template<typename ...Args>
    TDB_Op_Result CMemory_Db_Interface::Insert(const TDb_Element& key, const Args... values) const {
        std::vector<TDb_Element> values_vec {values... };
        return s_Memory_Database.Insert(key, values_vec);
    }

    template<typename ...Args>
    TDB_Op_Result CMemory_Db_Interface::Delete(const TDb_Element &key, const Args... values) const {
        std::vector<TDb_Element> values_vec {values... };
        return s_Memory_Database.Delete(key, values_vec);
    }

    TDB_Op_Result CMemory_Db_Interface::Find_Value(Find_Value_Functor auto find_functor) const {
        return {};  //TODO
    }
}