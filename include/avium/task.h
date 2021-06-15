/**
 * @file avium/task.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Coroutine (task) support.
 * @version 0.1
 * @date 2021-06-15
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

#ifndef AVIUM_TASK_H
#define AVIUM_TASK_H

#include "avium/core.h"

/// Holds the state of a coroutine.
AVM_CLASS(AvmTask, object, {
    struct
    {
        const AvmTask* previous;
        void* state;
        void* stack;
        str name;
        object retval;
        uint id;
    } _private;
});

/// If this is returned, it means the task completed.
#define AVM_TASK_EXITED ((void*)1)

typedef void (*AvmTaskEntryPoint)(object);

/**
 * @brief Creates a new task.
 *
 * @pre entry != NULL.
 * @post return != NULL.
 *
 * @param entry The task entry point.
 * @param value An argument to pass to the task or NULL.
 * @return The created task.
 */
AVMAPI AvmTask* AvmTaskNew(AvmTaskEntryPoint entry, object value);

/**
 * @brief Creates a new task.
 *
 * @pre entry != NULL.
 * @pre stackSize > 4096.
 * @pre name != NULL.
 * @pre stackSize % 16 == 0.
 * @post return != NULL.
 *
 * @param entry The task entry point.
 * @param value An argument to pass to the task or NULL.
 * @param stackSize The stack size.
 * @param name The task name.
 *
 * @return The created task.
 */
AVMAPI AvmTask* AvmTaskNewEx(AvmTaskEntryPoint entry,
                             object value,
                             uint stackSize,
                             str name);

/**
 * @brief Returns the main task.
 *
 * @post return != NULL.
 *
 * @return The main task.
 */
AVMAPI const AvmTask* AvmTaskGetMain(void);

/**
 * @brief Returns the current task.
 *
 * @post return != NULL.
 *
 * @return The current task.
 */
AVMAPI const AvmTask* AvmTaskGetCurrent(void);

/**
 * @brief Returns the name of a task.
 *
 * @pre self != NULL.
 * @post return != NULL.
 *
 * @param self The AvmTask instance.
 * @return The task name.
 */
AVMAPI str AvmTaskGetName(const AvmTask* self);

/**
 * @brief Returns the runtime ID of a task.
 *
 * @pre self != NULL.
 *
 * @param self The AvmTask instance.
 * @return The task ID.
 */
AVMAPI uint AvmTaskGetID(const AvmTask* self);

/**
 * @brief Switches to a task, pausing the current one.
 *
 * @pre self != NULL.
 *
 * @warning It is undefined behavior to call AvmTaskSwitchTo to switch to a
 *          previous task.
 *
 * @li A return of NULL is equivalent to a void return value
 *     (AvmTaskSwitchBack).
 *
 * @li A return of AVM_TASK_EXIT means that the task finished complete execution
 *     and has reset.
 *
 * @li A return of any other object is a normal return
 *     (AvmTaskReturn).
 *
 * @param self The AvmTask instance.
 * @return The value returned by the task.
 */
AVMAPI object AvmTaskSwitchTo(const AvmTask* self);

/**
 * @brief Switches to the previous task, retaining the current state.
 *
 * The calling AvmTaskSwitchTo will return NULL.
 *
 * @exception AvmDetailedError Tried to switch back from the main task.
 */
AVMAPI void AvmTaskSwitchBack(void);

/**
 * @brief Switches to the previous task, retaining the current state and
 *        returning a value.
 *
 * @pre value != NULL.
 * @pre value != AVM_TASK_EXIT.
 *
 * @param value The value to return.
 *
 * @exception AvmDetailedError Tried to switch back from the main task.
 */
AVMAPI void AvmTaskReturn(object value);

/**
 * @brief Exits the current task, signaling its completion.
 *
 * Using return or reaching the end of a coroutine is equivalent to calling
 * AvmTaskExit.
 *
 * @return This function never returns.
 */
AVMAPI never AvmTaskExit(void);

/**
 * @brief Runs a task completely and returns the last returned value.
 *
 * @pre self != NULL.
 * @post return != AVM_TASK_EXIT.
 *
 * @param self The AvmTask instance.
 * @return The last returned value from the task.
 */
AVMAPI object AvmTaskRun(const AvmTask* self);

#ifdef AVM_USE_COROUTINE_KEYWORDS
#define co_switch AvmTaskSwitchTo ///< Shortcut for AvmTaskSwitchTo
#define co_yield AvmTaskReturn    ///< Shortcut for AvmTaskReturn
#define co_back AvmTaskSwitchBack ///< Shortcut for AvmTaskSwitchBack
#define co_run  AvmTaskRun        ///< Shortcut for AvmTaskRun
#define co_exit return            ///< Shortcut for AvmTaskExit
#endif

#endif // AVIUM_TASK_H
