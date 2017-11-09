/*********************************************************************
This file is part of the uMOF library.
Copyright (C) 2014 Artem Shal
artiom.shal@gmail.com

The uMOF library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA.
**********************************************************************/

#pragma once

#include "utility.h"
#include "type.h"

namespace umof
{
    namespace detail
    {
        //Get return type table
        template<typename T, typename Void>
        struct ReturnTable;

        template<typename T>
        struct ReturnTable<T, True>
        {
            inline static TypeTable *get()
            {
                return nullptr;
            }
        };

        template<typename T>
        struct ReturnTable<T, False>
        {
            inline static const TypeTable *get()
            {
                return &Type<T>::table;
            }
        };

        //Function to unpack args properly
        template<typename T>
        inline static const TypeTable *getTable()
        {
            return &Type<T>::table;
        }

        template<typename C, typename R, typename... Args>
        struct ArgumentsBase
        {
            using Class = C;
            using Return = R;
            using SetArg = typename unpack::type_at<0, Args...>::type;

            using IsFree = typename std::is_void<Class>::type;
            using IsVoid = typename std::is_void<Return>::type;

            constexpr static unsigned count = sizeof...(Args);

            inline static const TypeTable **types()
            {
                static const TypeTable *staticTypes[] =
                {
                    ReturnTable<Return, IsVoid>::get(),
                    getTable<Args>()...
                };
                return staticTypes;
            }

            template<typename RR = R, typename CC = C, typename F, unsigned... Is>
            inline static auto call(F f, const void *object, const void *ret, const void **stack, unpack::indices<Is...>)
                -> typename std::enable_if<!std::is_void<RR>::value,
                typename std::enable_if<std::is_void<CC>::value>::type>::type
            {
                *(Return*)ret = f(
                    *(Args *)stack[Is]...
                    );
            }

            template<typename RR = R, typename CC = C, typename F, unsigned... Is>
            inline static auto call(F f, const void *object, const void *ret, const void **stack, unpack::indices<Is...>)
                -> typename std::enable_if<std::is_void<RR>::value,
                typename std::enable_if<std::is_void<CC>::value>::type>::type
            {
                f(
                    *(Args *)stack[Is]...
                    );
            }

            template<typename RR = R, typename CC = C, typename F, unsigned... Is>
            inline static auto call(F f, const void *object, const void *ret, const void **stack, unpack::indices<Is...>)
                -> typename std::enable_if<!std::is_void<RR>::value,
                typename std::enable_if<!std::is_void<CC>::value>::type>::type
            {
                *(Return*)ret = (static_cast<Class *>(const_cast<void *>(object))->*f)(
                    *(Args *)stack[Is]...
                    );
            }

            template<typename RR = R, typename CC = C, typename F, unsigned... Is>
            inline static auto call(F f, const void *object, const void *ret, const void **stack, unpack::indices<Is...>)
                -> typename std::enable_if<std::is_void<RR>::value,
                typename std::enable_if<!std::is_void<CC>::value>::type>::type
            {
                (static_cast<Class *>(const_cast<void *>(object))->*f)(
                    *(Args *)stack[Is]...
                    );
            }

            template<typename RR = R, typename F, unsigned... Is>
            inline static auto lambda(F f, const void *ret, const void **stack, unpack::indices<Is...>)
                -> typename std::enable_if<!std::is_void<RR>::value> ::type
            {
                *(Return*)ret = f(
                    *(Args *)stack[Is]...
                    );
            }

            template<typename RR = R, typename F, unsigned... Is>
            inline static auto lambda(F f, const void *ret, const void **stack, unpack::indices<Is...>)
                -> typename std::enable_if<std::is_void<RR>::value> ::type
            {
                f(
                    *(Args *)stack[Is]...
                    );
            }
        };

        template<typename Signature>
        struct MethodArguments
        {
            static_assert(std::is_function<Signature>::value, "The argument should be a function pointer");

            constexpr static unsigned count = 0;
            constexpr static unsigned size = 0;
            inline static const TypeTable **types();
        };

        //Regular function
        template<typename Return, typename... Args>
        struct MethodArguments<Return(*)(Args...)> : ArgumentsBase<void, Return, Args...> {};

        //Function with vararg
        template<typename Return, typename... Args>
        struct MethodArguments<Return(*)(Args..., ...)> : ArgumentsBase<void, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args..., ...)> : ArgumentsBase<Class, Return, Args...> {};

        //Member function
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)const> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)volatile> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)const volatile> : ArgumentsBase<Class, Return, Args...> {};

        //Function with lvalue ref qualifier
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)&> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)const&> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)volatile&> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)const volatile&> : ArgumentsBase<Class, Return, Args...> {};

        //Function with rvalue ref qualifier
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...) && > : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)const&&> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)volatile&&> : ArgumentsBase<Class, Return, Args...> {};
        template<typename Return, typename Class, typename... Args>
        struct MethodArguments<Return(Class::*)(Args...)const volatile&&> : ArgumentsBase<Class, Return, Args...> {};

        template<typename Function, Function func>
        struct MethodCall
        {
            using Args = MethodArguments<Function>;

            inline static void call(const void *object, const void *ret, const void **stack)
            {
                Args::call(func, object, ret, stack, unpack::indices_gen<Args::count>());
            }
        };

        template<typename Class, typename... Args>
        struct Constructor
        {
            inline static Class *call(void *place, Args... args)
            {
                return new(place)Class(args...);
            }
        };

        template<typename Class, typename... Args>
        struct ConstructorCall
        {
            using Pack = MethodArguments<decltype(&Constructor<Class, Args...>::call)>;

            inline static void call(const void *object, const void *ret, const void **stack)
            {
                Pack::call(&Constructor<Class, Args...>::call, object, ret, stack, unpack::indices_gen<Pack::count>());
            }
        };
    }
}
