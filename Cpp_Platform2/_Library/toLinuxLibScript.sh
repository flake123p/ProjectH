#!/bin/bash
lf --linux lib_clean.sh c.sh
lf --linux lib_build.sh b.sh
rm lib_clean.sh
mv c.sh lib_clean.sh
rm lib_build.sh
mv b.sh lib_build.sh

chmod 755 clean.sh
chmod 755 build.sh
