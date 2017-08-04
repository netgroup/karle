# RTT and LOSS calculator #

Simple Client/Server application for measuring Round Trip Time and One Way Loss

### What is this repository for? ###

* Simple application able to measure RTT and OWL on both sides. The application is meant to be efficient as much as possible and some functionalities have been sacrified.
* version 0.1: Architecture and RTT evaluation
* version 0.2: Improvements, bug fixes and OWL evaluation
* version 0.2.1: cmd line parameters for ka period and ka timeout

### How do I get set up? ###

* Configurable parameters are in global.h

* To compile the Client:

		make client

* To compile the Server:

		make server

* Run the node (Client or Server) providing with -k the keep alive period and -t the keep alive timeout:

		./node.bin -k 1 -t 50




