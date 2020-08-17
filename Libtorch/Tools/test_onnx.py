import onnxruntime as rt
import  numpy as np
import cv2
import warnings
warnings.filterwarnings('ignore')
def crop_bbox1(img,L=80,Crop_N=10):
    split_img = []
    # get shape
    H, W, C = img.shape

    for i in range(W // 3, W // 3 * 2 - L, 80):
        for j in range(H // 3, H // 3 * 2 - L, 80):
            if len(split_img) == Crop_N:
                break
            x1 = i
            y1 = j

            x2 = x1 + L
            y2 = y1 + L
            # print(x1,y1,x2,y2)
            # cv2.rectangle(img, (x1, y1), (x2, y2), [0, 0, 255],3)
            # tep = cv2.resize(img, (500, 500))

            image = img[y1:y2,x1:x2]
            cv2.imshow("testData", image)
            cv2.waitKey(0)
            split_img.append(image)
        if len(split_img) == Crop_N:
            break
    return split_img
image = cv2.imread("10.jpg")

split_img = crop_bbox1(image)


img = split_img[0].transpose(2, 0, 1)
transformed_image = np.expand_dims(img, axis=0)

sess = rt.InferenceSession("../Anti.onnx")
out = sess.run(["output"], {"input":transformed_image.astype(np.float32)})
print(out)



