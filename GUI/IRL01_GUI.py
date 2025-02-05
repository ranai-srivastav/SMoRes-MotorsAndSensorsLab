'''
CMU Sensors and Motors Lab
MRSD 2026
Team-G SMoRes
'''

import tkinter as tk
from tkinter import messagebox
from time import sleep
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

#Following linraries are only necessary to control audrino from laptop
from turtle import speed
from pyfirmata import ArduinoNano, SERVO

#-------------------------------------------------------------------------
#Example function for Servo motor control on audrino from laptop
#TODO: enable serial input and integrate data read
board = None #ArduinoNano('COM3')
pin = 8
#board.digital[pin].mode = SERVO
def CCW():
    return
    board.digital[3].write(1)
    board.digital[5].write(0)
    endAngle = int(rotationAngle.get())
    speed = int(rotationSpeed.get())
    delay = delaySelect(speed)
    for angle in range(0, endAngle):
        board.digital[pin].write(angle)
        sleep(delay)
    board.digital[3].write(0)
def delaySelect(speed):
    match speed:
        case 1: delay = 0.12
        case 2: delay = 0.09
        case 3: delay = 0.06
        case 4: delay = 0.03
        case 5: delay = 0.01
    return delay
#------------------------------------------------------------------

#Bottom Buttons-----------------------------------------
def aboutMsg():
    messagebox.showinfo("About",
    "Sensor and Motor Controls Lab - Task 3\n\n Team G-SMoRes \n\n February 2025")

def quitApp():
    board.exit()
    win.destroy()
#------------------------------------------------------------------

# GUI design -------------------------------------------------------------
win = tk.Tk()
win.title("Team G - SMoRes")
win.minsize(350,550)

#TODO:Change plot types and axis based on state change
# Create a Matplotlib figure
fig, ax = plt.subplots()
line, = ax.plot([], [])
ax.set_xlim(0, 200)
ax.set_ylim(0, 100)

# Embed the plot in the Tkinter window
canvas = FigureCanvasTkAgg(fig, master=win)
canvas.get_tk_widget().grid(column=2, row=10)

# Example sensor data
x_data = []
y_data = []
count = 0

def update_graph():
    global x_data, y_data, count

    #Simulaating read from serial input
    new_x = count
    new_y = np.random.randint(0, 100)
    count = count+1

    x_data.append(new_x)
    y_data.append(new_y)

    # Limiting data to 100 points
    x_data = x_data[-100:]
    y_data = y_data[-100:]

    line.set_data(x_data, y_data)
    ax.relim()
    ax.autoscale_view()
    canvas.draw()

    # Scheduling update at 100ms
    win.after(100, update_graph)

#TODO: Ensure Scale ranges are correct
#servo motor scale
Servo_motor = tk.Scale(win, bd=5,sliderlength=15, from_=0, to=360, orient=tk.HORIZONTAL)
Servo_motor.grid(column=3, row=1)
tk.Label(win, text="Servo Motor",font=("Arial", 18, "bold"), fg="blue").grid(column=1, row=1)

#stepper motor scale
stepper_motor = tk.Scale(win, bd=5,sliderlength=15, from_=-360, to=360, orient=tk.HORIZONTAL)
stepper_motor.grid(column=3, row=2)
tk.Label(win, text="Stepper Motor",font=("Arial", 18, "bold"), fg="blue").grid(column=1, row=2)

#DC motor velocity scale
DC_motor_Vel = tk.Scale(win, bd=5,sliderlength=15, from_=0, to=255, orient=tk.HORIZONTAL)
DC_motor_Vel.grid(column=3, row=3)
tk.Label(win, text="DC Motor Vel",font=("Arial", 18, "bold"), fg="blue").grid(column=1, row=3)

#DC motor Position scale
DC_motor_Pos = tk.Scale(win, bd=5,sliderlength=15, from_=0, to=255, orient=tk.HORIZONTAL)
DC_motor_Pos.grid(column=3, row=4)
tk.Label(win, text="DC Motor Pos",font=("Arial", 18, "bold"), fg="blue").grid(column=1, row=4)

#DC motor rotation direction button
rev_mtr = tk.StringVar(win, "1")
tk.Label(win, text="DC Motor Rotation",font=("Arial", 10, "bold")).grid(column=1, row=5)
tk.Radiobutton(win, text = "Clockwise", variable = rev_mtr, 
                value = "1", indicator = 0,
                background = "light blue").grid(column=2, row=5)
tk.Radiobutton(win, text = "Counter-Clockwise", variable = rev_mtr, 
                value = "0", indicator = 0,
                background = "light blue").grid(column=3, row=5)

#TODO: update based on serial input
#state value read from serial input
current_state = tk.Entry(win, bd=6, width=2)
current_state.grid(column=2, row=7)
current_state.insert(0, "1")
tk.Label(win, text="State",font=("Arial", 10, "bold")).grid(column=1, row=7)

#TODO: update based on serial input
#Current sensor intensity value read from serial input
sensor_read = tk.Entry(win, bd=6, width=2)
sensor_read.grid(column=2, row=8)
sensor_read.insert(0, "52")
tk.Label(win, text="Sensor Reading",font=("Arial", 10, "bold")).grid(column=1, row=8)

#TODO: Ensure the button only shows up when state is 5
#Button to initiate motor rotation by sending serial command
initiate_rotation_Btn = tk.Button(win, bd=5, bg='#89CFF0', text="Rotate", command=CCW)
initiate_rotation_Btn.grid(column=2, row=14)


aboutBtn = tk.Button(win, bg='yellow', text="About", command=aboutMsg)
aboutBtn.grid(column=1, row=14)

quitBtn = tk.Button(win, bg='red', text="QUIT", command=quitApp)
quitBtn.grid(column=3, row=14)

#update the plot in every loop
update_graph()

win.mainloop()