#!/bin/bash
set -x

toolnumbertwo -v -cd-dir . -objs vpa.o -r ../mutation/output/mutants/BlackScholes.cpp/4/vpa_float_report.csv -t VpaAprx -P gaconfig.param 100
