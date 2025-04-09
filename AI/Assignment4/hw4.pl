/*  Diagnosing Faults in a Smart Home System using ProbLog  

    Smart Home Components:
    - Motion sensors in living room and hallway
    - Light sensor
    - Temperature sensor
    - Thermostat
    - Power supply

    Usage example:
    problog hw4.pl -q 'query_name_here.'
*/

/* ========================= TASK 1 ========================= */

/* ------------ Probabilistic Facts: Component Failure Rates ------------ */

0.05::faulty_motion_sensor(living_room). 
0.05::faulty_motion_sensor(hallway).
0.02::faulty_light_sensor.
0.03::faulty_temp_sensor.
0.01::faulty_thermostat.
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

% evidence(movement(living_room), false).
% evidence(light_switch_on, false).
% evidence(temperature_set(high), false).
% evidence(power_failure, true).
% evidence(faulty_thermostat, true).
% evidence(faulty_temp_sensor, true).
% evidence(faulty_light_sensor, true).
% evidence(faulty_motion_sensor(hallway), true).

/* ------------ Queries ------------ */
query(motion_detected(living_room)).
query(motion_detected(hallway)).

query(light_on).

query(heating_on).
query(temp_reading_high).

/*
query(faulty_motion_sensor(living_room)).
query(faulty_motion_sensor(hallway)).
query(faulty_light_sensor).
query(faulty_temp_sensor).
query(faulty_thermostat).
query(power_failure).

query(no_heating).
query(incorrect_temp_reading).
*/