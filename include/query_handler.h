/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 08.12.21.
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>
#include <memory>
#include <functional>
#include <db/query.h>

namespace query {

    /**
     *  Query Handler processes individual queries as string requests with string arguments
     *  and executes right database query with parameters transformed into correct type.
     */
    class CQuery_Handler {
    private:
        std::unordered_map<std::string, std::function<std::unique_ptr<query::IQuery>()>> m_query_mapping;
    public:
        CQuery_Handler();

        /**
         * @brief Finds given query and if it is valid, passes given arguments to the database with this operation.
         * @param query name of the query to execute
         * @param parameters query parameters
         * @param output_stream output stream that results will be printed into
         */
        void Handle_Query(const std::string& query, const std::vector<std::string>& parameters, std::ostream& output_stream) const;

    private:
        std::unique_ptr<query::IQuery> Construct_Query(const std::string& query_name) const;
    };
}