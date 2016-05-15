HW2 7&8 MATLAB to Python
=======================

### MATLAB
1. <code>imread</code>: 開啟圖片
1. change image array to double<br/>
    <code>f1 = im2double(f) * 255; % MAXi = 255</code>
1. use function <code>imfilter</code> with attribute: <code>'replicate'</code> <br/>
    'replicate': Input array values outside the bounds of the array are assumed to equal the nearest array border value.<br/>
    [doc of imfilter](http://www.mathworks.com/help/images/ref/imfilter.html)

### Python
1. <code>scipy.misc.imread</code>: 開啟圖片，需要模組pillow(branch from Python Image Library) <br/>
    [doc](http://docs.scipy.org/doc/scipy-0.17.0/reference/generated/scipy.misc.imread.html)
1. <code>scipy.ndimage.convolve</code>: 此函數可以取代Matlab的imfilter <br/>
    參數'replicate'可以用'nearest'取代<br/>
    [Reference](http://stackoverflow.com/questions/22142369/the-equivalent-function-of-matlab-imfilter-in-python)
1. <code>scipy.misc.imsave</code>: 將圖片存檔
