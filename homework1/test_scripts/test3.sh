#!/bin/bash
echo "Testing process tree with multiple levels..."
./procman << END
create bash -c "sleep 30 & sleep 60 & wait"
tree
quit

