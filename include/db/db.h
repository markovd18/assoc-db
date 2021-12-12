/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 11.12.21.
 */

#pragma once

#include <optional>
#include <string>
#include <variant>
#include <utility>
#include <vector>
#include <memory>
#include <map_utils.h>

namespace db {

    /// Basic element which can be stored in database - of type int, double or string
    using TDb_Basic_Element = std::variant<int, double, std::string>;
    /// Pair of basic database elements
    class CPair;
    /// Element which can be stored in database - of type int, double, string or Pair
    using TDb_Element = std::variant<int, double, std::string, std::shared_ptr<CPair>>;
    /// Complete entry which can be stored in database - key - value pair
    using TDb_Entry = std::pair<TDb_Element, std::vector<TDb_Element>>;

    /**
     * @brief Concept describing a comparison function that checks whether the DB element suits the comparison.
     *
     * Comparison function has to take @a TDb_Element type as the only input parameter and has to return @a bool.
     */
    template<typename T>
    concept Find_Value_Functor = map::Find_Functor<T, TDb_Element>;


    class CPair {
    private:
        TDb_Element m_first;
        TDb_Element m_second;

    public:
        CPair(TDb_Element first, TDb_Element second) : m_first(std::move(first)), m_second(std::move(second)) {}

        [[nodiscard]] const TDb_Element &Get_First() const {
            return m_first;
        }

        [[nodiscard]] const TDb_Element &Get_Second() const {
            return m_second;
        }

        bool operator==(const CPair& other) {
            return m_first == other.m_first && m_second == other.m_second;
        }

        bool operator<(const CPair& other) {
            return m_first < other.m_first && m_second < other.m_second;
        }

        bool operator<=(const CPair& other) {
            return *this < other || *this == other;
        }

        bool operator>(const CPair& other) {
            return m_first > other.m_first && m_second > other.m_second;
        }

        bool operator>=(const CPair& other) {
            return *this > other || *this == other;
        }

        auto operator<=>(const CPair& other) {
            auto result = m_first <=> other.m_first;
            if (result != std::strong_ordering::equivalent) {
                return result;
            }

            return m_second <=> other.m_second;
        }
    };

    /**
     * @brief Result of every database request
     */
    struct TDB_Op_Result {
        /// indicator of successful request
        bool success = true;
        /// number of entries affected by the request
        size_t affected = 0;
        /// optional list of affected entries - for filtration requests only
        std::optional<std::vector<TDb_Entry>> affected_entries;
    };

    /**
     * @brief Interface of every type of database system - memory/file/relational/etc.
     */
    class IDatabase {
    public:
        /**
         * @brief Inserts new entry into the database.
         * @param key key
         * @param values list of values - may not be empty
         * @return result structure with success indicator and number of saved elements
         */
        virtual TDB_Op_Result Insert(TDb_Element key, std::vector<TDb_Element> values) = 0;

        /**
         * @brief Deletes an entry from the database.
         * @param key key of an entry to delete
         * @param values optional list of values to delete
         * @return result structure with success indicator and number of deleted elements
         *
         * If list of values is not passed, the entire database entry with given key is deleted. If values are passed,
         * only those specific values are deleted from the entry with given key. Fails when entry with given key does
         * not exist or when entry with given key does not contain any of given values.
         */
        virtual TDB_Op_Result Delete(const TDb_Element& key, const std::optional<std::vector<TDb_Element>>& values) = 0;

        /**
         * @brief Finds an entry which has key of given value.
         * @param key entry key
         * @return result structure with success indicator and found entry if exists
         */
        virtual TDB_Op_Result Key_Equals(const TDb_Element& key) const = 0;

        /**
         * @brief Finds all entries which have key of greater value than given key.
         * @param key comparison key
         * @return result structure with success indicator, number of found entries and their list
         */
        virtual TDB_Op_Result Key_Greater(const TDb_Element& key) const = 0;

        /**
         * @brief Finds all entries which have key of lesser value than given key.
         * @param key comparison key
         * @return result structure with success indicator, number of found entries and their list
         */
        virtual TDB_Op_Result Key_Less(const TDb_Element& key) const = 0;

        /**
         * @brief Finds all entries which contain given value on any position.
         * @param value value to find entries by
         * @return result structure with success indicator, number of found entries and their list
         */
        virtual TDB_Op_Result Find_Value(const TDb_Element& value) const = 0;

        /**
         * @brief Calculates an average value of stored numeric values.
         * @param key optional key of an entry which will be the only one considered for the computation
         * @param storage storage where the computed average will be stored
         * @return result structure with success indicator and number of found entries with numeric value
         */
        virtual TDB_Op_Result Average(const std::optional<TDb_Element>& key, double& storage) const = 0;

        /**
         * @brief Finds minimum value of all database entries among all stored value types.
         * @param key optional key of an entry which will be the only one considered for the search
         * @param storage storage where found minimum element will be stored
         * @return result structure with success indicator and number of searched entries
         */
        virtual TDB_Op_Result Min(const std::optional<TDb_Element>& key, TDb_Element& storage) const = 0;

        /**
         * @brief Finds maximum value of all database entries among all stored value types.
         * @param key optional key of an entry which will be the only one considered for the search
         * @param storage storage where found maximum element will be stored
         * @return result structure with success indicator and number of searched entries
         */
        virtual TDB_Op_Result Max(const std::optional<TDb_Element>& key, TDb_Element& storage) const = 0;
        virtual ~IDatabase() = default;
    };
}