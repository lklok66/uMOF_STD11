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

#include "type.h"
#include "method.h"

namespace umof
{
    namespace detail
    {
        template<typename T, typename C>
        struct FieldBase
        {
            using Storage = T;
            using Class = C;
        };

        template<typename T>
        struct Field;

        template<typename T, typename C>
        struct Field<T(C::*)> : public FieldBase<T, C> {};

        template<typename Member, Member field>
        struct Accessor
        {
            using Class = typename Field<Member>::Class;
            using Field = typename Field<Member>::Storage;
            using Storage = typename Type<Field>::Storage;

            inline static void get(const void *obj, const void *ret)
            {
                *(Storage*)ret = static_cast<Class *>(const_cast<void *>(obj))->*field;
            }

            inline static void set(const void *obj, const void *ret)
            {
                static_cast<Class *>(const_cast<void *>(obj))->*field = *(Storage*)ret;
            }
        };

        template<typename Method, Method method>
        struct Getter
        {
            using Args = MethodArguments<Method>;
            using Return = typename Args::Return;
            using Class = typename Args::Class;
            using Storage = typename Type<Return>::Storage;

            static_assert(Args::count == 0, "Getter method should not receive arguments");

            inline static void get(const void *obj, const void *ret)
            {
                *(Storage*)ret = (static_cast<Class *>(const_cast<void *>(obj))->*method)();
            }
        };

        template<typename Method, Method method>
        struct Setter
        {
            using Args = MethodArguments<Method>;
            using Class = typename Args::Class;
            using Storage = typename Args::SetArg;

            static_assert(Args::count == 1, "Setter method should have one argument");

            inline static void set(const void *obj, const void *val)
            {
                (static_cast<Class *>(const_cast<void *>(obj))->*method)(*(Storage *)val);
            }
        };

        template<typename Member, Member member>
        struct Property
        {
            using IsMethod = typename std::is_member_function_pointer<Member>::type;
            using IsField = typename std::is_member_object_pointer<Member>::type;

            static_assert(!std::is_same<IsField, IsMethod>::value, "Property should be field or method");

            struct Get
            {
                using ReadMethod = Getter<Member, member>;
                using MemberField = Accessor<Member, member>;
                using Read = typename std::conditional<IsMethod::value, ReadMethod, MemberField>::type;
                using Storage = typename Read::Storage;

                inline static const TypeTable *type()
                {
                    return &Type<Storage>::table;
                }

                inline static void get(const void *obj, const void *ret)
                {
                    Read::get(obj, ret);
                }
            };

            struct Set
            {
                using WriteMethod = Setter<Member, member>;
                using MemberField = Accessor<Member, member>;
                using Write = typename std::conditional<IsMethod::value, WriteMethod, MemberField>::type;
                using Storage = typename Write::Storage;

                inline static const TypeTable *type()
                {
                    return Type<Storage>::table();
                }

                inline static void set(const void *obj, const void *ret)
                {
                    Write::set(obj, ret);
                }
            };
        };
    }
}
