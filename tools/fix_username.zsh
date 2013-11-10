#!/usr/bin/env zsh

export BAD_NAME="Joel Courtois"
export COOL_NAME="Superman"
export COOL_EMAIL="super@man.universe"

git filter-branch -f --commit-filter '
    if [ "$GIT_COMMITTER_NAME" = "$BAD_NAME" ];
        then
                GIT_COMMITTER_NAME="$COOL_NAME";
                GIT_AUTHOR_NAME="$COOL_NAME";
                GIT_COMMITTER_EMAIL="$COOL_EMAIL";
                GIT_AUTHOR_EMAIL="$COOL_EMAIL";
                git commit-tree "$@";
        else
                git commit-tree "$@";
        fi' HEAD
