############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project mfb_vivado_hls
set_top mfb
add_files mfb.cpp
add_files -tb mfb_tb.cpp -cflags "-I"
open_solution "solution1"
set_part {xc7a35ticsg324-1l} -tool vivado
create_clock -period 10 -name default
#source "./mfb_vivado_hls/solution1/directives.tcl"
csim_design -compiler gcc
csynth_design
cosim_design -compiler gcc
export_design -format ip_catalog
