/***************************************************************************
 * Copyright 2015 Jacques Supcik
 * School of Engineering and Architecture of Fribourg.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***************************************************************************/

/*!
 *  \brief     Pomoduino - 25 minutes work, 5 minutes rest
 *  \author    Jacques Supcik
 *  \version   1.0.1
 *  \date      6-jan-2015 - 7-jan-2015
 *  \copyright Apache License, Version 2.0.
 */

#undef DEMO_MODE
#define DO_TICK

const int NUMBER_OF_RED_LEDS = 5;
const int RED_LEDS[]         = {12,11,10,9,8};
const int GREEN_LED          = 7;
const int SPEAKER            = 3;

const int BREAK_SOUND_FREQ     =  400; // Hz
const int BREAK_SOUND_DURATION =  500; // ms

const int WORK_SOUND_FREQ     = 1000; // Hz
const int WORK_SOUND_DURATION =  500; // ms

#ifdef DO_TICK
const int TICK_SOUND_FREQ     =  220; // Hz
const int TICK_SOUND_DURATION =    3; // ms
#endif

#ifndef DEMO_MODE
// Normal values (5 x 5 minutes)
const int BLINK_SPEED    = 500; // period = 1 sec
const unsigned long UNIT = 1000L * 60 * 5; // 5 minutes
#ifdef DO_TICK
const int TICK_SPEED     = 250;
#endif
#else
// Special "fast" values for demo mode.
const int BLINK_SPEED    = 100;
const unsigned long UNIT = 1000L * 2; // 2 seconds
#ifdef DO_TICK
const int TICK_SPEED     = 125;
#endif
#endif

void setup() {
    // Define all pins as output
    for (int i = 0; i < NUMBER_OF_RED_LEDS; i++) {
        pinMode(RED_LEDS[i], OUTPUT);
    }
    pinMode(GREEN_LED, OUTPUT);
    pinMode(SPEAKER,   OUTPUT);
}

void loop() {
    static unsigned long t0 = millis(); // start time
    static int ring_done = 0;
#ifdef DO_TICK
    static int tick_done = 0;
#endif
    unsigned long t = millis();
    unsigned long current_led = (t - t0) / UNIT;
    
    if (current_led > NUMBER_OF_RED_LEDS) { // End of break, go to work
        tone(SPEAKER, WORK_SOUND_FREQ, WORK_SOUND_DURATION);
        t0 = t; 
        ring_done = 0;
        // LEDs will be set during the next loop
    } else if (current_led == NUMBER_OF_RED_LEDS) { // Time for a break !
        if (!ring_done) { // ring the buzzer if not yet done
            tone(SPEAKER, BREAK_SOUND_FREQ, BREAK_SOUND_DURATION);
            ring_done = 1;
        }
        // Switch all red LEDs off
        for (int i = 0; i < NUMBER_OF_RED_LEDS; i++) {
            digitalWrite(RED_LEDS[i], LOW);
        }
        // Switch green LED on
        digitalWrite(GREEN_LED, HIGH);
    } else { // work in progress
#ifdef DO_TICK
        if ((t - t0 > WORK_SOUND_DURATION) && (t / TICK_SPEED % 2 == 0)) {
            if (! tick_done) {
                tone(SPEAKER, 220, 3);
                tick_done = 1;
            }
        } else {
            tick_done = 0;
        }
#endif
        // static red LEDs
        for (int i = 0; i < current_led; i++) {
            digitalWrite(RED_LEDS[i], HIGH);
        }
        // Blinking LED
        digitalWrite (
            RED_LEDS[current_led], (t / BLINK_SPEED) % 2 ? HIGH : LOW
        );
        // Switch other LEDs off
        for (int i = current_led + 1; i < NUMBER_OF_RED_LEDS; i++) {
            digitalWrite(RED_LEDS[i], LOW);
        }
        // Switch Green off too
        digitalWrite(GREEN_LED, LOW);
    }
}
