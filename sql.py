import sqlite3

# create a new database if the database doesn't already exist
with sqlite3.connect('sample.db') as connection:

    # get a cursor object used to execute SQL commands
    c = connection.cursor()

    # create the table
    c.execute('CREATE TABLE dados(temp TEXT, ldr TEXT, hora TEXT )')

    # insert dummy data into the table
    c.execute('INSERT INTO dados VALUES("30.4", "500", "2016/10/28")')
    