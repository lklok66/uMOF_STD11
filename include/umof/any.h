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

#include "config.h"
#include "type.h"
#include "detail/type.h"

#include <new>
#include <stdexcept>

namespace umof
{
    /*! \breif The Any class holds the copy of any data type.
    */
    class UMOF_EXPORT Any
    {
    public:
        /*! Constructs invalid Any holder.
        */
        Any();

        /*! Copy value from other holder.
        */
        Any(Any const& other);

        /*! Move value from other holder.
        */
        Any(Any &&other);

        /*! Destroys Any and contained object.
        */
        ~Any();

        /*! Constructs Any with the given value.
        */
        template<typename T>
        Any(T const& value);

        /*! Constructs Any with array value.
            Special constructor for static arrays.
        */
        template<typename T, std::size_t N>
        Any(T(&value)[N]);

        /*! Destroys containing object and set new value.
        */
        /*template<typename T>
        void reset(T const& value);*/

        /*! Destroys containing object.
        */
        void reset();
        void reset(detail::TypeTable *table);

        Type type() const;
        void *object() const;

    private:
        template<typename T>
        friend T* any_cast(Any*);
        friend class Method;

        const detail::TypeTable* _table;
        void* _object;
    };

    template<typename T>
    Any::Any(T const& x) :
        _table(&detail::Type<T>::table),
        _object(nullptr)
    {
        const T *src = &x;
        detail::Type<T>::clone(&src, &_object);
    }

    template<typename T, std::size_t N>
    Any::Any(T(&x)[N]) :
        _table(detail::Type<T*>::table()),
        _object(nullptr)
    {
        new (&_object) T*(&x[0]);
    }

    /*template<typename T>
    void Any::reset(T const& x)
    {
        if (_table)
            _table->static_delete(&_object);
        _table = Table<T>::get();
        const T *src = &x;
        Table<T>::clone(&src, &_object);
    }*/

    template <typename T>
    inline T* any_cast(Any* operand)
    {
        if (operand && operand->_table == &detail::Type<T>::table)
            return detail::Type<T>::cast(&operand->_object);

        return nullptr;
    }

    template <typename T>
    inline T* any_cast(Any const* operand)
    {
        return any_cast<T>(const_cast<Any*>(operand));
    }

    /*! Casts Any container to a given type T.
        \relates Any
        Use it as follows:
        \code{.cpp}
        Any a{5.0};
        double d = any_cast<double>(a);
        \endcode
    */
    template <typename T>
    inline T any_cast(Any& operand)
    {
        using nonref = typename std::remove_reference<T>::type;

        nonref* result = any_cast<nonref>(&operand);
        if (!result)
            throw std::runtime_error("Bad cast");
        return *result;
    }

    /*! Casts Any container to a given type T.
        \relates Any
        Use it as follows:
        \code{.cpp}
        Any a{5.0};
        double d = any_cast<double>(a);
        \endcode
    */
    template <typename T>
    inline T const& any_cast(Any const& operand)
    {
        using nonref = typename std::remove_reference<T>::type;
        return any_cast<nonref const&>(const_cast<Any&>(operand));
    }
}