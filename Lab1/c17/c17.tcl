set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl -vhdl ./c17/c17.vhd
elaborate
synthesize -to_generic
synthesize -to_mapped
write_hdl -generic c17 > ./c17/RC/c17Gen_pre.v
write_hdl -mapped c17 > ./c17/RC/c17Map_pre.v
report area > ./c17/RC/areaC17_pre.rep
report power > ./c17/RC/powerC17_pre.rep
report timing > ./c17/RC/timingC17_pre.rep
report dft_violations > ./c17/RC/violationsC17_pre.rep
set_attribute drc_first true
set_attribute avoid true OAI21X1
synthesize -to_mapped
write_hdl -generic c17 > ./c17/RC/c17Gen_post.v
write_hdl -mapped c17 > ./c17/RC/c17Map_post.v
report area > ./c17/RC/areaC17_post.rep
report power > ./c17/RC/powerC17_post.rep
report timing > ./c17/RC/timingC17_post.rep
report dft_violations > ./c17/RC/violationsC17_post.rep
