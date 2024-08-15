import express from "express";
import path from "path";
import fs from "fs";
import _ from "lodash";

export function images() {
  const router = express.Router();

  const imageDir = path.join(process.cwd(), "images");

  const images = fs.readdirSync(imageDir, {
    withFileTypes: false,
    encoding: "ascii",
  });

  const len = 16;
  const list_len = 4;

  router.get("/list", (req, res) => {
    const pageCandidate = Number.parseInt(req.query.p ?? 0);
    console.log("page:", pageCandidate);
    if (Number.isNaN(pageCandidate)) {
      res.sendStatus(400);
      return;
    }

    const page = pageCandidate;
    if (page + 1 > images.length / list_len) {
      res.send("".repeat(len * list_len));
      return;
    }

    const image_list = images
      .slice(page * list_len, page * list_len + list_len)
      .map((x, i) => (`${i}:` + x.padEnd(len, " ")).substring(0, len))
      .slice();
    console.log(image_list);

    res.send(image_list.join(""));
  });

  router.get("/get", (req, res) => {
    const id = req.query.id;
    if (!id || Array.isArray(id)) {
      res.sendStatus(400);
      res.send("bad id");
      return;
    }
    const entry = Number.parseInt(id);
    console.log({ entry });
    if (Number.isNaN(entry)) {
      res.sendStatus(400);
      return;
    }

    if (entry >= images.length) {
      res.sendStatus(400);
      return;
    }

    const image = images[id];

    console.log({ image });

    res.sendFile(path.join(process.cwd(), "images", image), {
      headers: {
        "Content-Type": "application/octet-stream",
      },
    });
  });

  return router;
}
