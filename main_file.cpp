#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <cstdlib>

#ifdef _WIN32
// Windows-specific includes
#include <windows.h>
#elif __linux__
// Linux-specific includes
#include <fstream>
#include <string>
#elif __APPLE__
// macOS-specific includes
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#endif

void fetchAndMonitorTemperature(double threshold, int interval) {
    while (true) {
        double temperature = 0.0;

#ifdef _WIN32
        // Fetch temperature using Windows API or third-party library
        temperature = 45.0; // Placeholder value
#elif __linux__
        std::ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");
        if (tempFile.is_open()) {
            int tempMilli;
            tempFile >> tempMilli;
            temperature = tempMilli / 1000.0;
            tempFile.close();
        } else {
            std::cerr << "Unable to access temperature sensor.\n";
        }
#elif __APPLE__
        // macOS: Implement temperature fetching using IOKit
        temperature = 50.0; // Placeholder value
#endif

        if (temperature > 0.0) {
            std::cout << "Current CPU Temperature: " << temperature << "°C\n";

            if (temperature > threshold) {
                std::cerr << "ALERT: CPU temperature exceeded " << threshold << "°C!\n";
                // Optional: Trigger a system notification
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

int main() {
    std::cout << "=== CPU Heat Monitor ===\n";

    double threshold;
    int interval;

    std::cout << "Enter temperature threshold (°C): ";
    std::cin >> threshold;

    std::cout << "Enter monitoring interval (seconds): ";
    std::cin >> interval;

    std::cout << "Monitoring CPU temperature...\n";

    // Start monitoring in a separate thread
    std::thread monitorThread(fetchAndMonitorTemperature, threshold, interval);
    monitorThread.join();

    return 0;
}
