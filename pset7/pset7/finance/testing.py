from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *
from collections import defaultdict

db = SQL("sqlite:///finance.db")

s = []
s.append('don1')
s.append('12345')

print(s[1])

