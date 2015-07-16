# POP-C-Client

## Requirements

* lib X11
* change the makefile link to your X11 library path

## Synopsis

This project is a [Post-Office-Protocol (POP)] Client for retrieving mails from an email Server. Only a part of the protocol has been considered for this implementation, that is to say the following requests from the protocol:

The two first to handle the loggin phase:
* USER username
* PASS password, for specifying the unencrypted password

The others for interacting with the server:
* LIST, to retrieve line by line the message ID followed by its size
* TOP n 0, to retrieve the SMTP headers of the message which has "n" has an ID
* QUIT, to quit the server and stop the client
* RETR n, to retrieve the message n

Two modes are proposed to interact with the server:
* text mode: everything happens via command line
* graphical mode : the loggin becomes graphical, a click on the message ID will save the message on the hard drive.

## Tests Automatiques

mode textuel :

    make t-test-1
    make t-test-2

mode cliquable :

    make c-test

mode graphique :

    make g-test
