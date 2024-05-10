const NES32_WASM_PATH = "wasm/nes32.wasm";

type pointer = number;

type emulate_nes_rom_t = (rom_start: pointer, rom_size: number) => void;
type malloc_t = (size: number) => pointer;
type heap_reset_t = () => void;
type any_to_any_t = (...args: any[]) => any;

interface NES_Emulator {
	memory: WebAssembly.Memory;
	malloc: malloc_t;
	emulate_nes_rom: emulate_nes_rom_t;
	heap_reset: heap_reset_t;
}

const nes_emulator: Promise<NES_Emulator> = WebAssembly.instantiateStreaming(
	fetch(NES32_WASM_PATH), {
		env: {
			putd: console.log
		}
	})
.then(w => {
	const memory = w.instance.exports.memory as WebAssembly.Memory;

	memory.grow(1);
		
	return {
		memory,
		malloc: w.instance.exports.malloc as malloc_t,
		emulate_nes_rom: w.instance.exports.emulate_nes_rom as emulate_nes_rom_t,
		heap_reset: w.instance.exports.heap_reset as heap_reset_t,
	};
});

function emulate_nes_rom(rom_data: ArrayBuffer): any {
	nes_emulator.then(_nes_emulator => {
		_nes_emulator.heap_reset();

		const buffer = new Uint8Array(rom_data);

		const len = buffer.length;
		const buf = _nes_emulator.malloc(len);

		new Uint8Array(_nes_emulator.memory.buffer, buf, len).set(buffer);

		_nes_emulator.emulate_nes_rom(buf, len);
	});
}

const rom_input = document.getElementById("rom") as HTMLInputElement;

rom_input.addEventListener('change', function() {
	const reader = new FileReader();

	reader.onload = function() {
		if(reader.result) emulate_nes_rom(this.result as ArrayBuffer);
	};

	if(this.files) reader.readAsArrayBuffer(this.files[0]);
}, false);
