import sys, re
import numpy
import skimage.measure
from scipy.ndimage import gaussian_filter
import cv2

def endOfRow(index, number_of_columns):
	if index % number_of_columns == (number_of_columns - 1):
		return True
	else:
		return False
def writeVertex(fileobject, vertex):
	fileobject.write("v {} {} {}\n".format(vertex[0], vertex[1], vertex[2]))
def writeFace(fileobject, face):
	fileobject.write("f {} {} {}\n".format(face[0], face[1], face[2]))
def writeUV(fileobject, uv):
	fileobject.write("u {} {}\n".format(uv[0], uv[1]))
	
def surface(image, filename, scale =[1,1,1], reverse=False):

	f = open(filename, 'w')
	ncol = image.shape[1]
	connections = numpy.array(([0,ncol,1], [1,ncol, ncol+1]), ndmin=2, dtype=int)
	
	vertices = numpy.empty((image.size, 3))
	vertices[:,0] = numpy.tile(numpy.array(range(image.shape[1])), image.shape[0]) * scale[0]
	vertices[:,1] = numpy.tile(numpy.array(range(image.shape[0])), (image.shape[1], 1)).transpose().reshape(-1,) * scale[1]
	vertices[:,2] = (image.reshape(-1,) - numpy.amin(image)) * scale[2]
	if reverse:
		vertices[:,2] = numpy.amax(vertices[:,2]) - vertices[:,2]
		
	print("writing vertices")
	for v in range(vertices.shape[0]):
		writeVertex(f, vertices[v,:])

	index = numpy.empty((vertices.shape[0]*2,3), dtype=int)
	iterations = vertices.shape[0] - ncol - 1

	count = 0
	for i in range(iterations):
		if not endOfRow(i, ncol):
			index[count*2:count*2+2,:] = i+connections
			latest_triangles = (i + connections)
			writeFace(f, latest_triangles[0,:])
			writeFace(f, latest_triangles[1, :])
			count+=1
	index = index[:count, :] 


	print("writing uvs")
	max_x = numpy.amax(vertices[:,0])
	max_y = numpy.amax(vertices[:,1])
	for u in range(vertices.shape[0]):
		writeUV(f, [(vertices[u,0])/max_x, (vertices[u,1])/max_y])
	f.close()
	
	return vertices, index

def writeObj(vertex, index, filename):
    f = open(filename, 'w')
    for i in range(vertex.shape[0]):
        f.write("v {} {} {}\n".format(vertex[i,0], vertex[i,1], vertex[i,2]))
    for j in range(index.shape[0]):
        f.write("f {} {} {}\n".format(index[j,0], index[j,1], index[j,2]))
	
    f.close()



image_path = sys.argv[1]
scale = numpy.array([float(s) for s in re.findall(r'-?\d+\.?\d*', sys.argv[2])])

image_file = cv2.imread(image_path)
gray_image = cv2.cvtColor(image_file, cv2.COLOR_BGR2GRAY)

img = numpy.asarray(gray_image, dtype=float)

filtered_img = img #skimage.measure.block_reduce(img, (5,5), numpy.max)


print(img.shape)
cv2.imwrite("Content\\texture.jpg", image_file)

vertices, index = surface(filtered_img, 'Content\\image_mesh.obj', scale,reverse=True)
# sparsify(vertices, index, 3)