# Server for the motor controller

Initially created to be used with motors that communicate via the use of MQTT protocol.

The Arduino sketch can be found at https://github.com/kottochii/ArduinoMQTTServo/

## Requirements
* A running Postgres server where [the tables will be stored](#database-structure)
* [libPQXX](https://github.com/jtv/libpqxx)
* A running MQTT server
* [MosquittoPP](https://github.com/eclipse/mosquitto) (particularly the client library)
* [boost::property_tree](https://github.com/boostorg/property_tree) for parsing the configuration file

### For compilation
* C++23
* [CrowCpp](https://github.com/CrowCpp/Crow)

## Compilation

Once you have all the preinstalled prerequisites, `make` should take care of the basic compilation of the software. Two options are accepted:
* `CXX` - the path to the C++ compiler
* `BUILD_FLAGS` - the flags passed to the compiler, if any required

## Database structure

In `./sql/motor_server.sql`, there is a copy of the database structure that should be used with the software. Keep in mind that the user or the group that owns all the tables should be created. Owned by `motor_server` by default.

## Configuration
The configuration file should be located in `/etc/motor_server/config.json`. This is set by `CONFIG_FILE` macro in `/app/main.cpp`. If it is not readable or does not have any of the required parameters, the server will refuse to start.
### Required parameters
#### `mqtt` group
`host` - host of the MQTT server
`port` - port of MQTT server
`topic` - topic at which MQTT server is trying to communicate with other devices
#### `pqxx` group
`connection_string` - string describing the connection initialiser of the PQXX server
#### `motors` group
For each motor, there should be a configuration for a single motor. At this point only one type is available, so this is mostly a list of the motors
#### `http` group
`port` - host on which to run the HTTP server

### Optional parameters
#### `http` group
`ssl_cert` - path to the SSL certificate to run HTTPS server
`ssl_key` - path to the SSL key to run HTTPS server
