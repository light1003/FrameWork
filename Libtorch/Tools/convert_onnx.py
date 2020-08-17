import sys

sys.path.insert(0, "..")
import cv2
import torch
from src.model_lib import MultiFTNet

class Pytorch_model:
    def __init__(self, model_path,  gpu_id=None):
        '''
        初始化pytorch模型
        :param model_path: 模型地址(可以是模型的参数或者参数和计算图一起保存的文件)
        :param gpu_id: 在哪一块gpu上运行
        '''
        self.gpu_id = gpu_id

        if self.gpu_id is not None and isinstance(self.gpu_id, int) and torch.cuda.is_available():
            self.device = torch.device("cuda:%s" % self.gpu_id)
        else:
            self.device = torch.device("cpu")
        print('device:', self.device)
        input_width = 80
        input_height = 80
        param = {
            'num_classes': 4,
            'img_channel': 3,
            'embedding_size': 128,
            'conv6_kernel': ((input_height + 15) // 16, (input_width + 15) // 16)}

        self.model = MultiFTNet.MultiFTNet(**param).to("cuda")


        state_dict = torch.load(model_path, map_location=self.device)
        keys = iter(state_dict)
        first_layer_name = keys.__next__()
        if first_layer_name.find('module.') >= 0:

            from collections import OrderedDict
            new_state_dict = OrderedDict()
            for key, value in state_dict.items():
                name_key = key[7:]
                new_state_dict[name_key] = value
            self.model.load_state_dict(new_state_dict)
        else:

            self.model.load_state_dict(state_dict)
        self.model.to(self.device)
        self.model.eval()

dbnet = Pytorch_model(
    '../saved_logs/snapshot/Anti_Spoofing_new_80x80/2020-07-28-06-12_Anti_Spoofing_new_80x80_model_iter-2200.pth', gpu_id=0)

#------------------------------export------------------------------
output_onnx = 'Anti.onnx'
print("==> Exporting model to ONNX format at '{}'".format(output_onnx))
input_names = ["input"]
# output_names = ["hm" , "wh"  , "reg"]
output_names = ["output"]
inputs = torch.randn(1, 3, 80, 80, requires_grad=True, device='cuda')
# dbnet.model是模型的实例化
torch_out = dbnet.model(inputs)
# 导出的主要函数
torch.onnx._export(dbnet.model,
                   inputs,
                   output_onnx,
                   export_params=True, verbose=False,
                   input_names=input_names,
                   output_names=output_names)


"""
#动态输入输出
batchsize = 1
channel = 3
height = 480
width = 384
inputs = torch.randn(batchsize, channel, height, width, requires_grad=True, device='cuda')
dynamic_axes={'input':{0:'batch_size', 1:'channel', 2:'height', 3:'width'},     
              'output': {0:'batch_size', 1:'channel', 2:'height', 3:'width'}}
# dbnet.model是模型的实例化
torch_out = dbnet.model(inputs)
# 导出的主要函数
torch.onnx._export(dbnet.model,
                   inputs,
                   output_onnx,
                   export_params=True, verbose=False,
                   input_names=input_names,
                   output_names=output_names,
                   opset_version=11,
                   dynamic_axes=dynamic_axes) 
"""


