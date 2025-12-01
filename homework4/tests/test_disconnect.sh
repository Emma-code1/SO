#!/bin/bash
# Start server and then abruptly kill a client to test disconnect handling
./bin/chat_server &
SV=$!
sleep 1

./bin/chat_client <<EOC &
victim
I will be killed
EOC

sleep 2
# find fd? simplification: kill all client processes
pkill -f bin/chat_client
sleep 2
kill $SV
