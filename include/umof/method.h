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

#include "arg.h"
#include "config.h"
#include "type.h"
#include "detail/table.h"

#include <string>

namespace umof
{
    class Api;

    /*! \breif The Method class provides meta information for method.
    */
    class UMOF_EXPORT Method
    {
    public:
        /*! Checks whether Method is valid object.
            Call to invalid method will result in application crash.
        */
        bool valid() const;

        /*! Returns the name of the method.
            \sa signature()
        */
        ConstString name() const;

        /*! Returns the signature of the method.
            The signature returned in normalized form. This means
            there is no whitespace between parameters and parameter
            names described as typeid().name().
            \sa name()
        */
        std::string signature() const;

        /*! Returns the return type of this method.
        */
        Type returnType() const;

        /*! Returns the number of arguments.
        */
        int parameterCount() const;

        /*! Returns the type of the argument at given index.
        */
        Type parmaeterType(int index) const;
        //ConstString parameterName(int index) const; //Not supported

        /*! Call the method with given args.
        */
        void call(const void *obj, const void *ret, const void **args) const;

        /*! Call the method with given args.
        */
        bool call(Arg obj, Arg ret, std::initializer_list<Arg> args) const;

    public:
        template<typename T>
        struct Holder;

        using Table = detail::MethodTable;

    private:
        template<typename T>
        friend struct Holder;
        friend class Api;

        Method(const Table *table);

    private:
        const Table *_table;

    };
}
