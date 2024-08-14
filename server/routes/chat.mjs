import express from "express";
import lo from "lowdb/node";

// Service to ask ChatGPT questions and get responses under 100 characters.

export async function chat(file) {
  const routes = express.Router();

  const db = await lo.JSONFilePreset(file, { inboxes: {} });

  routes.get("/recipients", (req, res) => {});

  routes.get("/send", (req, res) => {});

  routes.get("/inbox", (req, res) => {});

  return routes;
}
