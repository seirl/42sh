#! /usr/bin/env bash

LOC=0
RETCODE=0
RED="\033[1;31m"
GREEN="\033[1;32m"
RESET="\033[0m"

bad_pattern () {
    IFS=$'\n'
    for line in $(grep -n "$2" $1); do
        n=$(echo $line | cut -d: -f1)
        echo "\"${3:-$2}\" at $1:$n  --> $(echo $line | cut -d: -f2)"
        RETCODE=$(($RETCODE + 1))
    done
}

list_files() {
    for f in $1/*; do
        if [ -d $f ]; then
            list_files $f
        elif [ -f $f ]; then
            if [ "${f##*.}" == "c" ] || [ "${f##*.}" == "h" ]; then
                check_norm $f
                files_stat $f
                check_25 $f
                check_last_line $f
            fi
        fi
    done
}

files_stat() {
    LOC=$(($LOC + $(sed -n '/[^\ ]/p' $f | wc -l)))
}

check_last_line() {
    [ -z $(tail -1 $f) ] && echo "Trailling white line in $f"
}

check_25() {
    acc=0
    line=0;
    tot_line=0;
    last=""
    while IFS= read -rn1 c; do
        if [ -z "$c" ]; then           # \n
            if [ $acc -ne 0 ]; then
                if [ "$last" != "" ]; then
                    line=$(($line + 1))
                fi
            else
                if [ $line -gt 26 ]; then
                    echo "Function $(($line - 1)) lines at $1:$tot_line"
                    RETCODE=$(($RETCODE + 1))
                fi
                line=0
            fi
            tot_line=$(($tot_line + 1))
        fi
        if [ "$c" == "{" ]; then
            acc=$(($acc + 1))
        fi
        if [ "$c" == "}" ]; then
            acc=$(($acc - 1))
        fi
        #echo -ne "${c:-\n}"
        last="$c"
    done < $1
}

check_norm() {
    bad_pattern $1 "){"
    bad_pattern $1 ") {"
    bad_pattern $1 " for("
    bad_pattern $1 " while("
    bad_pattern $1 " do{"
    bad_pattern $1 " do$"
    bad_pattern $1 " if("
    bad_pattern $1 "sizeof("
    bad_pattern $1 "[a-zA-Z]=" "Spacing"
    bad_pattern $1 " ," "Spacing"
    bad_pattern $1 ",[^ \\\"]" "Spacing"
    bad_pattern $1 '[^\ #].*=\ \{2,\}.*[^\\]$' "Multiple blank char"
    bad_pattern $1 '[^\ #].* \{2,\}=.*[^\\]$' "Multiple blank char"
    bad_pattern $1 "[a-zA-Z0-9]\*\ " "* at wrong place"
    bad_pattern $1 "\ $" "Trailing whitespaces"
    bad_pattern $1 "(.*)[a-zA-Z_]" "Is that a cast ?"
    bad_pattern $1 ".\{80\}" "More than 80 columns"
    bad_pattern $1 "#\ *define\ *[a-zA-Z_]*[a-z][a-zA-Z_]*[\ \(\\]" "Macro uppercase"
    bad_pattern $1 "#\ *define\ *.*([a-z_].*)" "Macro arg name"
    bad_pattern $1 "#\ *define\ *.*(.*, [a-z_].*)" "Macro arg name"
}

if [ $# -ne 1 ]; then
    echo "Usage ./norm.sh directory"
    exit 0
fi

echo "#####################################"
echo -e "#       ${GREEN}Coding Style Check${RESET}          #"
echo "#####################################"

list_files $1
echo "Lines of code: $LOC"
if [ $RETCODE -eq 0 ]; then
    echo -e "$GREEN[OK]$RESET no errors were found"
    exit 0
fi
echo -e "$RED[FAIL]$RESET $RETCODE errors were found :("
exit 0 #$RETCODE
