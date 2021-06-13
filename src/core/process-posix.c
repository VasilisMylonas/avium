#include "avium/process.h"

#include "avium/typeinfo.h"

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

void AvmProcessTerminate(AvmProcess* self)
{
    pre
    {
        assert(self != NULL);
    }

    kill((pid_t)(ulong)self->_state, SIGTERM);
}

AvmProcess AvmProcessNew(str path, uint length, str args[])
{
    pre
    {
        assert(path != NULL);
    }

    char** argv = AvmAlloc(sizeof(str) * (length + 2));
    argv[0] = (char*)path;
    memcpy(&argv[1], args, length);
    argv[length + 2] = NULL;

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        throw(AvmErrorNew("TODO: Error creating process."));
        break;
    case 0:
        execvp(path, argv);
        _Exit(EXIT_FAILURE);
        break;
    default:
        return (AvmProcess){
            .__type = typeid(AvmProcess),
            ._state = (void*)(ulong)pid,
        };
    }
}

AvmExitCode AvmProcessWait(const AvmProcess* self)
{
    pre
    {
        assert(self != NULL);
    }

    int status;
    waitpid((pid_t)(ulong)self->_state, &status, 0);

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }
    else
    {
        // TODO
        return EXIT_FAILURE;
    }
}
