set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl ./c6288/c6288.v
elaborate
synthesize -to_generic
set_attribute drc_first true
synthesize -effort low -to_mapped
write_hdl -generic c6288 > ./c6288/RC/c6288Gen_low.v
write_hdl -mapped c6288 > ./c6288/RC/c6288Map_low.v
report area > ./c6288/RC/areac6288_low.rep
report power > ./c6288/RC/powerc6288_low.rep
report timing > ./c6288/RC/timingc6288_low.rep
report dft_violations > ./c6288/RC/violationsc6288_low.rep
synthesize -effort medium -to_mapped
write_hdl -generic c6288 > ./c6288/RC/c6288Gen_med.v
write_hdl -mapped c6288 > ./c6288/RC/c6288Map_med.v
report area > ./c6288/RC/areac6288_med.rep
report power > ./c6288/RC/powerc6288_med.rep
report timing > ./c6288/RC/timingc6288_med.rep
report dft_violations > ./c6288/RC/violationsc6288_med.rep
synthesize -effort high -to_mapped
write_hdl -generic c6288 > ./c6288/RC/c6288Gen_high.v
write_hdl -mapped c6288 > ./c6288/RC/c6288Map_high.v
report area > ./c6288/RC/areac6288_high.rep
report power > ./c6288/RC/powerc6288_high.rep
report timing > ./c6288/RC/timingc6288_high.rep
report dft_violations > ./c6288/RC/violationsc6288_high.rep
