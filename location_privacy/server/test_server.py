from flask import Flask, url_for
from flask import request
from flask import jsonify
from flask import json

app = Flask(__name__)

@app.route('/')
def api_root():
	return 'Welcome\n'

@app.route('/articles')
def api_articles():
	return 'List of ' + url_for('api_articles') + '\n'

@app.route('/articles/<articleid>')
def api_article(articleid):
	return 'You are reading ' + articleid + '\n'

@app.route('/hello')
def api_hello():
	data = {
		'hello'		:	'world',
		'number'	:	3
	}

	resp = jsonify(data)
	resp.status_code = 200
	resp.headers['Link'] = 'http://luisrei.com'

	return resp

@app.route('/echo', methods = ['GET', 'POST', 'PATCH', 'PUT', 'DELETE'])
def api_echo():
	if request.method == 'GET':
		return "ECHO: GET\n"

	elif request.method == 'POST':
		return "ECHO: POST\n"

	elif request.method == 'PATCH':
		return "ECHO: PATCH\n"

	elif request.method == 'PUT':
		return "ECHO: PUT\n"

	elif request.method == 'DELETE':
		return "ECHO: DELETE\n"

@app.route('/messages', methods = ['POST'])
def api_message():
	if request.headers['Content-Type'] == 'text/plain':
		return "Text Message: " + request.data

	elif request.headers['Content-Type'] == 'application/json':
		js = request.json
		print json.dumps(js)
		print "test: " + js.get("test")
		return "JSON Message: " + json.dumps(request.json)

	elif request.headers['Content-Type'] == 'application/octet-stream':
		f = open('./binary', 'wb')
		f.write(request.data)
		f.close()
		return "Binary message written!\n"
	else:
		return "415 Unsupported Media Type\n"

@app.errorhandler(404)
def not_found(error=None):
	message = {
		'status': 404,
		'message' : 'Not Found: ' + request.url,
	}
	resp = jsonify(message)
	resp.status_code = 404

	return resp

@app.route('/user/<userid>', methods = ['GET'])
def api_users(userid):
	users = {'1': 'john', '2': 'steve', '3': 'bill'}

	if userid in users:
		return jsonify({userid:users[userid]})
	else:
		return not_found()

if __name__ == '__main__':
	app.run(port=1234)
