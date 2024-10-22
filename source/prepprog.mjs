// NAME: prepare8xp.js
//
// This program takes an 8xp program as input and outputs
// a file with the same program, but the header stripped.
//
// `xxd -i` should then be used to create the C code that
// gets pasted into the esp32 sketch.
//
import fs from "fs";
import { prepare8xp } from "./prepare8xp.mjs";

const varBytes = prepare8xp(process.argv[2]);
const outFile = process.argv[3] ?? "out.var";

fs.writeFileSync(outFile, varBytes);
