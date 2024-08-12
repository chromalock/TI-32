import express from "express";
import cors from "cors";
import bodyParser from "body-parser";
import morgan from "morgan";
import dot from "dotenv";
import { chatgpt } from "./routes/chatgpt.mjs";
dot.config();

async function main() {
  const port = +(process.env.PORT ?? 8080);
  if (!port || !Number.isInteger(port)) {
    console.error("bad port");
    process.exit(1);
  }

  const app = express();
  app.use(morgan("dev"));
  app.use(cors("*"));
  app.use(
    bodyParser.raw({
      type: "image/jpg",
      limit: "10mb",
    })
  );

  // ChatGPT
  app.use("/gpt", await chatgpt());

  // Chat

  // Notes

  // Answers

  app.listen(port, () => {
    console.log(`listening on ${port}`);
  });
}

main();
