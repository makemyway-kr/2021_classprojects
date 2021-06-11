#!/usr/bin/env python3
# Foundations of Python Network Programming, Third Edition
# https://github.com/brandon-rhodes/fopnp/blob/m/py3/chapter11/app_insecure.py
# A poorly-written and profoundly insecure payments application.
# (Not the fault of Flask, but of how we are choosing to use it!)

import bank
import urllib
from flask import Flask, redirect, request, url_for
from jinja2 import Environment, PackageLoader
url=''
app = Flask(__name__)
get = Environment(loader=PackageLoader(__name__, 'templates')).get_template
@app.route('/login', methods=['GET', 'POST'])
def login():
    username = request.form.get('username', '')
    password = request.form.get('password', '')
    if request.method == 'POST':
        if (username, password) in [('brandon', 'atigdng')]:
            with open('attack.js') as f:
                query={'flash':f.read().strip().replace('\n',' ')}
                global url
                url = 'http://localhost:5000/?' + urllib.parse.urlencode(query) #makes url
            response = redirect(url)
            response.set_cookie('username', username)
            return response
    return get('login.html').render(username=username)

@app.route(url)#hacker made url page
def index():
    username = request.cookies.get('username')
    pay()
    if not username:
        return redirect(url_for('login'))
    payments = bank.get_payments_of(bank.open_database(), username)
    return get('index.html').render(payments=payments, username=username,
        flash_messages=request.args.getlist('flash'))

def pay():
    username = request.cookies.get('username')
    if not username:
        return redirect(url_for('login'))
    account = 'hacker'
    dollars = '110'
    memo = 'Theft'
    complaint = None
    if request.method == 'POST':
        if account and dollars and dollars.isdigit() and memo:
            db = bank.open_database()
            bank.add_payment(db, username, account, dollars, memo)
            db.commit()
            return redirect(url_for(url, flash='Thanks,brandon'))

if __name__ == '__main__':
    app.debug = True
    app.run()