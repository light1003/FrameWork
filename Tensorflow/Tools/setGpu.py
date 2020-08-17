import tensorflow as tf


gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.1,visible_device_list='0') # gpu ID allow_growth=False 先分配很少的显存，按需慢慢增加显存，但不会释放，碎片化
config = tf.ConfigProto(gpu_options=gpu_options)

serialized = config.SerializeToString()
print(','.join(list(map(hex, serialized))))
