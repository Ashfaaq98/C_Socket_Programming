# C_Socket_Programming
Implementing a simple concurrent HTTP Web Server and a Web Client

This Project is written in c language. A simple HTTP Web server and a HTTP client is implmented.
The Server handles multiple HTTP requests at the same time using the fork() function. 
The Web client will request a file from the Web Server and the Server accepts the request and replies 
the file to the client. If the requested file is not available then the Server will send an HTTP "404 Not Found"
message back to the client.

The server should take command line arguments specifying a port number at runtime to run the server.

#Example
#./server 35400   

35400 is the port number.

The client takes commamd line arguments specifying a server name or IP address, the port on which to contact the server,
the method to use {GET or PUT} and the path of the file.

#Example
#./client <server IP> <server port number> GET <filename>
  
  
