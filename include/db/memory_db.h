/**
 * Created by <a href='mailto:markov.david@seznam.cz'>David Markov</a> on 08.12.21.
 */

#pragma once

#include <tuple>

namespace db {

    class CMemory_Database {
    public:
        enum class Db_Operation {
            Equals,
            Greater_Than,
            Less_Than
        };

    };
}