import fs from "fs";

const bytes = new Uint8Array(fs.readFileSync(process.argv[2]));

const outBytes = bytes.subarray(74, bytes.length - 2);

fs.writeFileSync("./out.txb", outBytes);
