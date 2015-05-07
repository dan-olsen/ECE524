set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl ./c6288/c6288.v
elaborate
synthesize -to_generic
synthesize -to_mapped -effort high
write_hdl -generic c6288 > ./c6288/c6288Gen_high.v
write_hdl -mapped c6288 > ./c6288/c6288Map_high.v
report area > ./c6288/areac6288_high.rep
report power > ./c6288/powerc6288_high.rep
report timing > ./c6288/timingc6288_high.rep
report dft_violations > ./c6288/violationsc6288_high.rep
