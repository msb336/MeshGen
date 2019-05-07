import sys, re
from MeshHelperFunctions import *
import skimage.measure
from scipy.ndimage import gaussian_filter
import cv2




image_path = sys.argv[1]
scale = numpy.array([float(s) for s in re.findall(r'-?\d+\.?\d*', sys.argv[2])])
chunk_size = int(sys.argv[3])

image_file = cv2.imread(image_path)
gray_image = cv2.cvtColor(image_file, cv2.COLOR_BGR2GRAY)

img = numpy.asarray(gray_image, dtype=float)
image_set = splitImage(img, chunk_size)
filtered_img = img #skimage.measure.block_reduce(img, (5,5), numpy.max)


print(img.shape)
cv2.imwrite("Content\\texture.jpg", image_file)
for i in range(image_set.shape[2]):
    for j in range(image_set.shape[3]):
        offset = [j*chunk_size*scale[0], i*chunk_size*scale[1], 0]
        vertices, index = surface(image_set[:,:,i,j], 'Content\\MeshComponents\\image_mesh{}_{}.obj'.format(i,j), scale, offset, reverse=True)
# sparsify(vertices, index, 3)