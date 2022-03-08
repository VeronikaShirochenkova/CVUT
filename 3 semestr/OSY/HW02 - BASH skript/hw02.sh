#!/bin/bash
zp=0 #controla -z
count=0 #pocet souboru  
err=0 #pocet chyb
while getopts ":hz" opt; do
    case $opt in
    h) echo "Skript ktery cte radky ze vstupu. Pro radky zacinajici PATH bude mit output(FILE.../LINK.../DIR.../ERROR...)." 
       echo "Skript ma 2 nepovinné přepínače: "
       echo " -h napoveda ke skriptu;"
       echo " -z vytvori archiv 'output.tgz' ze souboru typu FILE."
       exit 0
        ;;
    z) zp=1
        ;;
    ?) exit 2
        ;;
    esac
done
declare -a FNAME 
SAVEIFS=$IFS
IFS=$'\n'
while read -r line
do
    p=${line:0:5} 
    path=${line:5} 
    if [ "${p}" = "PATH " ]; then 
        for obj in ${path}
        do
            if [ -L "${obj}" ]; then  
                echo "LINK '${obj}' '$(readlink "${obj}")'"
            elif [ -d "${obj}" ]; then 
                echo "DIR '${obj}'"
            elif [ -f "${obj}" ]; then
                count=$(( count+1 ))
                num=$(wc -l "${obj}") || exit 2
                str_num=${num%" ${obj}"}
                frst_str=$(head -n1 "${obj}") || exit 2
                FNAME+=("${obj}")
                if [[ "${str_num}" -eq 0 ]]; then
                    echo "FILE '${obj}' ${str_num}"
                else
                    echo "FILE '${obj}' ${str_num} '${frst_str}'"
                fi
            else 
                echo "ERROR '${obj}'" >&2 
                err=$(( err+1 ))
            fi
        done
    fi
    # pokud byl zadan -z
    if [[ "${zp}" -eq 1 ]]; then 
        if [[ "${count}" -eq 0 ]]; then 
            exit 2
        else
            tar -czf output.tgz "${FNAME[@]}"
        fi
    fi
done
# pokud byla alespon 1 chyba
if [[ "${err}" -gt 0 ]]; then 
    exit 1
fi
IFS=$SAVEIFS
exit 0
