# 🧗 MoonBoard LED Controller – ESP32 

This project controls a MoonBoard-style LED setup using an ESP32 on an overhanging climbing wall.  
LEDs are originally arranged in a zig-zag (snake) pattern from **0 to 149**, this code handles the pattern and makes it easier to understand and use by making the leds follow (x,y) axis


## 🔧 Features
- Control LEDs under each climbing hold
- Bluetooth serial communication (tested with Android app called 'Serial Bluetooth Terminal' by Kai Morich) *the current BT serial communication support Android only, IOS support will hopefully be implemented in future updates*
- Simple protocol for lighting sequences or problems

## 📁 File Structure
- `moonboard_final_test.ino` — Main Arduino sketch  
  Handles Bluetooth input, LED addressing, and LED updates.

## 🛠️ Hardware Requirements
- ESP32 Dev Board  
- WS2812B / SK6812 LED strip  
- 5V power supply/ USB connection to laptop
- Bluetooth-capable phone (for the app)

## 📡 Communication Protocol
| Command     | Description                       |
|-------------|-----------------------------------|
| `COORD x,y` | Light the led at position (x,y)   |
| `CLEAR`     | Clear all LEDs                    |
| `F1M`       | Final competition route 1 (female)|
| `F1F`       | Final competition route 1 (male)  |
| `F2F` `F2M` | Final route 2 (female/male)       |
|`F3F` `F3M`  | Final route 3 (female/male)       |

(Check the commented code for more command examples )

(Adjust this depending on the actual protocol in your code)

## 🚀 Getting Started
1. Install Arduino IDE
2. Install ESP32 board support
3. Install **FastLED** or **Adafruit NeoPixel** (whichever you're using)
4. Open the `.ino` file and upload it to the ESP32

## 📝 Notes
- If you face any issues with finding the port for the esp32 in the arduino software, or if you have any other questions, do not hesitate to contact me
- LED indices follow a snake pattern (see comments in code)
- Ensure the power supply is strong enough for all LEDs (do not use USB hubs to connect to the esp32, LEDs will not receive enough power to light them) 


## 📄 License
MIT License

