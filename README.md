# What is crossthewall

**crossthewall is a simple socks5-based proxy.** If you want to practice **C++11**, learn **Asio** or understand **sock5 protocol**, then this program may be a good choice.

Little story: crossthewall means "cross the Great Wall", a famous tourist spot in China. 

You may know that there are two "walls" in China. One is the Great Wall, another one is transparent.


# Requirements

- [Asio (nonboost version)](https://github.com/chriskohlhoff/asio) which is network library
- g++ with c++11
- python3 (optional)
- linux

# Notes

crossthewall may only works in **Linux**, other systems are not guaranteed. 

Asio lib is already in this repo, so you don't need to download Asio by your own.

For understanding, please read the source code. 

![image](https://github.com/xcw0754/crossthewall/blob/master/pics/first_version.png)


# Quick start

After you clone this repo, 

Step1:
`$cd crossthewall`

Step2:  compile
`$make`

Step3:  start server side
`$make start`

The server is now running. Now you can use a tcp client to connect it. 

There is a simple client which write in Python in this repo. You can use it like this:

`$make test`

# Contact

Send email to xcw0754@163.com
