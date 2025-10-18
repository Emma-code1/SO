#!/bin/bash
echo "Testing basic process creation and termination..."
./procman << END
create sleep 5
list
wait
quit
END
