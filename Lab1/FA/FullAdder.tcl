set_attribute lib_search_path /cadence/GPDK/osu_stdcells/lib/tsmc018/signalstorm
set_attribute library osu018_stdcells.lib
read_hdl -vhdl ./FA/FullAdder.vhd
elaborate
synthesize -to_generic
synthesize -to_mapped
write_hdl -generic FullAdder > ./FA/FullAdderGen.v
write_hdl -mapped FullAdder > ./FA/FullAdderMap.v
report area > ./FA/areaFA.rep

