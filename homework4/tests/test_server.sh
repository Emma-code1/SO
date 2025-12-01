#!/bin/bash
# Start server in background and show logs to screen
./bin/chat_server &
SERVER_PID=$!
echo "Server PID: $SERVER_PID"
sleep 1
echo "Server should be running. Kill with: kill $SERVER_PID"
tail --pid=$SERVER_PID -f /dev/null
