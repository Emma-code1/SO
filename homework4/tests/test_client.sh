#!/bin/bash
# Launch multiple clients in background (non-interactive demo)
# Note: clients expect interactive username; this demo uses here-doc for simple test
./bin/chat_client <<EOF &
alice
Hello from alice
/quit
