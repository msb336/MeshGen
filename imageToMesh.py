import sys, re
from MeshHelperFunctions import *
import skimage.measure
from scipy.ndimage import gaussian_filter
import cv2




image_path = sys.argv[1]
scale = numpy.array([float(s) for s in re.findall(r'-?\d+\.?\d*', sys.argv[2])])
chunk_size = int(sys.argv[3])

image_file = cv2.imread(image_path, cv2.IMREAD_UNCHANGED)
print("Shape of image: {}".format(image_file.shape))
if len(image_file.shape) == 3:
	if image_file.shape[2] == 4:
		b,g,r,a = cv2.split(image_file)
		if numpy.all((b+g+r) == 0):
			gray_image = a
			print("image file is alpha channel")
		else:
			gray_image = cv2.cvtColor(image_file, cv2.COLOR_BGR2GRAY)
	else:
		gray_image = cv2.cvtColor(image_file, cv2.COLOR_BGR2GRAY)
else:
	gray_image = image_file #cv2.cvtColor(image_file, cv2.COLOR_BGR2GRAY)

img = numpy.asarray(gray_image, dtype=float)
image_set = splitImage(img, chunk_size)
filtered_img = img #skimage.measure.block_reduce(img, (5,5), numpy.max)


print("total chunks: {}".format(image_set.shape[2] * image_set.shape[3]))
print(numpy.amax(image_file))
cv2.imwrite("Content\\texture.jpg", image_file)
writeHeaderFile('Content\\MeshComponents\\meshinfo', image_path, scale, chunk_size, image_set.shape[2], image_set.shape[3])
for i in range(image_set.shape[2]):
    for j in range(image_set.shape[3]):
        offset = [j*(chunk_size-1)*scale[0], i*(chunk_size-1)*scale[1], 0]
        vertices, index = surface(image_set[:,:,i,j], 'Content\\MeshComponents\\image_mesh{}.obj'.format(i+j*image_set.shape[2]), scale, offset, reverse=False)
# sparsify(vertices, index, 3)