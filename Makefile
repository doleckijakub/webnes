.PHONY: serve clean

serve: wasm/nes32.wasm js/nes.js index.html
	python3 -m http.server

js/nes.js: ts/nes.ts
	@mkdir -p js
	tsc

wasm/nes32.wasm: src/*.c | src/*.h
	@mkdir -p wasm
	clang \
	--target=wasm32 \
	-fno-builtin \
	--no-standard-libraries \
	-Wl,--export=emulate_nes_rom \
	-Wl,--export=malloc \
	-Wl,--allow-undefined \
	-Wl,--no-entry \
	-I./src/ \
	-o $@ \
	$<

clean:
	rm -rf js wasm
