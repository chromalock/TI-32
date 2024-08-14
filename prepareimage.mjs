import magick from "imagemagick";

async function main() {
  const input = process.argv[2];
  const output = process.argv[3] ?? `${input}.out.bmp`;

  console.log("input:", input);
  console.time("crop+interpolate");
  await new Promise((resolve, reject) =>
    magick.convert(
      [
        input,
        "-gravity",
        "center",
        "-crop",
        "96:63",
        "-type",
        "bilevel",
        "-depth",
        "1",
        "-monochrome",
        "-interpolate",
        "Integer",
        "-filter",
        "point",
        "-colors",
        "2",
        "-interpolative-resize",
        "96x63",
        "-monochrome",
        "-flip",
        "-negate",
        output,
      ],
      (err) => {
        console.timeEnd("crop+interpolate");
        console.log("output:", output);
        if (err) {
          reject(err);
          return;
        }
        resolve();
      }
    )
  );
}

main();
