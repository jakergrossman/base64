#!/bin/sh

BUILD_DIR=${BUILD_DIR:-build}
MAKE=${MAKE:-cmake}

set -ex

for subcommand in "$@"; do
    case ${subcommand} in
        build)
            case ${MAKE} in
                cmake)
                    cmake -E make_directory ${BUILD_DIR}
                    cmake -E chdir ${BUILD_DIR} cmake ..
                    cmake --build ${BUILD_DIR}
                    ;;
                make)
                    mkdir -p ${BUILD_DIR}
                    make
                    ;;
            esac
            ;;
        clean)
            rm -rf ${BUILD_DIR}
            ;;
        bear)
            { set +x; } 2>/dev/null
            if [ $(which bear) = "" ]; then
                >&2 echo "bear is not on PATH"
            else
                set -x
                ./${0} clean
                bear -- ./${0} ${BUILD_DIR}
            fi
            ;;
        test)
            ./test/run_tests.py
            ;;
        *)
            { set +x; } 2>/dev/null
            >&2 echo "Invalid subcommand \"${subcommand}\"" 
            exit 1
            ;;
    esac
done
