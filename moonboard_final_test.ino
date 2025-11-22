#include <FastLED.h>

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define LED_PIN 17
#define NUM_LEDS 150
#define BRIGHTNESS 100
#define NUM_ROWS 15
#define NUM_COLS 10

// Define hold types for routes
#define HOLD_START 1
#define HOLD_FINISH 2
#define HOLD_HAND 3
#define HOLD_FOOT 4

CRGB leds[NUM_LEDS];

// Hold structure to store both position and type
struct Hold {
    int x;
    int y;
    int type; // 1=start, 2=finish, 3=handhold, 4=foothold
};

// Function to convert (x,y) coordinates to LED index
// x = column (0 to 9, 0 is leftmost)
// y = row (0 to 14, 0 is bottom)
int coordToIndex(int x, int y) {
    if (x < 0 || x >= NUM_COLS || y < 0 || y >= NUM_ROWS) {
        return -1;  // Out of bounds
    }
    
    // Convert from leftmost (0) to rightmost (9) for calculation
    int adjustedX = NUM_COLS - 1 - x;
    
    // Calculate the LED index based on the snake pattern
    if (adjustedX % 2 == 0) {
        // Even columns (from right: 0,2,4,6,8) go bottom to top
        return adjustedX * NUM_ROWS + y;
    } else {
        // Odd columns (from right: 1,3,5,7,9) go top to bottom
        return adjustedX * NUM_ROWS + (NUM_ROWS - 1 - y);
    }
}

// Function to show the LED index for a given coordinate
void showCoordinate(int x, int y, CRGB color) {
    int index = coordToIndex(x, y);
    if (index >= 0 && index < NUM_LEDS) {
        leds[index] = color;
        FastLED.show();
        Serial.print("Coordinate (");
        Serial.print(x);
        Serial.print(",");
        Serial.print(y);
        Serial.print(") maps to LED index: ");
        Serial.println(index);
    } else {
        Serial.println("Invalid coordinate!");
    }
}

// Function to test the mapping by lighting up each LED in sequence
// and displaying its coordinates
void testMapping() {
    Serial.println("Testing coordinate mapping...");
    
    for (int i = 0; i < NUM_LEDS; i++) {
        FastLED.clear();
        leds[i] = CRGB::White;
        FastLED.show();
        
        // Find the coordinates for this index
        int x = -1, y = -1;
        for (int col = 0; col < NUM_COLS; col++) {
            for (int row = 0; row < NUM_ROWS; row++) {
                if (coordToIndex(col, row) == i) {
                    x = col;
                    y = row;
                    break;
                }
            }
            if (x != -1) break;
        }
        
        Serial.print("LED ");
        Serial.print(i);
        Serial.print(" is at coordinate (");
        Serial.print(x);
        Serial.print(",");
        Serial.print(y);
        Serial.println(")");
        
        delay(100);
    }
    
    clearAllLEDs();
}

// Enhanced function to define a route using typed holds
void showRoute(int numHolds, Hold holds[]) {
    clearAllLEDs();
    Serial.print("Showing route with ");
    Serial.print(numHolds);
    Serial.println(" holds");
    
    for (int i = 0; i < numHolds; i++) {
        int x = holds[i].x;
        int y = holds[i].y;
        int type = holds[i].type;
        int index = coordToIndex(x, y);
        
        if (index >= 0 && index < NUM_LEDS) {
            // Set color based on hold type
            switch (type) {
                case HOLD_START:
                    leds[index] = CRGB::Green;  // Start holds are green
                    Serial.print("START ");
                    break;
                case HOLD_FINISH:
                    leds[index] = CRGB::Green;  // Finish holds are green
                    Serial.print("FINISH ");
                    break;
                case HOLD_HAND:
                    leds[index] = CRGB::White;  // Hand holds are white
                    Serial.print("HAND ");
                    break;
                case HOLD_FOOT:
                    leds[index] = CRGB::Red;    // Foot holds are red
                    Serial.print("FOOT ");
                    break;
                default:
                    leds[index] = CRGB::Blue;   // Default color for unspecified types
                    Serial.print("OTHER ");
                    break;
            }
            
            FastLED.show();
            Serial.print("Hold ");
            Serial.print(i+1);
            Serial.print(" at (");
            Serial.print(x);
            Serial.print(",");
            Serial.print(y);
            Serial.print(") - LED index: ");
            Serial.println(index);
            delay(200);
        } else {
            Serial.print("Invalid coordinate at hold ");
            Serial.println(i+1);
        }
    }
}

// The basic route function for backward compatibility
void showRouteByCoords(int numHolds, int coords[][2], CRGB color) {
    clearAllLEDs();
    Serial.print("Showing route with ");
    Serial.print(numHolds);
    Serial.println(" holds");
    
    for (int i = 0; i < numHolds; i++) {
        int x = coords[i][0];
        int y = coords[i][1];
        int index = coordToIndex(x, y);
        
        if (index >= 0 && index < NUM_LEDS) {
            leds[index] = color;
            FastLED.show();
            Serial.print("Hold ");
            Serial.print(i+1);
            Serial.print(" at (");
            Serial.print(x);
            Serial.print(",");
            Serial.print(y);
            Serial.print(") - LED index: ");
            Serial.println(index);
            delay(200);
        } else {
            Serial.print("Invalid coordinate at hold ");
            Serial.println(i+1);
        }
    }
}

void clearAllLEDs() {
    FastLED.clear();
    FastLED.show();
    Serial.println("LEDs cleared");
}

void testAllLEDs() {
    Serial.println("Testing all LEDs");
    for (int i = 0; i < NUM_LEDS; i++) {
        FastLED.clear();
        leds[i] = CRGB::White;  // Bright white
        FastLED.show();
        delay(50);
    }
    clearAllLEDs();
}

void rainbowEffect() {
    Serial.println("Running rainbow pattern");
    clearAllLEDs();
    for (int j = 0; j < 256; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            int hue = (i * 255 / NUM_LEDS) + j;
            leds[i] = CHSV(hue, 255, 255);
        }
        FastLED.show();
        
    }
}

void routeOne() {
    clearAllLEDs();
    int route1[] = {0, 11, 22, 33, 44, 55, 66};
    int routeLength = sizeof(route1) / sizeof(route1[0]);

    Serial.println("Showing Route 1");
    for (int i = 0; i < routeLength; i++) {
        leds[route1[i]] = CRGB::Red;  // Red
        FastLED.show();
        delay(200);
    }
}

void routeTwo() {
    clearAllLEDs();
    int route2[] = {5, 15, 25, 24, 23, 33, 43, 42, 41};
    int routeLength = sizeof(route2) / sizeof(route2[0]);

    Serial.println("Showing Route 2");
    for (int i = 0; i < routeLength; i++) {
        leds[route2[i]] = CRGB::Green;  // Green
        FastLED.show();
        delay(200);
    }
}

void routeThree() {
    clearAllLEDs();
    int route3[] = {10, 20, 30, 40, 50, 60, 70, 13, 23, 33, 43, 53, 63, 73};
    int routeLength = sizeof(route3) / sizeof(route3[0]);

    Serial.println("Showing Route 3");
    for (int i = 0; i < routeLength; i++) {
        leds[route3[i]] = CRGB::Blue;  // Blue
        FastLED.show();
        delay(200);
    }
}

void routeFour() {
    clearAllLEDs();
    int route4[] = {2, 13, 24, 35, 46, 57, 68, 8, 17, 26, 35, 44, 53, 62};
    int routeLength = sizeof(route4) / sizeof(route4[0]);

    Serial.println("Showing Route 4");
    for (int i = 0; i < routeLength; i++) {
        leds[route4[i]] = CRGB::Orange;  // Orange
        FastLED.show();
        delay(200);
    }
}

void routeFive() {
    clearAllLEDs();
    int route5[] = {0, 30, 42, 55, 60, 74};
    int routeLength = sizeof(route5) / sizeof(route5[0]);

    // Start and finish holds
    int start = route5[0];
    int finish = route5[routeLength - 1];

    Serial.println("Showing Route 5");

    for (int i = 0; i < routeLength; i++) {
        if (route5[i] == start) {
            leds[route5[i]] = CRGB::Blue;  // Blue for start
        } else if (route5[i] == finish) {
            leds[route5[i]] = CRGB::Green;  // Green for finish
        } else {
            leds[route5[i]] = CRGB::White;  // White for the rest
        }
        FastLED.show();
        delay(200);
    }
}

// New example routes using the Hold structure
void showExampleRoute1() {
    // Define a route with start hold (green), foot holds (red), and hand holds (white)
    Hold route[] = {
        {0, 0, HOLD_START},   // Start hold at bottom left
        {1, 0, HOLD_FOOT},    // Foot hold
        {2, 1, HOLD_HAND},    // Regular hand hold
        {3, 2, HOLD_HAND},    // Regular hand hold
        {4, 3, HOLD_FOOT},    // Foot hold
        {5, 4, HOLD_HAND},    // Regular hand hold
        {6, 5, HOLD_HAND},    // Regular hand hold
        {7, 6, HOLD_FOOT},    // Foot hold
        {8, 7, HOLD_HAND},    // Regular hand hold
        {9, 8, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(10, route);
}

void showExampleRoute2() {
    // Define a more complex route
    Hold route[] = {
        {0, 5, HOLD_START},   // Start hold
        {1, 4, HOLD_FOOT},    // Foot hold
        {2, 4, HOLD_FOOT},    // Foot hold
        {1, 7, HOLD_HAND},    // Hand hold
        {2, 8, HOLD_HAND},    // Hand hold
        {3, 7, HOLD_HAND},    // Hand hold
        {4, 6, HOLD_FOOT},    // Foot hold
        {5, 7, HOLD_HAND},    // Hand hold
        {6, 8, HOLD_HAND},    // Hand hold
        {7, 9, HOLD_HAND},    // Hand hold
        {8, 10, HOLD_FINISH}  // Finish hold
    };
    
    showRoute(11, route);
}

void showExampleRoute3() {
    // Define a traverse route (horizontal movement)
    Hold route[] = {
        {0, 7, HOLD_START},   // Start hold
        {0, 5, HOLD_FOOT},    // Foot hold
        {1, 5, HOLD_FOOT},    // Foot hold
        {1, 7, HOLD_HAND},    // Hand hold
        {2, 7, HOLD_HAND},    // Hand hold
        {2, 5, HOLD_FOOT},    // Foot hold
        {3, 5, HOLD_FOOT},    // Foot hold
        {3, 7, HOLD_HAND},    // Hand hold
        {4, 7, HOLD_HAND},    // Hand hold
        {4, 5, HOLD_FOOT},    // Foot hold
        {5, 5, HOLD_FOOT},    // Foot hold
        {5, 7, HOLD_HAND},    // Hand hold
        {6, 7, HOLD_HAND},    // Hand hold
        {7, 7, HOLD_HAND},    // Hand hold
        {8, 7, HOLD_HAND},    // Hand hold
        {9, 7, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(16, route);
}


void fn_route1_male() {
    //final route 1
    Hold route[] = {
        {1, 1, HOLD_START},   // Start hold 1
        {2, 1, HOLD_START},   // Start hold 2

        {9, 0, HOLD_FOOT},    // Foot hold 1
        {0, 5, HOLD_FOOT},    // Foot hold 2
        {0, 7, HOLD_FOOT},    // Foot hold 3

        {5, 4, HOLD_HAND},    // Hand hold
        {4, 6, HOLD_HAND},    // Hand hold
        {7, 7, HOLD_HAND},    // Hand hold
        {6, 9, HOLD_HAND},    // Hand hold
        {2, 11, HOLD_HAND},   // Hand hold
        {3, 11, HOLD_HAND},   // Hand hold
        {0, 12, HOLD_HAND},   // Hand hold

        {3, 14, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(13, route);
}


void fn_route1_female() {//apparently no changes in code, foothold added below leds
    //final route 1
    Hold route[] = {
        {1, 1, HOLD_START},   // Start hold 1
        {2, 1, HOLD_START},   // Start hold 2

        {9, 0, HOLD_FOOT},    // Foot hold 1
        {0, 5, HOLD_FOOT},    // Foot hold 2
        {0, 7, HOLD_FOOT},    // Foot hold 3

        {5, 4, HOLD_HAND},    // Hand hold
        {4, 6, HOLD_HAND},    // Hand hold
        {7, 7, HOLD_HAND},    // Hand hold
        {6, 9, HOLD_HAND},    // Hand hold
        {2, 11, HOLD_HAND},   // Foot hold
        {3, 11, HOLD_HAND},   // Foot hold
        {0, 12, HOLD_HAND},   // Hand hold

        {3, 14, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(13, route);
}


void fn_route2_male() {
    //final route 2 potentially missing footholds
    Hold route[] = {
        {0, 1, HOLD_START},   // Start hold 1
        {3, 2, HOLD_START},   // Start hold 2

        {4, 1, HOLD_FOOT},    // Foot hold 1
        {7, 4, HOLD_FOOT},    // Foot hold 2
        {9, 4, HOLD_FOOT},    // Foot hold 3

        {5, 5, HOLD_HAND},    // Hand hold
        {2, 5, HOLD_HAND},    // Hand hold
        {4, 7, HOLD_HAND},    // Hand hold
        {3, 8, HOLD_HAND},    // Hand hold
        {6, 10, HOLD_HAND},   // Hand hold
        {7, 10, HOLD_HAND},   // Hand hold
        {6, 12, HOLD_HAND},   // Hand hold

        {3, 14, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(13, route);
}


void fn_route2_female() {

    Hold route[] = {
        {0, 1, HOLD_START},   // Start hold 1
        {3, 2, HOLD_START},   // Start hold 2

        {4, 1, HOLD_FOOT},    // Foot hold 1
        {7, 4, HOLD_FOOT},    // Foot hold 2
        {9, 4, HOLD_FOOT},    // Foot hold 3
        {5, 6, HOLD_FOOT},    // Foot hold 4

        {5, 5, HOLD_HAND},    // Hand hold
        {2, 5, HOLD_HAND},    // Hand hold
        {4, 7, HOLD_HAND},    // Hand hold
        {3, 8, HOLD_HAND},    // Hand hold
        {6, 10, HOLD_HAND},   // Hand hold
        {7, 10, HOLD_HAND},   // Hand hold
        {6, 12, HOLD_HAND},   // Hand hold

        {3, 14, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(14, route);
}


void fn_route3_male() {
    Hold route[] = {
        {4, 1, HOLD_START},   // Start hold 1
        {8, 3, HOLD_START},   // Start hold 2

        {3, 1, HOLD_FOOT},    // Foot hold 1
        {5, 5, HOLD_FOOT},    // Foot hold 2

        {9, 5, HOLD_HAND},    // Hand hold
        {3, 6, HOLD_HAND},    // Hand hold
        {6, 6, HOLD_HAND},    // Hand hold
        {7, 8, HOLD_HAND},    // Hand hold
        {5, 9, HOLD_HAND},    // Hand hold
        {1, 10, HOLD_HAND},   // Hand hold

        {4, 14, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(11, route);
}


void fn_route3_female() {
    Hold route[] = {
        {4, 1, HOLD_START},   // Start hold 1
        {8, 3, HOLD_START},   // Start hold 2

        {3, 1, HOLD_FOOT},    // Foot hold 1
        {5, 5, HOLD_FOOT},    // Foot hold 2
        {2, 7, HOLD_FOOT},    // Foot hold 3

        {9, 5, HOLD_HAND},    // Hand hold
        {3, 6, HOLD_HAND},    // Hand hold
        {6, 6, HOLD_HAND},    // Hand hold
        {7, 8, HOLD_HAND},    // Hand hold
        {5, 9, HOLD_HAND},    // Hand hold
        {1, 10, HOLD_HAND},   // Hand hold
        {4, 12, HOLD_HAND},   // Hand hold

        {4, 14, HOLD_FINISH}   // Finish hold
    };
    
    showRoute(13, route);
}


void setup() {
    Serial.begin(115200);

    SerialBT.begin("MoonBoardESP32"); // Name of your ESP32 Bluetooth device

    delay(1000);
    Serial.println("\nLED Control System Starting...");

    FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS); //jareb badel grb lel rgb wala okhrin
    FastLED.setBrightness(BRIGHTNESS);
    clearAllLEDs();

    Serial.println("Available commands:");
    Serial.println("TEST - Test all LEDs");
    Serial.println("TEST_MAP - Test coordinate mapping");
    Serial.println("COORD x,y[,type] - Light up coordinate (type: H/HAND=white, F/FOOT=red, S/START/E/END=green)");
    Serial.println("RAINBOW - Rainbow pattern");
    Serial.println("R1-R5 - Legacy routes");
    Serial.println("ROUTE1 - Basic example route");
    Serial.println("CLEAR - Clear all LEDs");
    Serial.println("F1M - final route 1 male");
    Serial.println("F1F - final route 1 female");
    Serial.println("Ready for commands!");
}

void loop() {
    if (Serial.available() || SerialBT.available()) {
        String command;
        if (Serial.available()) {
            command = Serial.readStringUntil('\n'); // Read from Serial Monitor
        } else {
            command = SerialBT.readStringUntil('\n'); // Read from Bluetooth
        }

        command.trim();

        if (command == "TEST") {
            testAllLEDs();
        } else if (command == "TEST_MAP") {
            testMapping();
        } else if (command.startsWith("COORD ")) {
            String coords = command.substring(6);
            int commaPos = coords.indexOf(',');
            if (commaPos > 0) {
                int x = coords.substring(0, commaPos).toInt();
                String remainder = coords.substring(commaPos + 1);

                // Check for optional hold type parameter
                int secondCommaPos = remainder.indexOf(',');
                int y;
                CRGB color = CRGB::White; // Default to hand hold (white)

                if (secondCommaPos > 0) {
                    y = remainder.substring(0, secondCommaPos).toInt();
                    String holdType = remainder.substring(secondCommaPos + 1);
                    holdType.trim();
                    holdType.toUpperCase();

                    // Set color based on hold type
                    if (holdType == "F" || holdType == "FOOT") {
                        color = CRGB::Red;  // Foot hold
                    } else if (holdType == "S" || holdType == "START" || holdType == "E" || holdType == "END"  ) {
                        color = CRGB::Green;  // Start hold
                    } else {
                        color = CRGB::White;  // Hand hold (default or explicit H/HAND)
                    }
                } else {
                    y = remainder.toInt();
                }

                showCoordinate(x, y, color);
            }
        } else if (command == "RAINBOW") {
            rainbowEffect();
        } else if (command == "R1") {
            routeOne();
        } else if (command == "R2") {
            routeTwo();
        } else if (command == "R3") {
            routeThree();
        } else if (command == "R4") {
            routeFour();
        } else if (command == "R5") {
            routeFive();
        } else if (command == "ROUTE1") {
            // Example route defined by coordinates (x,y)
            int routeCoords[][2] = {
                {0, 0},  // Bottom left
                {1, 1},
                {2, 2},
                {3, 3},
                {4, 4},
                {5, 5},
                {6, 6},
                {7, 7},
                {8, 8},
                {9, 9}   // Middle right
            };
            showRouteByCoords(10, routeCoords, CRGB::Red);
        } else if (command == "CLIMB1") {
            showExampleRoute1();
        } else if (command == "CLIMB2") {
            showExampleRoute2();
        } else if (command == "CLIMB3") {
            showExampleRoute3();
        } 

        else if (command == "F1M") {
            fn_route1_male();
        } 
        else if (command == "F1F") {
            fn_route1_female();
        } 

        else if (command == "F2M") {
            fn_route2_male();
        } 
        else if (command == "F2F") {
            fn_route2_female();
        } 

        else if (command == "F3M") {
            fn_route3_male();
        } 
        else if (command == "F3F") {
            fn_route3_female();
        } 

        else if (command == "CLEAR") {
            clearAllLEDs();
        }
        Serial.println("Command complete!");
    }
    delay(10);
}