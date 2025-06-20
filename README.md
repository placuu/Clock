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






Made by [placuu](https://github.com/placuu)
