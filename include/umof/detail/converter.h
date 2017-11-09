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

#include <sstream>

namespace umof
{
    namespace detail
    {
        template<class U, class Y>
        struct Converter
        {
            inline static U convert(Any a)
            {
                return static_cast<U>(any_cast<Y>(a));
            }
        };

        template<class U>
        struct Converter <U, const char *>
        {
            inline static U convert(Any a)
            {
                U u;
                std::istringstream(any_cast<const char *>(a)) >> u;
                return u;
            }
        };
    }
}
