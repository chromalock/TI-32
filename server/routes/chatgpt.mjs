import express from "express";
import openai from "openai";
import i264 from "image-to-base64";
import jimp from "jimp";

export async function chatgpt() {
  const routes = express.Router();

  const gpt = new openai.OpenAI();

  // simply answer a question
  routes.get("/ask", async (req, res) => {
    const question = req.query.question ?? "";
    if (Array.isArray(question)) {
      res.sendStatus(400);
      return;
    }

    try {
      const result = await gpt.chat.completions.create({
        messages: [
          {
            role: "system",
            content:
              "You are answering questions for students. Keep responses under 100 characters and only answer using uppercase letters.",
          },
          { role: "user", content: question },
        ],
        model: "gpt-4o-mini",
      });

      res.send(result.choices[0]?.message?.content ?? "no response");
    } catch (e) {
      console.error(e);
      res.sendStatus(500);
    }
  });

  // solve a math equation from an image.
  routes.post("/solve", async (req, res) => {
    try {
      const contentType = req.headers["content-type"];
      console.log("content-type:", contentType);

      if (contentType !== "image/jpg") {
        res.status(400);
        res.send(`bad content-type: ${contentType}`);
      }

      const image_data = await new Promise((resolve, reject) => {
        jimp.read(req.body, (err, value) => {
          if (err) {
            reject(err);
            return;
          }
          resolve(value);
        });
      });

      const image_path = "./to_solve.jpg";

      await image_data.writeAsync(image_path);
      const encoded_image = await i264(image_path);
      console.log("Encoded Image: ", encoded_image.length, "bytes");
      console.log(encoded_image.substring(0, 100));

      const question_number = req.query.n;

      const question = question_number
        ? `What is the answer to question ${question_number}?`
        : "What is the answer to this question?";

      console.log("prompt:", question);

      const result = await gpt.chat.completions.create({
        messages: [
          {
            role: "system",
            content:
              "You are a helpful math tutor, specifically designed to help with basic arithmetic, but also can answer a broad range of math questions from uploaded images. You should provide answers as succinctly as possible, and always under 100 characters. Be as accurate as possible.",
          },
          {
            role: "user",
            content: [
              {
                type: "text",
                text: `${question} Do not explain how you found the answer. If the question is multiple-choice, give the letter answer.`,
              },
              {
                type: "image_url",
                image_url: {
                  url: `data:image/jpeg;base64,${encoded_image}`,
                  detail: "high",
                },
              },
            ],
          },
        ],
        model: "gpt-4o",
      });

      res.send(result.choices[0]?.message?.content ?? "no response");
    } catch (e) {
      console.error(e);
      res.sendStatus(500);
    }
  });

  return routes;
}
