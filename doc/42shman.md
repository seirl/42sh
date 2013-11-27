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
*TODO*

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
