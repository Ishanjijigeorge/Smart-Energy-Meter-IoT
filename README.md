# ⚡ Smart Energy Meter with IoT Billing

This project implements a smart electricity meter using the ESP8266, ADS1115, and SCT-013 current sensor. It measures real-time power consumption, uploads data to Firebase, and automatically generates a monthly bill using Google Apps Script. The goal is to replace manual meter reading with an accurate and automated system.

## 🔥 Features

- Real-time current, power, and energy monitoring  
- 16-bit high-accuracy ADC (ADS1115)  
- Automatic bill calculation based on energy consumed  
- Cloud data storage using Firebase  
- Monthly bill sent automatically via email  
- Remote monitoring from any device  

## 🔧 Hardware Used

- ESP8266 NodeMCU  
- ADS1115 16-bit ADC module  
- SCT-013 current sensor  
- ZMPT101B voltage sensor (optional)  
- 5V relay module  
- BC547 transistor, resistors, capacitor  
- Breadboard and jumper wires  

## 💻 Software Used

- Arduino IDE  
- Firebase Realtime Database  
- Google Apps Script  
- NTP Client for time updates  

## ⚙️ How the System Works

1. The SCT-013 sensor measures AC current.  
2. ADS1115 converts the analog signal to 16-bit digital data.  
3. ESP8266 calculates power and energy values.  
4. ESP8266 uploads energy, power, bill amount, and timestamp to Firebase.  
5. Google Apps Script runs monthly and sends the electricity bill via email.  

## 📁 Repository Structure

hardware/ – circuit diagram and hardware notes  
firmware/ – Arduino code (main.ino)  
cloud/ – Apps Script and Firebase setup  
documentation/ – project report and presentation  
images/ – prototype images and screenshots  

## 📸 Results

- Accurate real-time energy readings  
- Successful cloud syncing with Firebase  
- Automated monthly billing system  
- Fully working prototype tested with various loads  

## 🚀 Future Improvements

- Voltage + current dual sensing  
- Mobile application dashboard  
- Smart load control automation  
- Dynamic tariff billing  
- Solar panel integration support  

## 👥 Team - NEXUS 4

Ishan Jiji George  
Sherry John  
Manuel James  
Sona Salu
