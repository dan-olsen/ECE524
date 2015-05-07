set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl -verilog c17.v
elaborate
synthesize -to_generic
synthesize -to_mapped
write_hdl -generic c17 > c17Gen.v
write_hdl -mapped c17 > c17Map.v
report area > areaC17.rep
rm c17
