set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl -vhdl ./c17/c17.vhd
elaborate
synthesize -to_generic
synthesize -to_mapped
write_hdl -generic c17 > ./c17/c17Gen_pre.v
write_hdl -mapped c17 > ./c17/c17Map_pre.v
report area > ./c17/areaC17_pre.rep
report power > ./c17/powerC17_pre.rep
report timing > ./c17/timingC17_pre.rep
report dft_violations > ./c17/violationsC17_pre.rep
elaborate
