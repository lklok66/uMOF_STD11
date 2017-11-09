#pragma once

#include "detail/type.h"
#include "detail/method.h"
#include "detail/event.h"

namespace umof
{
    template<typename... Args>
    class Event
    {
        using Pack = detail::ArgumentsBase<void, void, Args...>;

    public:
        Event() {}

        template<typename Functor>
        bool connect(const Arg& object, Functor f);
        template<typename Functor>
        bool connect(Functor f);
        bool connect(const Arg& object, Method method)
        {
            if (method.returnType().valid())
                return false;

            if (Pack::count != method.parameterCount())
                return false;

            for (int i = 0; i < Pack::count; ++i)
            {
                Type argType(Pack::types()[i + 1]);
                if (method.parmaeterType(i) != argType)
                    return false;
            }

            _methods.push_back(std::make_pair(object, method));
            return true;
        }
        void operator()(Args... args);

    private:
        std::vector<std::pair<Arg, Method>> _methods;

    };

    template<typename... Args>
    void Event<Args...>::operator()(Args... args)
    {
        for (unsigned int i = 0; i < _methods.size(); ++i)
        {
            auto object = _methods[i].first;
            auto method = _methods[i].second;
            method.call(object, Arg(), { args... });
        }
    }

    template<typename... Args>
    template<typename Functor>
    bool Event<Args...>::connect(const Arg& object, Functor f)
    {
        return connect(object, Method(detail::UAdaptor<Functor>::table(f)));
    }

    template<typename... Args>
    template<typename Functor>
    bool Event<Args...>::connect(Functor f)
    {
        return connect(Arg(), Method(detail::UAdaptor<Functor>::table(f)));
    }
}
