/**
 * @file avium/thread.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Cross platform thread API.
 * @version 0.1
 * @date 2021-06-08
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

#ifndef AVIUM_THREAD_H
#define AVIUM_THREAD_H

#include "avium/core.h"

AVM_CLASS(AvmMutex, object, { void* _state; });

AVMAPI AvmMutex AvmMutexNew(bool isRecursive);
AVMAPI void AvmMutexUnlock(const AvmMutex* self);
AVMAPI void AvmMutexLock(const AvmMutex* self);

AVM_CLASS(AvmBarrier, object, { void* _state; });

AVMAPI AvmBarrier AvmBarrierNew(uint count);
AVMAPI void AvmBarrierWait(const AvmBarrier* self);

/// Represents a thread of execution.
AVM_CLASS(AvmThread, object, {
    AvmThrowContext* _context;
    void* _state;
    str _name;
    AvmMutex _lock;
    bool _isAlive;
    bool _isDetached;
    uint _stackSize;
});

/// The callback used as a thread entry point.
typedef void (*AvmThreadEntryPoint)(object);

/**
 * @brief Creates a new thread of execution with the default options.
 *
 * @pre Parameter @p entry must not be NULL.
 *
 * @param entry The callback to start executing.
 * @param value An argument to pass to the created thread (can be NULL).
 * @return The created thread object.
 */
AVMAPI AvmThread* AvmThreadNew(AvmThreadEntryPoint entry, object value);

/**
 * @brief Creates a new thread of execution.
 *
 * @pre Parameter @p entry must not be NULL.
 * @pre Parameter @p name must not be NULL.
 *
 * @param entry The callback to start executing.
 * @param value An argument to pass to the created thread (can be NULL).
 * @param stackSize The size of the thread stack.
 * @param name The name of the thread.
 * @return The created thread object.
 */
AVMAPI AvmThread* AvmThreadNewEx(AvmThreadEntryPoint entry,
                                 object value,
                                 uint stackSize,
                                 str name);

/**
 * @brief Determines whether a thread is detached.
 *
 * @pre Parameter @p self must not be NULL.
 *
 * @param self The thread instance.
 * @return true if the thread is detached, otherwise false.
 */
AVMAPI bool AvmThreadIsDetached(const AvmThread* self);

/**
 * @brief Determines whether a thread has stopped executing.
 *
 * @pre Parameter @p self must not be NULL.
 *
 * @param self The thread instance.
 * @return true if the thread is still executing, otherwise false.
 */
AVMAPI bool AvmThreadIsAlive(const AvmThread* self);

/**
 * @brief Returns the name of a thread.
 *
 * @pre Parameter @p self must not be NULL.
 *
 * @param self The thread instance.
 * @return The thread name.
 */
AVMAPI str AvmThreadGetName(const AvmThread* self);

/**
 * @brief Returns the current thread object.
 *
 * @return The current thread.
 */
AVMAPI const AvmThread* AvmThreadGetCurrent();

/**
 * @brief Waits for a thread to finish executing.
 *
 * @pre Parameter @p self must not be NULL.
 *
 * @param self The thread instance.
 * @return The thread exit code.
 */
AVMAPI AvmExitCode AvmThreadJoin(AvmThread* self);

/**
 * @brief Detaches a thread from the current one.
 *
 * AvmThreadJoin can not be called after this.
 *
 * @pre Parameter @p self must not be NULL.
 *
 * @param self The thread instance.
 */
AVMAPI void AvmThreadDetach(AvmThread* self);

/// Yields to another thread.
AVMAPI void AvmThreadYield();

/**
 * @brief Pauses the current thread for a certain amount of time.
 *
 * @param ms The time to pause the thread for.
 */
AVMAPI void AvmThreadSleep(uint ms);

/**
 * @brief Exits the current thread.
 *
 * @param code The exit code to return.
 * @return This function never returns.
 */
AVMAPI never AvmThreadExit(AvmExitCode code);

/**
 * @brief Terminates a thread.
 *
 * @pre Parameter @p self must not be NULL.
 *
 * @param self The thread instance.
 */
AVMAPI void AvmThreadTerminate(AvmThread* self);

/**
 * @brief Gets the size of the a thread stack.
 *
 * @pre Parameter @p self must not be NULL.
 *
 * @param self The thread instance.
 * @return The thread's stack size.
 */
AVMAPI uint AvmThreadGetStackSize(const AvmThread* self);

#endif // AVIUM_THREAD_H
