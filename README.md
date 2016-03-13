# FTP_Client
An FTP swarming download client implemented in C at socket level

## Basic Architecture
`argparser`: An argument parser based on `argp.h`.
`configparser`: A parser that parses config file for swarming mode.
`cftpclient`: The core of `mftp`. It interacts with FTP servers.
`main`: The main logic of the program.
`utils`: Some utility functions used in the program.

## Build
Simply run `make`. The makefile is included.

## Known Issues
### Error 450
When the network speed is fast enough, and -b is specified with relative small numbers (less than 5MB on a 100 Mbit/s network, larger on faster networks), the error will be return from the server, indicating the file is busy. It's likely because the server was not able to handle rapid file access like this. If 450 happens (or `mftp` exits with random status), try to set num_bytes a larger number to have `mftp` requested to server less frequently.

### Failure
If any of the servers specified in configure file fails to finish transmission, the whole download process will be aborted. Although there might be a downloaded file, it is likely corrupted.  
