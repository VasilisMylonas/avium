/**
 * @file avium/list.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Interface for list-like collections.
 * @version 0.1
 * @date 2021-05-25
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

#ifndef AVIUM_COLLECTIONS_LIST_H
#define AVIUM_COLLECTIONS_LIST_H

#include "avium/error.h"
#include "avium/typeinfo.h"

/// Interface for list-like collections.
AVM_INTERFACE(AvmList);

/**
 * @brief Returns the length of an AvmList.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @return The length of the AvmList.
 */
AVMINLINE uint AvmListGetLength(const AvmList* self)
    __virtual(uint, FnEntryGetLength);

/**
 * @brief Returns the capacity of an AvmList.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @return The capacity of the AvmList.
 */
AVMINLINE uint AvmListGetCapacity(const AvmList* self)
    __virtual(uint, FnEntryGetCapacity);

/**
 * @brief Returns the type of the contained items.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @return The type of the contained items.
 */
AVMINLINE const AvmType* AvmListGetItemType(const AvmList* self)
    __virtual(const AvmType*, FnEntryGetItemType);

/**
 * @brief Returns a reference to the object at the specified index in an
 *        AvmList.
 *
 * @pre Parameter @p self must be not null.
 *
 * The index must be less than the collection length.
 *
 * @param self The AvmList instance.
 * @param index The object index.
 * @return A reference to the object at the specified index.
 */
AVMINLINE object AvmListItemAt(const AvmList* self, uint index)
    __virtual(object, FnEntryGetItemType, index);

/**
 * @brief Inserts an object at the specified index in an AvmList.
 *
 * The index must be less or equal to the collection length.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p value must be not null.
 *
 * @param self The AvmList instance.
 * @param index The index to insert the object at.
 * @param value The object to insert.
 *
 * @return Any error that occurred.
 */
AVMINLINE AvmError* AvmListInsert(AvmList* self, uint index, object value)
    __virtual(AvmError*, FnEntryInsert, index, value);

/**
 * @brief Removes the object at the specified index from an AvmList, calling
 *        finalizers.
 *
 * The index must be less than the collection length.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @param index The object index.
 * @return Any error that occurred.
 */
AVMINLINE AvmError* AvmListRemove(AvmList* self, uint index)
    __virtual(AvmError*, FnEntryRemove, index);

/**
 * @brief Removes all objects from an AvmList, calling finalizers.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 */
AVMINLINE void AvmListClear(AvmList* self) __virtual(void, FnEntryClear);

/**
 * @brief Inserts a value at the end of an AvmList.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @param value The value to push.
 * @return Any error that occurred.
 */
AVMAPI AvmError* AvmListPush(AvmList* self, object value);

AVMAPI object AvmListPop(AvmList* self);
AVMAPI bool AvmListContains(AvmList* self, object value);
AVMAPI uint AvmListIndexOf(AvmList* self, object value);

#endif // AVIUM_COLLECTIONS_LIST_H
