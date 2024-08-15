// @ts-check
import { convertImage } from "./image.mjs";
import fs, { writeFileSync } from "fs";
import path from "path";

async function main() {
  const input = process.argv[2];
  const output = process.argv[3] ?? `./cache/${path.parse(input).name}.out.bmp`;

  await convertImage(input, output);
  const bytes = fs.readFileSync(output);
  const truncated = bytes.subarray(bytes.length - 756, bytes.length).map((x) => ~x);
  writeFileSync(`server/images/${path.parse(input).name}`, truncated);
}

main();
