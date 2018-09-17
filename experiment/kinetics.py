'''
Kinematics + kinetics test
'''
from tkinter import Canvas, Tk
from datetime import datetime
import numpy as np

# screen
width = 800
height = 800

# section consists of bone (line) and sprig joint to which the bone is attached, joints have spring stifness and angle
class Section:
    def __init__(self, posx, posy, xangle, stiffness, length, mass):
        # joint position
        self.posx = posx
        self.posy = posy
        # bone angle to x-axis in static position around joint
        self.base_xangle = xangle
        # bone can currently be turned in different direction than base by some outside force
        self.curent_xangle = self.base_xangle 
        # length of a bone
        self.length = length
        # stiffness of joint spring
        self.stiffness = stiffness
        # mass of bone
        self.mass = mass
        # rotation movement of joint (radii per sec)
        self.velocity = 0
        # change in velocity
        self.acceleration = 0

    # return beginning (joint side) and ending points of a section in current xangle orientation
    def get_endpoints(self):
        endx = self.posx + np.cos(self.curent_xangle) * self.length
        endy = self.posy + np.sin(self.curent_xangle) * self.length
        return [self.posx, self.posy, endx, endy]

    # return force vector that holds bone in its given place in terms of joint bending
    def get_force(self):
        forcex = np.cos(self.base_xangle) * self.stiffness
        forcey = np.sin(self.base_xangle) * self.stiffness
        return [forcex, forcey]

    # rotate about joint by given velocity in given time interval
    def rotate(self, dt):
        # s = v * t
        self.curent_xangle += self.velocity * dt

# setup
###############################################################
tk = Tk()
canvas = Canvas(tk, width=width, height=height)
tk.title("Kinetics")
canvas.pack()

###############################################################
# sections
section1 = Section(width / 2, 0, np.pi / 2, 0.3, 100, 22.5)
line = canvas.create_line(section1.get_endpoints(), width=5)
#foeces
wind_force = np.array([0.3, 0.0])

###############################################################
# events
# mouse
def callback(event):
    print ("clicked at", event.x, event.y)
    #canvas.coords(line, (event.x, event.y,event.x+100, event.y+100))
# key events
def key(event):
    print( "pressed", repr(event.char))
    if event.char == 'a':
        wind_force[0] = 0.0
canvas.bind("<Button-1>", callback)
canvas.focus_set()
canvas.bind("<Key>", key)
# time based movement
current_time = datetime.now().microsecond
while True:
    prev_time = current_time
    current_time = datetime.now().microsecond
    dt = 1.0 / (current_time - prev_time)
    #####################################
    resultant_force = np.array(section1.get_force()) + wind_force
    force_xangle = np.arccos(resultant_force[0]/np.sqrt(resultant_force.dot(resultant_force)))
    #section1.curent_xangle = force_xangle

    # a = F/m
    section1.acceleration = (force_xangle - section1.curent_xangle) / section1.mass
    # rate at which velocity is changing
    section1.velocity += section1.acceleration * dt
    section1.rotate(dt)
    canvas.coords(line, section1.get_endpoints())
    tk.update()

tk.mainloop()