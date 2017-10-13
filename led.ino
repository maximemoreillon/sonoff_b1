/*
 * Original code from Tasmota
 * https://github.com/arendst/Sonoff-Tasmota
 */

void LED_DI_pulse(uint8_t times)
{
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(DI_pin, HIGH);
    digitalWrite(DI_pin, LOW);
  }
}

void LED_DCK_pulse(uint8_t times)
{
  for (uint8_t i = 0; i < times; i++) {
    digitalWrite(DCK_pin, HIGH);
    digitalWrite(DCK_pin, LOW);
  }
}

void LED_write_data(uint8_t data)
{
  for (uint8_t i = 0; i < 4; i++) {    // Send 8bit Data
    digitalWrite(DCK_pin, LOW);
    digitalWrite(DI_pin, (data & 0x80));
    digitalWrite(DCK_pin, HIGH);
    data = data << 1;
    digitalWrite(DI_pin, (data & 0x80));
    digitalWrite(DCK_pin, LOW);
    digitalWrite(DI_pin, LOW);
    data = data << 1;
  }
}

void LED_setup()
{
  // GPIO setup
  pinMode(DI_pin, OUTPUT);
  pinMode(DCK_pin, OUTPUT);
  
  uint8_t chips = 2; //  2 chips for SONOFF B1

  LED_DCK_pulse(chips * 32);   // Clear all duty register
  delayMicroseconds(12);             // TStop > 12us.
  // Send 12 DI pulse, after 6 pulse's falling edge store duty data, and 12
  // pulse's rising edge convert to command mode.
  LED_DI_pulse(12);
  delayMicroseconds(12);             // Delay >12us, begin send CMD data
  for (uint8_t n = 0; n < chips; n++) {    // Send CMD data
    LED_write_data(0x18);     // ONE_SHOT_DISABLE, REACTION_FAST, BIT_WIDTH_8, FREQUENCY_DIVIDE_1, SCATTER_APDM
  }
  delayMicroseconds(12);             // TStart > 12us. Delay 12 us.
  // Send 16 DI pulse, at 14 pulse's falling edge store CMD data, and
  // at 16 pulse's falling edge convert to duty mode.
  LED_DI_pulse(16);
  delayMicroseconds(12);             // TStop > 12us.
}

void LED_set(uint8_t duty_r, uint8_t duty_g, uint8_t duty_b, uint8_t duty_w, uint8_t duty_c)
{
  uint8_t channels[2] = { 4, 6 };
  uint8_t didx = 1; // no idea what this does
  uint8_t duty[2][6] = {{ duty_r, duty_g, duty_b, duty_w, 0, 0 },        // Definition for RGBW channels
                        { duty_w, duty_c, 0, duty_g, duty_r, duty_b }};  // Definition for RGBWC channels

  delayMicroseconds(12);             // TStop > 12us.
  for (uint8_t channel = 0; channel < channels[didx]; channel++) {
    LED_write_data(duty[didx][channel]);  // Send 8bit Data
  }
  delayMicroseconds(12);             // TStart > 12us. Ready for send DI pulse.
  LED_DI_pulse(8);              // Send 8 DI pulse. After 8 pulse falling edge, store old data.
  delayMicroseconds(12);             // TStop > 12us.
}
