#!/bin/bash

echo "Welcome in testing mode."

mkdir toUpperRec
mkdir toUpperSingle
mkdir toLowerRec
mkdir toLowerSingle 
mkdir toSedRec
mkdir toSedSingle

cd toUpperSingle
touch "toUpper"
cd ..

cd toUpperRec
touch "toUpper1"
touch "toUpper2"
mkdir toUpperRec1
cd toUpperRec1
touch "toUpper1"
cd .. 
cd ..

cd toLowerSingle
touch "TOLOWER" 
cd ..

cd toLowerRec
touch "TOLOWER1"
touch "TOLOWER2"
mkdir TOLOWERREC1
cd TOLOWERREC1
touch "TEST"
cd ..
cd ..

cd toSedSingle
touch "abcde"
cd ..

cd toSedRec
touch "abcde"
touch "cdebaabcde"
mkdir toSedRec2
cd toSedRec2
touch "abcdef"
cd ..
cd ..

echo "File creation was finished."

echo "Test1: to argument"
./modify.sh 

echo "Test2: -h flag."
./modify.sh -h

echo "Test3: no file path."
./modify.sh -u

echo "Test4: no file path. Sed given"
./modify.sh s/abc/cde/

echo "Test5: toUpperSingle/toUpper should be uppercase"
./modify.sh -u toUpperSingle/toUpper

echo "Test6: toUpperRec - all files should be uppercase - RECURSIVE MODE"
./modify.sh -u -r toUpperRec

echo "Test7: toLowerSingle/TOLOWER should be lowercase"
./modify.sh -l toLowerSingle/TOLOWER

echo "Test7: toLowerRec - all files should be lowercase - RECURSIVE MODE"
./modify.sh -l -r toLowerRec

echo "Test8: toSedSingle/abcde should be cdede"
./modify.sh s/abc/cde/ toSedSingle/abcde

echo "Test9: toSedRec - all files should be modifyied. abc should be changed to cde - RECURSIVE MODE"
./modify.sh -r s/abc/cde/ toSedRec


exit 0