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

#include <umof/method.h>

namespace umof
{
    Method::Method(const detail::MethodTable *table) :
        _table(table)
    {
    }

    bool Method::valid() const
    {
        return (_table != nullptr);
    }

    ConstString Method::name() const
    {
        return _table->name;
    }

    std::string Method::signature() const
    {
        std::string sig(_table->name);
        int pos = sig.rfind(':');
        if (pos != -1)
            sig = sig.substr(pos + 1, sig.size() - pos);
        sig += '(';

        for (unsigned int i = 0; i < _table->argc; ++i)
        {
            Type arg(_table->types[i + 1]);
            sig += arg.name();
            sig += ',';
        }
        if (_table->argc > 0)
            sig[sig.size() - 1] = ')';
        else
            sig += ')';

        return sig;
    }

    Type Method::returnType() const
    {
        return Type(_table->types[0]);
    }

    int Method::parameterCount() const
    {
        return _table->argc;
    }

    /*TypeList Method::parameterTypes() const
    {
        if (_table)
            return _table->types;
        else
            return TypeList();
    }*/

    Type Method::parmaeterType(int index) const
    {
        return Type(_table->types[index + 1]);
    }

    void Method::call(const void *obj, const void *ret, const void **args) const
    {
        _table->invoker(obj, ret, args);
    }

    bool Method::call(Arg obj, Arg ret, std::initializer_list<Arg> args) const
    {
        if (args.size() < _table->argc)
            return false;

        if (ret.type != _table->types[0])
            return false;

        const void *raw_args[10] = { nullptr };

        auto arg = args.begin();
        unsigned int i = 0;
        for (; i < _table->argc; ++arg, ++i) {
            if (arg->type == _table->types[i + 1])
                raw_args[i] = arg->data;
            else
                return false;
        }

        _table->invoker(obj.data, ret.data, raw_args);

        return true;
    }
}
