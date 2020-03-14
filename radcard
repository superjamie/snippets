#!/bin/bash

# Radcard
# Script to control radeon DPM power saving
# Ref: https://wiki.archlinux.org/index.php/ATI#Powersaving
# Version: 2019-02-12
# License: GPLv3
# Authors: jamie.bainbridge@gmail.com

CARDPATH="/sys/class/drm/card0/device"

do_set() {
    case "$1" in
        bat*)
            sudo sh -c "echo battery > $CARDPATH/power_dpm_state"
            ;;
        bal*)
            sudo sh -c "echo balanced > $CARDPATH/power_dpm_state"
            ;;
        per*)
            sudo sh -c "echo performance > $CARDPATH/power_dpm_state"
            ;;
        a*)
            sudo sh -c "echo auto > $CARDPATH/power_dpm_force_performance_level"
            ;;
        l*)
            sudo sh -c "echo low > $CARDPATH/power_dpm_force_performance_level"
            ;;
        h*)
            sudo sh -c "echo high > $CARDPATH/power_dpm_force_performance_level"
            ;;
        *)
            do_usage
            ;;
    esac
}

do_get() {
    echo -n "power_dpm_state: "; cat "$CARDPATH/power_dpm_state"
    echo -n "power_dpm_force_performance_level: "; cat "$CARDPATH/power_dpm_force_performance_level"
}

do_usage() {
    echo "Usage: $(basename "$0") [get|set [battery|balanced|performance|auto|low|high|bat|bal|per|a|h|l]]"
    exit 1
}

case "$1" in
    "set")
        shift
        for VAR in "$@"; do
          do_set "$VAR"
        done
        do_get
        ;;
    "get")
        do_get
        ;;
    *)
        do_usage
        ;;
esac

exit 0
