/*  Diagnosing Faults in a Smart Home System using ProbLog  

    Smart Home Components:
    - Motion sensors in living room and hallway
    - Light sensor
    - Temperature sensor
    - Thermostat
    - Power supply
    
    HafelySmart_home.pl
*/

/* ========================= TASK 1 ========================= */

/* ------------ Probabilistic Facts: Component Failure Rates ------------ */
/*
0.05::faulty_motion_sensor(living_room). 
0.05::faulty_motion_sensor(hallway).
0.02::faulty_light_sensor.
0.03::faulty_temp_sensor.
0.01::faulty_thermostat.
0.01::power_failure.
*/
0.03::faulty_motion_sensor(living_room). 
0.03::faulty_motion_sensor(hallway).
0.01::faulty_light_sensor.
0.04::faulty_temp_sensor.
0.02::faulty_thermostat.
0.01::power_failure.


/* ------------ Motion Sensor ------------ */
movement(living_room).
movement(hallway).

motion_detected(Room) :- 
    not(faulty_motion_sensor(Room)),
    movement(Room).

% possible causes for no movement
no_motion(Room) :- not(movement(Room)).
no_motion(Room) :- faulty_motion_sensor(Room).

/* ------------ Light Sensor ------------ */
light_switch_on.

light_on :- 
    not(faulty_light_sensor),
	light_switch_on.
    
% possible causes for no light detection
no_light :- not(light_switch_on).
no_light :- faulty_light_sensor.
no_light :- power_failure.

/* ------------ Heating System - Thermostat & Temperature Sensor ------------ */
temperature_set(high).

heating_on :- 
    not(faulty_thermostat), 
    not(power_failure),
    temperature_set(high).

temp_reading_high :- 
    not(faulty_temp_sensor),
    temperature_set(high).

% possible causes for no heating
no_heating :- not(temperature_set(high)).
no_heating :- faulty_thermostat.
no_heating :- power_failure.
incorrect_temp_reading :- faulty_temp_sensor.
incorrect_temp_reading :- temperature_set(high), not(temp_reading_high).


/* ========================= TASK 2 ========================= */

/* ------------ World State Observations & Evidence ------------ */

/* ------------ Set 1 ------------ */
% Living room motion detected, light detected
%evidence(movement(living_room), true).
%evidence(light_on, true).

/* ------------ Set 2 ------------ */
% Heating on, no heat detected
%evidence(temperature_set(high), true).
%evidence(temp_reading_high, false).

/* ------------ Set 3 ------------ */
% Light switch on, heat off
%evidence(light_switch_on, true).
%evidence(heating_on, false).

/* ------------ Set 4 ------------ */
% Power failure
%evidence(power_failure, true).

/* ------------ Set 5 ------------ */
% Motion in hallway, faulty temp reading
evidence(movement(hallway), true).
evidence(faulty_temp_sensor, true).

/* ------------ Queries ------------ */

query(faulty_motion_sensor(hallway)).
query(faulty_motion_sensor(living_room)).
query(power_failure).
query(faulty_light_sensor).
query(faulty_temp_sensor).
query(faulty_thermostat).
