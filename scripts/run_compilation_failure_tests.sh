#!/bin/sh
#
#  Try to compile all programs in the test/compilation_failure directory.
#  Compilation must fail and the error message must match the pattern in the
#  corresponding .pattern file.
#

DIR="test/compilation_failure"
CXX=${CXX:-clang++}

if [ `uname -s` = "Darwin" ]; then
    CXXFLAGS="$CXXFLAGS -stdlib=libc++"
fi

exit_code=0
for test_code in $DIR/*.cpp; do
    name=`basename $test_code .cpp`

    result=`${CXX} -std=c++11 -c -o /dev/null -I. ${CXXFLAGS} ${test_code} 2>&1`
    status=$?

    pattern_file=$DIR/$name.pattern

    if [ $status = 1 ]; then
        if echo $result | grep -q "`cat ${pattern_file}`"; then
            echo "$name [OK]"
        else
            echo "$name [FAILED - wrong error message]"
            echo "compile error was:"
            printf '%s\n' "$result"
            exit_code=1
        fi
    else
        echo "$name [FAILED - compile was successful]"
        exit_code=1
    fi

done

exit ${exit_code}

