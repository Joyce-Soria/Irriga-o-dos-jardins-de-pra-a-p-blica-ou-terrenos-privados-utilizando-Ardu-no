
from flask import Flask, request
import sqlite3

app = Flask(__name__)

def init_db():
    conn = sqlite3.connect('irrigation.db')
    cursor = conn.cursor()
    cursor.execute('''CREATE TABLE IF NOT EXISTS Irrigation
                      (id INTEGER PRIMARY KEY, moisture INTEGER, status TEXT)''')
    conn.commit()
    conn.close()

@app.route('/update', methods=['GET'])
def update():
    moisture = request.args.get('value')
    status = request.args.get('status')
    conn = sqlite3.connect('irrigation.db')
    cursor = conn.cursor()
    cursor.execute("INSERT INTO Irrigation (moisture, status) VALUES (?, ?)", (moisture, status))
    conn.commit()
    conn.close()
    return "Data received", 200

if __name__ == '__main__':
    init_db()
    app.run(host='0.0.0.0', port=5000)
