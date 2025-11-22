from flask import Flask, render_template, send_from_directory

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("calendar.html")

@app.route("/events.json")
def events():
    return send_from_directory("static", "events.json")

if __name__ == "__main__":
    app.run(debug=True)
