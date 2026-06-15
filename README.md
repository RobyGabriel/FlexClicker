# 🖱️ FlexClicker

A lightweight, high-performance, and feature-rich autoclicker built natively in C++ using the WinAPI. Designed to automate both mouse clicks and keyboard presses with precision and zero overhead.

---

## ⚠️ Disclaimer & Antivirus Notes
* **Anti-Cheat Warning:** Use this tool responsibly. Using autoclickers in multiplayer games may violate their Terms of Service and result in a ban. The "Jitter" feature reduces detection risks but does not guarantee immunity.
* **False Positives:** This application uses low-level Windows APIs (SendInput, GetAsyncKeyState, and high-resolution timers) to simulate input and detect hotkeys. As a result, some antivirus software may incorrectly identify it as suspicious or flag it as a false positive. The source code is fully open and safe to audit.

---

## ✨ Features

### ⚙️ Core Functionality
* **Custom CPS:** Set your desired Clicks Per Second (CPS) precisely.
* **Dual Mode:** 
    * **Mouse Mode:** Automate left or right clicks. Quickly switch between them using <kbd>RCTRL</kbd> + <kbd>Custom Key</kbd>.
    * **Keyboard Mode:** Simulate any keyboard key press repeatedly.

### 🛠️ Settings & Customization
* **Custom Toggle Key:** Bind any key to start/stop the clicker instantly.
* **Enable Jitter:** Randomizes the delay between clicks to simulate human behavior and bypass basic anti-cheat detection.
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
   git clone https://github.com/RobyGabriel/FlexClicker.git
   ```
2. Open the FlexClicker.slnx file in Visual Studio.

3. Set the build configuration to Release and your target architecture (e.g., x64).

4. Build the solution (Ctrl + Shift + B).

## 🎮 How To Use

1. **Set CPS:** Open the application and set your desired CPS.

2. **Configure Keys:** Go to Settings to configure your preferred shortcut keys.

3. **Choose Mode:**

    * **Mouse Mode:** Use <kbd>RCTRL</kbd> + <kbd>Custom Key</kbd> to quickly swap between Left and Right click.

    * **Keyboard Mode:** Select the specific key you want to automate.

4. **Tweak Settings:** (Optional) Check **Enable Jitter** for natural timing variations or **Show Overlay** to see your stats on screen.

5. **Activate:** Press your **Custom Toggle Key** to Start/Stop the automation!

## 🛠️ Built With

* Language: C++ (ISO C++17 or newer)

* Framework: Native Windows API (WinAPI) for GUI, Global Hotkeys, and   Input Simulation.

* IDE: Visual Studio

---

## 🔒 Copyright & Terms of Use

This project is personal intellectual property. **All rights reserved.** You are free to view the source code, download it, and compile it for personal use. However, **you do not have permission** to copy, modify, redistribute, or use any part of this source code in other public or commercial projects without explicit written consent from the author.