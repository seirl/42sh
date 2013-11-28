42sh 1 "General Commands Manual"
================================

NAME
----

42sh - The ROOTS SHell: a shell that don't segfault with only 2 bytes.

SYNOPSIS
--------

`42sh` [options] [file]

COPYRIGHT
---------
    42sh is licensed under the Beer-Ware License by the hs24 team.

DESCRIPTION
-----------

`42sh` is probably the best SHell on the Internet. `42sh` is an equivalent of
'bash -posix' meaning that it is intended to be a conformant implementation of
the SHell and Utilies portion of the IEEE POSIX specification.

Special built-in commands are available, the following are fully fonctionnal:
- alias: define or display aliases
- break: exit from while, fr or until loop
- cd: change the working directory
- echo: display a line of text
- exec: execute comands and open, close or copy file descriptors
- exit: cause the shell to exit
- history: manipulate the history file
- set: set options and positional parameters
- source: evaluate a file or resource as a TCL script
- unalias: remove alias definition
- unset: unset options and positional parameters

OPTIONS
-------

`-c` *string*
  If the -c option is present, then the commands are read from *string*.

`[-+]O` *shell option*
  Set or unset one of the shell options accepted by the shopt *builtin*.

`--norc`
  Ignore the resources files *~/.42shrc* and */etc/42shrc*

`--ast-print`
  Display the Abstract Syntax Tree of all the commands.

`--version`
  Display the version on standard output and exit.

`--help`
  Display a usage message on standard output and exit.

BUILTINS
--------

`alias [alias-name[=string] ...]`
    define or display aliases
    The alias utility shall create or define again the alias definition.

`break [n]`
    exit from while, fr or until loop.
    The 'n' parameter is optionnal. The default and smallest value possible
    is 1. The break built-in causes the execution to immediately
    return from a loop.

`cd [directory | -]`
    change the working directory
    The argument is optionnal. If the user doesn’t specify any directory.
    If no directory operand is given and the *HOME* environment variable is
    set, the *cd* command shall behave as if the parameter was the value of
    the *HOME* environment variable.
    Else, the shell shall change the working directory to the value of
    *directory* if valid.

`echo [OPTION] [STRING]`
    Display a line of text to the standard output
    -n: do not ouput the trailing newline character
    -e: enable interpretation of backslash escapes
    -E: disable interpretation of backslash escapes

`exec [command [argument ...]]`
    execute comands and open, close or copy file descriptors.
    The *exec* utility shall open, copy, clode file descriptors as commanded
    through redirections from the command.
    If exec is specified with command, it shall replace the shell with
    command without creating a process. Redirections affect the current
    shell execution environment.

`exit [n]`
    cause the shell to exit
    The *exit* built-in shall cause the shell to exit with the exit number
    specified by the variable *n*.

`history ?option ?arg arg ...?`
    manipulate the history file

`set [options][arguments]`
`set -o|+o`
    set options and positional parameters
    If no options or arguments are specified, set shall write the values of all
    shell variable in the collation sequence of the current locale.

`source filename`
    evaluate a file or resource as a TCL script
    This built-in takes the content of the specified file and passes it to the
    TCL interpreter as a text script.

`unalias alias-name|-1`
    remove alias definition
    -a: remove all alias definitions frm the current shell execution
    environment

`unset [-fv] name ...`
    unset options and positional parameters
    Each viariable or function specified by name shall be unset.
    If -v is specified, name refers to a variable.
    If -f is specified, name refers to a function

AUTHOR
------

Rémi \`halfr\` Audebert

Nassim \`Nass\` Eddequiouaq

Paul \`Dettorer\` Hervot

Antoine \`Serialk\` Pietri

Adrien \`schischi\` Schildknecht

SEE ALSO
--------

__bash__(1) __beer__(7) __sh__(1)

BUG REPORTS
-----------
If you find a bug in 42sh, you should report it. But please make sure to run
the latest version of 42sh available at *http://42sh.greenit-over-the-cloud.fr*

To report a bug, just send a letter or sms to *Dave Null*, with the following
things:

  - The version number
  - If you are using FreeBSD or not
  - A description of the bug and how to reproduce it
  - A tasteful beer
