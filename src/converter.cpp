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

#include <umof/converter.h>

namespace umof
{
    Any Converter<int>::convert(const Any& a)
    {
        if (Type::from<float>() == a.type())
            return detail::Converter<int, double>::convert(a);
        else if (Type::from<const char *>() == a.type())
            return detail::Converter<int, const char *>::convert(a);

        return any_cast<int>(a);
    }

    bool Converter<int>::canConvert(const Any& a)
    {
        return (Type::from<int>() == a.type()
            || Type::from<float>() == a.type()
            || Type::from<const char *>() == a.type());
    }

    Any Converter<float>::convert(const Any& a)
    {
        if (Type::from<int>() == a.type())
            return Any(detail::Converter<float, int>::convert(a));
        else if (Type::from<const char *>() == a.type())
            return Any(detail::Converter<float, const char *>::convert(a));

        return any_cast<float>(a);
    }

    bool Converter<float>::canConvert(const Any& a)
    {
        return (Type::from<float>() == a.type()
            || Type::from<int>() == a.type()
            || Type::from<const char *>() == a.type());
    }
}
