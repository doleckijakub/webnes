.PHONY: serve clean

serve: wasm/nes32.wasm js/nes.js index.html
	python3 -m http.server

js/nes.js: ts/nes.ts
	@mkdir -p js
	tsc

SOURCE_FILES = $(wildcard src/*.c)

wasm/nes32.wasm: $(SOURCE_FILES)
	@mkdir -p wasm
	clang \
	--target=wasm32 \
	-fno-builtin \
	--no-standard-libraries \
	-Wl,--export=emulate_nes_rom \
	-Wl,--export=malloc \
	-Wl,--export=heap_reset \
	-Wl,--allow-undefined \
	-Wl,--no-entry \
	-I./src/ \
	-o $@ \
	$(SOURCE_FILES)

clean:
	rm -rf js wasm
