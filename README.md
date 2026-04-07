<div align="center"><p>
    <a href="https://github.com/bulb-light/ArduinoProjects_dff/pulse">
      <img alt="Last commit" src="https://img.shields.io/github/last-commit/bulb-light/ArduinoProjects_dff?style=for-the-badge&logo=starship&color=8bd5ca&logoColor=D9E0EE&labelColor=302D41"/>
    </a>
    <a href="https://github.com/bulb-light/ArduinoProjects_dff/blob/master/LICENSE">
      <img alt="License" src="https://img.shields.io/github/license/bulb-light/ArduinoProjects_dff?style=for-the-badge&logo=starship&color=ee999f&logoColor=D9E0EE&labelColor=302D41" />
    </a>
    <a href="https://github.com/bulb-light/ArduinoProjects_dff/stargazers">
      <img alt="Stars" src="https://img.shields.io/github/stars/bulb-light/ArduinoProjects_dff?style=for-the-badge&logo=starship&color=c69ff5&logoColor=D9E0EE&labelColor=302D41" />
    </a>
    <a href="https://github.com/bulb-light/ArduinoProjects_dff/issues">
      <img alt="Issues" src="https://img.shields.io/github/issues/bulb-light/ArduinoProjects_dff?style=for-the-badge&logo=bilibili&color=F5E0DC&logoColor=D9E0EE&labelColor=302D41" />
    </a>
    <a href="https://github.com/bulb-light/ArduinoProjects_dff">
      <img alt="Repo Size" src="https://img.shields.io/github/repo-size/bulb-light/ArduinoProjects_dff?color=%23DDB6F2&label=SIZE&logo=codesandbox&style=for-the-badge&logoColor=D9E0EE&labelColor=302D41" />
    </a>
    <a href="https://www.youtube.com/@david-dff-bulblight">
      <img alt="follow on Youtube" src="https://img.shields.io/twitter/follow/david-dff-bulblight?style=for-the-badge&logo=youtube&color=8aadf3&logoColor=D9E0EE&labelColor=302D41" />
    </a>
</div>

# ArduinoProjects_dff
### (Arduino and ESP32)

This repository contains a collection of Arduino and ESP32 projects covering a variety of topics.

Click the title to explore.

| Project | Description | Tools and Tech Stack |
|---|---|---|
| [TemperatureControlLM35](https://github.com/bulb-light/ArduinoProjects_dff/tree/main/TemperatureControlLM35) | PID control of a simple temperature plant | PID, Arduino, C++, LM35 sensor |
| [DCMotorSpeedCurrentReading](https://github.com/bulb-light/ArduinoProjects_dff/tree/main/DCMotorSpeedCurrentReading) | DC motor speed and current reading | Speed Estimator, Digital Filters, Current INA219 sensor, Arduino, C++ |
| [DCMotorSpeedControlPI](https://github.com/bulb-light/ArduinoProjects_dff/tree/main/DCMotorSpeedControlPI) | PI speed control for a DC motor | Speed Estimator, Digital Filters, PI controller, Arduino, C++|
| [DCMotorCascadedSpeedControlPI](https://github.com/bulb-light/ArduinoProjects_dff/tree/main/DCMotorCascadedSpeedControlPI) | Cascaded PI speed control for a DC motor: PI control in the outer loop and PI current in the inner loop | Speed Estimator, Digital Filters, PI controller, Arduino, C++|

---
## 🔍 Explore important topics

[![Discrete-time PID controller](https://img.shields.io/badge/Discrete%20Time%20PID-Go-Color?style=for-the-badge)](https://github.com/bulb-light/PID_lib_dff/blob/main/DiscreteTimePID.md)
[![Digital Filters](https://img.shields.io/badge/Digital%20Filters-Go-Color?style=for-the-badge)](https://github.com/bulb-light/DigitalFilter_dff)
[![Speed Estimator](https://img.shields.io/badge/DC%20Motor%20Speed%20EStimator-Go-Color?style=for-the-badge)](https://github.com/bulb-light/SpeedEstimator_dff)
[![Arduino and ESP32 in VSCODE](https://img.shields.io/badge/Arduino%20and%20ESP32%20in%20VSCODE-Go-Color?style=for-the-badge)](https://youtube.com/playlist?list=PLBDyYA13fp43iDQHjRhtriKRwh3nSMSt7&si=Qqkj07vAAFYU0ftQ)

---
### Installation/Usage
1. Clone this repository:
   ```bash
   git clone --recursive https://github.com/bulb-light/ArduinoProjects_dff.git
   ```
   If you have already cloned the project without `--recursive`, run these commands from the project root:
   ```bash
   git submodule init
   git submodule update --recursive
   ```
   This will fetch and checkout the required submodule content.
   
   If you want to update the submodules to the lastest commit, run the following command from the project root:
   ```bash
   git submodule update --init --recursive --remote
   ```

3. Open the project in your preferred IDE.
4. Upload the `src/main.cpp` file to your Arduino board.

---
📝 License Change Notice

- As of 2025-11-16, this project has been relicensed from GNU GPL v3 to the MIT License.
- All new releases going forward will be distributed under the MIT License.
- Previous versions (before this date) remain under the GPL v3 for anyone who has already obtained them. 
- The full text of the MIT License is now available in the [LICENSE](./LICENSE) file in this repository.

---

## 📜 License Structure

- This repository is **licensed under the MIT License** (see `LICENSE` at the root).  
- Each project has its own `LICENSE` file, which is **also MIT**, but reflects the correct copyright author for that project.  

---

## 📫 How to Reach Me

| Platform | Handle / Link |
|---|---|
| Email | davidcs.ee.10@gmail.com |
| LinkedIn | [david](https://www.linkedin.com/in/davidcsee/) |
| Tiktok | [david_dff_bulblight](https://www.tiktok.com/@david_dff_bulblight)|
| YouTube| [david-dff](https://www.youtube.com/@david-dff-bulblight)|

---

## 🔗 Connect & Collaborate

I’m open to collaboration on open source, side projects, or mentoring.  
Feel free to reach out!

If you appreciate my work, you can support its development and maintenance. Improve the quality of the libraries by providing issues and Pull Requests, or make a donation.

Thank you.

---

*“You only live once, but if you do it right, once is enough.”*
