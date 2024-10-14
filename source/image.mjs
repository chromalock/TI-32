// @ts-check
import magick from "imagemagick";

async function magic(args) {
  await /** @type {Promise<void>} */ (
    new Promise((resolve, reject) =>
      magick.convert(args, (err, ...args) => {
        // console.log(err, ...args);
        if (err) {
          reject(err);
          return;
        }
        resolve();
      })
    )
  );
}

export async function convertImage(input, output) {
  console.time("crop+interpolate");

  await magic([
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
  ]);
  console.timeEnd("crop+interpolate");
  console.log("output:", output);
}
