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

#include "table.h"
#include "utility.h"

#include <typeinfo>
#include <type_traits>

#if defined(__GNUC__)
#include <cxxabi.h>
#include <cstdlib>
struct memfree
{
    memfree(const char *n) : name(n) {}
    ~memfree() { free((void*)name); }
    const char *name;
};
#endif

namespace umof
{
    namespace detail
    {
        //Basic function implementation
        template<typename T, typename Small>
        struct TypeFuncs;

        //Basic functions for small type
        template<typename T>
        struct TypeFuncs<T, True>
        {
            static const char *name()
            {
#if defined(__GNUC__)
                static int status = -4;
                static memfree mem(abi::__cxa_demangle(typeid(T).name(), 0, 0, &status));
                return (status == 0) ? mem.name : typeid(T).name();
#else
                return typeid(T).name();
#endif
            }
            static void create(void** dest)
            {
                new (*dest) T();
            }
            static void construct(void** dest)
            {
                new (*dest) T();
            }
            static void destroy(void** x)
            {
                reinterpret_cast<T*>(x)->~T();
            }
            static void destruct(void** x)
            {
                reinterpret_cast<T*>(x)->~T();
            }
            static void clone(void* const* src, void** dest)
            {
                new (dest)T(*reinterpret_cast<T const*>(src));
            }
            static void move(void* const* src, void** dest)
            {
                *reinterpret_cast<T*>(dest) =
                    *reinterpret_cast<T const*>(src);
            }
        };

        //Basic functions for larger type
        template<typename T>
        struct TypeFuncs<T, False>
        {
            static const char *name()
            {
#if defined(__GNUC__)
                static int status = -4;
                static memfree mem(abi::__cxa_demangle(typeid(T).name(), 0, 0, &status));
                return (status == 0) ? mem.name : typeid(T).name();
#else
                return typeid(T).name();
#endif
            }
            static void create(void** dest)
            {
                *dest = new T();
            }
            static void construct(void** dest)
            {
                new (*dest) T();
            }
            static void destroy(void** x)
            {
                // destruct and free memory
                delete (*reinterpret_cast<T**>(x));
            }
            static void destruct(void** x)
            {
                // destruct only, we'll reuse memory
                (*reinterpret_cast<T**>(x))->~T();
            }
            static void clone(void* const* src, void** dest)
            {
                *dest = new T(**reinterpret_cast<T* const*>(src));
            }
            static void move(void* const* src, void** dest)
            {
                **reinterpret_cast<T**>(dest) =
                    **reinterpret_cast<T* const*>(src);
            }
        };

        template<typename T>
        struct Type
        {
            using IsPointer = typename std::is_pointer<T>::type;
            using Decay = typename std::decay<T>::type;
            using PointerDecay = typename std::add_pointer<
                typename std::decay<
                typename std::remove_pointer<Decay>
                ::type>::type>::type;
            using Storage = typename std::conditional<IsPointer::value, PointerDecay, Decay>::type;
            using Small = typename std::integral_constant<bool, (sizeof(Storage) <= sizeof(void*))>;

            static const TypeTable table;

            template<typename S = Small>
            inline static typename std::enable_if<S::value, void>::type clone(const Decay **src, void **dest)
            {
                new (dest)Storage(*reinterpret_cast<Storage const*>(*src));
            }

            template<typename S = Small>
            inline static typename std::enable_if<!S::value, void>::type clone(const Decay **src, void **dest)
            {
                *dest = new Storage(**src);
            }

            template<typename S = Small>
            inline static typename std::enable_if<S::value, Decay*>::type cast(void **object)
            {
                return const_cast<Decay*>(reinterpret_cast<Storage*>(object));
            }

            template<typename S = Small>
            inline static typename std::enable_if<!S::value, Decay*>::type cast(void **object)
            {
                return const_cast<Decay*>(reinterpret_cast<Storage*>(*object));
            }
        };

        template<typename T>
        const TypeTable Type<T>::table = 
        {
            TypeFuncs<Storage, Small>::name,
            TypeFuncs<Storage, Small>::create,
            TypeFuncs<Storage, Small>::construct,
            TypeFuncs<Storage, Small>::destroy,
            TypeFuncs<Storage, Small>::destruct,
            TypeFuncs<Storage, Small>::clone,
            TypeFuncs<Storage, Small>::move,
            sizeof(Storage),
            Small::value
        };
    }
}
