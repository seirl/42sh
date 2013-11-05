42sh 1 "General Commands Manual"
================================

NAME
----

42sh - The ROOTS SHell

SYNOPSIS
--------

`42sh` [options] [file]

DESCRIPTION
-----------

`42sh` is probably the best SHell on the Internet. `42sh` is an equivalent of
'bash -posix' meaning that it is intended to be a conformant implementation of
the SHell and Utilies portion of the IEEE POSIX specification.

OPTIONS
-------

`-c` *string*
  If the -c option is present, then the commands are read from *string*.
  If there are more than one argument, each following argument present after
*string* are assigned to the positional parameter starting at $0.

`-i`
  If the -i option is present, then `42sh` will be in interactive mode.

`-foo` 42

`-bar` 42

SHELL GRAMMAR
-------------

Not implemented yet.

AUTHOR
------

Remi \`halfr\` Audebert

Nassim \`Nass\` Eddequiouaq

Adrien \`schischi\` Schildknecht

Paul \`Dettorer\` Hervot

Antoine \`Serialk\` Pietri

SEE ALSO
--------

bash(1)
