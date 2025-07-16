# Process Algorithm Scheduler

A graphical application developed using **GTK+** that simulates various **CPU scheduling algorithms**. This tool provides a user-friendly interface for inputting process details and visualizing the scheduling sequence, completion times, and average waiting times. Ideal for educational purposes, it helps users understand and compare the behavior of different CPU scheduling techniques.

---

## 📚 Table of Contents

- [Features](#features)
- [Algorithms Implemented](#algorithms-implemented)
- [Project Structure](#project-structure)
- [Dependencies](#dependencies)
- [How to Run](#how-to-run)
- [Screenshots](#screenshots)
- [Contributing](#contributing)
- [License](#license)
- [Authors](#authors)

---

## 🚀 Features

- **Interactive GUI**: Built with GTK+ for an intuitive user experience.
- **Process Input**: Enter process ID, arrival time, burst time, and priority.
- **Multiple Scheduling Algorithms**: Simulates a variety of popular CPU scheduling techniques.
- **Result Visualization**: Shows scheduling order, completion times, and average waiting time for each algorithm.
- **Process Management**: Add new processes and reset all data easily.

---

## ⚙️ Algorithms Implemented

- **Shortest Job Next (SJN)**: Selects the process with the shortest burst time.
- **Longest Job First (LJF)**: Selects the process with the longest burst time.
- **First Come First Serve (FCFS)**: Processes are scheduled in their arrival order.
- **Priority Scheduling**: Processes are scheduled based on assigned priorities (higher priority runs first).
- **Round Robin (RR)**: Each process is given a fixed time slice in a rotating fashion.

---

## 📁 Project Structure

- **Process Definition**: `Process` struct with fields: process ID, arrival time, burst time, priority, completion time, and waiting time.
- **Global Variables**: Arrays for process storage, process count, and GTK widgets.
- **Utility Functions**: Includes functions to reset data, compute average waiting time, and display results.
- **Scheduling Algorithms**: Individual implementations for each scheduling method.
- **GTK Callbacks**: Handles GUI interactions like button clicks.
- **Main Function**: Initializes the GTK+ application and connects components.

---

## 🧰 Dependencies

Make sure the following are installed before running the project:

- [GTK+](https://www.gtk.org/)
- [GLib](https://developer.gnome.org/glib/)

### Installation

#### On **Debian/Ubuntu**:
```bash
sudo apt-get update
sudo apt-get install libgtk-3-dev
```

#### On **Fedora**:
```bash
sudo dnf install gtk3-devel
```

#### On **macOS (with Homebrew)**:
```bash
brew install gtk+3
```

#### On **Windows (MSYS2/MinGW-w64)**:
Follow the GTK+ installation guide for MSYS2:
https://www.gtk.org/docs/installations/windows/

---

## 🛠️ How to Run

### Step 1: Clone the Repository
```bash
git clone https://github.com/your-username/process-algorithm-scheduler.git
cd process-algorithm-scheduler
```

### Step 2: Compile the Source Code
Use `gcc` or `clang` with GTK+ flags:
```bash
gcc -o scheduler main.c $(pkg-config --cflags --libs gtk+-3.0)
```

Replace `main.c` with your actual source file if different.

### Step 3: Run the Application
```bash
./scheduler
```

---

## 🤝 Contributing

We welcome contributions!

1. **Fork** the repo  
2. **Create** a new branch: `git checkout -b feature/YourFeature`  
3. **Commit** your changes: `git commit -m 'Add some feature'`  
4. **Push** to the branch: `git push origin feature/YourFeature`  
5. **Open** a Pull Request

---

