const NES32_WASM_PATH = "wasm/nes32.wasm";

type pointer = number;

type emulate_nes_rom_t = (rom_start: pointer, rom_size: number) => void;
type malloc_t = (size: number) => pointer;
type heap_reset_t = () => void;
type any_to_any_t = (...args: any[]) => any;

interface NES_ROM_Emulator {
	memory: WebAssembly.Memory;
	malloc: malloc_t;
	emulate_nes_rom: emulate_nes_rom_t;
	heap_reset: heap_reset_t;
}

const nes_rom_emulator: Promise<NES_ROM_Emulator> = WebAssembly.instantiateStreaming(
	fetch(NES32_WASM_PATH), {
		env: {
			putd: alert
		}
	})
.then(w => {
	const memory = w.instance.exports.memory as WebAssembly.Memory;

	// TODO: memory.grow when needed
		
	return {
		memory,
		malloc: w.instance.exports.malloc as malloc_t,
		emulate_nes_rom: w.instance.exports.emulate_nes_rom as emulate_nes_rom_t,
		heap_reset: w.instance.exports.heap_reset as heap_reset_t,
	};
});

const rom_input = document.getElementById("rom") as HTMLInputElement;

rom_input.addEventListener('change', () => {
	const reader = new FileReader();

	reader.onload = () => {
		console.log(reader.result)
	};

	if(rom_input.files) reader.readAsArrayBuffer(rom_input.files[0]);
}, false);
