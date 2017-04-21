//qubie module implementation summary

#include "qubie_t.h"
#include "qubie.h"
#include "qubie_bt_communicator.h"
#include "qubie_log.h"
#include "qubie_observations.h"
#include "qubie_wifi_monitor.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static const state_t legal_update_states[] = {RUNNING, STOPPED, POWERED_OFF};
//@design must be synced to typedef enum {POWERED_ON, BOOTING, RUNNING, STOPPED, POWERED_OFF} state_t;
const char *state_strings[] = {"powered on", "booting", "running", "stopped", "powered off"};

//constructor
qubie_t *make_qubie(){
	qubie_t *qubie_struct = malloc(sizeof(struct qubie));
	qubie_observations_t observations = make_qubie_observations("qubie_observations.csv");
	qubie_struct->state = POWERED_ON;
	qubie_struct->log = make_qubie_logger("qubie_log.txt");
	memcpy((qubie_observations_t *)&qubie_struct->observations, &observations, sizeof(struct observations));
	//free(&observations); //@TODO is this freed automatically when the function exists?
	qubie_struct->wifi_monitor = make_wifi_monitor(qubie_struct);
	qubie_struct->bt_communicator = make_bt_communicator(qubie_struct);
	qubie_struct->legal_update_states = legal_update_states;
	return qubie_struct;
};

//@ TODO define predicates in acsl file

// ====================================================================
// @bon QUERIES
// ====================================================================

// qubie status
state_t state(qubie_t *self){
	return self->state;
};

// pointer to qubie's log, a list of log entries with some added functionality
qubie_logger_t *get_log(qubie_t *self){
	return self->log;
};
// pointer to qubie's observations, a list of contact records
qubie_observations_t observations(qubie_t *self){
	return self->observations;
};
// pointer to wifi monitor
wifi_monitor_t *wifi_monitor(qubie_t *self){
	return self->wifi_monitor;
};
// pointer to bluetooth communicator
bt_communicator_t *bt_communicator(qubie_t *self){
	return self->bt_communicator;
};

//@ ensures {stopped, powered_off} == Result
const state_t *qubie_legal_update_states(qubie_t *self){
	return self->legal_update_states;
};

//@ TODO add relevant predicates to avoid error prone syntax
/*@ ensures Result == (state == POWERED_ON)
 * ensures log.empty
 * ensures observations.empty
 */
bool powered_on(qubie_t *self){
	return POWERED_ON == self->state;
};
//@ ensures Result == (state == BOOTING)
bool booting(qubie_t *self){
	return BOOTING == self->state;
};
//@ ensures Result == (state == RUNNING)
bool running(qubie_t *self){
	return RUNNING == self->state;
};
//@ ensures Result == (state == STOPPED)
bool stopped(qubie_t *self){
	return STOPPED == self->state;
};
//@ ensures Result == (state == POWERED_OFF)
bool powered_off(qubie_t *self){
	return POWERED_OFF == self->state;
};
/*@ ensures log.logged(QUBIE_STATE , state_strings[state]) &&
 *  (!bt_communicator.subscribed || bt_communicator.action_published(state))
 */
bool action_published(qubie_t *self, state_t the_state){
	//@TODO if !logged no need to check published_to_bt
	bool action_logged = logged(self->log, QUBIE_STATE , (void *)state_strings[the_state]);
	bool published_to_bt;
	if (subscribed(self->bt_communicator)) {
		published_to_bt = true;
	} else {
		published_to_bt = bt_communicator_action_published(self->bt_communicator, the_state);
	}
	return action_logged && published_to_bt;
};

// ====================================================================
// @bon COMMANDS
// ====================================================================

/*@ ensures (state == new_state);
 *  ensures action_published(new_state);
 */
void set_and_publish(qubie_t *self, state_t new_state){
	self->state=new_state;
	qubie_publish_action(self, new_state);
};

/*@ TODO requires there is no other qubie;
 *  ensures (state == POWERED_ON);
 *  ensures action_published(state);
 */
void power_on(qubie_t *self){
	set_and_publish(self, POWERED_ON);
};

/*@ requires (state == POWERED_ON);
 *  ensures (state == BOOTING);
 *  ensures action_published(state);
 */
void start_booting(qubie_t *self){
	boot_wifi(self->wifi_monitor);
	//@TBD is action needed for bt_communicator?
	set_and_publish(self, BOOTING);
};

/*@ requires (state == BOOTING);
 *  ensures (state == RUNNING);
 *  ensures action_published(state);
 */
void start_running(qubie_t *self){
	start_wifi(self->wifi_monitor);
	set_and_publish(self, RUNNING);
};

/*@ requires (state == RUNNING);
 *  ensures (state == STOPPED);
 *  ensures action_published(state);
 */
void stop_running(qubie_t *self){
	stop_wifi(self->wifi_monitor);
	set_and_publish(self, STOPPED);
};

/*@ ensures (state == POWERED_OFF);
 *  ensures action_published(state);
 */
void power_off(qubie_t *self){
	//@TBD move cleanup code to the relevant modules.
	fclose(self->log->log_fp);
	fclose(self->observations.observations_fp);
	set_and_publish(self, POWERED_OFF);
};

//@ensures (state == RUNNING);
void power_on_boot_and_run(qubie_t *self){
	power_on(self);
	start_booting(self);
	start_running(self);
};

//@TODO define qubie_legal_update_state(the_state)
/*@ requires qubie_legal_update_state(the_state);
 * 	ensures the_state == state;
 */
void update_state(qubie_t *self, state_t the_state){
	//@TODO switch to an array of function pointers
	if (STOPPED == the_state){
		stop_running(self);
	} else if (POWERED_OFF == the_state) {
		power_off(self);
	} else {
		//not a legal state
		//@assert(false)
		assert(false);
	}
};

/*@ ensures action_published(the_state)
 */
void qubie_publish_action(qubie_t *self, state_t the_state){
	add_log_entry(self->log, QUBIE_STATE , (void *)state_strings[the_state]);
	if (subscribed(self->bt_communicator)){
		bt_communicator_publish_action(self->bt_communicator, the_state);
	}
};

/*@ ensures observations.contains(the_contact_record)
 * 	ensures log.logged()
 */
//delta {observations, log}
void record_observation(qubie_t *self, contact_record_t the_contact_record){
	//@design the contract record belongs to observations which will eventually free the memory
	//log the data from the log entry first, while it is certain to exist.
	add_log_entry(self->log, QUBIE_DETECTED_DEVICE, &the_contact_record);
	add_contact_record(self->observations, the_contact_record);
};

















