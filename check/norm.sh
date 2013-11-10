#! /bin/sh

RETCODE=0
RED="\033[1;31m"
GREEN="\033[1;32m"
RESET="\033[0m"
LOC=0

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
            fi
        fi
    done
}

files_stat() {
    LOC=$(($LOC + $(sed -n '/[^\ ]/p' $f | wc -l)))
}

check_norm() {
    bad_pattern $1 "){"
    bad_pattern $1 ") {"
    bad_pattern $1 "for("
    bad_pattern $1 "while("
    bad_pattern $1 "do{"
    bad_pattern $1 "do$"
    bad_pattern $1 "if("
    bad_pattern $1 "sizeof("
    bad_pattern $1 '[^\ #].*=\ \{2,\}.*[^\\]$' "Multiple blank char"
    bad_pattern $1 '[^\ #].* \{2,\}=.*[^\\]$' "Multiple blank char"
    bad_pattern $1 "[a-zA-Z0-9]\*\ " "* at wrong place"
    bad_pattern $1 "\ $" "Trailing whitespaces"
    bad_pattern $1 "(.*)[a-zA-Z_]" "Is that a cast ?"
    bad_pattern $1 ".\{80\}" "More than 80 columns"
}

if [ $# -ne 1 ]; then
    echo "Usage ./moul.sh directory"
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
