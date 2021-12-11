/**
 * Created by <a href='mailto:markov.david@seznam.cz'>David Markov</a> on 08.12.21.
 */


#include <query_handler.h>

#include <unordered_map>
#include <stdexcept>

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
    const auto it = m_query_mapping.find(query);
    if (it == m_query_mapping.end()) {
        throw std::invalid_argument("Unknown query request! " + query + " operation does not exist.");
    }

    switch (it->second) {
        case Query::Insert:
            break;
        case Query::Delete:
            break;
        case Query::Key_Equals:
            break;
        case Query::Key_Greater:
            break;
        case Query::Key_Less:
            break;
        case Query::Find_Value:
            break;
        case Query::Average:
            break;
        case Query::Max:
            break;
        case Query::Min:
            break;
    }
}

query::CQuery_Handler::CQuery_Handler() : m_query_mapping({
    { Insert, Query::Insert },
    { Delete, Query::Delete },
    { Key_Equals, Query::Key_Equals },
    { Key_Greater, Query::Key_Greater },
    { Key_Less, Query::Key_Less  },
    { Find_Value, Query::Find_Value },
    { Average, Query::Average },
    { Max, Query::Max },
    { Min, Query::Min }
}) {}
