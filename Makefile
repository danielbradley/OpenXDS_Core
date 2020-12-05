CFLAGS=-Wno-unused-function

all: build

build:
	cd libopenxds_core_io; CFLAGS=$(CFLAGS) build --force --all

