# AquaSense

AquaSense is a desktop application that helps you check if a water sample is safe to drink or not. It is built using C++, Qt 6, and basic machine learning logic. The goal was to provide a simple interface for users to enter values and instantly get a prediction based on water quality parameters.

---

## Project Overview

This app was originally created as a college project. Users can enter values like pH, turbidity, sulfate, TDS, etc., and the app will predict whether the water is drinkable. The prediction is made by sending the input to a backend API which responds with the result.

The app was configured to use Azure Functions for backend processing, but since the API is not included in this repository, you can connect it to your own logic or service by updating the URLs in the code.

Important note: I did not track this project while building it. I uploaded it to GitHub almost a year later. So some hardcoded paths or placeholder areas may exist, and I might have forgotten what I initially planned to improve.

---

## Features

- Input-to-Prediction: Users enter water values, which are sent to an API that returns a prediction
- GUI built with Qt 6: A desktop interface that is clean and easy to use
- Fully customizable backend: You can connect it to any API by changing the URLs and parameters

---

## Backend Configuration

Originally, I used Azure Functions as the backend. You can switch to any other service by updating the API endpoint and request logic.

Please note that I did not use environment variables. The API URL and keys are directly written in the code. You can update that to follow better practices later.

---

## Notes

- API keys and URLs are hardcoded in the code, not stored in environment variables
- File paths are specific to my machine, so you may need to change them before running
- This is a local desktop app, not a web-based one

---

## Screenshots

Below are some screenshots to show how the app looks and works.

### Splash screen



---

## Tech Stack

- C++
- Qt 6
- Python (for backend logic)
- CMake
- Azure Functions 

---

## Author

This project was created by Manav Sharma.  
GitHub profile: [https://github.com/manavsharma1142](https://github.com/manavsharma1142)
