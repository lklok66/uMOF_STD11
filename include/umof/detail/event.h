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

#include <functional>

namespace umof
{
    namespace detail
    {
        template<typename Signature, typename Method>
        struct UMethodHelper
        {
            using Args = MethodArguments<Signature>;
            static Method functor;

            inline static void call(const void *object, const void *ret, const void **stack)
            {
                Args::call(functor, object, ret, stack, unpack::indices_gen<Args::count>());
            }
        };

        template<typename Signature>
        struct UMethodAdaptor
        {
            using Args = MethodArguments<Signature>;

            template<typename Functor>
            static const MethodTable *table(Functor f)
            {
                UMethodHelper<Signature, Functor>::functor = f;
                static const MethodTable table = {
                    "",
                    &UMethodHelper<Signature, Functor>::call,
                    MethodArguments<Signature>::count,
                    MethodArguments<Signature>::types()
                };
                return &table;
            }
        };

        template<typename Signature, typename Method>
        Method UMethodHelper<Signature, Method>::functor;

        template<typename Signature, typename Functor>
        struct UFunctorHelper
        {
            using Args = MethodArguments<Signature>;
            static Functor functor;

            inline static void call(const void *object, const void *ret, const void **stack)
            {
                Args::lambda(functor, ret, stack, unpack::indices_gen<Args::count>());
            }
        };

        template<typename Signature, typename Functor>
        Functor UFunctorHelper<Signature, Functor>::functor;

        template<typename S>
        struct UFunctorAdaptor
        {
            using Signature = decltype(&S::operator());
            using Args = MethodArguments<Signature>;

            template<typename Functor>
            static const MethodTable *table(Functor f)
            {
                UFunctorHelper<Signature, Functor>::functor = f;
                static const MethodTable table = {
                    "",
                    &UFunctorHelper<Signature, Functor>::call,
                    MethodArguments<Signature>::count,
                    MethodArguments<Signature>::types()
                };
                return &table;
            }
        };

        template<typename Functor>
        struct UAdaptor
        {
            static_assert(is_callable<Functor>::value, "Argument should be callable");

            using IsObject = std::is_class<Functor>;
            using AdaptorType = typename std::conditional<
                IsObject::value,
                UFunctorAdaptor<Functor>,
                UMethodAdaptor<Functor>
            >::type;

            template<typename Object>
            static const MethodTable *table(Object o)
            {
                return AdaptorType::table(o);
            }
        };
    }
}
