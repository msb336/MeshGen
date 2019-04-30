import numpy
from numpy.random import randint as rand
import pptk
import collections

import matplotlib.pyplot as pyplot
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import


def voxelToPoint(grid):
    wall = numpy.zeros((grid.size, 3))
    space = numpy.zeros((grid.size, 3))
    #print(grid.shape)
    count = 0
    for i in range(grid.shape[0]):
        for j in range(grid.shape[1]):
            for k in range(grid.shape[2]):
                wall[count,:] = numpy.array((i,j,k))
                if grid[i,j,k]:
                    space[count,:] = numpy.array((255,0,0))
                else:
                    space[count,:] = numpy.array((0,255,0))
                count += 1
    return wall, space
def appendVerticesIfUnique(center_point, vertices):
    d = 0.5
    points = center_point + numpy.array(([-d,-d,-d], [-d, d, -d], [d, d, -d], [d, -d, -d],
                                         [-d,-d,d], [-d, d, d], [d, d, d], [d, -d, d]), ndmin=2)

    vertices = numpy.append(vertices, points, axis=0)
    return vertices
def appendIfUnique(new_values, array):
	index_to_add = new_values
	# print("checking if unique. index size {}".format(array.shape[0]))
	# for i in range(array.shape[0]):
		# for row in range(new_values.shape[0]):
			# value = new_values[row,:]
			# if collections.Counter(value.reshape(3,)) == collections.Counter(array[i,:].reshape(3,)):
				# index_to_add = np.delete(index_to_add, row, 0)
	array = numpy.append(array, index_to_add, axis=0)
	return array
		
def voxelToMesh(grid):
    cube_connections = numpy.array(([0,1,2], [0,2,3],   # Bottom
                                    [0,3,7], [0,4,7],   # Front
                                    [4,5,7], [5,6,7],   # Top
                                    [1,2,6], [1,5,6],   # Back
                                    [0,1,5], [0,4,5],   # Left
                                    [3,2,6], [3,7,6]    # Right
                                    ), ndmin=2, dtype=int)
    # cube_normals = numpy.array(([0,0,-1], [0,0,-1],
				# [1,0,0], [1,0,0],
				# [0,0,1], [0,0,1],
				# [-1,0,0], [-1,0,0],
				# [0,-1,0], [0,-1,0],
				# [0,1,0], [0,1,0]), ndmin=2)

    vertices = numpy.empty((0,3))
    index = numpy.empty((0,3), dtype=int)
    normals= numpy.empty((0,3))
    for i in range(grid.shape[0]):
        for j in range(grid.shape[1]):
            for k in range(grid.shape[2]):
                if not grid[i,j,k]:
                    vertices = appendVerticesIfUnique(numpy.array([i,j,k], ndmin=2), vertices)
                    index = appendIfUnique(vertices.shape[0]+cube_connections, index)
    return vertices, index

def writeObj(vertex, index, normals, filename):
    f = open(filename, 'w')
    print(index.shape)
    for i in range(vertex.shape[0]):
        f.write("v {} {} {}\n".format(vertex[i,0], vertex[i,1], vertex[i,2]))
    for j in range(index.shape[0]):
        f.write("f {} {} {}\n".format(index[j,0], index[j,1], index[j,2]))

    f.close()                    


def generateMaze(width=81, height=51, depth=21, complexity=.75, density=.9):
    # Only odd shapes
    shape = ((height // 2) * 2 + 1, (width // 2) * 2 + 1,  (depth // 2) * 2 + 1)
    # Adjust complexity and density relative to maze size
    complexity = int(complexity * (5 * (shape[0] + shape[1] + shape[2]))) # number of components
    density    = int(density * ((shape[0] // 2) * (shape[1] // 2) * (shape[2] // 2))) # size of components
    # Build actual maze
    Z = numpy.zeros(shape, dtype=bool)
    # Fill borders
    Z[0,:,:] = Z[-1,:,:] = 1
    Z[:,0,:] = Z[:,-1,:] = 1
    Z[:,:,0] = Z[:,:,-1] = 1

    # Make aisles
    for i in range(density):
        x, y, z = rand(0, shape[1] // 2) * 2, rand(0, shape[0] // 2) * 2,  rand(0, shape[2] //2) * 2 # pick a random position
        Z[y, x, z] = 1
        for j in range(complexity):
            neighbours = []
            if x > 1:             neighbours.append((y, x - 2, z))
            if x < shape[1] - 2:  neighbours.append((y, x + 2, z))
            if y > 1:             neighbours.append((y - 2, x, z))
            if y < shape[0] - 2:  neighbours.append((y + 2, x, z))
            if z > 1:             neighbours.append((y, x, z - 2))
            if z < shape[2] - 2:  neighbours.append((y, x, z + 2))
            if len(neighbours):
                y_,x_, z_ = neighbours[rand(0, len(neighbours) - 1)]
                if Z[y_, x_, z_] == 0:
                    Z[y_, x_, z_] = 1
                    Z[y_ + (y - y_) // 2, x_ + (x - x_) // 2, z_ + (z - z_) // 2] = 1
                    x, y, z = x_, y_, z_
    return Z

maze = generateMaze(20,10,10, 0.2, 0.99)
vertices, index = voxelToMesh(maze)

scale = 300
writeObj(vertices*scale, index, normals, "maze.obj")
