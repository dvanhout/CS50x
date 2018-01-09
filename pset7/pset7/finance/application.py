from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from collections import namedtuple

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """Show shares of all stocks"""

    # get list of all stocks for user
    db_rows = db.execute("SELECT SUM(shares), stock FROM history WHERE user=:user_id GROUP BY stock HAVING SUM(shares)>0", user_id=session["user_id"])

    # check that user has stocks
    if len(db_rows) == 0:
        return apology("You have no stocks")

    # instantiate data structure for table's cells
    rows = []
    
    # get user's current available
    cash = db.execute("SELECT cash FROM users where id = :user_id", user_id=session["user_id"])
    
    # variable to store user's position (cash + stock value)
    total = 0
    
    # get stock data
    for i in range(len(db_rows)):

        # get stock information
        stock_info = lookup(db_rows[i]["stock"])
        
        # build list of stock data - don't get negative (sold) stock info)
        rows.append((db_rows[i]["stock"], stock_info["name"], db_rows[i]["SUM(shares)"], '${:,.2f}'.format(stock_info["price"]), '${:,.2f}'.format(db_rows[i]["SUM(shares)"] * stock_info["price"])))
        
        # store stock value's
        total += db_rows[i]["SUM(shares)"] * stock_info["price"]
    
    # calculate total <stock value + cash>
    total += cash[0]['cash']
    
    # show the page
    return render_template("index.html", rows=rows, cash='${:,.2f}'.format(cash[0]['cash']), total='${:,.2f}'.format(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
     # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # get stock, checking if it is valid
        symbol = request.form.get("symbol")
        if not symbol:
            return("symbol not provied")
        
        # query for stock information, return error if it's invalid
        purchase = lookup(symbol)
        if not purchase:
            return apology("stock cannot be found")
        
        # get user input shares to buy
        shares = request.form.get("shares")
        
        # check that shares are entered and are a digit
        if not shares or not shares.isdigit() or int(shares) < 1:
            return apology("enter valid number of shares")
        
        # check how much cash the user has available
        cash_available = db.execute("SELECT cash FROM users WHERE id = :user_id AND cash >= :shares * :purchase_price", user_id=session['user_id'], shares=shares, purchase_price=purchase['price'])
        
        # make sure user has sufficient cash
        if len(cash_available) < 1:
            return apology("insufficient funds")
        
        else:
            # subtract from user's cash
            db.execute("UPDATE users SET cash = (cash - :shares * :purchase_price) WHERE id = :user_id", shares=shares, purchase_price=purchase["price"], user_id=session['user_id'])

            # insert purchase into history table
            db.execute("INSERT INTO history (user, stock, price, shares) VALUES (:user, :stock, :price, :shares)", user=session['user_id'], stock=purchase["symbol"], price=purchase["price"], shares=shares)
            
        # tell user that sale was successful 
        flash("Purchase made!")
        
        # back to index to display position
        return redirect(url_for("index"))
        
            
    else:
        # not found
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # query database for user's history
    transactions = db.execute("SELECT stock, date, price, shares FROM history WHERE user = :user_id", user_id=session["user_id"])
    
    # show history
    return render_template("history.html", transactions=transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "POST":

        # ensure symbol was submitted
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("symbol not provided")
        
        # query for the stock info
        quote = lookup(symbol)

        # ensure symbol provided was valid
        if not quote:
            return apology("invalid symbol")
        
        # show the details of the stock
        return render_template("quoted.html", name=quote['name'], symbol=quote['symbol'], price=quote['price'])
        
    else:
        return render_template("quote.html")
    

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id 
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure password and password_verify match
        elif request.form.get("password_verify") != request.form.get("password"):
            return apology("passwords do not match!")
    
        # secure the password
        hash = pwd_context.encrypt(request.form.get("password"))
        
        # try to insert username into table
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash=hash)
      
        # check if username is already in db
        if not result:
            return apology("sorry, username already exists, please try another username")
            
        # log in user and query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("register.html")
        

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # get stock symbol from user, checking if it is valid
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return("invalid stock symbol")
        
        # get shares from user, checking if it's valid and not blank
        shares = request.form.get("shares")
        if not shares or not shares.isdigit() or int(shares) < 1:
            return apology("enter valid number of shares")

        # check that user had enough stocks (buy(s) logged as positive, sell(s) logged as negative, therefore sum shares to give total)
        total_stock = db.execute("SELECT SUM(shares) FROM history WHERE user = :user_id AND stock = :stock", user_id = session['user_id'], stock=stock["symbol"])
        if total_stock[0]["SUM(shares)"] < int(shares):
            # user tried to sell more stocks than owned
            return apology("not enough shares")

        else:
            # update history, store shares as negative value in db
            db.execute("INSERT INTO history (user, stock, price, shares) VALUES (:user, :stock, :price, :shares * -1)", user=session['user_id'], stock=stock["symbol"], price=stock['price'], shares=shares)

            # update cash position in user table
            db.execute("UPDATE users SET cash = (cash + :shares * :stock_price) WHERE id = :user_id", shares=shares, stock_price=stock["price"], user_id=session['user_id'])
            
            
        # Confirm to user that sale was made
        flash("Sold!")
        
        #return to index to see current holdings
        return redirect(url_for("index"))            
            
    else:
        return render_template("sell.html")
        
@app.route("/settings", methods=["GET", "POST"])
@login_required
def settings():
    """Sell shares of stock."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        new_pass = request.form.get("new_pass")
        pass_confirm = request.form.get("pass_confirm")
        
        # check that password was entered
        if not new_pass:
            return apology('enter a password')
        
        # check that passwords are same
        if new_pass != pass_confirm:
            return apology("passwords don't match")
        
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