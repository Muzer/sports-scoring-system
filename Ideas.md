# Ideas #

  * Use QT Creator to create the GUI and use a CLI interface for the server.
  * 4 threads:
    * TCP (responsible for recieving and processing most of the messages)
    * DB (writes out the array to the DB)
    * Busy (writes busy when the server is busy to any clients attempting to connect. Busyness controlled by variable bool is\_busy)
    * Keepalive (disconnects clients that have not sent the keepalive signal for 2 minutes).


Keepalive should not check any TCP by itself, but instead the TCP thread should set a variable for each client (bool ping\_sent[client\_number](int.md)) when it has sent a KEEPALIVE. Then this thread should pick that up and reset the timer (int timer[client\_number](int.md)) to 0. This thread's main job will be sleeping; it will increment the timer for each client by 1 every second.


We should build this with hardcoded support for max 50 clients. That sounds resonable to me. Any more than that and the server should do something sensible, and the client should interpret that sensibly.