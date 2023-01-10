
all: btime install

btime: btime.c

install: btime
	install $< ~/.local/bin
