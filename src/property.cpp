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

#include <umof/property.h>

namespace umof
{
    Property::Property(const detail::PropertyTable *table) :
        _table(table)
    {
    }

    ConstString Property::name() const
    {
        return _table->name;
    }

    bool Property::valid() const
    {
        return (_table != nullptr);
    }

    Type Property::type() const
    {
        return Type(_table->type);
    }

    void Property::get(Arg obj, Arg ret) const
    {
        if (ret.type == _table->type)
            return _table->reader(obj.data, ret.data);
    }

    void Property::set(Arg obj, Arg value) const
    {
        if (value.type == _table->type)
            _table->writer(obj.data, value.data);
    }
}
