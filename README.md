# VRT Command Line Tools

Command line tools for packets following the ANSI/VITA 49.0 Radio Transport (VRT) standard.

## Under development

**Note that this project is still under development. It may not be well tested and may not have all features implemented.**

## Getting Started

**Remember to clone submodules as well, with `git clone --recurse-submodules ...`**

In project root folder (`vrt_tools_cmd/`):
```
mkdir Release
cd Release
cmake ..
make -j4
sudo make install
```

This installs a bunch of VRT command line tools.

### VRT Print

Prints VRT packet information. For example:
```bash
vrt_print signal.vrt
```
May result in:
```
--------------------------------------------------------------------------------
#                                                                              0
Packet type                                IF Data packet with Stream Identifier
tsm                                                                         Fine
TSI                                                               None/Undefined
TSF                                                               None/Undefined
Packet count                                                                   0
Packet size [words]                                                          515
Stream ID                                                             0xDEADBEEF
Body size [words]                                                            512
Reference lock                                                              true
--------------------------------------------------------------------------------
#                                                                              1
Packet type                                IF Data packet with Stream Identifier
tsm                                                                         Fine
TSI                                                               None/Undefined
TSF                                                               None/Undefined
Packet count                                                                   0
Packet size [words]                                                          515
Stream ID                                                             0xDEADBEEF
Body size [words]                                                            512
Reference lock                                                              true
--------------------------------------------------------------------------------
...
```

### VRT Split

Splits a VRT file into multiple depending on class ID and stream ID. For example, splitting a file `signal.vrt` containing a VRT packet stream with two different stream IDs *0xABABABAB* and *0x12345678*:
```bash
vrt_split signal.vrt
```
Results in output files `signal_ABABABAB.vrt` and `signal_12345678.vrt` with all packets from the original file with stream ID *0xABABABAB* in the first and all packets with stream ID *0x12345678* in the second output file.

### VRT Merge

Merges multiple VRT files into a single file and sorts them by time. Assumes packets in input files are ordered by time stamps.

## VRT Length

Calculates number of packets and time difference for all different streams in a VRT packet file.

## VRT Packet loss

Simulate packet loss by generating a file with some VRT packets missing.

## VRT Socket

Send packets over a socket with the same time interval as suggested by packet timestamps in a VRT packet file.

### Prerequisites

* C++17 compiler, such as GCC
* CMake
* Build system such as GNU Make
* (Google test framework, for the tests)

## Author

**Emil Berg**

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
