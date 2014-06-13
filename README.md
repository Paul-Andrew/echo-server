echo-server
===========

Instructions
------------

1. If you do not already have a github account, create one.
2. Fork this project.
3. Update the LICENSE file with whatever license you prefer.
4. Add a copyright, license, and warranty notice at the top of each source file
   you create.
 * If you use the LICENSE file included, the information you need to include at
   the top of each source file can be found in the *License* section of this
   README. Just change __your-name-here__ to your full name.
5. Add a hello world program written in C to the project. The program should
   compile with GCC on a x86\_64 GNU/Linux system running Debian Stable. It
   will be compiled with the following flags:

          gcc -std=c99 -pedantic -Wall -Werror -Wextra -O0 -g -ggdb hello.c -o hello
6. Create an echo server.  It should be accessible via TCP on 127.0.0.1:7777.
 * The application will be compiled on Debian GNU/Linux using GCC with the
   following command:

          gcc -std=c99 -pedantic -Wall -Werror -Wextra -O0 -g -ggdb echo.c -o echo
 * It will be tested with:

          telnet 127.0.0.1 7777
 * The application should read up to 64KiB of data then write that data
   back to the client over the established connection, and then close the
   connection.  The server does not need to support concurrent connections.
   However, it should support multiple connections in serial.
7. Extra Credit: 
 * Modify the echo server program to log to standard error the IP
   address and port that the client has connected from.
 * Modify the echo server program to accept both TCP and UDP connections
 * Modify the echo server program to accept arbitrarily long NULL terminated strings on TCP port 7778
8. Add any notes regarding your solution to the *Solution Notes* section.
9. Bonus Points: Like you, we are pedantic nerds. Please let us know if you
   find anything during this process or in our instructions that could use
   improvement.
10. Submit a link to your completed solution as well as any questions to
   <rreiher@camber.com>.


Solution Notes
--------------
For two concurrent servers, fork() was used. Both TCP and UDP used socket() and bind(), with  structs of type sockaddr_in to facilitate passing information to the socket functions.  TCP connections use listen() and accept() and later close() to link with a client, while UDP are able to skip to recvfrom(), which is needed to direct traffic. I was able to create a third client on 7778 using TCP, but I could not force it to accept arbitrary length messages.  I had a few ideas all of which did not pan out. Either my terminal of my clipboard only accommodate 4096 characters, which is short of the 64k goal, but I believe the limit is my system and not the program.  I used NCAT to test the UDP echo server, as I could not get telnet to work with that protocol. I would have liked to meet all the bullet points, but I am proud of what I was able to accomplish without prior knowledge of POSIX sockets.


License
-------

This file is part of Echo-Server.

Copyright (c) 2014 Paul Ferguson
Echo-Server is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option) any
later version.

Echo-Server is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
Echo-Server.  If not, see <http://www.gnu.org/licenses/>.
