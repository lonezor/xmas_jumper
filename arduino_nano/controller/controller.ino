// Durations in milliseconds
#define DURATION_EXTREMELY_SLOW (30000)
#define DURATION_VERY_SLOW (15000)
#define DURATION_SLOW (5000)
#define DURATION_DEFAULT (2500)
#define DURATION_FAST (1000)
#define DURATION_VERY_FAST (500)
#define DURATION_EXTREMELY_FAST (50)

// GPIO setup
const int shift_reg_74hc595_latch_pin = 6;
const int shift_reg_74hc595_clock_pin = 5;
const int shift_reg_74hc595_data_pin = 3;
const int toggle_btn_pin = 8;

// Modes
enum
{
  led_mode_all_on,
  led_mode_single_star_cycle_very_fast,
  led_mode_single_star_cycle_very_slow,
  led_mode_stack_cycle_very_fast,
  led_mode_stack_cycle_mixed_speed,
  led_mode_rotation_cycle_very_fast,
  led_mode_rotation_cycle_very_slow,
  led_mode_nr_entries,
};

// Animation step
typedef struct {
  uint16_t led_data;
  uint16_t duration;
} led_action_t;

/********************* ANIMATION ARRAYS ****************************/

const led_action_t single_star_animation_very_fast[] = {
  {0x00, DURATION_VERY_FAST},
  {0x01, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
  {0x02, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
  {0x04, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
  {0x08, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
  {0x10, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
  {0x20, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
  {0x40, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
  {0x80, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
};
const led_action_t single_star_animation_very_slow[] = {
  {0x00, DURATION_VERY_SLOW},
  {0x01, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
  {0x02, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
  {0x04, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
  {0x08, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
  {0x10, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
  {0x20, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
  {0x40, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
  {0x80, DURATION_VERY_SLOW},
  {0x00, DURATION_VERY_SLOW},
};
const int star_cycle_len = sizeof(single_star_animation_very_slow) / sizeof(led_action_t);

const led_action_t led_mode_stack_animation_very_fast[] = {
  {0x00, DURATION_VERY_FAST},
  {0x01, DURATION_VERY_FAST},
  {0x03, DURATION_VERY_FAST},
  {0x07, DURATION_VERY_FAST},
  {0x0f, DURATION_VERY_FAST},
  {0x1f, DURATION_VERY_FAST},
  {0x3f, DURATION_VERY_FAST},
  {0x7f, DURATION_VERY_FAST},
  {0xff, DURATION_VERY_FAST},
  {0xff, DURATION_VERY_FAST},
  {0x7f, DURATION_VERY_FAST},
  {0x3f, DURATION_VERY_FAST},
  {0x1f, DURATION_VERY_FAST},
  {0x0f, DURATION_VERY_FAST},
  {0x07, DURATION_VERY_FAST},
  {0x03, DURATION_VERY_FAST},
  {0x01, DURATION_VERY_FAST},
  {0x00, DURATION_VERY_FAST},
};
const led_action_t led_mode_stack_animation_mixed_speed[] = {
  {0x00, DURATION_EXTREMELY_FAST},
  {0x01, DURATION_EXTREMELY_FAST},
  {0x03, DURATION_EXTREMELY_FAST},
  {0x07, DURATION_EXTREMELY_FAST},
  {0x0f, DURATION_EXTREMELY_FAST},
  {0x1f, DURATION_EXTREMELY_FAST},
  {0x3f, DURATION_EXTREMELY_FAST},
  {0x7f, DURATION_EXTREMELY_FAST},
  {0xff, DURATION_EXTREMELY_FAST},
  {0xff, DURATION_DEFAULT},
  {0x7f, DURATION_DEFAULT},
  {0x3f, DURATION_DEFAULT},
  {0x1f, DURATION_DEFAULT},
  {0x0f, DURATION_DEFAULT},
  {0x07, DURATION_DEFAULT},
  {0x03, DURATION_DEFAULT},
  {0x01, DURATION_DEFAULT},
  {0x00, DURATION_DEFAULT},
};
const int stack_cycle_len = sizeof(led_mode_stack_animation_mixed_speed) / sizeof(led_action_t);

const led_action_t rotation_animation_extremely_fast[] = {
  {0x01, DURATION_EXTREMELY_FAST},
  {0x02, DURATION_EXTREMELY_FAST},
  {0x04, DURATION_EXTREMELY_FAST},
  {0x08, DURATION_EXTREMELY_FAST},
  {0x10, DURATION_EXTREMELY_FAST},
  {0x20, DURATION_EXTREMELY_FAST},
  {0x40, DURATION_EXTREMELY_FAST},
  {0x80, DURATION_EXTREMELY_FAST},
};
const led_action_t rotation_animation_very_slow[] = {
  {0x01, DURATION_DEFAULT},
  {0x02, DURATION_DEFAULT},
  {0x04, DURATION_DEFAULT},
  {0x08, DURATION_DEFAULT},
  {0x10, DURATION_DEFAULT},
  {0x20, DURATION_DEFAULT},
  {0x40, DURATION_DEFAULT},
  {0x80, DURATION_DEFAULT},
};
const int rotation_cycle_len = sizeof(rotation_animation_very_slow) / sizeof(led_action_t);

// State variables
int led_mode = led_mode_all_on;
uint16_t led_idx = 0;
const int loop_delay_ms = 10;
uint16_t iteration = 0;
uint16_t elapsed_time = 0;
int prev_btn_value = 0;

static void shift_out_data(uint16_t data)
{
  digitalWrite(shift_reg_74hc595_latch_pin, LOW);
  shiftOut(shift_reg_74hc595_data_pin, shift_reg_74hc595_clock_pin,  MSBFIRST, data);
  digitalWrite(shift_reg_74hc595_latch_pin, HIGH);
}

void setup()
{
    pinMode(shift_reg_74hc595_latch_pin, OUTPUT);
    pinMode(shift_reg_74hc595_clock_pin, OUTPUT);
    pinMode(shift_reg_74hc595_data_pin, OUTPUT);
    pinMode(toggle_btn_pin,INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
  int btn_value = digitalRead(toggle_btn_pin);

  switch(led_mode) {
    /**********************************************************************
    ************* STAR ANIMATION (APPEAR / DISAPPEAR) *********************
    **********************************************************************/
    case led_mode_single_star_cycle_very_fast:
    case led_mode_single_star_cycle_very_slow: {
      uint16_t duration = single_star_animation_very_fast[led_idx].duration;
      uint16_t led_data = single_star_animation_very_fast[led_idx].led_data;
      if (led_mode == led_mode_single_star_cycle_very_slow) {
          duration = single_star_animation_very_slow[led_idx].duration;
          led_data = single_star_animation_very_slow[led_idx].led_data;
      }

      // Time to change state
      if (elapsed_time > duration) {
        // Write to shift register
        shift_out_data(led_data);

        // Last led animation entry
        led_idx++;
        if (led_idx >= star_cycle_len) {
          led_idx = 0;
        }

        // Reset time
        elapsed_time = 0;
      }
      break;
    }

    /**********************************************************************
    ************* STACK ONTOP OF EACH OTHER (IF ARRANGED VERTICALLY) ******
    **********************************************************************/
     case led_mode_stack_cycle_very_fast:
     case led_mode_stack_cycle_mixed_speed: {
      uint16_t duration = led_mode_stack_animation_very_fast[led_idx].duration;
      uint16_t led_data = led_mode_stack_animation_very_fast[led_idx].led_data;
      if (led_mode == led_mode_stack_cycle_mixed_speed) {
          duration = led_mode_stack_animation_mixed_speed[led_idx].duration;
          led_data = led_mode_stack_animation_mixed_speed[led_idx].led_data;
      }

      // Time to change state
      if (elapsed_time > duration) {
        // Write to shift register
        shift_out_data(led_data);

        // Last led animation entry
        led_idx++;
        if (led_idx >= stack_cycle_len) {
          led_idx = 0;
        }

        // Reset time
        elapsed_time = 0;
      }
      break;
    }

    /**********************************************************************
    ************************* ROTATION ANIMATION **************************
    **********************************************************************/
    case led_mode_rotation_cycle_very_fast:
    case led_mode_rotation_cycle_very_slow: {
      uint16_t duration = rotation_animation_extremely_fast[led_idx].duration;
      uint16_t led_data = rotation_animation_extremely_fast[led_idx].led_data;
      if (led_mode == led_mode_rotation_cycle_very_slow) {
          duration = rotation_animation_very_slow[led_idx].duration;
          led_data = rotation_animation_very_slow[led_idx].led_data;
      }

      // Time to change state
      if (elapsed_time > duration) {
        // Write to shift register
        shift_out_data(led_data);

        // Last led animation entry
        led_idx++;
        if (led_idx >= rotation_cycle_len) {
          led_idx = 0;
        }

        // Reset time
        elapsed_time = 0;
      }
      
      break;
    }

    /**********************************************************************
    ***************************** ALWAYS ON *******************************
    **********************************************************************/
    case led_mode_all_on: {
      // Write to shift register
        shift_out_data(0xff);
        break;
    }
    
  }

  // Round robin toggle button
  if (btn_value == 0 && prev_btn_value == 1) {
    // force all off
    shift_out_data(0x00);

    elapsed_time = 0;
    led_idx = 0;

    led_mode++;
    if (led_mode == led_mode_nr_entries) {
      led_mode = 0;
    }
  }

  prev_btn_value = btn_value;
  
  delay(loop_delay_ms);

  iteration++;
  elapsed_time += loop_delay_ms;
}
