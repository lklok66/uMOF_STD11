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

namespace umof
{
    class Api;

    /*! \breif The Method class provides meta information for property.
    */
    class UMOF_EXPORT Property
    {
    public:
        /*! Checks whether Property is valid object.
            Call to invalid property will result in application crash.
        */
        bool valid() const;

        /*! Returns the name of the property.
            \sa type()
        */
        ConstString name() const;

        /*! Returns the type of the property.
            \sa name()
        */
        Type type() const;

        /*! Return the value of the property.
        */
        void get(Arg obj, Arg ret) const;

        /*! Sets the given value to the property.
        */
        void set(Arg obj, Arg value) const;

    public:
        template<typename T>
        struct Holder;

        using Table = detail::PropertyTable;

    private:
        template<typename T>
        friend struct Holder;
        friend class Api;

        Property(const Table *table);

    private:
        const Table *_table;

    };
}
