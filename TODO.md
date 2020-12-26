* Use exhale/breathe/sphinx instead
* Make input_stream more efficient. Perhaps do not copy all byte swap bytes and do not parse all fields
* input_stream could perhaps return a normal C pointer
* When sample rate is supplied as a command line parameter it should be able to set it for a Class/Stream ID only
* When IF context sample rate is in a packet it should be for a specific Class/Stream ID only
* Add libvrt submodule
* Make libsocket a submodule

# Last
* Check that header guards are all right
* Static analysis
* Delete copy constructors
* Add const where applicable
