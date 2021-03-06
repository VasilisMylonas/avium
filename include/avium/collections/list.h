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

#include "avium/types.h"

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
AVMAPI uint AvmListGetLength(const AvmList* self);

/**
 * @brief Returns the capacity of an AvmList.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @return The capacity of the AvmList.
 */
AVMAPI uint AvmListGetCapacity(const AvmList* self);

/**
 * @brief Returns the type of the contained items.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @return The type of the contained items.
 */
AVMAPI const AvmType* AvmListGetItemType(const AvmList* self);

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
AVMAPI object AvmListItemAt(const AvmList* self, uint index);

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
 */
AVMAPI void AvmListInsert(AvmList* self, uint index, object value);

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
 */
AVMAPI void AvmListRemove(AvmList* self, uint index);

/**
 * @brief Removes all objects from an AvmList, calling finalizers.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 */
AVMAPI void AvmListClear(AvmList* self);

/**
 * @brief Inserts a value at the end of an AvmList.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p value must be not null.
 *
 * @param self The AvmList instance.
 * @param value The value to push.
 */
AVMAPI void AvmListPush(AvmList* self, object value);

/**
 * @brief Removes a value from the end of an AvmList.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmList instance.
 * @return A reference to a heap allocated clone of the value.
 */
AVMAPI object AvmListPop(AvmList* self);

/**
 * @brief Determines whether an AvmList contains a specified value.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p value must be not null.
 *
 * @param self The AvmList instance.
 * @param value The value.
 * @return true if the AvmList contains the value, otherwise false.
 */
AVMAPI bool AvmListContains(const AvmList* self, object value);

/**
 * @brief Returns the index of the first occurrence of a value in an AvmList.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p value must be not null.
 *
 * @param self The AvmList instance.
 * @param value The value.
 * @return The index of the value, or AvmInvalid.
 */
AVMAPI uint AvmListIndexOf(const AvmList* self, object value);

#endif // AVIUM_COLLECTIONS_LIST_H
