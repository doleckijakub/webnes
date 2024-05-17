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

(async () => {

let uint8_view: undefined | Uint8Array = undefined;
function get_uint8_view() {
	return uint8_view || (uint8_view = new Uint8Array(nes_emulator.memory.buffer));
}

const nes_emulator: NES_Emulator = await WebAssembly.instantiateStreaming(
	fetch(NES32_WASM_PATH), {
		env: {
			console_log: console.log,
			puts: (ptr: pointer) => {
				const buffer = get_uint8_view();

				let str = '';
				let index = ptr;

				while (buffer[index] !== 0) {
					str += String.fromCharCode(buffer[index]);
					index++;
				}

				console.log(str);
			}
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
	nes_emulator.heap_reset();

	const buffer = new Uint8Array(rom_data);

	const len = buffer.length;
	const buf = nes_emulator.malloc(len);

	new Uint8Array(nes_emulator.memory.buffer, buf, len).set(buffer);

	nes_emulator.emulate_nes_rom(buf, len);
}

const rom_input = document.getElementById("rom") as HTMLInputElement;

rom_input.addEventListener('change', function() {
	const reader = new FileReader();

	reader.onload = function() {
		if(reader.result) emulate_nes_rom(this.result as ArrayBuffer);
	};

	if(this.files) reader.readAsArrayBuffer(this.files[0]);
}, false);

})();
