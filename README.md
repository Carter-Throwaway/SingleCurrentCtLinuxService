# Single Current Count Linux Service

## How to Run
### Prerequisites:
- Language support for C/C++
- CMake v3.0
    - (On Mac) 
        ```
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

        brew install cmake
        ```
    - (On Linux)
        ```
        sudo apt-get install cmake
        ```
- telnet (testing)
    - (On Mac)
        ```
        brew install telnet
        ```
    - (On Linux)
        ```
        sudo apt-get install telnet
        ```

### Set up build:
1. Open Terminal and navigate to the directory containing this README.
2. If a build directory exists:
    ```
    rm -rf build
    ```
3. 
    ```
    mkdir build
    cd build
    cmake ../
    make
    ```
### Running:
1. To start server:
    ```
    cd src
    ./SingleCurrentCtLinuxService <PORT>
    ```
    (working directory should be build/src)

### Testing:
1. To run unit tests:
    ```
    //anywhere in build directory:
    make test
    ```
2. Testing the server with `telnet`
    ```
    telnet localhost <PORT>
    .
    .
    .
    <Type '^]' (control + ] + enter) keys>
    telnet> set crlf
    ```
3. Testing SIGTERM handling:
    ```
    lsof -i tcp:<PORT>
    kill -s TERM <PID OF SingleCurr...>
    ```


#### Supported Commands
- INCR *int*
- DECR *int*
- OUTPUT


## How to Run as a Linux Service
### 1. Create a .service file. *e.g. singleCurrCt.service*
### 2. Place the .service file in your system's systemd/system directory 
e.g.`/etc/systemd/system/singleCurrCt.service`
### 3. Fill out the .service file like so:
```
    [Unit]
    Description=singleCurrCt tcp server
    After=network.target
    StartLimitIntervalSec=0

    [Service]
    Type=simple
    Restart=always
    RestartSec=1
    User=<your user name>
    ExecStart=./</path/to/>SingleCurrentCtLinuxService 8089

    [Install]
    WantedBy=multi-user.target

```
### 4. Start the service:
```
systemctl start singleCurrCt
```

### 5. Enable automatic start upon boot:
```
systemctl enable singleCurrCt
```

## Critiques and Comments:
The provided specification suggested `epoll` or `select` to be used as the primary method of managing multiple connections with commands coming concurrently into the TCP server. Whilst researching, I found the most material on `epoll`. However, because my solution needed to be developed on my Mac, the `epoll` equivalent was `kqueue` which would severely limit my solution to only working on Macs. Thus, the 'epoll vs. select' decision was somewhat made for me - I needed to use `select`.

 I am generally a developer who focuses on creating a Minimal Viable Product (MVP) for my Version 1. That is I prioritise getting all the basic requirements completed in usually a simpler working version before iterating over my solution to mould the best working product. Because iterating over my product, making improvements, and adjusting for new knowledge/requirements are a part of this process, I, of course, already have critiques for this solution! 

The main critique on this implementation is more a reflection of my experience with `select`. I was dabbling with this functionality for the first time, though conceptually polling event queues is by no means new to me. Following my MVP process, I started with getting one aspect of the `select` functionality set completed and working. This presented itself as - "I need to get the queued client(s) accepted and connected to my listening server." The way to access those queued client(s) was using select and an 'fd_set' for read events, the first of three polling options(Read, Write, Exception). After my connections were being accepted, I moved on to polling read events from my client(s). I now had a working method to poll queued read events for a given socket descriptor from my `pollServerForRead()` function, so I just needed to create a way of holding onto active connections' socket descriptors and do the same poll action for those. This is where my critique exists.

I do not think this split-polling is the correct way to use `select`. In ConnectionManager's `handleConnection()` function, the current solution polls my server for read events (to accept new connections) and then loops through all active connections and does the same for those client(s) (to look for incoming commands). The current implementation will only handle incoming commands from client(s) in the order at which they were added to the member vector not the order at which they were given from the clients. At this point in the solution, I also had already added my `signalHandler()` to my server loop for handling SIGTERMs and shutting down all connections. Using `signal()` and a `signalHandler()` function is how I had caught and cascaded actions in previous projects.  

If you are well versed in select's functionality, you probably very easily noticed my 'polling the client(s)' portion of this solution as a bit odd. My next iteration for this solution would be to see if the incoming commands are either already queued in the read events or in the write events queue of the server. Thus, I could get rid of readClientForRead() and instead just use the TCP server as my access point for handling incoming commands. The same goes for exception handling too! 

The bottom line is that I simply didn't know how much was being handled for me here behind the scenes. Once I got a working way of accessing queued items for a socket descriptor, I cascaded that into all aspects of the solution, and the result was an MVP. Thus, my next iteration would use this gained knowledge to try and just access the INCR, DECR, and OUTPUT (and even the SIGTERM) commands **just via the server** using select's other available event actions. A further iteration would look to improve security on the server.

### *Testing*
A further iteration on testing would entail getting a working script for spinning up 1024 clients that 'communicated' with my server for a couple minutes and adding more test cases. I added a few test cases here to show you that I am comfortable writing unit tests and testing my solution on multiple levels.

### Closing Comments
 Though the ConnectionManager still has plenty of room for improvement as stated above, this solution is a fairly modular one. 
 
 You could send me an entirely different set of commands tomorrow, and the scaffolding is already in place for making a seamless adjustment. In an industry space, a single executable might have multiple avenues for incoming and outgoing signals. Spinning up three TCP servers to different ports and supporting three vastly different APIs could be a very plausible requirement. This solution has an established flow of development for future requirements:
 1. Make a TCP server for a given port
 2. Create an API subclass that establishes an I/O requirements for `InputCommands` and/or `OutputCommands`
 3. Tie any special connection handling for the new API into ConnectionManager (Search 'newApi' in the class) 
 4. Generate a new ConnectionManager instance passing the new TCP server and new API item

 Also, adding to the existing set of commands is relatively easy with adjustments being made only where expected. I came from a fairly large C/C++ codebase with Feature Owners who changed their minds on decisions at the drop of a hat. Modularity and Maintainability are and will always be in my eyes very important design decisions.