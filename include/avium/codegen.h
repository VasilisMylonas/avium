/**
 * @file avium/codegen.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Runtime code generation utilities.
 * @version 0.2
 * @date 2021-05-12
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

#ifndef AVIUM_CODEGEN_H
#define AVIUM_CODEGEN_H

#include "avium/core.h"
#include "avium/typeinfo.h"

/// A helper object for creating AvmType instances.
AVM_CLASS(AvmTypeBuilder, AvmType, { uint _reserved; });

/**
 * @brief Creates an AvmTypeBuilder with a base type.
 *
 * @pre Parameter @p type must be not null.
 *
 * @param type The base type.
 * @return The created AvmTypeBuilder.
 */
AVMAPI AvmTypeBuilder* AvmTypeBuilderBasedOn(const AvmType* type);

/**
 * @brief Sets the name for the AvmType to be created by an AvmTypeBuilder.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p name must be not null.
 *
 * @param self The AvmTypeBuilder instance.
 * @param name The name for the type.
 */
AVMAPI void AvmTypeBuilderSetName(AvmTypeBuilder* self, str name);

/**
 * @brief Sets the VFT for the AvmType to be created by an AvmTypeBuilder.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p vft must be not null.
 *
 * @param self The AvmTypeBuilder instance.
 * @param legnth The length of the VFT.
 * @param vft The VFT for the type.
 */
AVMAPI void AvmTypeBuilderSetVFT(AvmTypeBuilder* self,
                                 uint length,
                                 AvmCallback vft[]);

/**
 * @brief Adds a member to an AvmTypeBuilder.
 *
 * @pre Parameter @p self must be not null.
 * @pre Parameter @p type must be not null.
 *
 * @param self The AvmTypeBuilder instance.
 * @param type The type of the member to be added.
 */
AVMAPI void AvmTypeBuilderAddMember(AvmTypeBuilder* self, const AvmType* type);

#endif // AVIUM_CODEGEN_H
