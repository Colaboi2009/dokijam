all: compile play

PROJECT := "doki_jam"

compile:
	@mkdir -p build \
		&& cd build \
		&& cmake -S ../ -B . \
		&& make \
		&& cd .. \

release:
	@mkdir -p build \
		&& cd build \
		&& cmake -S ../ -B . \
		&& make \
		&& cd .. \

play:
	@./build/$(PROJECT)

killall:
	ps -h | grep $(PROJECT) | grep -v grep | awk '{print $1}' | xargs -r kill -9

