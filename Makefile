cc = gcc
cflags = -Wall -Wextra -pedantic -std=c99
build_dir = build

srcs = juno.c
objs = $(srcs:%.c=$(build_dir)/%.o)

$(build_dir)/juno: $(objs)
	$(cc) $(cflags) -o $@ $^

$(build_dir)/%.o: %.c | $(build_dir)
	$(cc) $(cflags) -c -o $@ $<

$(build_dir):
	mkdir -p $(build_dir)

.phony: clean
clean:
	rm -rf $(build_dir)
