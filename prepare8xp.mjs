// NAME: prepare8xp.js
//
// This program takes an 8xp program as input and outputs
// a file with the same program, but the header stripped.
//
// `xxd -i` should then be used to create the C code that
// gets pasted into the esp32 sketch.
//
import fs from "fs";

export function prepare8xp(path) {
  const bytes = new Uint8Array(fs.readFileSync(path));

  const programBytes = bytes.subarray(74, bytes.length - 2);
  const varBytes = new Uint8Array([programBytes.length & 0xff, (programBytes.length >> 8) & 0xff, ...programBytes]);

  return varBytes;
}
