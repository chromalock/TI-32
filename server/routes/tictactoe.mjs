import express from "express";

export function tictactoe() {
  const router = express.Router();

  router.get("/move", (req, res) => {});

  router.get("/start", (req, res) => {});

  return router;
}
