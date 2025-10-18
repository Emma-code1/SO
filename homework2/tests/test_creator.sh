#!/bin/bash
# Test zombie_creator
echo "Testing zombie_creator..."
./zombie_creator 5 &
CREATOR_PID=$!
sleep 1
ps aux | grep 'Z'
kill $CREATOR_PID
