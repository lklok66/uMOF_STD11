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

#include <umof/enumeration.h>

namespace umof
{
    Enumeration::Enumeration(const detail::EnumerationTable *table) :
        _table(table)
    {
    }

    bool Enumeration::valid() const
    {
        return (_table != nullptr);
    }

    ConstString Enumeration::name() const
    {
        return _table->name;
    }

    int Enumeration::keyCount() const
    {
        return _table->count;
    }

    ConstString Enumeration::key(int index) const
    {
        return _table->table[index].name;
    }

    int Enumeration::value(int index) const
    {
        return _table->table[index].value;
    }

    int Enumeration::keyToValue(const char *key) const
    {
        for (unsigned int i = 0; i < _table->count; ++i)
        {
            if (_table->table[i].name == key)
                return _table->table[i].value;
        }

        return -1;
    }

    ConstString Enumeration::valueToKey(int value) const
    {
        for (unsigned int i = 0; i < _table->count; ++i)
        {
            if (_table->table[i].value == value)
                return _table->table[i].name;
        }

        return "";
    }
}