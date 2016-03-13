### Basic Architecture
`argparser`: An argument parser based on `argp.h`.
`configparser`: A parser that parses config file for swarming mode.
`cftpclient`: The core of `mftp`. It interacts with FTP servers.
`main`: The main logic of the program.
`utils`: Some utility functions used in the program.

### Build
Simply run `make`. The makefile is included.

### Known Issues
#### Error 450
When the network speed is fast enough, and -b is specified with relative small numbers (less than 5MB on a 100 Mbit/s network, larger on faster networks), the error will be return from the server, indicating the file is busy. It's likely because the server was not able to handle rapid file access like this. If 450 happens (or `mftp` exits with random status), try to set num_bytes a larger number to have `mftp` requested to server less frequently.

#### Failure
If any of the servers specified in configure file fails to finish transmission, the whole download process will be aborted. Although there might be a downloaded file, it is likely corrupted.  

### Benchmark
#### Single server
##### ftp.ucsb.edu
```
tyler@tylerRMBP:Debug $ time ./mftp -s ftp.ucsb.edu -f /pub/mirrors/ls-lR.gz

real	0m9.440s
user	0m0.103s
sys	  0m0.432s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

##### pogo
```
tyler@tylerRMBP:Debug $ time ./mftp -s 192.168.1.102 -f /ls-lR.gz

real	0m4.257s
user	0m0.062s
sys	  0m0.378s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

#### Two servers
`config2`:
```
ftp://anonymous:123@192.168.1.102/ls-lR.gz
ftp://anonymous:123@ftp.ucsb.edu/pub/mirrors/ls-lR.gz
```

##### Without num_bytes
###### Active
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2 -a

real	0m4.663s
user	0m0.114s
sys	  0m0.437s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

###### Passive
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2

real	0m4.663s
user	0m0.119s
sys	  0m0.476s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

##### With num_bytes
###### Active -b 10000000 (10MB)
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2 -a -b 10000000

real	0m3.242s
user	0m0.101s
sys	  0m0.397s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```
###### Passive -b 10000000 (10MB)
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2 -b 10000000

real	0m3.261s
user	0m0.109s
sys 	0m0.445s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

###### Active -b 15000000 (15MB)
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2 -a -b 15000000

real	0m3.359s
user	0m0.110s
sys	  0m0.429s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

###### Passive -b 15000000 (15MB)
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2 -b 15000000

real	0m4.087s
user	0m0.110s
sys 	0m0.415s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

###### Active -b 20000000 (20MB)
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2 -a -b 20000000

real	0m3.632s
user	0m0.106s
sys	  0m0.407s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```

###### Active -b 20000000 (20MB)
```
tyler@tylerRMBP:Debug $ time ./mftp -w config2 -b 20000000

real	0m3.681s
user	0m0.109s
sys	  0m0.438s
tyler@tylerRMBP:Debug $ md5 ls-lR.gz
MD5 (ls-lR.gz) = b7a08dc0d164d5233e257854808d9416
```
