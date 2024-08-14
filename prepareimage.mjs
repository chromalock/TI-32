import im from "imagemagick";

async function main() {
  const input = process.argv[2];
  const output = process.argv[3] ?? `${input}.out.bmp`;

  console.log("crop+interpolate:", input);
  await new Promise((resolve, reject) =>
    im.convert(
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
        output,
      ],
      (err, meta) => {
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
