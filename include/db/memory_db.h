/**
 * Created by <a href='mailto:markov.david@seznam.cz'>David Markov</a> on 08.12.21.
 */

#pragma once

#include <tuple>
#include <variant>
#include <map>
#include <vector>
#include <db/db.h>
#include <functional>
#include <concepts>

namespace db {

    /**
     * @brief Concept describing a comparison function that checks whether the DB element suits the comparison.
     *
     * Comparison function has to take @a TDb_Full_Element type as the only input parameter and has to return @a bool.
     */
    template<typename T>
    concept Find_Value_Functor = requires (T a, TDb_Full_Element e) {
        { a(e) } -> std::convertible_to<bool>;
    };

    /**
     * @brief Programmer interface for database communication. Internally transfers request into valid form
     * and sends it to the standard database interface.
     */
    class CMemory_Database final {
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
        size_t Insert(const TDb_Full_Element& key, const Args... values) const;

        /**
         * @brief Deletes an entry with given key. If no value is passed, deletes the entire entry.
         * If values are passed, deletes only those values from the entry.
         * @tparam Args variadic template argument - element types have to be of type @a TDb_Full_element
         * @param key key of entry to delete
         * @param values values to delete
         * @return number of deleted values
         */
        template<typename ...Args>
        size_t Delete(const TDb_Full_Element& key, const Args... values) const;

        /**
         * @brief
         * @param key
         * @param db_operation
         * @return
         */
        [[nodiscard]] std::vector<TDb_Full_Element> Search_Key(const TDb_Full_Element& key,
                                                               NDb_Operation db_operation) const;

        TDb_Entry Find_Value(Find_Value_Functor auto find_functor) const;
    };


    template<typename ...Args>
    size_t CMemory_Database::Insert(const TDb_Full_Element& key, const Args... values) const {
        std::vector<TDb_Full_Element> values_vec { values... };
        return 0;
    }

    template<typename ...Args>
    size_t CMemory_Database::Delete(const TDb_Full_Element &key, const Args... values) const {
        std::vector<TDb_Full_Element> values_vec { values... };
        return 0;
    }

    std::vector<TDb_Full_Element> CMemory_Database::Search_Key(const TDb_Full_Element &key,
                                                               const CMemory_Database::NDb_Operation db_operation) const  {
        return {};
    }

    TDb_Entry CMemory_Database::Find_Value(Find_Value_Functor auto find_functor) const {
        return {};
    }
}