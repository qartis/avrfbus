AVR with Nokia FBUS
=============

This code connects an AVR atmega88 with a Nokia 3310 mobile phone and any NMEA-compliant GPS receiver, to create a cheap GPS tracker. Send it an SMS, and it will reply with its current GPS coordinates.

Features
----------

By using scavenged parts, the cost of this project can be very low. I bought one for about $5 online (actually I splurged and got a wall charger for an additional $0.50). These phones (Nokia 3310/6110) are what suicide bombers put into their vests, because they're essentially disposable and very easy to interface with.

Start off by gutting the phone. We can store the LCD for use in a later project (there are many articles online that explain how to interface with it).

License
----------

Some of the FBUS handling code (fbus.c) is from n61sms.c linked below. I don't know its license, but everything except for fbus.c is released to the public domain.

Credits
----------

http://www.embedtronics.com/nokia/fbus.html
The most well-known reference for the FBUS protocol.

http://www.affinix.com/~justin/nokia/n61sms.c
Example FBUS code written by Justin Karneges. License unknown.

http://www.gnokii.org/
The grandfather of most FBUS reverse-engineering efforts. GPL licensed.
