#  OLOG 
import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from flask_jsglue import JSGlue
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from collections import namedtuple

from helpers import *

# configure application
app = Flask(__name__)
JSGlue(app)

API_KEY = 'AIzaSyB_3VU-RQD0dLYJ-pAAIjvRvDKTMLYou74'

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure db 
db = SQL("sqlite:///olog.db")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # user logs in using email OR username...query database
        # get username_email from form
        #username_email = request.form.get("username_email")
        rows = db.execute("SELECT * FROM users WHERE (username=:username OR email=:email)", username=request.form.get("useremail"), email=request.form.get("useremail"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            flash("invalid username or password")
            return redirect(url_for("login"))

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


#@app.route("/logout")
#def logout():
#    """Log user out."""

    # forget any user_id
#    session.clear()

    # redirect user to login form
#    return redirect(url_for("login"))

@app.route("/")
#@login_required
def index():
    return render_template("trips.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id 
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":


        # secure the password
        hash = pwd_context.encrypt(request.form.get("password"))
        
        # try to insert username, email, and password into table
        result = db.execute("INSERT INTO users (username, email, hash) VALUES (:username, :email, :hash)", username=request.form.get("username"), email=request.form.get("email"), hash=hash)
      
        # check if username/email is already in db...unique fields return null
        if not result:
            flash("username OR email already exists!!")
            return render_template("register.html")
            
        # log in user and query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("register.html")
        


@app.route("/trips")
#@login_required
def trips():
    """Render map."""
    return render_template("trips.html", key=API_KEY)


@app.route("/training")
#@login_required
def training():
    return render_template("training.html")

@app.route("/certs")
#@login_required
def certs():
    return render_template("certs.html")
    
    
@app.route("/logout")
#@login_required
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        new_pass = request.form.get("new_pass")
        pass_confirm = request.form.get("pass_confirm")
        
        
        # secure the password
        hash = pwd_context.encrypt(request.form.get("new_pass"))
        
        # update table
        result = db.execute("UPDATE users SET hash = :hash WHERE username = :user_id", hash = hash, user_id = session["user_id"])

        # inform user
        flash("password changed!")
        
        # go to index
        return redirect(url_for("index"))

    else:
        return render_template("settings.html")
        
@app.route("/saveTrip", methods=["GET", "POST"])
@login_required
def saveTrip():
    
#    if request.method == "POST":
         
        # get trip data
        
#        start_date = request.form.get("start_date")
#        end_date = request.form.get("end_date")
#        trail = request.form.get("trail")
 #       conditions = request.form.get("conditions")
  #      elevation = request.form.get("elevation")
   #     weather = request.form.get("weather")
    #    role = request.form.get("role")
        

#        markers_array = []
#        markers_array = request.form.get("markers")
 #       paths_array = []
  #      request.form.get("path")
        
        # store data in to the db
    #    db.execute("UPDATE trips SET start_date=:start_date, end_date=:end_date, trail=:trail, conditions=:conditions, elevation=:elevation, weather=:weather, role=:role", start_date=start_date, end_date=end_date, trail=trail, conditions=conditions, elevation=elevation, weather=weather, role=role)
        
        # select number of records from 
     #   trip_id = db.execute("SELECT trip_id FROM trips WHERE user_id=:user_id", user_id=session["user_id"])
        
        # update markers table in db
      #  for i in range(len(markers_array)):
       #     db.execute("UPDATE markers SET trip_id=:trip_id, lat=:mlat, lng=:mlng, description=:description", trip_id=trip_id , mlat=markers[i][1], mlng=[i][2] , description="something in here")
        
        # update paths table in db
#        for j in range(len(paths_array)):
 #           db.execute("UPDATE paths SET trip_id=:trip_id, lat=:plat, lng=:plng", trip_id=trip_id, plat=paths_array[i][lat:], plng=paths_array[i][lng:])

    return render_template("trips.html", key=API_KEY)       