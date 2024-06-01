const NES32_WASM_PATH = "wasm/nes32.wasm";
const FRAMEBUFFER_WIDTH = 256;
const FRAMEBUFFER_HEIGHT = 240;
const FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT * 4;

const KEY_A      = 0x80;
const KEY_B      = 0x40;
const KEY_SELECT = 0x20;
const KEY_START  = 0x10;
const KEY_UP     = 0x08;
const KEY_DOWN   = 0x04;
const KEY_LEFT   = 0x02;
const KEY_RIGHT  = 0x01;

type KeyControllerMapping_t = {
	[key: string]: [number, number]
}

const KEY_CONTROLLER_MAPPINGS: KeyControllerMapping_t = {
	'KeyX': [ 0, KEY_A ],
	'KeyZ': [ 0, KEY_B ],
	'KeyQ': [ 0, KEY_SELECT ],
	'KeyE': [ 0, KEY_START ],
	'KeyW': [ 0, KEY_UP ],
	'KeyS': [ 0, KEY_DOWN ],
	'KeyA': [ 0, KEY_LEFT ],
	'KeyD': [ 0, KEY_RIGHT ],
}

type pointer = number;

type malloc_t = (size: number) => pointer;
type __heap_reset_t = () => void;

type nes_init_t = () => void;
type nes_load_rom_t = (rom_start: pointer, rom_size: number) => void;
type nes_get_framebuffer_t = () => pointer;
type nes_emulate_frame_t = () => void;
type nes_set_key_state_t = (controller: number, key: number, pressed: boolean) => void;

type logger_t = typeof console.log;

interface NES_Emulator {
	memory: WebAssembly.Memory;
	
	malloc: malloc_t;
	__heap_reset: __heap_reset_t;

	nes_init: nes_init_t;
	nes_load_rom: nes_load_rom_t;
	nes_get_framebuffer: nes_get_framebuffer_t;
	nes_emulate_frame: nes_emulate_frame_t;
	nes_set_key_state: nes_set_key_state_t;
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

function log_cstring(ptr: pointer, logger: logger_t) {
	const buffer = get_uint8_view();

	let str = '';
	let index = ptr;

	while (buffer[index] !== 0) {
		str += String.fromCharCode(buffer[index]);
		index++;
	}

	logger(str);
}

const nes_emulator: NES_Emulator = await WebAssembly.instantiateStreaming(
	fetch(NES32_WASM_PATH), {
		env: {
			puts:  (ptr: pointer) => log_cstring(ptr, console.log),
			wputs: (ptr: pointer) => log_cstring(ptr, console.warn),
			eputs: (ptr: pointer) => log_cstring(ptr, console.error),
			tputs: (ptr: pointer) => log_cstring(ptr, console.trace),
		}
	})
.then(w => {
	const iexports = w.instance.exports;
	const memory = iexports.memory as WebAssembly.Memory;

	memory.grow(1);
		
	return {
		memory,

		malloc: iexports.malloc as malloc_t,
		__heap_reset: iexports.__heap_reset as __heap_reset_t,

		nes_init: iexports.nes_init as nes_init_t,
		nes_load_rom: iexports.nes_load_rom as nes_load_rom_t,
		nes_get_framebuffer: iexports.nes_get_framebuffer as nes_get_framebuffer_t,
		nes_emulate_frame: iexports.nes_emulate_frame as nes_emulate_frame_t,
		nes_set_key_state: iexports.nes_set_key_state as nes_set_key_state_t,
	};
});

function update_canvas() {
	const framebuffer_ptr = nes_emulator.nes_get_framebuffer();
	const framebuffer = new Uint8Array(nes_emulator.memory.buffer, framebuffer_ptr, FRAMEBUFFER_SIZE);
	canvas_image_data.data.set(framebuffer);
	canvas_ctx!.putImageData(canvas_image_data, 0, 0);
}

function nes_load_rom(rom_data: ArrayBuffer) {
	rom_input.style.display = "none";
	canvas.style.display = "";

	nes_emulator.__heap_reset();

	const buffer = new Uint8Array(rom_data);

	const len = buffer.length;
	const buf = nes_emulator.malloc(len);

	new Uint8Array(nes_emulator.memory.buffer, buf, len).set(buffer);

	nes_emulator.nes_load_rom(buf, len);
}

function start_emulation() {
	function emulation_loop() {
		console.log("emulation_loop");
		nes_emulator.nes_emulate_frame();
		update_canvas();
		requestAnimationFrame(emulation_loop);
	}
	requestAnimationFrame(emulation_loop);
}

nes_emulator.nes_init();

rom_input.addEventListener('change', function() {
	const reader = new FileReader();

	reader.onload = function() {
		if(reader.result) {
			nes_load_rom(this.result as ArrayBuffer);
			start_emulation();
		}
	};

	if(this.files) reader.readAsArrayBuffer(this.files[0]);
}, false);

document.addEventListener('keydown', function(e) {
	const mapping = KEY_CONTROLLER_MAPPINGS[e.code];
	if (!mapping) return;
	nes_emulator.nes_set_key_state(mapping[0], mapping[1], true);
});

document.addEventListener('keyup', function(e) {
	const mapping = KEY_CONTROLLER_MAPPINGS[e.code];
	if (!mapping) return;
	nes_emulator.nes_set_key_state(mapping[0], mapping[1], false);
});

})();
