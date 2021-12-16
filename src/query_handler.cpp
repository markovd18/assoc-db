/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 08.12.21.
 */


#include <query_handler.h>

#include <unordered_map>
#include <stdexcept>
#include <db/query.h>

static const char* Insert = "INSERT";
static const char* Delete = "DELETE";
static const char* Key_Equals = "KEY_EQUALS";
static const char* Key_Greater = "KEY_GREATER";
static const char* Key_Less = "KEY_LESS";
static const char* Find_Value = "FIND_VALUE";
static const char* Average = "AVERAGE";
static const char* Max = "MAX";
static const char* Min = "Min";

void query::CQuery_Handler::Handle_Query(const std::string &query, const std::vector<std::string> &parameters, std::ostream& output_stream) const {
    const std::unique_ptr<query::IQuery> query_ptr = Construct_Query(query);
    if (!query_ptr) {
        throw std::invalid_argument("Unknown query request! " + query + " operation does not exist.");
    }

    query_ptr->Execute(parameters, output_stream);
}

query::CQuery_Handler::CQuery_Handler() : m_query_mapping({
    { Insert, query::Create_Query<query::CInsert_Query> },
    { Delete, query::Create_Query<query::CDelete_Query> },
    { Key_Equals, query::Create_Query<query::CKey_Equals_Query> },
    { Key_Greater, query::Create_Query<query::CKey_Greater_Query> },
    { Key_Less, query::Create_Query<query::CKey_Less_Query>  },
    { Find_Value, query::Create_Query<query::CFind_Value_Query> },
    { Average, query::Create_Query<query::CAverage_Query> },
    { Max, query::Create_Query<query::CMax_Query> },
    { Min, query::Create_Query<query::CMin_Query> }
}) {}

std::unique_ptr<query::IQuery> query::CQuery_Handler::Construct_Query(const std::string& query_name) const {
    const auto found_pair = m_query_mapping.find(query_name);
    if (found_pair == m_query_mapping.end()) {
        return nullptr;
    }

    return found_pair->second();
}
