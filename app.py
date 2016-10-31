from flask import Flask, request, g, render_template
import datetime
import sqlite3
import json
app = Flask(__name__)

lista = {}

@app.route("/" , methods=['GET', 'POST'])
def index():
	global lista

	t = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
	temp = request.values.get("temp")
	ldr = request.values.get("ldr")
	
	conn = sqlite3.connect('sample.db')
	c = conn.cursor()
	c.execute("INSERT into dados (temp, ldr, hora) VALUES (?, ?, ?)", (temp, ldr, t))
	conn.commit()
	conn.close()
	lista = {'temperatura': temp, 'ldr': ldr, 'hora': t}
	print(ldr)
	print(temp)
	print(t)
	
	return "ok"

@app.route("/value" , methods=['GET', 'POST'])
def value():
	data = [lista['hora'], lista['temperatura']]
	return json.dumps(data)

@app.route("/graph")
def graph():
	
	return render_template('chart.html')


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=9090, debug=True)