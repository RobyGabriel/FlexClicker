# 🖱️ Advanced AutoClicker (Mouse & Keyboard)

A lightweight, high-performance, and feature-rich AutoClicker built natively in C++ using the WinAPI. Designed to automate both mouse clicks and keyboard presses with precision and zero overhead.

---

## ✨ Features

### ⚙️ Core Functionality
* **Custom CPS:** Set your desired Clicks Per Second (CPS) precisely.
* **Dual Mode:** * **Mouse Mode:** Automate left or right clicks. Quickly switch between them using `RCTRL + [Your Custom Key]`.
    * **Keyboard Mode:** Simulate any keyboard key press repeatedly.

### 🛠️ Settings & Customization
* **Custom Toggle Key:** Bind any key to start/stop the clicker instantly.
* **Enable Jitter:** Adds natural, micro-mouse movements (jitter) to simulate human behavior and bypass basic anti-cheat detection.
* **Show Overlay:** Toggle a sleek, on-screen display.

### 📺 Smart Overlay
Keep track of your settings in real-time with a lightweight, on-screen overlay showing:
* **Status:** On / Off
* **Active Key:** The key being simulated (Keyboard Mode)
* **Click Type:** Left / Right (Mouse Mode)
* **Current CPS:** Real-time speed indicator

---

## 🚀 Getting Started

### Prerequisites
* Windows 10 / 11
* No external runtimes required (Native WinAPI)!
* *For compiling:* Visual Studio 2022 (with "Desktop development with C++" workload).

### Installation & Running
If you just want to use the application, go to the **Releases** page and download the latest `.exe`.

To build it from source:
1. Clone the repository:
   ```bash
   git clone [https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git](https://github.com/RobyGabriel/FlexClicker.git)
   ```
2. Open the .sln file in Visual Studio.

3. Set the build configuration to Release and your target architecture (e.g., x64).

4. Build the solution (Ctrl + Shift + B).

## 🎮 How To Use

    Open the application and set your desired CPS.

    Go to Settings to configure your preferred shortcut keys.

    Choose your Mode:

        Mouse Mode: Use RCTRL + [Your Custom Key] to quickly swap between Left and Right click.

        Keyboard Mode: Select the specific key you want to automate.

    (Optional) Check Enable Jitter for natural movement or Show Overlay to see your stats on screen.

    Press your Custom Toggle Key to Start/Stop the automation!

## 🛠️ Built With

    Language: C++ (ISO C++17 or newer)

    Framework: Native Windows API (WinAPI) for GUI, Global Hotkeys, and Input Simulation.

    IDE: Visual Studio