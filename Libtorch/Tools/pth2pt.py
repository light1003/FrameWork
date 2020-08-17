import torch 


 # 加载模型
from src.model_lib import MultiFTNet

input_width = 80
input_height = 80

param = {
    'num_classes': 4,
    'img_channel': 3,
    'embedding_size': 128,
    'conv6_kernel': ((input_height + 15) // 16, (input_width + 15) // 16)}

model = MultiFTNet.MultiFTNet(**param).cuda()


model_path = "../saved_logs/snapshot/Anti_Spoofing_new_80x80/2020-07-28-06-12_Anti_Spoofing_new_80x80_model_iter-2200.pth"
state_dict = torch.load(model_path, map_location={"cuda": 0})
keys = iter(state_dict)
first_layer_name = keys.__next__()
if first_layer_name.find('module.') >= 0:

    from collections import OrderedDict

    new_state_dict = OrderedDict()
    for key, value in state_dict.items():
        name_key = key[7:]
        new_state_dict[name_key] = value
    model.load_state_dict(new_state_dict)
else:

    model.load_state_dict(state_dict)



example = torch.rand(1, 3, 80, 80).cuda() # 注意，我这里导出的是CUDA版的模型，因为我的模型是在GPU中进行训练的
model = model.eval()

traced_script_module = torch.jit.trace(model, example)
output = traced_script_module(torch.ones(1,3,80,80).cuda())
traced_script_module.save('torch_script_eval.pt')
print(output)
