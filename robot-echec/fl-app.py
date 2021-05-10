#!/usr/bin/python

from flask import request,jsonify
from flask_api import FlaskAPI

app=FlaskAPI(__name__)

@app.route('/',methods=["GET"])
def api_root():
    return {"message":"Robot echec avec IA DeepLearning"}

@app.route('/',methods=["POST"])
def send_location():
    return {"request":request.get_data()}

if __name__=="__main__":
    app.run()

