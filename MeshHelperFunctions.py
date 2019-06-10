import numpy, cv2
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
def writeColor(fileobject, color):
    fileobject.write("c {} {} {} {}\n".format(color[0], color[1], color[2], color[3]))
    
def imageToXYZ(image, scale):
    vertices = numpy.empty((image.size, 3))
    vertices[:,0] = numpy.tile(numpy.array(range(image.shape[1])), image.shape[0]) * scale[0]
    vertices[:,1] = numpy.tile(numpy.array(range(image.shape[0])), (image.shape[1], 1)).transpose().reshape(-1,) * scale[1]
    vertices[:,2] = (image.reshape(-1,) - numpy.amin(image)) * scale[2]
    return vertices


def surface(image, filename, scale =[1,1,1], offset=[0,0], reverse=False):

    f = open(filename, 'w')
    ncol = image.shape[1]

    connections = numpy.array(([0,ncol,1], [1,ncol, ncol+1]), ndmin=2, dtype=int)
    vertices = imageToXYZ(image, scale) + numpy.array((offset), ndmin=2)
    # if numpy.amax(numpy.abs(vertices[:,2])):
        # color = vertices[:,2] / numpy.amax(numpy.abs(vertices[:,2]))
    # else:
        # color = vertices[:,2]*0
        # print("we here")
        # input()
    

    if reverse:
        vertices[:,2] = numpy.amax(vertices[:,2]) - vertices[:,2]

    #print("writing vertices")
    for v in range(vertices.shape[0]):
        writeVertex(f, vertices[v,:])
        # writeColor(f, [color[v],color[v],color[v],1.0])
        color = [numpy.random.random(),numpy.random.random(),numpy.random.random(),1]
        writeColor(f, color)
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


    #print("writing uvs")
    max_x = numpy.amax(vertices[:,0])
    max_y = numpy.amax(vertices[:,1])
    for u in range(vertices.shape[0]):
        writeUV(f, [(vertices[u,0])/max_x, (vertices[u,1])/max_y])
    f.close()

    return vertices, index


def splitImage(image, chunk_size):
    row_depth = (image.shape[0]) / chunk_size
    col_depth = (image.shape[1]) / chunk_size
    images = numpy.empty(( chunk_size, chunk_size, int(row_depth), int(col_depth) ))
    for i in range(0, image.shape[0]-chunk_size-1, chunk_size):
        for j in range(0, image.shape[1]-chunk_size-1, chunk_size):
            images[:,:,int(i/chunk_size),int(j/chunk_size)] = image[i:(i+chunk_size), j:(j+chunk_size)]

    return images
         

def writeHeaderFile(header_filename, image_name, scale, chunk_size, rows, columns):
    header_string = "source_image: {}\nx_scale: {}\ny_scale: {}\nz_scale: {}\nchunk_size: {}\nrows: {}\ncolumns: {}".format(
        image_name, scale[0], scale[1], scale[2], chunk_size, rows, columns)
    with open(header_filename, 'w') as f:
        f.write(header_string)
