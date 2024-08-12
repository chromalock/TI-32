from flask import Flask
from flask import request
import pyautogui as gui

app = Flask(__name__)


@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"


@app.route("/discord")
def message():
    gui.moveTo(5000, 10)
    gui.click()
    gui.write(request.args.get("message"))
    gui.press('enter')
    return "200"
