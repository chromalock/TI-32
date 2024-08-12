// NAME: prepare8xp.js
//
// This program takes an 8xp program as input and outputs
// a file with the same program, but the header stripped.
//
// `xxd -i` should then be used to create the C code that
// gets pasted into the esp32 sketch.
//
import fs from "fs";

const bytes = new Uint8Array(fs.readFileSync(process.argv[2]));

const programBytes = bytes.subarray(74, bytes.length - 2);
const varBytes = new Uint8Array([programBytes.length & 0xff, (programBytes.length >> 8) & 0xff, ...programBytes]);

const outFile = process.argv[3] ?? "out.var";

fs.writeFileSync(outFile, varBytes);
