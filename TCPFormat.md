In this file, comments for the TCP commands are specified C++-style. These will not be sent and will cause a parse error if they are sent. They are just for convenience for this wiki page.


The following are special characters:

$ indicates line ending

; is the separator

% is the escape character


If these are entered as part of the name or any other string, they should be escaped:

%dollar%
%semicolon%
%percent%

I might think of more special characters.

The first two are obvious, but the escape character might need a bit of clarification. If a student did not compete or finish/qualify (all throws were bad for example), one of the following is used:

%dnf%                        //Did not finish (track) or qualify (field - all throws/jumps were bad)

%dnc%                        //Did not compete. The student and the reserve (or two students) were both off so there were not enough people, so nobody could compete in this event.

%empty%                      //If the user left a field empty during table creation, could cause problems otherwise

(Obviously these will be represented by radio boxes, along with "Student competed" in which case it would just send the times as normal, in the client).



There should be no spaces after semicolons. That was a mistake I made. Just ignore any spaces after semicolons as if they didn't exist.

## Config ##

The server should have an xml file in the same folder of it called config.xml (see ConfigDotXmlExample ), this will hold all the table data (i.e. Events, Competing Years, etc), apart from the records, which are stored in records.xml. Also, the server folder will have at least one database XML file, and this will hold all the tables that were specified in the config.xml (which specifies the filename).  When the server starts, if the database.db and config.xml match then it wont do anything.

## Table setup ##

We want to be able to set up the table via the GUI. This will go something like this:

NEWTABLE name;filename$

```
NEWTABLE BWS Sports Day 2009;bwssportsday2009.xml$ // Initialise the table
```

This lets the server know you are going to make a new table. However, since there is only 1 table per user account, we need to make sure that user doesn't already have a table. If there is already a table, the server responds:

TABLEEXISTSREPLACE name$

(Where name is the name of the table that already exists)

then, the client has to send:

REPLACE$

or

KEEP$

Then, if there wasn't already a table, or if the client replied REPLACE, the server sends:

GOON$

The client then proceeds to sent the table.

```
SETGROUPS 5; Year 7; Year 8; Year 9; Year 10; Year 12$ // 5 groups, names of each group

BEGIN Year 7$ // Begin the year 7 group
EVENTS 5$ // No. events
EVENT 100 metre sprint; TIMELOW; 12; 13; 11; 10; 9; 8; 7; 6; 5; 4; 3; 2; 1$ // Name, type (TIMELOW (lowest best)/TIMEHIGH (highest best)/DISTANCE/POINTS), number of places, score for each place
// ...
EVENT Javelin; DISTANCE; 12; 13; 11; 10; 9; 8; 7; 6; 5; 4; 3; 2; 1$
EVENT Relay; TIMELOW; 28; 28; 27; 26; 25; 24; 23; 22; 21; 20; 19; 18; 17; 16; 15; 14; 13; 12; 11; 10; 9; 8; 7; 6; 5; 4; 3; 2; 1$
END Year 7$
// And so on
ENDDATABLOCK$ // Signifies that the command has finished
```

The server, once it reads ENDDATABLOCK, will react, by sending all the data back "in its own words" (in other words, reading it out of the array it just created). It should be in the same format, and in an ideal world, exactly the same.

The client will then display the data, and ask the user if it is OK. If it is, the client sends:

OK$

Else:

NOK$

(and then after the delay whilst the user edits the data, the new set of data).

The server will then edit the config file, and create the skeleton database. It will then return:

CREATED$

or:

CREATEERROR Permission denied$ // for example, it could be anything, like File Exists. It will start with CREATEERROR though.

## Keepalive ##

No matter its state (unless obviously crashed), every minute, the client will send:

KEEPALIVE$

This makes the server keep the connection open.

## Inputting data ##

Coming soon...

## Requesting the database ##

The client sends:

REQUESTDB$

The server then sends either:

NOTABLE$

if there is no table for this user account, or

COMMENCEDB$

if there is.

The server then sends:

```
TABLE name;filename.xml$
GROUPS number;group1;group2;group3;group4 (etc)$
BEGIN Year 7$ // Begin the year 7 group
EVENTS 5$ // No. events
EVENT 100 metre sprint; TIMELOW; 12; 13; 11; 10; 9; 8; 7; 6; 5; 4; 3; 2; 1$ // Name, type (TIMELOW (lowest best)/TIMEHIGH (highest best)/DISTANCE/POINTS), number of places, score for each place
COMPETITOR name;form;time$
COMPETITOR name;form;time$
// ...
EVENTEND 100 metre sprint$
// ...
EVENT Javelin; DISTANCE; 12; 13; 11; 10; 9; 8; 7; 6; 5; 4; 3; 2; 1$
COMPETITOR name;form;distance$
// ...
EVENTEND Javelin$
EVENT Relay; TIMELOW; 28; 28; 27; 26; 25; 24; 23; 22; 21; 20; 19; 18; 17; 16; 15; 14; 13; 12; 11; 10; 9; 8; 7; 6; 5; 4; 3; 2; 1$
// No competitors have been entered yet, so it just says:
EVENTEND Relay;
END Year 7$
// And so on
ENDDATABLOCK$ // Signifies that the command has finished
```

As you can see, it's virtually identica to creating the table, just with added commands for the people.



## Update ##

Once the database has been updated, the DB thread will send:

UPDATE$

To all clients. The clients will then request the database (as above), unless they are "in the middle of something". Once a client finishes submitting data, it obviously requests the whole database.

## Requesting records database ##

Coming soon...

## Initial setting of records database ##

(theoretically should be once, and only once, ever, since the database should be carried over to next year).

## Busy ##
Before any data is sent from the client to the server, the client should send:

HANDSHAKE$

If the server is busy dealing with another request, it sends:

BUSY$

Else:

ACK$ // Stands for Acknowledge (not sure how to spell it but it's usually abbreviated ACK :p)

Then the client gets on with the data

## Authentication ##

Other than the AUTH message, the server should NOT accept any messages from any client until authentication is complete. The format for authentication is:

AUTH username; password$

eg

AUTH foo; bar$

The server will then reply:

AUTHGOOD$

or

AUTHBAD$

Simple!