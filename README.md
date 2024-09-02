# pH and Soil Moisture Monitoring System

This project is an Arduino-based system that monitors soil moisture and pH levels. It uses an SD card for logging data, a DS3231 RTC for timekeeping, and a LiquidCrystal I2C display for real-time information. The system can control pH adjustment relays and manage soil moisture based on sensor readings.

## Components

- **Arduino Board**
- **nRF24L01 Radio Module**
- **DS3231 Real-Time Clock (RTC)**
- **LiquidCrystal I2C Display**
- **Soil Moisture Sensor**
- **pH Sensor**
- **SD Card Module**
- **Relay Modules for pH Adjustment**

## Libraries Used

- `SPI.h`
- `SD.h`
- `DS3231.h`
- `Wire.h`
- `LiquidCrystal_I2C.h`

## Code Explanation

### Libraries and Objects

- **Libraries**: Includes `SPI.h`, `SD.h`, `DS3231.h`, `Wire.h`, and `LiquidCrystal_I2C.h` for communication and control.
- **DS3231 RTC**: Handles timekeeping.
- **LiquidCrystal I2C**: Manages display output.

### Setup Function

- Initializes serial communication, LCD display, RTC, and SD card.
- Configures relay pins for pH and soil moisture control.
- Turns off relays initially.

### Loop Function

- Reads and averages pH values from the sensor.
- Logs pH and soil moisture data to the SD card.
- Adjusts pH levels and soil moisture based on sensor readings.
- Displays current pH and soil moisture on the LCD.

### Data Logging

- Logs pH value, soil moisture, and voltage readings to an SD card.
- Includes timestamps using the DS3231 RTC.

## Notes

- **Error Handling**: The code includes error messages if the SD card fails to initialize.
- **Calibration**: Adjust pH offset and target values as needed based on your specific setup and calibration.

## Disclaimer

This code includes a humorous note from the original author:


**Note**: Use this code responsibly and ensure compliance with local regulations and safety standards.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

For any questions or issues, please contact the author at [dave.borja@cbsua.edu.ph].



