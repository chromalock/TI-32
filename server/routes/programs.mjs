import express from "express";
import path from "path";
import fs from "fs";
import _ from "lodash";
import * as p8 from "../../prepare8xp.mjs";

export function programs() {
  const router = express.Router();

  const programDir = path.join(process.cwd(), "programs");

  const programs = fs.readdirSync(programDir, {
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
    if (page + 1 > Math.ceil(programs.length / list_len)) {
      console.log("sending blank");
      res.send("".repeat(len * list_len));
      return;
    }

    const program_list = programs
      .slice(page * list_len, page * list_len + list_len)
      .map((x, i) => (`${i}:` + x.padEnd(len, " ")).substring(0, len))
      .slice();
    console.log(program_list);

    res.send(program_list.join(""));
  });

  router.get("/get_name", (req, res) => {
    const id = req.query.id;
    if (!id || Array.isArray(id)) {
      res.status(400);
      res.send("bad id");
      return;
    }
    const entry = Number.parseInt(id);
    console.log({ entry });
    if (Number.isNaN(entry)) {
      res.sendStatus(400);
      return;
    }

    if (entry >= programs.length) {
      res.sendStatus(400);
      return;
    }

    const program = programs[id].substring(0, 10).toUpperCase();
    res.send(program);
  });

  router.get("/get", (req, res) => {
    const id = req.query.id;
    if (!id || Array.isArray(id)) {
      res.status(400);
      res.send("bad id");
      return;
    }
    const entry = Number.parseInt(id);
    console.log({ entry });
    if (Number.isNaN(entry)) {
      res.sendStatus(400);
      return;
    }

    if (entry >= programs.length) {
      res.sendStatus(400);
      return;
    }

    const program = programs[id];

    console.log({ program });

    const bytes = Buffer.from(p8.prepare8xp(path.join(process.cwd(), "programs", program)));

    res.setHeader("Content-Type", "application/octet-stream");
    res.send(bytes);
  });

  return router;
}
