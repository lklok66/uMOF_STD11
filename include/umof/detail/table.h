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

#include "../conststring.h"

namespace umof
{
    class Api;

    namespace detail
    {
        using CallMember = void(*)(const void *, const void *, const void **);
        using GetMember = void(*)(const void *, const void *);
        using SetMember = void(*)(const void *, const void *);

        using Name = const char *(*)();
        using Create = void(*)(void**);
        using Construct = void(*)(void**);
        using Destroy = void(*)(void**);
        using Destruct = void(*)(void**);
        using Clone = void(*)(void* const*, void**);
        using Move = void(*)(void* const*, void**);

        //Table with basic functions
        struct TypeTable
        {
            Name name;
            Create create;
            Construct construct;
            Destroy destroy;
            Destruct destruct;
            Clone clone;
            Move move;
            unsigned int size;
            bool is_small;
        };

        struct EnumeratorTable
        {
            ConstString name;
            const unsigned int value;
        };

        struct EnumerationTable
        {
            ConstString name;
            const unsigned int count;
            const EnumeratorTable *table;
        };

        /* \breif Internal struct to store method meta information.
        */
        struct MethodTable
        {
            ConstString name;
            CallMember invoker;
            const unsigned int argc;
            const TypeTable **types;
        };

        /* \breif Internal struct to store property meta information.
        */
        struct PropertyTable
        {
            ConstString name;
            const TypeTable *type;
            GetMember reader;
            SetMember writer;
        };

        /*! \breif Internal struct to store class meta information.
        */
        struct ApiTable
        {
            ConstString name;
            const Api *super;
            const MethodTable *methods;
            const PropertyTable *props;
            const EnumerationTable *enums;
            const unsigned int methodCount;
            const unsigned int propCount;
            const unsigned int enumCount;
        };
    }
}