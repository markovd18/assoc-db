/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 12.12.21.
 */

#pragma once

#include <map>
#include <utility>
#include <vector>

namespace map {

    /**
     * @brief Concept defining a predicate for searching in a map.
     * @tparam T type of the predicate
     * @tparam E element passed into the predicate
     */
    template<typename T, typename E>
    concept Find_Functor = requires(T a, E e) {
        { a(e) } -> std::convertible_to<bool>;
    };

    /**
     * @brief Collects all entries from given iterator. @a it has to be an iterator over @a map.
     * @tparam K key type of @a map
     * @tparam V value type of @a map
     * @param it iterator over @a map
     * @param map map to iterate over
     * @return collected entries
     */
    template<typename K, typename V>
    std::vector<std::pair<K, V>> Collect_Entries(auto &it, const std::map<K, V>& map) {
        std::vector<std::pair<K, V>> entries;
        while (it != map.end()) {
            entries.emplace_back(*it);
            it++;
        }

        return entries;
    }

    /**
     * @brief Finds all @a map entries whose value meets the @a predicate.
     * @tparam K key type of @a map
     * @tparam V value type of @a map
     * @param map map to search in
     * @param predicate predicate that must be met
     * @return list of found entries
     */
    template<typename K, typename V>
    std::vector<std::pair<K, V>> Find_Value_In(const std::map<K, V>& map,
                                         Find_Functor<V> auto predicate) {
        auto it = std::find_if(map.begin(), map.end(), [&predicate](const std::pair<K, V>& element) -> bool { return predicate(element.second); });
        return Collect_Entries(it, map);
    }

    /**
     * @brief Finds all @a map entries whose key meets the @a predicate.
     * @tparam K key type of @a map
     * @tparam V value type of @a map
     * @param map map to search in
     * @param predicate predicate that must be met
     * @return list of found entries
     */
    template<typename K, typename V>
    std::vector<std::pair<K, V>> Find_Key_In(const std::map<K, V>& map,
                                               Find_Functor<K> auto predicate) {
        auto it = std::find_if(map.begin(), map.end(), [&predicate](const std::pair<K, V>& element) -> bool { return predicate(element.first); });
        return Collect_Entries(it, map);
    }

}
