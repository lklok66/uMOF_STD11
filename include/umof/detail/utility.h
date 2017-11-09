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

namespace umof
{
    namespace detail
    {
        // Bool template type
        using True = std::true_type;
        using False = std::false_type;

        // Number of elements in array
        template <typename T, unsigned int N>
        constexpr unsigned int countof(T const (&)[N])
        {
            return N;
        }

        // Callable traits
        template<typename T>
        struct is_callable_impl
        {
        private:
            typedef char(&yes)[1];
            typedef char(&no)[2];

            struct Fallback { void operator()(); };
            struct Derived : T, Fallback {};

            template<typename U, U> struct Check;

            template<typename>
            static yes test(...);

            template<typename C>
            static no test(Check<void (Fallback::*)(), &C::operator()>*);

        public:
            static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
        };

        template<typename T>
        struct is_method :
            std::conditional <
            std::is_member_function_pointer<T>::value
            || std::is_function<T>::value,
            std::true_type,
            std::false_type
            > ::type
        {
        };

        template<typename T>
        struct is_callable :
            std::conditional <
            std::is_class<T>::value,
            is_callable_impl<T>,
            is_method<T>
            > ::type
        {
        };

        //Helper stuff
        namespace unpack
        {
            template<unsigned...> struct indices {};

            template<unsigned N, unsigned... Is>
            struct indices_gen : indices_gen <N - 1, N - 1, Is...> {};

            template<unsigned... Is>
            struct indices_gen<0, Is...> : indices <Is...> {};

            template<unsigned N, typename... R>
            struct type_at;

            template<unsigned N, typename T, typename... R>
            struct type_at<N, T, R...>
            {
                using type = typename type_at<N - 1, R...>::type;
            };

            template<typename T, typename... R>
            struct type_at<0, T, R...>
            {
                using type = T;
            };

            template<unsigned N>
            struct type_at<N>
            {
                using type = void;
            };
        }
    }
}