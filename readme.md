# ✈️ Flight Reservation System

Welcome to the Flight Reservation System! This is a desktop application built with C++ and the Qt framework, designed to provide a simple and intuitive interface for searching, booking, and managing flight itineraries.

## ✨ Features

This application allows users to:

* **Search for Flights:** Find flights between two airports on a specific date.
* **Filter by Direct Flights:** Option to show only direct flights or include connecting flights (up to 2 stops).
* **Flexible Passenger Count:** Specify the number of passengers for the booking.
* **Intelligent Sorting:** Search results are automatically sorted by the best fare, and then by the shortest duration.
* **Book Itineraries:** Select a flight from the search results and enter passenger details to create a booking.
* **Generate PNR:** A unique 6-character PNR (Passenger Name Record) is generated for each booking.
* **Cancel Bookings:** A simple PNR-based cancellation feature.

## 🛠️ Tech Stack

* **Core Logic:** C++17
* **GUI Framework:** Qt 6
* **Build System:** CMake
* **Data Storage:** CSV files for simplicity and portability.

## 🚀 Getting Started

Follow these instructions to get a copy of the project up and running on your local machine.

### Prerequisites

You will need the following software installed on your system:

* **A C++ Compiler:** Microsoft Visual C++ (MSVC) is recommended for Windows.
* **CMake:** Version 3.22 or higher.
* **Qt 6:** The application is built against Qt 6. You can download it from the [official Qt website](https://www.qt.io/download).

### Installation & Running

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Vedant-Rajput22/FlightReservation.git
    cd FlightReservation
    ```

2.  **Configure the Build Script:**
    Open the `build_run_msvc.bat` file and update the `QT_DIR` variable to point to your Qt 6 installation directory.
    ```batch
    REM ====== Change this if your Qt version/folder differs ======
    set QT_DIR=C:\path\to\your\Qt\6.x.x\msvcXXXX_64
    ```

3.  **Run the build script:**
    Execute the batch file from your terminal. This will configure the project with CMake, build it, and run the application.
    ```bash
    .\build_run_msvc.bat
    ```

## 📂 Project Structure

The project is organized into the following main directories:

* `data/`: Contains the CSV files for airports, flights, passengers, and bookings.
* `include/`: Header files for the core logic and UI components.
    * `core/`: Contains the business logic (e.g., `Storage`, `SearchService`, `ReservationService`).
    * `ui/`: Contains headers for the user interface components.
* `src/`: Source code implementation files.
    * `core/`: Implementation of the core logic.
    * `ui/`: Implementation of the user interface.
* `app/`: Contains the `main.cpp` entry point for the application.

## 📊 Data Model

The application uses a simple CSV-based data store for persistence.

* `airports.csv`:
    * `code`: IATA airport code (e.g., BOM)
    * `name`: Full airport name
    * `city`: City where the airport is located
    * `country`: Country where the airport is located
    * `tz`: Timezone in `Area/Location` format
* `flights.csv`:
    * `flightNo`: Unique flight identifier
    * `from`, `to`: Departure and arrival airport codes
    * `depHHMM`, `arrHHMM`: Departure and arrival times in HH:MM format
    * `distanceKm`: The distance of the flight in kilometers
    * `baseFare`: The base cost of the flight ticket
    * `seatsTotal`, `seatsAvailable`: Seat capacity and availability
    * `days`: A string of numbers (1-7 for Mon-Sun) indicating the days the flight operates
* `passengers.csv`:
    * `paxId`: Unique passenger identifier
    * `name`, `email`, `phone`: Passenger contact information
* `bookings.csv`:
    * `pnr`: The unique Passenger Name Record for the booking
    * `flightNo`, `date`: Flight and date of travel
    * `paxId`: The ID of the passenger
    * `status`: Booking status (e.g., "BOOKED", "CANCELLED")
    * `farePaid`: The fare paid for the booking
    * `createdAt`: Timestamp of when the booking was made
