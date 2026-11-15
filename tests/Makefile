# Host tests (no PlatformIO, no hardware): `make -C tests test`
.PHONY: test native js
CXX ?= g++
FIX := fixtures

test: native js firmware-syntax

native: $(FIX)
	$(CXX) -std=c++17 -Wall -Wextra -O1 -o /tmp/ipas_core_tests native/test_core.cpp
	/tmp/ipas_core_tests $(FIX)

js:
	timeout 120 node --test js/*.test.js

$(FIX):
	mkdir -p $(FIX)

# Syntax-check the Arduino code against API-shaped stubs (does NOT replace a real PlatformIO build)
.PHONY: firmware-syntax
firmware-syntax:
	$(CXX) -std=c++17 -fsyntax-only -Wall -Wextra -Istubs -I../include ../src/main.cpp
