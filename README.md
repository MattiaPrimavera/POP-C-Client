# POP-C-Client

## Requirements

* X11 Library
* change the makefile link to X11 library according to its path on your system

## Synopsis

This project is a [Post-Office-Protocol (POP)](https://fr.wikipedia.org/wiki/Post_Office_Protocol) Client for retrieving mails from an email Server. Only a part of the protocol has been considered in this implementation, that is to say the following requests from the protocol:

The two first to handle the login phase:
* `USER` username
* `PASS` password, for specifying the unencrypted password

The others for interacting with the server:
* `LIST`, to retrieve line by line the message ID followed by its size
* `TOP n 0`, to retrieve the SMTP headers of the message which has `n` has an ID
* `QUIT`, to quit the server and stop the client
* `RETR n`, to retrieve the message `n`

Three modes are proposed to interact with the server:
* text mode: everything happens via command line
* clickable mode: the login becomes graphical, a click on the message ID will save the message on the hard drive
* graphical mode: a complete message can be read on a new window instead of being saved (third mode is still a work in progress)

## Automatic Tests

Text mode:

	make t-test-1
	make t-test-2

Clickable mode:

	make c-test

Graphical mode:

	make g-test

## Clean

To clean your project directory from temporary and executable files:

	make cleanall
