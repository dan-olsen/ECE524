set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl -vhdl ./c17/c17.vhd
elaborate
synthesize -to_generic
set_attribute drc_first true
set_attribute avoid true NAND2X1
synthesize -to_mapped
write_hdl -generic c17 > ./c17/c17Gen_post.v
write_hdl -mapped c17 > ./c17/c17Map_post.v
report area > ./c17/areaC17_post.rep
report power > ./c17/powerC17_post.rep
report timing > ./c17/timingC17_post.rep
report dft_violations > ./c17/violationsC17_post.rep
elaborate
