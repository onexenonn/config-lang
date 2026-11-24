#!/usr/bin/env bash
set -euo pipefail

if [ ! -x ./config ]; then
    make
fi

exec ./config "$@"
