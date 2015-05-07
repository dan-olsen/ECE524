set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl -verilog c6288.v
elaborate
synthesize -to_generic
synthesize -to_mapped
write_hdl -generic c6288 > c6288Gen.v
write_hdl -mapped c6288 > c6288Map.v
report area > areaC6288.rep
rm c6288
