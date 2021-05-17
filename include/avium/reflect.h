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

AVM_CLASS(AvmModule, object, {
    void* _handle;
    str _name;
});

AVMAPI AvmModule AvmModuleLoad(str path);
AVMAPI const AvmModule* AvmModuleGetCurrent(void);
AVMAPI str AvmModuleGetName(const AvmModule* self);
AVMAPI bool AvmModuleHasSymbol(const AvmModule* self, str name);
AVMAPI const AvmType* AvmModuleGetType(const AvmModule* self, str name);
AVMAPI AvmFunction AvmModuleGetFunction(const AvmModule* self, str name);
AVMAPI void* AvmModuleGetVariable(const AvmModule* self, str name);

#endif // AVIUM_REFLECT_H
