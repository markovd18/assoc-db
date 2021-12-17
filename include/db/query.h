/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 16.12.21.
 */

#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <memory>
#include "db.h"

namespace query {

    class IQuery {
    public:
        void Execute(const std::vector<std::string>& arguments, std::ostream& output_stream) const;

    private:
        virtual void Execute(const std::vector<db::TDb_Element>& arguments, std::ostream& output_stream) const = 0;
    };

    template<typename T>
    std::unique_ptr<IQuery> Create_Query() {
        return std::make_unique<T>();
    }

    class CInsert_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CDelete_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CKey_Equals_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CKey_Greater_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CKey_Less_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CFind_Value_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CAverage_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CMax_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };

    class CMin_Query final : public IQuery {
    private:
        void Execute(const std::vector<db::TDb_Element> &arguments, std::ostream &output_stream) const override;
    };
}