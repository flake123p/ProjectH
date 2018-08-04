#!/bin/bash

echo ====== Clean Library ... ======
rm ./Makefiles/*.d --force --verbose
rm ./build/*.o     --force --verbose
rm ./archive/*.a   --force --verbose

