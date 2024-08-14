import express from "express";
import fs from "fs";

export function cheatsheet() {
  const router = express.Router();

  router.get("/", (req, res) => {
    const result = fs.readFileSync("note.var");
    console.log({
      note: result.length,
    });
    res.contentType("application/octet-stream");
    res.send(result);
  });

  return router;
}
