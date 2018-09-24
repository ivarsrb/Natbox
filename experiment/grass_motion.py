'''
Simulation of grass skeleton movement
'''
from tkinter import Canvas, Tk
from datetime import datetime
import numpy as np
import time

# Classes
###############################################################
def normalize(vector):
    if(np.linalg.norm(vector) == 0):
        return np.array([0,0])
    else:
        return vector / np.linalg.norm(vector)

'''
Blade consists of edges x and blade vectors --->
x----------------->x------>x------->x
edge can be thought of as a section of blade going to that edge:  -------------->x
each edge holds mass / stiffness of this section and it rotates around previous edge (except first one)
'''
class Edge:
    # each edge has a blade that goes from previous edge to this, except first one
    def __init__(self, xpos, ypos):
        # base position
        self.stat_pos = np.array([xpos,ypos])
        # current position
        self.pos = self.stat_pos.copy()
        # mass of a blade
        self.mass = 0.0001
        # stiffness of a blade
        #self.stiffness = 1.0
        # angular velocity
        self.ang_velocity = 0.0
    # get a vector of blade direction (from prev edge to this edge) This is also origin of this blade
    def get_blade_vector_static(self, prev_edge):
        return self.stat_pos - prev_edge.stat_pos 

    def get_blade_vector_cur(self, prev_edge):
        return self.pos - prev_edge.pos 

    # get normalized vector perpendicular to blade direction 
    def get_blade_normal_static(self, prev_edge):
        blade_vec = self.get_blade_vector_static(prev_edge)
        return normalize(np.array([blade_vec[1], -blade_vec[0]]))

    def get_blade_normal_cur(self, prev_edge):
        blade_vec = self.get_blade_vector_cur(prev_edge)
        return normalize(np.array([blade_vec[1], -blade_vec[0]]))

    # get angle by how much current position is displaced compared to static position
    def get_angular_displacement(self, prev_edge):
        return np.arccos(np.vdot(self.get_blade_vector_static(prev_edge),self.get_blade_vector_cur(prev_edge)) / 
                                 (np.linalg.norm(self.get_blade_vector_static(prev_edge)) * np.linalg.norm(self.get_blade_vector_cur(prev_edge))) )
    # get xangle of current blade rotated positionprev_edge.pos[1]
    def get_xangle(self, prev_edge):
        return np.arccos(np.vdot(normalize(self.get_blade_vector_cur(prev_edge)) ,np.array([1.0,0.0])))

    # get length of a blade from this to previous edge
    def get_blade_length(self, prev_edge):
        return np.linalg.norm(self.get_blade_vector_static(prev_edge))

    # rotate this edge around previous edge by additional angle
    def rotate_by_angle_delta(self, prev_edge, angle):
        blade_length = self.get_blade_length(prev_edge)
        # first find already existing angle of blade, then add givendelta
        full_angle = self.get_xangle(prev_edge) + angle
        self.pos[0] = prev_edge.pos[0] + np.cos(full_angle) * blade_length
        self.pos[1] = prev_edge.pos[1] + np.sin(full_angle) * blade_length
        #self.pos[0] = np.cos(full_angle) * blade_length
        #self.pos[1] = np.sin(full_angle) * blade_length


class Blade:
    def __init__(self, xpos, ypos, edge_list):
        self.pos = np.array([xpos,ypos])
        self.edge_list = edge_list
    # return list of edge points for rendering
    def get_edge_points(self, zoom, origin, yswap):
        point_list = []
        for edge in self.edge_list:
            # adjust iven actual coordinates to display on this canvas
            point_list.append(origin[0] + edge.pos[0]*zoom)
            point_list.append(yswap - (origin[1] + edge.pos[1]*zoom))
        return point_list

class Wind:
    def __init__(self):
        self.velocity = np.array([1.0, 0.0])
        self.intensity = 0.0
        self.on = True
    def update(self, dt):
        if self.on:
            change_koef = 500
            self.intensity += change_koef * dt
            self.velocity[0] = (np.sin(self.intensity) + 1) / 2.0
    def turn_on(self):
        self.on = True
        self.intensity = 0
    def turn_off(self):
        self.on = False
        self.velocity = np.array([0.0, 0.0])
    def switch(self):
        self.velocity[0] *= -1.0 
###############################################################
# screen
width = 800.0
height = 800.0
# setup
###############################################################
tk = Tk()
canvas = Canvas(tk, width=width, height=height)
tk.title("Grass motion")
canvas.pack()
# Initialization
###############################################################
blade = Blade(0.0,0.0, [Edge(0.0,0.0), 
                        Edge(0.0,1.0),
                        Edge(0.0,2.0),
                        Edge(0.0,3.0),
                        #Edge(0.5,1.5),
                        #Edge(0.0,3.0),

                        ])
wind = Wind()

# actual blade
blade_origin = [width/2.0, 0.0]
blade_zoom = 100.0
blade_line = canvas.create_line([0,0,0,0], width=6, arrow='last', fill='green')
# forces for debugging
forces_origin = [500.0,500.0]
forces_zoom = 100.0
windf_line = canvas.create_line([0,0,0,0], width=5, fill='blue', arrow='last')
restf_line = canvas.create_line([0,0,0,0], width=5, fill='red', arrow='last')
dampf_line = canvas.create_line([0,0,0,0], width=5, fill='orange', arrow='last')
fullf_line = canvas.create_line([0,0,0,0], width=5, fill='yellow', arrow='last')


wind_origin = [150.0,width-100]
wind_zoom = 100.0
wind_vector = canvas.create_line([0,0,0,0], width=3, fill='blue', arrow='last')

# events
###############################################################
# mouse
def callback(event):
    print ("clicked at", event.x, event.y)
    #canvas.coords(line, (event.x, event.y,event.x+100, event.y+100))
# key events
def key(event):
    #print( "pressed", repr(event.char))
    if event.char == 'a':
        wind.turn_off()
    if event.char == 's':
        wind.switch() 

canvas.bind("<Button-1>", callback)
canvas.focus_set()
canvas.bind("<Key>", key)
# time based movement
current_time = datetime.now().microsecond
while True:
    time.sleep(0.01)
    prev_time = current_time
    current_time = datetime.now().microsecond
    dt = 1.0 / (current_time - prev_time)
    # Update
    #####################################
    # bending motion
    # for each edge except the first
    for i in range(1, len(blade.edge_list)):
        this_edge =  blade.edge_list[i]
        prev_edge = blade.edge_list[i-1]
        # Wb = S*omega*(wind_vel*En)*En
        wind_force = 1 * np.vdot(wind.velocity, this_edge.get_blade_normal_cur(prev_edge)) * this_edge.get_blade_normal_cur(prev_edge) 
        # Rb = Ktip * delta_fi * ((Ee_stat - Ee_cur) / |Ee_stat - rest_force[0]Ee_cur|)
        rest_force = 5 * this_edge.get_angular_displacement(prev_edge) * normalize(this_edge.get_blade_vector_static(prev_edge) - this_edge.get_blade_vector_cur(prev_edge))
        # D = -c*ang_vel
        # velocity should be vector
        dampening_force = 0.01 * this_edge.ang_velocity

        total_force = wind_force + rest_force #+ dampening_force
        # torque (angular force)
        # N = Ee x F
        torque = np.cross(this_edge.get_blade_vector_cur(prev_edge), total_force)
        #print(torque)
        # moment of inertia
        # I = mr^2
        inertia = this_edge.mass * this_edge.get_blade_length(prev_edge) ** 2
        # angular acceleration
        # a = N / I
        ang_accel = torque / inertia
        # angular velocity
        this_edge.ang_velocity += ang_accel * dt
        #print(ang_accel )
        # rotation angle
        delta_etha = this_edge.ang_velocity * dt
        #print("vel ",this_edge.ang_velocity, ", etha ", delta_etha )
        #print(i)
        this_edge.rotate_by_angle_delta(prev_edge, delta_etha)
       
        #print("torque ", torque, ", abg accel ", ang_accel, ", velo ", this_edge.ang_velocity, ",  delta etha ",delta_etha , " , dt ", dt)

        # forces for debugging
        canvas.coords(windf_line,[ forces_origin[0], width - forces_origin[1],  forces_origin[0] + forces_zoom*wind_force[0], width - (forces_origin[1] + forces_zoom*wind_force[1])] )
        canvas.coords(restf_line,[ forces_origin[0], width - forces_origin[1],  forces_origin[0] + forces_zoom*rest_force[0], width - (forces_origin[1] + forces_zoom*rest_force[1])] )
        #canvas.coords(dampf_line,[ forces_origin[0], width - forces_origin[1],  forces_origin[0] + forces_zoom*dampening_force[0], width - (forces_origin[1] + forces_zoom*dampening_force[1])] )
        #canvas.coords(fullf_line,[ origin[0], origin[1],  origin[0] + magnify*total_force[0], origin[1] + magnify*total_force[1]] )
        

    canvas.coords(blade_line, blade.get_edge_points(blade_zoom, blade_origin, height)) 
    tk.update()
    # wind visualization and update
    #wind.update(dt)
    canvas.coords(wind_vector, [ wind_origin[0], width - wind_origin[1],  wind_origin[0] + wind_zoom*wind.velocity[0], width - (wind_origin[1] + wind_zoom*wind.velocity[1]) ])
    
tk.mainloop()
