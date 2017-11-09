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

#include <umof/type.h>

namespace umof
{
    Type::Type(const detail::TypeTable *table) :
        _table(table)
    {
    }

    bool Type::operator==(const Type& other)
    {
        return _table == other._table;
    }

    bool Type::operator!=(const Type& other)
    {
        return _table != other._table;
    }

    bool Type::valid() const
    {
        return (_table != nullptr);
    }

    const char *Type::name() const
    {
        return _table->name();
    }

    int Type::size() const
    {
        return _table->size;
    }

    void *Type::construct(void *where, void *const copy) const
    {
        if (copy)
        {
            _table->clone(&copy, &where);
            return where;
        }
        else
        {
            _table->construct(&where);
            return where;
        }
    }

    void *Type::create(void *const copy) const
    {
        void *where = nullptr;
        _table->create(&where);
        if (copy)
            _table->clone(&copy, &where);
        return where;
    }

    void Type::destroy(void *data) const
    {
        _table->destroy(&data);
    }

    void Type::destruct(void *data) const
    {
        _table->destruct(&data);
    }
}
