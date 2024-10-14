import express from "express";
import axios from "axios";

const API_KEY = "";

// 1.5 pro exp 8-27 (uncomment to use)
//const API_URL = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-pro-exp-0827:generateContent";

// 1.5 flash
const API_URL = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent";


export async function googleApi() {
  const routes = express.Router();

  // Simple question answering
  routes.get("/ask", async (req, res) => {
    const question = req.query.question ?? "";
    if (Array.isArray(question)) {
      res.sendStatus(400);
      return;
    }

    try {
      const response = await axios.post(`${API_URL}?key=${API_KEY}`, {
        contents: [
          {
            role: "user",
            parts: [{ text: question }]
          }
        ],
        systemInstruction: {
          role: "user",
          parts: [{ text: "Do not use emojis. Do not use symbols." }]
        },
        generationConfig: {
          temperature: 1,
          topK: 64,
          topP: 0.95,
          maxOutputTokens: 1024,
          responseMimeType: "text/plain"
        }
      });

      const result = response.data.candidates[0]?.content?.parts[0]?.text || "No response (ERROR)";
      res.send(result);
    } catch (e) {
      console.error(e);
      res.sendStatus(500);
    }
  });

  return routes;
}
