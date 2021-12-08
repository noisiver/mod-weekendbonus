#!/usr/bin/env bash
MOD_WEEKENDBONUS_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source $MOD_WEEKENDBONUS_ROOT"/conf/conf.sh.dist"

if [ -f $MOD_WEEKENDBONUS_ROOT"/conf/conf.sh" ]; then
    source $MOD_WEEKENDBONUS_ROOT"/conf/conf.sh"
fi
