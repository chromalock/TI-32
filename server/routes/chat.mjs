import express from "express";
import { readFileSync, writeFileSync } from "fs";
import lodash from "lodash";

// Service to ask ChatGPT questions and get responses under 100 characters.

let rooms = {};

const chatFile = "chat.json";

function updateChat(f) {
  writeFileSync(chatFile, JSON.stringify(rooms, null, 2));
  const current = JSON.parse(readFileSync(chatFile, "ascii"));
  rooms = f(current) ?? rooms;
  writeFileSync(chatFile, JSON.stringify(rooms));
}

function formatChat(n) {
  return rooms[n]?.flatMap((x) => lodash.chunk(x, 16).map((x) => x.join("").padEnd(16, " "))) ?? [];
}

const list_len = 7;

export async function chat() {
  const routes = express.Router();

  routes.get("/messages", (req, res) => {
    const page = Number.parseInt(req.query.p ?? 0);
    if (Number.isNaN(page)) {
      res.sendStatus(400);
      return;
    }
    const c = Number.parseInt(req.query.c);
    if (Number.isNaN(c)) {
      res.sendStatus(400);
      return;
    }

    const messages = formatChat(c);
    const p = Math.max(Math.ceil(messages.length / list_len - 1 - page), 0);

    if (p * list_len > messages.length) {
      res.send("");
      return;
    }

    const rangeEnd = Math.min((p + 1) * list_len, messages.length);
    const rangeStart = Math.max(rangeEnd - list_len, 0);

    const chatLines = messages.slice(rangeStart, rangeEnd);

    console.log(chatLines);

    res.send(chatLines.join(""));
  });

  routes.get("/send", (req, res) => {
    const msg = req.query.m;
    console.log({ msg });
    if (typeof msg !== "string") {
      res.sendStatus(400);
      return;
    }
    const c = Number.parseInt(req.query.c);
    if (Number.isNaN(c)) {
      res.sendStatus(400);
      return;
    }

    const id = req.query.id.substring(0, 3);

    updateChat((k) => {
      k[c] ??= [];
      k[c].push(`${id}:${msg}`);
      return k;
    });

    res.sendStatus(200);
  });

  return routes;
}
