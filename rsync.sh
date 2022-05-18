#!/bin/bash
nprocesses=$(ps aux | grep -w rclone | wc -l)
if  [ $nprocesses -gt 2 ]; then
    echo "$(date "+%d.%m.%Y %T") Exit, $nprocesses processes already running."
    exit 1
fi
for (( ; ; ))
do
    rclone -v sync remote:rclone $ALIGNMENT/VCNL4010_measurement/data/test/ > $ALIGNMENT/VCNL4010_measurement/data/rclone.log 2> $ALIGNMENT/VCNL4010_measurement/data/rclone.err
    sleep 30
done

