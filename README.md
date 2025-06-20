# 🕒 Clock
A simple and customizable clock application/library for displaying and managing time in various formats. Useful for projects that need to show the current time, countdowns, timers, or schedule-based actions.

## ✨ Features

- **Current Time Display:** Shows the current time in 12/24-hour format.
- **Countdown Timer:** Set up countdowns for events.
- **Stopwatch:** Simple stopwatch functionality.
- **Time Zone Support:** Display time in any time zone.
- **Extensible:** Easy to extend with new features
- **Customizable UI:** Options to change UI

## 🛠️ Setting Up Visual Studio with SFML
To build and run this project with SFML in Visual Studio:
1. **Install SFML:**
   - Download the SFML SDK from [SFML Downloads](https://www.sfml-dev.org/download.php).
   - Extract it to a known location (e.g., `C:\Libraries\SFML`).

2. **Configure Visual Studio Project:**
   - Open your project in Visual Studio.
   - Right-click your project in Solution Explorer, select **Properties**.
   - Go to **C/C++ > General > Additional Include Directories** and add the SFML `include` folder (e.g., `C:\Libraries\SFML\include`).
   - Go to **Linker > General > Additional Library Directories** and add the SFML `lib` folder (e.g., `C:\Libraries\SFML\lib`).
   - Go to **Linker > Input > Additional Dependencies** and add the required SFML libraries (e.g., `sfml-graphics-d.lib`, `sfml-window-d.lib`, `sfml-system-d.lib` for Debug; remove the `-d` for Release).

3. **Copy DLLs:**
   - Copy the required SFML `.dll` files from the SFML `bin` directory to your project’s output directory (e.g., `Debug` or `Release` folder).

4. **Set Runtime Library (if needed):**
   - Make sure your project and SFML use the same runtime library (e.g., both use **Multi-threaded DLL (/MD)**).

5. **Build and Run:**
   - Build your project (Ctrl+Shift+B).
   - Run your application (F5).

**For detailed instructions, see the [SFML official tutorial](https://www.sfml-dev.org/tutorials/2.6/start-vc.php).**

## 🧭 How to Use

After installing and launching the Clock app, you’ll see the main interface. Here’s how to navigate and use the core features:

When you open the app, you’ll see the home screen with four main buttons:

- **Clock:**  
  Shows the current time. 

- **Stopwatch:**  
  Go to the "Stopwatch" tab. Press **Space** to start or pause the stopwatch (toggle). Press **R** at any time to reset and clear the time.

- **Timer:**  
  Go to the "Timer" tab. Enter your desired countdown time using the format `HHMMSS` (for example, `1030` means 10 minutes and 30 seconds,`13023` means 1 hour, 30 minutes and 23 seconds , similar to the Android timer). Press **Enter** to    start the countdown. When the timer finishes, you’ll receive a notification. Press **R** to reset the timer and stop the notification.

- **Alarm:**  
  Go to the "Alarm" tab. Enter the desired alarm time using the same format as the timer (e.g., `0730` for 7:30). After entering the time, type `AM` or `PM` to set the period. The alarm will be set automatically. When it’s time, the app     will alert you. Press **R** to reset the alarm and stop the notification.

- **Navigation with ESC:**  
  Press **Esc** to go back one step:
  - If you are in the Timer tab, pressing **Esc** returns you to the home screen.
  - If you are on the home screen, pressing **Esc** will exit the app.






Made by [placuu](https://github.com/placuu)
