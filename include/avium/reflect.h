/**
 * @file avium/reflect.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Program introspection utilities.
 * @version 0.1
 * @date 2021-05-16
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

#ifndef AVIUM_REFLECT_H
#define AVIUM_REFLECT_H

#include "avium/core.h"

/**
 * @brief Loads a type based on its qualified name.
 *
 * The qualified name is in the form of library@typename.
 *
 * @pre Parameter @p name must be not null.
 *
 * @param name The qualified name of the type.
 * @return The AvmType instance.
 */
AVMAPI const AvmType* AvmReflectLoadType(str name);

/// Represents a loaded shared object.
AVM_CLASS(AvmModule, object, {
    void* _handle;
    str _name;
});

/**
 * @brief Loads the shared object at the specified path.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path to the share object file.
 * @return The loaded module.
 */
AVMAPI AvmModule* AvmModuleLoad(str path);

/**
 * @brief Unloads a module.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmModule instance.
 */
AVMAPI void AvmModuleUnload(AvmModule* self);

/**
 * @brief Gets a pointer to the currently running module.
 *
 * @return The currently running module.
 */
AVMAPI const AvmModule* AvmModuleGetCurrent(void);

/**
 * @brief Gets the name of a module.
 *
 * @pre Parameter @p self must be not null.
 *
 * @param self The AvmModule instance.
 * @return The name of the module.
 */
AVMAPI str AvmModuleGetName(const AvmModule* self);

/**
 * @brief Determines whether a symbol exists on the specified module.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p name must be not null.
 *
 * @param self The AvmModule instance.
 * @param name The name of the symbol.
 * @return true if the symbol exists, otherwise false.
 */
AVMAPI bool AvmModuleHasSymbol(const AvmModule* self, str name);

/**
 * @brief Gets an AvmType instance from a module.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p name must be not null.
 *
 * @param self The AvmModule instance.
 * @param name The name of the type.
 * @return The AvmType instance.
 */
AVMAPI const AvmType* AvmModuleGetType(const AvmModule* self, str name);

/**
 * @brief Gets a function from a module.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p name must be not null.
 *
 * @param self The AvmModule instance.
 * @param name The name of the function.
 * @return The function.
 */
AVMAPI AvmFunction AvmModuleGetFunction(const AvmModule* self, str name);

/**
 * @brief Gets a variable from a module.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p name must be not null.
 *
 * @param self The AvmModule instance.
 * @param name The name of the variable.
 * @return A pointer to the variable.
 */
AVMAPI void* AvmModuleGetVariable(const AvmModule* self, str name);

#endif // AVIUM_REFLECT_H
