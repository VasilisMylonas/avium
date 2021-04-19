/**
 * @file examples/classes.c
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Example on classes.
 * @version 0.1
 * @date 2021-04-19
 *
 * @copyright Copyright (c) 2021 Vasilis Mylonas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * How to declare a class.
 *
 * In this example, lets say that we want to declare a simple 'Point' class,
 * that will contain the 2 members for storing the x and y coordinates.
 *
 * We will use the AVM_CLASS macro like so:
 */

// You should include this.
#include <avium/types.h>

AVM_CLASS(Point, object, {
    int _x;
    int _y;
})

/*
 * We have now declared the class.
 *
 * The AVM_CLASS macro takes 3 arguments. The first is the name of the class, in
 * this case 'Point'. The second is the name of the base class. In this example,
 * because we don't want to inherit from another class, we can just put
 * 'object'. The third and last parameter is the class definition enclosed in
 * braces ('{'). This is the actual data of the class. Note that we cannot have
 * a class with no members. By convention, 'private' members should be prefixed
 * with a single underscore.
 *
 * A note for type sizes:
 *
 * The size of a class is calculated as so:
 *
 * M + (B - 8) + 8
 *
 * Where M is the size of the class members and B is the size of the base class.
 * In the above example sizeof(Point) will equal:
 *
 * 2 * sizeof(int) + sizeof(object)
 *
 * Which in x86_64 will be: 2 * 4 + 8 = 16
 *
 * Each class also has a hidden member at the start of it (named _type). This
 * member should always point to a statically allocated AvmType instance
 * corresponding to the class type. This contains information about the class
 * methods (vtable), the class name and the class size.
 *
 * To create the AvmType instance for our Point class, we will use the AVM_TYPE
 * macro like so:
 */

// You should include this.
#include <avium/runtime.h>

AVM_TYPE(Point, {[FnEntryDtor] = NULL});

/*
 * As you can see, the AVM_TYPE macro takes 2 parameters, the class name and a
 * mysterious brace enclosed thing. The second parameter is actually the type's
 * vtable.
 *
 * More explanation about this can be found in the vtable.c example (TODO) but
 * it not important right now.
 *
 * Note, that while the class declaration (AVM_CLASS) should probably be in the
 * public interface (*.h), the type information about that class (AVM_TYPE) does
 * not need to be public, it is marked static anyways, and can be placed in the
 * implementation file (*.c). The way we get access to it will be explained
 * below.
 *
 * A talk about constructors:
 *
 * A constructor is just a function that creates an instance of class and should
 * be named accordingly. Examples of constructor names are: AvmStringNew,
 * AvmStringFrom, etc. Along with performing any type-specific initialization,
 * the constructor is also responsible for connecting the type information with
 * the object instance, ie it should initialize the ._type member. This can be
 * done with the AVM_GET_TYPE macro.
 *
 * Lets create an example constructor for our Point type.
 */

// You should include this.
#include <avium/runtime.h>

Point PointFrom(int x, int y) {
    return (Point){
        ._x = x,
        ._y = y,
        ._type = AVM_GET_TYPE(Point),
    };
}

/*
 * Now this was really simple, wasn't it? The AVM_GET_TYPE macro only takes the
 * type name as an argument and returns a reference to the type information for
 * that type. Note that this macro should normally only be used from
 * constructors.
 */
