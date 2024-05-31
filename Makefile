.PHONY: serve clean

HOST_PORT ?= 8080

serve: wasm/nes32.wasm js/nes.js index.html
	python3 -m http.server $(HOST_PORT)

js/nes.js: ts/nes.ts tsconfig.json
	@mkdir -p js
	tsc

SOURCE_FILES = $(wildcard src/*.c)

wasm/nes32.wasm: $(SOURCE_FILES)
	@mkdir -p wasm
	clang \
	--target=wasm32 \
	-fno-builtin \
	--no-standard-libraries \
	-Wl,--export=nes_init \
	-Wl,--export=nes_load_rom \
	-Wl,--export=nes_get_framebuffer \
	-Wl,--export=nes_emulate_frame \
	-Wl,--export=nes_set_key_state \
	-Wl,--export=malloc \
	-Wl,--export=__heap_reset \
	-Wl,--allow-undefined \
	-Wl,--no-entry \
	-I./src/ \
	-o $@ \
	$(SOURCE_FILES)

clean:
	rm -rf js wasm
