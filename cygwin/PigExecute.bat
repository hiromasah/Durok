@echo off

cygwin\bin\bash --login -c "../../pig/bin/Pig -l ../../tmp -F -x local ../../tmp/script_generated.txt"
