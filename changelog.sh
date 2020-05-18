#!/bin/bash
# prints kernel RPM changelog, filtered to one component
# eg: changelog.sh ixgbe | less

#set -x
if [ -n $1 ]; then
    LATEST=$(rpm -q kernel | sort -n -r | head -1)
    rpm -q --changelog "$LATEST" | egrep "\[.*el[0-9]\]|$1" | awk '/^\*/{versionheader=$0;printheader=1}/^\-/{if (printheader==1) print versionheader;printheader=0;print $0}'                                                                
fi
