make clean
export CFLAGS="-Wall -W -Wextra -O0"
make default -j $((nproc + 1))
