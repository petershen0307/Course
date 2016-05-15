from scipy import misc
from scipy import ndimage
import numpy as np
import matplotlib.pyplot as plt


def filtering_image(image_path, filter):
    origin_image = misc.imread(image_path, mode='L')
    # plt.imshow(origin_image, cmap='Greys_r')
    # plt.show()
    print(origin_image)
    origin_image_double = origin_image.astype(np.float64)
    print(type(origin_image_double[0][0]))

    print(filter)

    filter_image = ndimage.convolve(origin_image_double, filter, mode='nearest')
    misc.imsave('low_pass_filter_{0}{1}.gif'.format(len(filter), len(filter[0])), filter_image, format='gif')
    # plt.imshow(filter_image, cmap='Greys_r')
    # plt.show()
    print(filter_image)
    print('origin_image_double n1: {0}'.format(len(origin_image_double)))
    print('origin_image_double n2: {0}'.format(len(origin_image_double[0])))

    print('filter_image n1: {0}'.format(len(filter_image)))
    print('filter_image n2: {0}'.format(len(filter_image[0])))
    return origin_image_double, filter_image


def calc_psnr(image_path, filter):
    origin_image_double, filter_image = filtering_image(image_path, filter)
    MAXI = 255
    MSE = 0
    for n1 in range(0, MAXI):
        for n2 in range(0, MAXI):
            MSE += ((origin_image_double[n1][n2] - filter_image[n1][n2]) ** 2)
    MSE /= (MAXI * MAXI)
    print('MSE: {0}'.format(MSE))
    PSNR = 10 * np.log10(MAXI ** 2 / MSE)
    print('PSNR: {0}'.format(PSNR))
    return PSNR


if '__main__' == __name__:
    image_path = 'digital-images-week2_quizzes-lena.gif'
    # question 7
    low_pass_filter_33 = np.array([[1 / 9]*3]*3)
    calc_psnr(image_path, low_pass_filter_33)
    # question 8
    low_pass_filter_55 = np.array([[1 / 25]*5]*5)
    calc_psnr(image_path, low_pass_filter_55)
