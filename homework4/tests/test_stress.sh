#!/bin/bash
# Stress: spawn many headless clients that connect, send a message, and quit
COUNT=10
for i in $(seq 1 $COUNT); do
  (
    ./bin/chat_client <<EOC
user$i
Hello from user$i
/quit
EOC
  ) &
done
wait
echo "Stress test finished"
