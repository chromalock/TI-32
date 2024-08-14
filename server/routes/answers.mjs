import express from "express";
import csv from "csv-database";
import bodyParser from "body-parser";

export async function answers(file) {
  const routes = express.Router();

  const db = await csv(file, ["question", "answer"]);

  routes.get("/answer", async (req, res) => {
    const q = req.query.question;
    if (Array.isArray(q) || !q) {
      res.status(400);
      res.send("bad question");
      return;
    }
    console.log(q);
    console.log(await db.get({}));
    const result = await db.get({ question: q });
    console.log(result);
    if (result[0]) {
      res.send(result[0].answer);
    } else {
      res.status(404);
      res.send("no answer");
    }
  });

  routes.post("/answer", bodyParser.text("*/*"), async (req, res) => {
    const q = req.query.question;
    if (Array.isArray(q) || !q) {
      res.status(400);
      res.send("bad question");
      return;
    }
    const a = req.body;
    await db.delete({ question: q });
    await db.add({ question: q, answer: a });
  });

  return routes;
}
