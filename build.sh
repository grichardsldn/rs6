#!/bin/sh -f
cd dev_testdevice
make
cd ../dev_stepseq
make
cd ../dev_seq
make
cd ../dev_mixer
make
cd ../dev_midirx
make
cd ../dev_sub
make
cd ../dev_recorder
make
cd ../dev_samp
make
cd ../dev_sub102
make
cd ../dev_strings
make
cd ../dev_smodel
make
cd ../dev_reva
make
cd ../dev_shaper1
make
cd ../dev_sampdrum
make
cd ../ui
make

cd ..


make

