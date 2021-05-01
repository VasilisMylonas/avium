/**
 * @file examples/vtable.c
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Example on vtables.
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
 * How vtables work in Avium.
 *
 * This example assumes that you have taken a look at examples/classes.c first.
 *
 * In Avium vtables, also referred to as virtual function tables (VFT), are
 * implemented as arrays of function pointers. These are stored in the type
 * information for a class. Currently all VFT's have a max capacity of
 * AVM_VFT_SIZE but this is subject to change. Each entry in the VFT points to
 * a specific function. They are useful for implementing interfaces and
 * overriding functions.
 *
 * For example, remember our Point class from the previous example? Let's say
 * that we want to add a related function called PointDistance that calculates
 * the distance of that point from 0.0. An implementation could be like so:
 */

uint PointDistance(Point* self) {
    return sqrt(pow(self._x, 2) + pow(self._y, 2));
}

/*
 * And this would work just fine for our situation.
 *
 * But this only works for 2D points. What if we had a 3D point? We would have
 * to write a new class an create new function (Point3DDistance?). Because the
 * point distance operation can work for 1D, 2D and 3D points. It would be nice
 * to have 1 public interface that works with all of them. Let's see how we can
 * do that.
 */

// In the public interface:

AVM_CLASS(Point1D, object, { int _x; })

AVM_CLASS(Point2D, object, {
    int _x;
    int _y;
})

AVM_CLASS(Point3D, object, {
    int _x;
    int _y;
    int _z;
})

// We will discuss more about this later.
MY_API uint PointDistance(object self);

// In the implementation file:

// These can be marked static.

static uint Point1DDistance(Point1D* self) { ... }
static uint Point2DDistance(Point2D* self) { ... }
static uint Point3DDistance(Point3D* self) { ... }

// Because each entry in the VFT is represented by an index, let's reserve one
// for our function:

// This could also be in an enum.
#define DistanceEntry 20

// Now, when we use AVM_TYPE to define the type information we overwrite the
// entry at the index defined by DistanceEntry (20 in our case) with the
// corresponding functions.

AVM_TYPE(Point1D, {[DistanceEntry] = Point1DDistance})
AVM_TYPE(Point1D, {[DistanceEntry] = Point2DDistance})
AVM_TYPE(Point1D, {[DistanceEntry] = Point3DDistance})

// Depending on your compiler and/or compile options, you may have to cast the
// functions to the AvmFunction type:

AVM_TYPE(Point1D, {[DistanceEntry] = (AvmFunction)Point1DDistance})
AVM_TYPE(Point1D, {[DistanceEntry] = (AvmFunction)Point2DDistance})
AVM_TYPE(Point1D, {[DistanceEntry] = (AvmFunction)Point3DDistance})

// Typedefing this here is not required but is good for understanding.
typedef uint (*DistanceFunc)(object);

// In this function we just have to call the DistanceEntry entry in the provided
// object.
uint PointDistance(object self) {
    // There are plans to make this simpler. But for now this how it is done.

    const AvmType* type = AvmObjectGetType(self);
    return ((DistanceFunc)AvmTypeGetFunction(type, DistanceEntry))(self);
}

/*
 * It wasn't that hard was it? Now calling PointDistance works with Point1D,
 * Point2D, Point3D and Point4D is we decide to create such a thing in the
 * future.
 *
 * You may have noticed that our function has a small problem. Its self
 * parameter is of type object (aka void*) which means that we can pass anything
 * we want and the compiler won't warn us.
 *
 * See how we can fix this in the interface.c example (TODO).
 */
