# Hospital Management System

A comprehensive Qt 6-based GUI application for managing hospital operations, including patient registration, doctor assignment, consultation sessions, and hospital proximity matching.

## Features

- **Smart Hospital Proximity Matching**: Uses BFS algorithm to rank hospitals by proximity to patient location
- **Patient Registration**: Register patients with symptoms and automatic doctor assignment
- **Doctor Management**: Track doctor availability and specializations
- **Consultation Sessions**: Conduct and record consultation sessions between patients and doctors
- **Dashboard**: Real-time statistics on hospitals, doctors, and patient status
- **Database Persistence**: SQLite database for storing all hospital, doctor, and patient data

## Dependencies & Prerequisites

### System Requirements

- **Operating System**: Windows 10/11, macOS 10.13+, or Linux (Ubuntu 18.04+)
- **RAM**: Minimum 4GB (8GB recommended)
- **Disk Space**: 2GB for Qt installation + 500MB for project

### Required Dependencies

#### 1. **Qt 6.x Framework** (MANDATORY)
   - **Version**: Qt 6.2 or later (Qt 6.5+ recommended for best compatibility)
   - **Components Required**:
     - Qt Core
     - Qt GUI
     - Qt Widgets
     - Qt SQL (for SQLite support)
   - **Installation**:
     - Download from: https://www.qt.io/download-open-source
     - Or use online installer: https://download.qt.io/official_releases/online_installers/
     - Windows users can run the provided `install_qt.ps1` script

#### 2. **C++ Compiler** (MANDATORY)
   - **Windows**: 
     - MSVC 2019+ (Visual Studio Community Edition)
     - OR MinGW 11.2+ (GCC-based)
     - Run `install_mingw.ps1` for automated MinGW setup
   - **macOS**: 
     - Clang (included with Xcode Command Line Tools)
     - Install via: `xcode-select --install`
   - **Linux**: 
     - GCC 9+ or Clang 10+
     - Ubuntu: `sudo apt-get install build-essential`

#### 3. **Build Tools** (MANDATORY)
   - **qmake**: Included with Qt (recommended)
   - **CMake**: Optional alternative (3.16+)
   - **Make/MinGW32-Make**: 
     - Windows MinGW: `mingw32-make`
     - Linux/macOS: `make`

#### 4. **SQLite** (INCLUDED)
   - SQLite is bundled with Qt SQL module
   - No separate installation needed
   - Database file: `hms_data.db` (auto-created)

### Optional Dependencies

- **Qt Creator IDE**: Recommended for development (included with Qt)
- **Git**: For version control (optional)
- **CMake**: Alternative to qmake for building

## Build Instructions

### Prerequisites Checklist

Before building, verify you have:
- [ ] Qt 6.2+ installed with Qt Core, GUI, Widgets, and SQL modules
- [ ] C++ compiler installed (MSVC, MinGW, GCC, or Clang)
- [ ] qmake in your system PATH
- [ ] C++17 support enabled in compiler

### Building with qmake (Recommended)

#### Windows (with MinGW)

1. **Open PowerShell and navigate to project:**
   ```powershell
   cd C:\Users\HP\Desktop\project\HospitalManagementSystem
   ```

2. **Generate Makefiles using qmake:**
   ```powershell
   qmake HospitalManagementSystem.pro
   ```
   - If qmake not found, add Qt bin to PATH:
   ```powershell
   $env:Path += ";C:\Qt\6.5.0\mingw_64\bin"
   qmake HospitalManagementSystem.pro
   ```

3. **Build the project:**
   ```powershell
   mingw32-make
   ```
   - If mingw32-make not found, ensure MinGW is installed via `install_mingw.ps1`

4. **Run the application:**
   ```powershell
   .\release\HospitalManagementSystem.exe
   ```

#### Windows (with MSVC)

1. **Open Visual Studio Command Prompt:**
   - Search for "Developer Command Prompt for VS 2022"

2. **Navigate and build:**
   ```cmd
   cd C:\Users\HP\Desktop\project\HospitalManagementSystem
   qmake HospitalManagementSystem.pro
   nmake
   .\release\HospitalManagementSystem.exe
   ```

#### macOS

1. **Install Xcode Command Line Tools (if not installed):**
   ```bash
   xcode-select --install
   ```

2. **Navigate to project:**
   ```bash
   cd ~/Desktop/project/HospitalManagementSystem
   ```

3. **Build:**
   ```bash
   qmake HospitalManagementSystem.pro
   make
   ./HospitalManagementSystem.app/Contents/MacOS/HospitalManagementSystem
   ```

#### Linux (Ubuntu/Debian)

1. **Install dependencies:**
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential qt6-base-dev qt6-sql-sqlite
   ```

2. **Build:**
   ```bash
   cd ~/Desktop/project/HospitalManagementSystem
   qmake HospitalManagementSystem.pro
   make
   ./HospitalManagementSystem
   ```

### Building with Qt Creator (Easiest Method)

1. **Open Qt Creator**
2. **File → Open File or Project**
3. **Select**: `HospitalManagementSystem.pro`
4. **Configure Project**:
   - Select your Qt 6 kit (e.g., "Desktop Qt 6.5.0 MinGW 64-bit")
   - Click "Configure Project"
5. **Build**: Press `Ctrl+B` or **Build → Build Project**
6. **Run**: Press `Ctrl+R` or **Build → Run**

### Troubleshooting Build Issues

| Issue | Solution |
|-------|----------|
| **qmake not found** | Add Qt bin directory to PATH: `C:\Qt\6.5.0\mingw_64\bin` |
| **mingw32-make not found** | Run `install_mingw.ps1` or install MinGW separately |
| **Qt modules not found** | Ensure Qt SQL module is installed via Qt Maintenance Tool |
| **C++17 not supported** | Update compiler: MinGW 11.2+, MSVC 2019+, GCC 9+ |
| **SQLite not found** | Qt SQL module includes SQLite; reinstall Qt if missing |
| **Permission denied (Linux/macOS)** | Run: `chmod +x HospitalManagementSystem` |

### Building with Qt Creator

1. Open Qt Creator
2. Click **File → Open File or Project**
3. Select `HospitalManagementSystem.pro`
4. Configure the project with your Qt 6 kit
5. Click **Build → Build Project** (or press Ctrl+B)
6. Click **Build → Run** (or press Ctrl+R) to launch the application

## How to Run

### Quick Start (Recommended)

**Using Qt Creator:**
1. Open Qt Creator
2. Open `HospitalManagementSystem.pro`
3. Press `Ctrl+R` to build and run

### From Command Line

#### Windows (PowerShell)
```powershell
cd C:\Users\HP\Desktop\project\HospitalManagementSystem
qmake HospitalManagementSystem.pro
mingw32-make
.\release\HospitalManagementSystem.exe
```

#### macOS/Linux
```bash
cd ~/Desktop/project/HospitalManagementSystem
qmake HospitalManagementSystem.pro
make
./HospitalManagementSystem
```

### Automated Setup Scripts

**Windows Users:**
- `install_qt.ps1` - Automated Qt 6 installation
- `install_mingw.ps1` - Automated MinGW compiler installation
- `build_run.ps1` - Build and run the application
- `clean_build.ps1` - Clean build (removes build artifacts)

**Usage:**
```powershell
# Run setup scripts
.\install_qt.ps1
.\install_mingw.ps1

# Build and run
.\build_run.ps1

# Clean build
.\clean_build.ps1
```

### Application Usage

1. **Dashboard**: View system statistics and recent activity
2. **Hospitals**: Browse all registered hospitals and their details
3. **Doctors**: View available doctors and their specializations
4. **Register Patient**: 
   - Enter patient details (name, age, location, symptoms)
   - System automatically ranks nearby hospitals
   - Select a hospital and register
   - Doctor is automatically assigned based on symptoms
   - Consultation session is conducted
5. **Patients**: View all registered patients and their status
6. **About**: Information about the application

### First Run

On first run, the application will:
1. Create SQLite database file: `hms_data.db`
2. Initialize database tables (hospitals, doctors, patients, consultations)
3. Populate sample data for demonstration
4. Display the main dashboard

**Note**: Database file is created in the project root directory.

## OOP Concepts Demonstrated

| Concept | Implementation | Location |
|---------|-----------------|----------|
| **Encapsulation** | Private data members with public getters/setters | `domain/person.h`, `domain/doctor.h`, `domain/patient.h` |
| **Inheritance** | `Doctor` and `Patient` inherit from `Person` base class | `domain/person.h`, `domain/doctor.h`, `domain/patient.h` |
| **Polymorphism** | Virtual `displayInfo()` method overridden in derived classes | `domain/person.h`, `domain/doctor.cpp`, `domain/patient.cpp` |
| **Abstraction** | Pure virtual methods in `Person` base class | `domain/person.h` |
| **Composition** | `Hospital` contains multiple `Doctor` objects | `domain/hospital.h`, `domain/hospital.cpp` |
| **Aggregation** | `ConsultationSession` references `Patient` and `Doctor` | `domain/consultationsession.h` |
| **Encapsulation (Data)** | `DatabaseManager` encapsulates all database operations | `database/databasemanager.h` |
| **Abstraction (Algorithm)** | `ProximityEngine` abstracts hospital ranking logic | `database/domain/proximityengine.h` |
| **Single Responsibility** | Each class has one clear purpose | All domain classes |
| **Dependency Injection** | `SystemManager` receives `DatabaseManager` | `manager/systemmanager.h` |

## Project Structure

```
HospitalManagementSystem/
├── domain/                    # Core domain classes
│   ├── person.h              # Abstract base class
│   ├── doctor.h/cpp          # Doctor class (inherits Person)
│   ├── patient.h/cpp         # Patient class (inherits Person)
│   ├── hospital.h/cpp        # Hospital class
│   ├── consultationsession.h/cpp  # Consultation session
│   └── proximityengine.h/cpp  # Proximity matching algorithm
├── database/                  # Database layer
│   ├── databasemanager.h/cpp  # SQLite database operations
│   └── domain/
│       └── proximityengine.h/cpp  # Database-specific proximity engine
├── gui/                       # Qt GUI components
│   ├── mainwindow.h/cpp       # Main application window
│   ├── dashboardpage.h/cpp    # Dashboard page
│   ├── hospitalspage.h/cpp    # Hospitals listing page
│   ├── doctorspage.h/cpp      # Doctors listing page
│   ├── registerpatientpage.h/cpp  # Patient registration page
│   ├── patientspage.h/cpp     # Patients listing page
│   ├── aboutpage.h/cpp        # About page
│   └── consultationresultdialog.h/cpp  # Consultation result dialog
├── manager/                   # Business logic layer
│   └── systemmanager.h/cpp    # System orchestration
├── main.cpp                   # Application entry point
└── HospitalManagementSystem.pro  # Qt project file
```

## Database

The application uses SQLite for data persistence. The database file (`hms_data.db`) is automatically created on first run and contains the following tables:

- **hospitals**: Hospital information and capacity
- **doctors**: Doctor details and availability
- **patients**: Patient information and status
- **consultations**: Consultation session records

## Key Classes

### Domain Layer
- **Person**: Abstract base class for Doctor and Patient
- **Doctor**: Represents a medical professional with specialization
- **Patient**: Represents a patient with symptoms and status
- **Hospital**: Manages doctors and patient registrations
- **ConsultationSession**: Records consultation between patient and doctor
- **ProximityEngine**: Implements hospital ranking algorithm

### Database Layer
- **DatabaseManager**: Handles all SQLite operations

### GUI Layer
- **MainWindow**: Application main window with navigation
- **SystemManager**: Orchestrates business logic and data flow

## Algorithm: Hospital Proximity Matching

The system uses a **Breadth-First Search (BFS)** algorithm to calculate proximity scores:

1. Each hospital has a list of primary sectors
2. For a patient in a given sector, the system calculates the shortest path distance
3. Hospitals are ranked by proximity score (0-100):
   - **95**: Direct match (hospital in patient's sector)
   - **80**: 1 hop away
   - **65**: 2 hops away
   - **52**: 3 hops away
   - **40**: 4+ hops or unreachable

## Troubleshooting

### Build Issues

| Problem | Cause | Solution |
|---------|-------|----------|
| **qmake: command not found** | Qt not in PATH | Add Qt bin to PATH or use full path: `C:\Qt\6.5.0\mingw_64\bin\qmake.exe` |
| **mingw32-make: command not found** | MinGW not installed | Run `install_mingw.ps1` or install MinGW from https://www.mingw-w64.org/ |
| **error: 'QMainWindow' file not found** | Qt headers missing | Reinstall Qt with GUI and Widgets modules |
| **undefined reference to 'sqlite3_...'** | SQLite not linked | Ensure Qt SQL module is installed |
| **C++17 required but not available** | Old compiler | Update: MinGW 11.2+, MSVC 2019+, GCC 9+, Clang 10+ |
| **Project ERROR: Qt version not found** | Qt kit not configured | Configure Qt kit in Qt Creator: Tools → Options → Kits |
| **Multiple definition of 'main'** | Duplicate main.cpp | Ensure only one main.cpp in project |

### Runtime Issues

| Problem | Cause | Solution |
|---------|-------|----------|
| **Database errors / corrupted data** | Corrupted `hms_data.db` | Delete `hms_data.db` and restart application |
| **No doctors available** | Empty database | Restart application to populate sample data |
| **GUI not displaying / blank window** | Missing Qt platform plugin | Ensure Qt platform plugin path is correct |
| **Application crashes on startup** | Missing dependencies | Verify all Qt modules are installed |
| **Cannot connect to database** | File permissions issue | Ensure write permissions in project directory |
| **Proximity matching not working** | Database not initialized | Delete `hms_data.db` and restart |

### Platform-Specific Issues

#### Windows
- **DLL not found errors**: Copy Qt DLLs to executable directory or add Qt bin to PATH
- **Visual C++ Runtime error**: Install Visual C++ Redistributable from Microsoft
- **PowerShell execution policy**: Run `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`

#### macOS
- **Code signature invalid**: Run `codesign --force --deep --sign - HospitalManagementSystem.app`
- **Qt framework not found**: Ensure Qt is installed via official installer or Homebrew
- **Permission denied**: Run `chmod +x HospitalManagementSystem.app/Contents/MacOS/HospitalManagementSystem`

#### Linux
- **libQt6Core.so.6 not found**: Install Qt libraries: `sudo apt-get install qt6-base`
- **Permission denied**: Run `chmod +x HospitalManagementSystem`
- **Display server issues**: Ensure X11 or Wayland is properly configured

### Dependency Verification

**Check Qt Installation:**
```bash
qmake --version
```
Should show: `QMake version 3.1` and `Using Qt version 6.x.x`

**Check Compiler:**
```bash
# Windows MinGW
mingw32-gcc --version

# Windows MSVC
cl.exe

# macOS/Linux
gcc --version
```

**Check SQLite Support:**
```bash
sqlite3 --version
```

### Getting Help

1. Check the troubleshooting table above
2. Review build output for specific error messages
3. Ensure all prerequisites are installed
4. Try clean build: `clean_build.ps1` (Windows) or `make clean && qmake && make` (Unix)
5. Verify file permissions and directory access

## License

This project is developed as an educational demonstration of OOP concepts in C++ with Qt 6.

## Author

Developed as part of the Hospital Management System project at Bahria University.

---

## Appendix: Dependency Reference

### Qt 6 Modules Used

| Module | Purpose | Included |
|--------|---------|----------|
| **Qt Core** | Core functionality, data structures | ✓ Required |
| **Qt GUI** | Graphics, events, window system | ✓ Required |
| **Qt Widgets** | UI components (buttons, dialogs, etc.) | ✓ Required |
| **Qt SQL** | Database connectivity, SQLite support | ✓ Required |

### Compiler Requirements

| Compiler | Minimum Version | Recommended | Platform |
|----------|-----------------|-------------|----------|
| **MSVC** | 2019 (v142) | 2022 (v143) | Windows |
| **MinGW** | 11.2 | 12.0+ | Windows |
| **GCC** | 9.0 | 11.0+ | Linux |
| **Clang** | 10.0 | 14.0+ | macOS/Linux |

### Environment Variables (Optional)

Set these for advanced configuration:

```bash
# Qt installation path
QT_PATH=C:\Qt\6.5.0\mingw_64

# Add to system PATH
PATH=%QT_PATH%\bin;%PATH%

# SQLite database location
HMS_DB_PATH=./hms_data.db
```

### Installation Verification Commands

```bash
# Verify Qt installation
qmake -v

# Verify compiler
gcc --version          # Linux/macOS
cl.exe                 # Windows MSVC
mingw32-gcc --version  # Windows MinGW

# Verify SQLite
sqlite3 --version

# Verify C++ standard support
g++ -std=c++17 --version
```

### Common Qt Installation Paths

| OS | Default Path |
|----|--------------|
| **Windows** | `C:\Qt\6.5.0\mingw_64` or `C:\Qt\6.5.0\msvc2022_64` |
| **macOS** | `/Users/username/Qt/6.5.0/macos` |
| **Linux** | `/opt/Qt/6.5.0/gcc_64` or `/usr/lib/x86_64-linux-gnu/qt6` |

### Useful Links

- **Qt Official**: https://www.qt.io/
- **Qt Download**: https://www.qt.io/download-open-source
- **Qt Documentation**: https://doc.qt.io/qt-6/
- **MinGW**: https://www.mingw-w64.org/
- **SQLite**: https://www.sqlite.org/
- **C++ Standard**: https://en.cppreference.com/w/cpp/17
