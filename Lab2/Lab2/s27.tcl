set search_path "/synopsys/GPDK/SAED_EDK90nm/Digital_Standard_Cell_Library/synopsys/models"
set link_library "saed90nm_max.db"
set target_library "saed90nm_max.db"
set symbol_library "saed90nm_max.db"
analyze -library WORK -format verilog {/grad/aljubouri/ece524/Prime/s27.v}
elaborate s27 -architecture verilog -library WORK
link
compile -exact_map
create_clock -period z -waveform {x y} {CK} #declare value
set_input_delay 0.1 -min -clock CK [remove_from_collection [all_inputs] [get_port CK] ]
set_input_delay 0.4 -max -clock CK [remove_from_collection [all_inputs] [get_port CK] ]
set_output_delay 0.1 -min -clock CK [all_outputs]
set_output_delay 0.4 -max -clock CK [all_outputs]
set_wire_load_model -name 8000 -library saed90nm_max
compile -exact_map -map_effort high -area_effort high -power_effort high
write -hierarchy -format verilog -output /grad/aljubouri/ece524/Prime/s27_syn.v
report_area > s27_area.txt
report_power > s27_power.txt
check_timing > s27_time.txt
report_constraint > s27_cons.txt
remove_design -all
