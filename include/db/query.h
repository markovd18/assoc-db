/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 16.12.21.
 */

#pragma once

#include <vector>
#include <string>
#include <ostream>

namespace query {

    class IQuery {
    public:
        virtual void Execute(const std::vector<std::string>& arguments, std::ostream& output_stream) = 0;
    };

    class CInsert_Query final : public IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CDelete_Query final : public IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CKey_Equals_Query final : IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CKey_Greater_Query final : IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CKey_Less_Query final : IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CFind_Value_Query final : IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CAverage_Query final : IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CMax_Query final : IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };

    class CMin_Query final : IQuery {
    public:
        void Execute(const std::vector<std::string> &arguments, std::ostream &output_stream) override;
    };
}