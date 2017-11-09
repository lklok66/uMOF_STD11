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

#include <umof/any.h>

namespace umof
{
    Any::Any() :
        _table(nullptr),
        _object(nullptr)
    {
    }

    Any::Any(Any const& x) :
        _table(nullptr),
        _object(nullptr)
    {
        if (x._table != nullptr)
        {
            x._table->clone(&x._object, &_object);
            _table = x._table;
        }
        else
            reset();
    }

    Any::Any(Any &&x) :
        _table(nullptr),
        _object(nullptr)
    {
        if (x._table != nullptr)
        {
            x._table->clone(&x._object, &_object);
            _table = x._table;
            x._table = nullptr;
            x._object = nullptr;
        }
        else
            reset();
    }

    Any::~Any()
    {
        if (_table)
            _table->destroy(&_object);
    }

    void Any::reset()
    {
        if (_table != nullptr)
        {
            _table->destroy(&_object);
            _table = nullptr;
            _object = nullptr;
        }
    }

    void Any::reset(detail::TypeTable *table)
    {
        if (_table != nullptr)
            _table->destroy(&_object);

        _table = table;
        _table->create(&_object);
    }

    Type Any::type() const
    {
        return Type(_table);
    }

    void *Any::object() const
    {
        if (!_table)
            return nullptr;

        if (_table->is_small)
            return (void *)&_object;
        else
            return _object;
    }
}