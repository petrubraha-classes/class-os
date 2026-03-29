#!/bin/bash
./producer.sh | ./consumer.sh ; echo "Consumer's exit code: $?"
