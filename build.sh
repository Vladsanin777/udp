make clean
export CFLAGS="-Wall -W -Wextra -O0 -g -fsanitize=address"
make default -j $((nproc + 1))
