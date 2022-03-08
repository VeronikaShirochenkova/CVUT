#!/bin/bash

while getopts ":habcd" opt; do
    case $opt in
    h) echo "Funkce skriptu urcena nasledujicimi prepinaci:"
       echo " -h napoveda ke skriptu;"
       echo " -a skript vypise soubory PDF z aktualniho adresare;"
       echo " -b precte text ze vstupu a vypise radky zacinajici cislem, bez tohoto cisla;"
       echo " -c precte text ze vstupu a vypise vsechny vety kazdou na novy radek."
       exit 0
       ;;
    a) ARRAY=$(find . -iname '*.pdf' | sort )
	    for file in ${ARRAY[*]} 
	    do
	      echo "${file#$'\./'}"
	    done
       exit 0
       ;;
    b) sed -n -e 's/^[-+]\?[0-9]\+//p'
       exit 0
       ;;
    c) tr '\n' ' ' | sed -e 's/\. \{1\}/\.\n/g' | sed -e 's/\! \{1\}/\!\n/g' | sed -e 's/\? \{1\}/\?\n/g' | sed -n -e '/^[[:upper:]]/p'
	    exit 0
       ;;
    d) var=${2////\\/}
       sed -e '/[[:alpha:]]*\#[ ]*include[ ]*/s/[<]/\<'"$var"'/' -e '/[[:alpha:]]*\#[ ]*include[ ]*\"[[:print:]]*\"/s/\"/\"'"$var"'/'
       exit 0
       ;;
    ?) echo "command not found"
       echo " -h napoveda ke skriptu;"
       echo " -a skript vypise soubory PDF z aktualniho adresare;"
       echo " -b precte text ze vstupu a vypise radky zacinajici cislem, bez tohoto cisla;"
       echo " -c precte text ze vstupu a vypise vsechny vety kazdou na novy radek."
       exit 1
       ;;
    esac
done
