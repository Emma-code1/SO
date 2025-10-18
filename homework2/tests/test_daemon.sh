#!/bin/bash
# Test process_daemon
echo "Testing process_daemon..."
./process_daemon &
DAEMON_PID=$!
sleep 10
ps aux | grep process_daemon
kill $DAEMON_PID
