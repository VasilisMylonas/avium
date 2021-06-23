#ifndef AVIUM_VECTOR_H
#define AVIUM_VECTOR_H

#include "avium/core.h"

AVM_CLASS(AvmVector, object, {
    struct
    {
        const AvmClass* type;
        void* items;
        uint length;
        uint capacity;
    } _private;
});

/**
 * @brief Creates a vector of the specified type and capacity.
 *
 * @pre type != NULL.
 *
 * @param type The type of the vector items.
 * @param capacity The number of items to preallocate space for.
 * @return The created vector.
 */
AVMAPI AvmVector AvmVectorNew(const AvmClass* type, uint capacity);

/**
 * @brief Returns the number of items in the vector.
 *
 * @pre self != NULL.
 *
 * @param self The AvmVector instance.
 * @return The number of items in the vector.
 */
AVMAPI uint AvmVectorGetLength(const AvmVector* self);

/**
 * @brief Returns the vector capacity.
 *
 * @pre self != NULL.
 *
 * @param self The AvmVector instance.
 * @return The capacity of the vector.
 */
AVMAPI uint AvmVectorGetCapacity(const AvmVector* self);

/**
 * @brief Returns the type of the vector items.
 *
 * @pre self != NULL.
 * @post return != NULL.
 *
 * @param self The AvmVector instance.
 * @return The type of the vector items.
 */
AVMAPI const AvmClass* AvmVectorGetType(const AvmVector* self);

/**
 * @brief Inserts an item to a vector shifting all other items.
 *
 * @pre self != NULL.
 * @pre index < AvmVectorGetLength(self).
 * @pre item != NULL.
 *
 * @param self The AvmVector instance.
 * @param index The index to insert the item at.
 * @param item The item to insert.
 */
AVMAPI void AvmVectorInsert(AvmVector* self, uint index, object item);

/**
 * @brief Pushes an item to the end of a vector.
 *
 * @pre self != NULL.
 * @pre item != NULL.
 *
 * @param self The AvmVector instance.
 * @param item The item to push.
 */
AVMAPI void AvmVectorPush(AvmVector* self, object item);

/**
 * @brief Pops the last item off the end of a vector and returns it.
 *
 * @pre self != NULL.
 * @pre AvmVectorGetLength(self) != 0.
 *
 * @param self The AvmVector instance.
 * @return The last item.
 */
AVMAPI object AvmVectorPop(AvmVector* self);

/**
 * @brief Returns a reference to the last object in a vector.
 *
 * @pre self != NULL.
 *
 * @param self The AvmVector instance.
 * @return The object reference.
 */
AVMAPI object AvmVectorPeek(const AvmVector* self);

/**
 * @brief Removes the item at the specified index from a vector, shifting all
 *        other items.
 *
 * @pre self != NULL.
 * @pre index < AvmVectorGetLength(self).
 *
 * @param self The AvmVector instance.
 * @param index The index of the item to remove.
 */
AVMAPI void AvmVectorRemove(AvmVector* self, uint index);

/**
 * @brief Reserves space for additional items.
 *
 * @pre self != NULL.
 *
 * @param self The AvmVector instance.
 * @param capacity The extra capacity to reserve.
 */
AVMAPI void AvmVectorReserve(AvmVector* self, uint capacity);

AVMAPI object AvmVectorGet(const AvmVector* self, uint index);

#endif // AVIUM_VECTOR_H
