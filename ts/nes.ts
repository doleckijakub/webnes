const NES32_WASM_PATH = "wasm/nes32.wasm";
const FRAMEBUFFER_WIDTH = 256;
const FRAMEBUFFER_HEIGHT = 240;
const FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4;

type pointer = number;

type malloc_t = (size: number) => pointer;
type heap_reset_t = () => void;

type init_nes_t = () => void;
type load_nes_rom_t = (rom_start: pointer, rom_size: number) => void;
type get_nes_framebuffer_t = () => pointer;
type emulate_nes_frame_t = () => void;
type set_nes_key_state_t = (key: number, pressed: bool) => void;

type any_to_any_t = (...args: any[]) => any;

interface NES_Emulator {
	memory: WebAssembly.Memory;
	
	malloc: malloc_t;
	heap_reset: heap_reset_t;

	init_nes: init_nes_t;
	load_nes_rom: load_nes_rom_t;
	get_nes_framebuffer: get_nes_framebuffer_t;
	emulate_nes_frame: emulate_nes_frame_t;
	set_nes_key_state: set_nes_key_state_t;
}

(async () => {

const rom_input = document.getElementById("rom") as HTMLInputElement;

const canvas = document.getElementById('nes-canvas') as HTMLCanvasElement;
const canvas_ctx = canvas.getContext('2d');
const canvas_image_data = canvas_ctx!.createImageData(256, 240);

let uint8_view: undefined | Uint8Array = undefined;
function get_uint8_view() {
	return uint8_view || (uint8_view = new Uint8Array(nes_emulator.memory.buffer));
}

const nes_emulator: NES_Emulator = await WebAssembly.instantiateStreaming(
	fetch(NES32_WASM_PATH), {
		env: {
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
	const iexports = w.instance.exports;
	const memory = iexports.memory as WebAssembly.Memory;

	memory.grow(1);
		
	return {
		memory,

		malloc: iexports.malloc as malloc_t,
		heap_reset: iexports.heap_reset as heap_reset_t,

		init_nes: iexports.init_nes as init_nes_t,
		load_nes_rom: iexports.load_nes_rom as load_nes_rom_t,
		get_nes_framebuffer: iexports.get_nes_framebuffer as get_nes_framebuffer_t,
		emulate_nes_frame: iexports.emulate_nes_frame as emulate_nes_frame_t,
		set_nes_key_state: iexports.set_nes_key_state as set_nes_key_state_t,
	};
});

function update_canvas() {
	const framebuffer_ptr = nes_emulator.get_framebuffer();
	const framebuffer = new Uint8Array(nes_emulator.memory.buffer, framebuffer_ptr, FRAMEBUFFER_SIZE);
	canvas_image_data.data.set(framebuffer);
	canvas_ctx.putImageData(canvas_image_data, 0, 0);
}

function load_nes_rom(rom_data: ArrayBuffer) {
	nes_emulator.heap_reset();

	const buffer = new Uint8Array(rom_data);

	const len = buffer.length;
	const buf = nes_emulator.malloc(len);

	new Uint8Array(nes_emulator.memory.buffer, buf, len).set(buffer);

	nes_emulator.load_nes_rom(buf, len);
}

nes_emulator.init_nes();

rom_input.addEventListener('change', function() {
	const reader = new FileReader();

	reader.onload = function() {
		if(reader.result) load_nes_rom(this.result as ArrayBuffer);
	};

	if(this.files) reader.readAsArrayBuffer(this.files[0]);
}, false);

})();
