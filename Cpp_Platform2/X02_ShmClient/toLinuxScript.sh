#!/bin/bash
lf --linux clean.sh c.sh
lf --linux build.sh b.sh
lf --linux run.sh r.sh
rm clean.sh
mv c.sh clean.sh
rm build.sh
mv b.sh build.sh
rm run.sh
mv r.sh run.sh

chmod 755 clean.sh
chmod 755 build.sh
chmod 755 run.sh
