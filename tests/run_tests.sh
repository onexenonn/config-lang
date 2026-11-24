#!/usr/bin/env bash
set -euo pipefail

BIN="./config"
FAILED=0

for input in tests/cases/*.in; do
    base="${input%.in}"
    expected="${base}.out"
    actual="/tmp/config_lang_actual.toml"
    errors="/tmp/config_lang_errors.txt"

    if ! "$BIN" < "$input" > "$actual" 2> "$errors"; then
        echo "FAIL (exit code): $input"
        cat "$errors"
        FAILED=1
        continue
    fi

    if ! diff -u "$expected" "$actual"; then
        echo "FAIL (output): $input"
        FAILED=1
    else
        echo "PASS: $input"
    fi
done

if "$BIN" < tests/error.in > /dev/null 2> /tmp/config_lang_error.txt; then
    echo "FAIL: syntax error was not detected"
    FAILED=1
else
    if grep -qi "syntax error" /tmp/config_lang_error.txt; then
        echo "PASS: syntax error"
    else
        echo "FAIL: expected syntax error message"
        FAILED=1
    fi
fi

exit "$FAILED"
