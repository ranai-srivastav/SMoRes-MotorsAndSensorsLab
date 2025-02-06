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
import serial
import re

UNITS=["Ohms","cms","Lux","inches","NONE"]

#establishing serial connection
arduino = serial.Serial(port='COM3',   baudrate=9600, timeout=.1)
arduino.set_buffer_size(rx_size = 10, tx_size = 10)

#READ LATEST SERIAL SENSOR DATA INPUTS
def read_state_sensor():
    global state, curr_sensor_read, new_data
    #arduino.flush()
    #arduino.flushInput()
    #arduino.flushOutput()
    #sleep(0.5)
    if arduino.in_waiting > 0:
        data_raw = arduino.readline()
        data_read = data_raw.decode()
        pattern = r"^\d{1},\d{1,}$"
        if re.search(pattern, data_read):
            #print(data_read)
            state = int(data_read.split(',')[0])
            curr_sensor_read = int(data_read.split(',')[1])
            new_data = True
    # Scheduling update at 100ms
    win.after(10, read_state_sensor)

#Send command to rotate motors
def rotate_motors():
    servo_val = int(Servo_motor.get())
    stepper_val = int(stepper_motor.get())
    DC_Pos_val = int(DC_motor_Pos.get())
    DC_Vel_val = int(DC_motor_Vel.get())
    if DC_Vel_val!=0:
        DC_Vel_val = ((DC_Vel_val/360)*100)+100
    print(rev_mtr.get())
    if rev_mtr.get()=="0":
        DC = DC_Vel_val*-1
    else:
        DC = DC_Vel_val
    arduino.write(bytes("rp"+str(servo_val),  'utf-8'))
    sleep(1.5)
    arduino.write(bytes("ss"+str(stepper_val),  'utf-8'))
    sleep(1.5)
    arduino.write(bytes("dp"+str(DC),  'utf-8'))
    sleep(0.5)
    return
    
#Bottom Buttons-----------------------------------------
def aboutMsg():
    messagebox.showinfo("About",
    "Sensor and Motor Controls Lab - Task 3\n\n Team G-SMoRes \n\n February 2025")

def quitApp():
    plt.close('all')
    arduino.close()
    win.destroy()
#------------------------------------------------------------------

# GUI design -------------------------------------------------------------
win = tk.Tk()
win.title("Team G - SMoRes")
win.minsize(350,550)

#TODO:Change plot types and axis based on state change
# Create a Matplotlib figure
fig, ax = plt.subplots()
line = ax.plot([])
ax.set_xlim(0, 1000)
ax.set_ylim(0, 1000)

# Embed the plot in the Tkinter window
canvas = FigureCanvasTkAgg(fig, master=win)
canvas.get_tk_widget().grid(column=2, row=10)

# Example sensor data
y_data = []
count = 0
state = 0
prev_state = 0
curr_sensor_read = 0
new_data = False

def update_graph():
    global x_data, y_data, count, curr_sensor_read, state, prev_state,new_data
    if state != prev_state:
        y_data = []
        prev_state = state
        unit.delete('1.0', tk.END)
        unit.insert(tk.END, UNITS[int(state)])
        if state==4:
            initiate_rotation_Btn.config(state=tk.NORMAL)
        else:
            initiate_rotation_Btn.config(state=tk.DISABLED)
        
    if new_data==True:
        #sensor_read.delete(tk.START,tk.END)
        sensor_read.delete('1.0', tk.END)
        current_state.delete('1.0', tk.END)
        sensor_read.insert(tk.END, str(curr_sensor_read))
        current_state.insert(tk.END, str(state))
        y_data.append(curr_sensor_read)
        new_data=False
        count = count+1

    # Limiting data to 100 points
    y_data = y_data[-1000:]

    #line.set_data(y_data)
    if count==10:
        ax.cla()
        ax.plot(y_data)
        ax.relim()
        ax.autoscale_view()
        ax.set_ylabel(UNITS[int(state)])
        canvas.draw()
        count=0

    # Scheduling update at 10ms
    win.after(10, update_graph)

#TODO: Ensure Scale ranges are correct
#servo motor scale
Servo_motor = tk.Scale(win, bd=5,sliderlength=15, from_=0, to=180, orient=tk.HORIZONTAL)
Servo_motor.grid(column=3, row=1)
tk.Label(win, text="Servo Motor",font=("Arial", 26, "bold"), fg="blue").grid(column=1, row=1)

#stepper motor scale
stepper_motor = tk.Scale(win, bd=5,sliderlength=15, from_=-100, to=100, orient=tk.HORIZONTAL)
stepper_motor.grid(column=3, row=2)
tk.Label(win, text="Stepper Motor",font=("Arial", 26, "bold"), fg="blue").grid(column=1, row=2)

#DC motor velocity scale
DC_motor_Vel = tk.Scale(win, bd=5,sliderlength=15, from_=0, to=360, orient=tk.HORIZONTAL)
DC_motor_Vel.grid(column=3, row=3)
tk.Label(win, text="DC Motor Vel",font=("Arial", 26, "bold"), fg="blue").grid(column=1, row=3)

#DC motor Position scale
DC_motor_Pos = tk.Scale(win, bd=5,sliderlength=15, from_=0, to=100, orient=tk.HORIZONTAL)
DC_motor_Pos.grid(column=3, row=4)
tk.Label(win, text="DC Motor Pos",font=("Arial", 26, "bold"), fg="blue").grid(column=1, row=4)

#DC motor rotation direction button
rev_mtr = tk.StringVar(win, "1")
tk.Label(win, text="DC Motor Rotation",font=("Arial", 18, "bold")).grid(column=1, row=5)
tk.Radiobutton(win, text = "Clockwise", variable = rev_mtr, 
                value = "1", indicator = 0,
                background = "light blue").grid(column=2, row=5)
tk.Radiobutton(win, text = "Counter-Clockwise", variable = rev_mtr, 
                value = "0", indicator = 0,
                background = "light blue").grid(column=3, row=5)

#TODO: update based on serial input
#state value read from serial input
current_state = tk.Text(win, bd=6, height = 1, width = 2)
current_state.grid(column=2, row=7)
current_state.insert(tk.END, "0")
tk.Label(win, text="State",font=("Arial", 18, "bold")).grid(column=1, row=7)

#TODO: update based on serial input
#Current sensor intensity value read from serial input
#sensor_read = tk.Entry(win, bd=6, width=5)
sensor_read = tk.Text(win, bd=6, height = 1, width = 4)
sensor_read.grid(column=2, row=8)
sensor_read.insert(tk.END, "0")
tk.Label(win, text="Sensor Reading",font=("Arial", 18, "bold")).grid(column=1, row=8)
unit = tk.Text(win, bd=6, height = 1, width = 6)
unit.grid(column=3, row=8)
unit.insert(tk.END, UNITS[0])

#TODO: Ensure the button only shows up when state is 5
#Button to initiate motor rotation by sending serial command
initiate_rotation_Btn = tk.Button(win, bd=5, bg='#89CFF0', text="Rotate", command=rotate_motors)
initiate_rotation_Btn.grid(column=2, row=14)
if state!=4:
    initiate_rotation_Btn.config(state=tk.DISABLED)
else:
    initiate_rotation_Btn.config(state=tk.NORMAL)

aboutBtn = tk.Button(win, bg='yellow', text="About", command=aboutMsg)
aboutBtn.grid(column=1, row=14)

quitBtn = tk.Button(win, bg='red', text="QUIT", command=quitApp)
quitBtn.grid(column=3, row=14)

#update the plot in every loop
read_state_sensor()
update_graph()


win.mainloop()